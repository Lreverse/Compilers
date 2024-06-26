#include "semantic.h"


void Program(Tnode *node)
{
    ExtDefList(node->lchild);
}

void ExtDefList(Tnode *node)
{
    if (node == NULL)    // 空产生式
        return;
    ExtDef(node->lchild);
    ExtDefList(node->lchild->rsibling);
}

/* 全局定义 */
void ExtDef(Tnode *node)
{
    Type type = Specifier(node->lchild);
    char Node_name[32];
    strcpy(Node_name, node->lchild->rsibling->name);
    if(!strcmp(Node_name, "ExtDecList"))
    {
        ExtDecList(node->lchild->rsibling, type);
    }
    else if(!strcmp(Node_name, "SEMI"))
    {
        return;
    }
    else if(!strcmp(Node_name, "FunDec"))
    {
        FunDec(node->lchild->rsibling, type);
        CompSt(node->lchild->rsibling->rsibling, type);
    }
}

/* 
 *  类型描述符
 *  
 *  返回值：每个结点的类型
 *  
 */
Type Specifier(Tnode *node)
{
    Type type = (Type)malloc(sizeof(Type_));
    char Node_name[32];
    strcpy(Node_name, node->lchild->name);
    if (!strcmp(Node_name, "TYPE"))
    {
        type->kind = BASIC;
        type->u.basic = TYPE(node->lchild);
    }
    else if (!strcmp(Node_name, "StructSpecifier"))
    {
        type->kind = STRUCTURE;
        type->u.structure = NULL;
        StructSpecifier(node->lchild, type);
    }
    return type;
}


void ExtDecList(Tnode *node, Type type)
{
    VarDec(node->lchild, type, VARIABLE);
    if (node->lchild->rsibling)
    {
        // 对于`int a, b, c;`的情况，进行递归调用
        ExtDecList(node->lchild->rsibling->rsibling, type);
    }
}

enum BASIC_TYPE TYPE(Tnode *node)
{
    if (!strcmp(node->value, "int"))
        return BASIC_INT;
    else if (!strcmp(node->value, "float"))
        return BASIC_FLOAT;
}

/* 结构体定义，在此处插入符号表 */
void StructSpecifier(Tnode *node, Type type)
{
    // 结构体类型定义
    if (!strcmp(node->lchild->rsibling->name, "OptTag"))   // 说明该结构体有名字
    {
        DefList(node->lchild->rsibling->rsibling->rsibling, type);

        /* Error type 16 */
        if (check(SymbolTable, node->lchild->rsibling->lchild->value, STRUC))
        {
            printf("Error type 16 at Line %d: Duplicated name \"%s\"\n", node->lineno, node->lchild->rsibling->lchild->value);
            return;
        }
        HashNode Hnode = createHnode(node->lchild->rsibling->lchild->value, type);
        insertHnode(SymbolTable, Hnode);
    }
    else if (!strcmp(node->lchild->rsibling->name, "LC"))   // 匿名结构体
    {
        DefList(node->lchild->rsibling->rsibling, type);
        
        // 给匿名结构体取名字=所有域的名字之和
        // 由于在createHnode时，是直接赋值字符地址的，所以这里需要malloc
        FieldList p = type->u.structure;
        char *name = (char*)malloc(sizeof(char) * 32);
        strcpy(name, "anonymous");
        while(p)
        {
            strcat(name, p->name);
            p = p->tail;
        }
        HashNode Hnode = createHnode(name, type);
        insertHnode(SymbolTable, Hnode);
    }
    // 声明结构体变量
    else
    {
        Type type_struc = check(SymbolTable, node->lchild->rsibling->lchild->value, STRUC);
        if (type_struc == NULL)
        {
            /* Error type 17 */
            printf("Error type 17 at Line %d: Undefined structure \"%s\"\n", node->lineno, node->lchild->rsibling->lchild->value);
        }
        else
        {
            type->u.structure = type_struc->u.structure;
        }
    }
}

/* 获得变量名和类型，将其插入到符号表中 */
FieldList VarDec(Tnode *node, Type type, enum VarDec_flag flag)
{
    if (!strcmp(node->lchild->name, "ID"))
    {
        if (flag == VARIABLE)
        {
            if(check(SymbolTable, node->lchild->value, VARIABLE))
            {
                printf("Error type 3 at Line %d: Redefined variable \"%s\"\n", node->lineno, node->lchild->value);
            }
            else
            {
                HashNode Hnode = createHnode(node->lchild->value, type);
                insertHnode(SymbolTable, Hnode);
            }
            return NULL;
        }
        else if (flag == PARAMETER)
        {
            // 将函数的形参也添加到符号表中
            if(check(SymbolTable, node->lchild->value, VARIABLE))
            {
                printf("Error type 3 at Line %d: Redefined variable \"%s\"\n", node->lineno, node->lchild->value);
            }
            else
            {
                HashNode Hnode = createHnode(node->lchild->value, type);
                insertHnode(SymbolTable, Hnode);
            }
            // 生成FieldList链接到函数结点
            FieldList argv = (FieldList)malloc(sizeof(FieldList_));
            argv->name = node->lchild->value;
            argv->type = type;
            argv->tail = NULL;
            return argv;
        }
        else if (flag == STRUC)   // 结构体声明
        {
            // 生成FieldList链接到结构体结点
            FieldList argv = (FieldList)malloc(sizeof(FieldList_));
            argv->name = node->lchild->value;
            argv->type = type;
            argv->tail = NULL;
            return argv;
        }
    }
    else if (!strcmp(node->lchild->name, "VarDec"))
    {
        Type type_array = (Type)malloc(sizeof(Type_));
        type_array->kind = ARRAY;
        type_array->u.array.size = atoi(node->lchild->rsibling->rsibling->value);
        type_array->u.array.elem = type;
        VarDec(node->lchild, type_array, flag);
    }
    /* int a[10][3] */
}

/*
 *  定义列表
 * 
 *  如果是由结构体派生而来的，则需要进行将结构体里的变量链接(在Dec处链接)
 */
void DefList(Tnode *node, Type struc_type)
{
    if (node == NULL)   // 空产生式
        return;
    Def(node->lchild, struc_type);
    DefList(node->lchild->rsibling, struc_type);
}

void Def(Tnode *node, Type struc_type)
{
    Type type = Specifier(node->lchild);
    DecList(node->lchild->rsibling, type, struc_type);
}

void DecList(Tnode *node, Type type, Type struc_type)
{
    Dec(node->lchild, type, struc_type);
    if (node->lchild->rsibling)
    {
        // 对于逗号，进行递归调用
        DecList(node->lchild->rsibling->rsibling, type, struc_type);
    }
}

void Dec(Tnode *node, Type type, Type struc_type)
{
    if (struc_type == NULL)
    {
        VarDec(node->lchild, type, VARIABLE);
    }
    else
    {
        FieldList structure = VarDec(node->lchild, type, STRUC);
        /* Error type 15 */
        FieldList p = struc_type->u.structure;
        while(p)
        {
            if (!strcmp(structure->name, p->name))
            {
                printf("Error type 15 at Line %d: Redefined field \"%s\"\n", node->lineno, structure->name);
                return;
            }
            p = p->tail;
        }
        structure->tail = struc_type->u.structure;     // 采用头插法
        struc_type->u.structure = structure;
    }

    if (node->lchild->rsibling)
    {
        // 定义时赋值
        if (!strcmp(node->lchild->rsibling->name, "ASSIGNOP") && struc_type == NULL)
        {
            Type type_cmp = Exp(node->lchild->rsibling->rsibling);
            if (type->kind != type_cmp->kind)
            {
                printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->lineno);
            }
            else
            {
                if (type->kind == BASIC)
                {
                    if (type->u.basic != type_cmp->u.basic)
                        printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->lineno);
                }
                // else if(type->kind == ARRAY)
                // {
                //     Type type1 = get_array_type(type);
                //     Type type2 = get_array_type(type_cmp);
                // }
            }
        }
        else if (!strcmp(node->lchild->rsibling->name, "ASSIGNOP") && struc_type != NULL)
        {
            printf("Error type 15 at Line %d: Prohibit initialization of variables in structure\n", node->lineno);
        }
    }
}

/* 函数定义(在此处插入结点) */
void FunDec(Tnode *node, Type rtnType)
{
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = FUNCTION;
    type->u.function.rtnType = rtnType;
    type->u.function.argc = 0;
    type->u.function.argv = NULL;
    if (!strcmp(node->lchild->rsibling->rsibling->name, "VarList"))
    {
        VarList(node->lchild->rsibling->rsibling, type);
    }
    if (check(SymbolTable, node->lchild->value, PARAMETER))
    {
        printf("Error type 4 at Line %d: Redefined function \"%s\"\n", node->lineno, node->lchild->value);
    }
    else
    {
        HashNode Hnode = createHnode(node->lchild->value, type);
        insertHnode(SymbolTable, Hnode);
    }
}

void VarList(Tnode *node, Type type)
{
    ParamDec(node->lchild, type);
    if (node->lchild->rsibling)
        VarList(node->lchild->rsibling->rsibling, type);
}

/* 形参定义 */
/* 为每个形参生成FieldList，将其插入到函数type的argv中，并将参数个数+1 */
void ParamDec(Tnode *node, Type type)
{
    Type param_type = Specifier(node->lchild);
    FieldList argv = NULL;
    argv = VarDec(node->lchild->rsibling, param_type, PARAMETER);
    FieldList p = type->u.function.argv;   // 采用尾插法
    if (!p)
    {
        type->u.function.argv = argv;
    }
    else
    {
        while(p->tail)
            p = p->tail;
        p->tail = argv;
    }
    type->u.function.argc++;
}

void CompSt(Tnode *node, Type rtn_type)
{
    if (!strcmp(node->lchild->rsibling->name, "RC"))
        return;
    if (!strcmp(node->lchild->rsibling->rsibling->name, "RC"))
    {
        if (!strcmp(node->lchild->rsibling->name, "DefList"))
            DefList(node->lchild->rsibling, NULL);
        else if (!strcmp(node->lchild->rsibling->name, "StmtList"))
            StmtList(node->lchild->rsibling, rtn_type);
        return;
    }
    DefList(node->lchild->rsibling, NULL);
    StmtList(node->lchild->rsibling->rsibling, rtn_type);
    // 因为DefList和StmtList都可以为空产生式，所以如果这里不进行比较，会导致传入错误结点，导致后续分析出错，出现段错误
}

void StmtList(Tnode *node, Type rtn_type)
{
    if (node == NULL)  // 空产生式
        return;
    Stmt(node->lchild, rtn_type);
    StmtList(node->lchild->rsibling, rtn_type);
}

void Stmt(Tnode *node, Type rtn_type)
{
    if (!strcmp(node->lchild->name, "Exp"))
    {
        Exp(node->lchild);
    }
    else if (!strcmp(node->lchild->name, "CompSt"))
    {
        CompSt(node->lchild, rtn_type);
    }
    else if (!strcmp(node->lchild->name, "RETURN"))
    {
        // 需要判断返回类型
        Type type = Exp(node->lchild->rsibling);
        if (type == NULL)
            return;
        /* Error type 8 */
        if (type_cmp(type, rtn_type))
        {
            printf("Error type 8 at Line %d: Type mismatched for return\n", node->lineno);
        }
    }
    else if (!strcmp(node->lchild->name, "IF"))
    {
        Exp(node->lchild->rsibling->rsibling);
        Stmt(node->lchild->rsibling->rsibling->rsibling->rsibling, rtn_type);
        if (node->lchild->rsibling->rsibling->rsibling->rsibling->rsibling != NULL)   // 遇到"ELSE"
        {
            Stmt(node->lchild->rsibling->rsibling->rsibling->rsibling->rsibling->rsibling, rtn_type);
        }
    }
    else if(!strcmp(node->lchild->name, "WHILE"))
    {
        Exp(node->lchild->rsibling->rsibling);
        Stmt(node->lchild->rsibling->rsibling->rsibling->rsibling, rtn_type);
    }
}

/* 处理表达式 */
Type Exp(Tnode *node)
{
    if (!strcmp(node->lchild->name, "Exp"))
    {
        Type type1, type2;
        if (!strcmp(node->lchild->rsibling->name, "ASSIGNOP"))
        {
            type1 = Exp(node->lchild);
            type2 = Exp(node->lchild->rsibling->rsibling);

            /* 下层type不合法 */
            if (type1 == NULL)
                return NULL;
            if (type2 == NULL)
                return NULL;
            
            /* Error type6 */
            if (!strcmp(node->lchild->lchild->name, "ID") && node->lchild->lchild->rsibling == NULL) {}
            else if (!strcmp(node->lchild->lchild->name, "Exp") && !strcmp(node->lchild->lchild->rsibling->name, "LB")) {}
            else if (!strcmp(node->lchild->lchild->name, "Exp") && !strcmp(node->lchild->lchild->rsibling->name, "DOT")) {}
            else
            {
                printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable\n", node->lineno);
                return NULL;
            }

            /* Error type 5*/
            if (type1->kind != type2->kind)  // 基本类型不一致
                printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->lineno);
            else if (type1->kind == BASIC)   // 如果两个都是BASIC类型
                if (type1->u.basic != type2->u.basic)
                    printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->lineno);
            else if (type1->kind == ARRAY)    // 如果两个都是ARRAY类型
            {
                type1 = get_array_type(type1);
                type2 = get_array_type(type2);
                if (type1->kind != type2->kind)
                    printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->lineno);
                else if (type1->kind == BASIC)
                    if (type1->u.basic != type2->u.basic)
                        printf("Error type 5 at Line %d: Type mismatched for assignment\n", node->lineno);
                else if (type1->kind == STRUCTURE)
                {
                    // 如果是结构体数组，暂时不做处理
                }
            }
            return NULL;
        }
        else if (!strcmp(node->lchild->rsibling->name, "LB"))  // 访问数组
        {
            type1 = Exp(node->lchild);
            type2 = Exp(node->lchild->rsibling->rsibling);

            /* 下层type不合法 */
            if (type1 == NULL)
                return NULL;
            if (type2 == NULL)
                return NULL;
            
            if (!strcmp(node->lchild->lchild->name, "ID"))
            {
                /* Error type 10 */
                if (type1->kind != ARRAY)
                {
                    printf("Error type 10 at Line %d: \"%s\" is not an array\n", node->lineno, node->lchild->lchild->value);
                    return NULL;
                }
                type1 = get_array_type(type1);
            }

            /* Error type 12 */
            if (type2->kind != BASIC)
            {
                printf("Error type 12 at Line %d: \"%s\" is not an integer\n", node->lineno, node->lchild->rsibling->rsibling->lchild->value);
                return NULL;
            }
            else if (type2->u.basic != BASIC_INT)
            {
                printf("Error type 12 at Line %d: \"%s\" is not an integer\n", node->lineno, node->lchild->rsibling->rsibling->lchild->value);
                return NULL;
            }
            return type1;
        }
        else if (!strcmp(node->lchild->rsibling->name, "DOT"))  // 访问结构体
        {
            type1 = Exp(node->lchild);
            /* Error type 13 */
            if(type1->kind != STRUCTURE)
            {
                printf("Error type 13 at Line %d: Illegal use of \".\"\n", node->lineno);
                return NULL;
            }

            // 遍历对应结构体的所有对象，查看是否有ID所对应的值的变量
            FieldList p = type1->u.structure;
            while (p)
            {
                if (!strcmp(p->name, node->lchild->rsibling->rsibling->value)) 
                {
                    return p->type;
                }
                p = p->tail;
            }
            /* Error type 14 */
            printf("Error type 14 at Line %d: Non-existent field \"%s\"\n", node->lineno, node->lchild->rsibling->rsibling->value);
            return NULL;
        }
        else  // 处理操作符
        {
            type1 = Exp(node->lchild);
            type2 = Exp(node->lchild->rsibling->rsibling);

            /* 下层type不合法 */
            if (type1 == NULL)
                return NULL;
            if (type2 == NULL)
                return NULL;

            if (type1->kind != type2->kind)  // 基本类型不一致
            {
                printf("Error type 7 at Line %d: Type mismatched for operands\n", node->lineno);
                return NULL;
            }
            else if (type1->kind == BASIC)   // 如果两个都是BASIC类型
            {
                if (type1->u.basic != type2->u.basic)
                {
                    printf("Error type 7 at Line %d: Type mismatched for operands\n", node->lineno);
                    return NULL;
                }
            }
            else if (type1->kind == ARRAY)    // 如果两个都是ARRAY类型，直接报错
            {
                printf("Error type 7 at Line %d: Type mismatched for operands\n", node->lineno);
                return NULL;
            }
            return type1;
        }
    }
    else if (!strcmp(node->lchild->name, "ID"))
    {
        if (node->lchild->rsibling == NULL)   // 说明这是一个变量名
        {
            Type type = check(SymbolTable, node->lchild->value, VARIABLE);
            if(!type)
            {
                printf("Error type 1 at Line %d: Undefined variable \"%s\"\n", node->lineno, node->lchild->value);
                return NULL;
            }
            else
            {
                return type;
            }
        }
        else   // 说明这是个函数名
        {
            Type type = check(SymbolTable, node->lchild->value, VARIABLE);
            if (type)
            {
                printf("Error type 11 at Line %d: \"%s\" is not a function\n", node->lineno, node->lchild->value);
                return NULL;
            }
            type = check(SymbolTable, node->lchild->value, PARAMETER);
            if (!type)
            {
                printf("Error type 2 at Line %d: Undefined function \"%s\"\n", node->lineno, node->lchild->value);
                return NULL;
            }
            else
            {
                FieldList argv = (FieldList)malloc(sizeof(FieldList_));   // 头指针
                argv->tail = NULL;
                if (!strcmp(node->lchild->rsibling->rsibling->name, "Args"))
                {
                    Args(node->lchild->rsibling->rsibling, argv);
                }

                /* Error type 9 (比较形参和实参的数目和类型是否一致) */
                FieldList p = type->u.function.argv, p_cmp = argv->tail;
                // 判断类型
                while (p && p_cmp)
                {
                    if (type_cmp(p->type, p_cmp->type))
                    {
                        printf("Error type 9 at Line %d: Function \"%s(", node->lineno, node->lchild->value);
                        p = type->u.function.argv;
                        while (p)
                        {
                            print_type_name(p->type);
                            p = p->tail;
                            if (p)
                                printf(", ");
                        }

                        printf(")\" is not applicable for arguments \"(");
                        p_cmp = argv->tail;
                        while (p_cmp)
                        {
                            print_type_name(p_cmp->type);
                            p_cmp = p_cmp->tail;
                            if (p_cmp)
                                printf(", ");
                        }
                        printf(")\"\n");
                        break;
                    }
                    p = p->tail;
                    p_cmp = p_cmp ->tail;
                }
                // 判断数量
                if (p != NULL || p_cmp != NULL)
                {
                    printf("Error type 9 at Line %d: Function \"%s(", node->lineno, node->lchild->value);
                        p = type->u.function.argv;
                        while (p)
                        {
                            print_type_name(p->type);
                            p = p->tail;
                            if (p)
                                printf(", ");
                        }

                        printf(")\" is not applicable for arguments \"(");
                        p_cmp = argv->tail;
                        while (p_cmp)
                        {
                            print_type_name(p_cmp->type);
                            p_cmp = p_cmp->tail;
                            if (p_cmp)
                                printf(", ");
                        }
                        printf(")\"\n");
                }
                return get_function_rtnType(type);
            }
        }
    }
    else if (!strcmp(node->lchild->name, "INT"))
    {
        Type type = (Type)malloc(sizeof(Type_));
        type->kind = BASIC;
        type->u.basic = BASIC_INT;
        return type;
    }
    else if (!strcmp(node->lchild->name, "FLOAT"))
    {
        Type type = (Type)malloc(sizeof(Type_));
        type->kind = BASIC;
        type->u.basic = BASIC_FLOAT;
        return type;
    }
    else if (!strcmp(node->lchild->name, "LP"))
    {
        Type type = NULL;
        type = Exp(node->lchild->rsibling);
        return type;
    }
    else if (!strcmp(node->lchild->name, "MINUS"))
    {
        Type type = NULL;
        type = Exp(node->lchild->rsibling);
        return type;
    }
    else if (!strcmp(node->lchild->name, "NOT"))
    {
        Type type = NULL;
        type = Exp(node->lchild->rsibling);
        return type;
    }
}

/* 处理语句中的函数参数 */
void Args(Tnode *node, FieldList argv)
{
    Type type = Exp(node->lchild);
    FieldList p = (FieldList)malloc(sizeof(FieldList_));
    p->type = type;
    p->tail = NULL;
    argv->tail = p;
    if (node->lchild->rsibling != NULL)
    {
        Args(node->lchild->rsibling->rsibling, p);
    }
}

/* 获取数组的基本类型 */
Type get_array_type(Type type)
{
    Type elem = type->u.array.elem;
    while(elem && elem->kind == ARRAY)
    {
        elem = elem->u.array.elem;
    }
    return elem;
}

/* 获取函数的返回类型*/
Type get_function_rtnType(Type type)
{
    return type->u.function.rtnType;
}

/*
 *  判断两个type是否一致
 *  
 *  返回值：0代表一致；1代表不一致
 * 
 */
int type_cmp(Type type1, Type type2)
{
    if (type1->kind != type2->kind)  // 基本类型不一致
    {
        return 1;
    }
    else if (type1->kind == BASIC)  // 如果两个都是BASIC类型
    {
        if (type1->u.basic != type2->u.basic)
            return 1;
    }
    return 0;
}

/* 打印类型的名字 */
void print_type_name(Type type)
{
    if (type->kind == BASIC)
    {
        if (type->u.basic == BASIC_INT)
            printf("int");
        else if (type->u.basic == BASIC_FLOAT)
            printf("float");
    }
}