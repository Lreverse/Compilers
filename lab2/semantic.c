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
        CompSt(node->lchild->rsibling->rsibling);
    }
}

/* 类型描述符 */
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
        // StructSpecifier(node->lchild, type);
    }
    return type;
}


void ExtDecList(Tnode *node, Type type)
{
    VarDec(node->lchild, type, VARIABLE);
    if (node->lchild->rsibling)
    {
        // 对于int a, b, c;的情况，进行递归调用
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
// FieldList StructSpecifier(Tnode *node, Type type)
// {
    
// }

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

void DefList(Tnode *node)
{
    if (node == NULL)   // 空产生式
        return;
    Def(node->lchild);
    DefList(node->lchild->rsibling);
}

void Def(Tnode *node)
{
    Type type = Specifier(node->lchild);
    DecList(node->lchild->rsibling, type);
}

void DecList(Tnode *node, Type type)
{
    Dec(node->lchild, type);
    if (node->lchild->rsibling)
    {
        // 对于逗号，进行递归调用
        DecList(node->lchild->rsibling->rsibling, type);
    }
}

void Dec(Tnode *node, Type type)
{
    VarDec(node->lchild, type, VARIABLE);
    // 这里还有赋值的情况
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

void CompSt(Tnode *node)
{
    if (!strcmp(node->lchild->rsibling->name, "RC"))
        return;
    if (!strcmp(node->lchild->rsibling->name, "DefList"))
        DefList(node->lchild->rsibling);
    if (!strcmp(node->lchild->rsibling->rsibling->name, "StmtList"))  
        StmtList(node->lchild->rsibling->rsibling);
    // 因为DefList和StmtList都可以为空产生式，所以如果这里不进行比较，会导致传入错误结点，导致后续分析出错，出现段错误
}

void StmtList(Tnode *node)
{
    if (node == NULL)  // 空产生式
        return;
    Stmt(node->lchild);
    StmtList(node->lchild->rsibling);
}

void Stmt(Tnode *node)
{
    if (!strcmp(node->lchild->name, "Exp"))
    {
        Exp(node->lchild);
    }
    else if (!strcmp(node->lchild->name, "CompSt"))
    {
        CompSt(node->lchild);
    }
    else if (!strcmp(node->lchild->name, "RETURN"))
    {
        printf("in return\n");
        Exp(node->lchild->rsibling);
        printf("out return\n");
    }
    else if (!strcmp(node->lchild->name, "IF"))
    {

    }
    else if(!strcmp(node->lchild->name, "WHILE"))
    {

    }
    
}

/* 处理表达式 */
Type Exp(Tnode *node)
{
    if (!strcmp(node->lchild->name, "Exp"))
    {
        Type type1, type2;
        type1 = Exp(node->lchild);
        type2 = Exp(node->lchild->rsibling->rsibling);
        if (!strcmp(node->lchild->rsibling->name, "ASSIGNOP"))
        {
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
            /* Error type 10 */
            Type type_name = Exp(node->lchild);
            if (type_name->kind != ARRAY)
            {
                printf("Error type 10 at Line %d: \"%s\" is not an array\n", node->lineno, node->lchild->lchild->value);
                return NULL;
            }

            Type type = Exp(node->lchild->rsibling->rsibling);
            /* Error type 12 */
            if (type->kind != BASIC)
            {
                printf("Error type 12 at Line %d: \"%s\" is not an integer\n", node->lineno, node->lchild->rsibling->rsibling->lchild->value);
                return NULL;
            }
            else if (type->u.basic != BASIC_INT)
            {
                printf("Error type 12 at Line %d: \"%s\" is not an integer\n", node->lineno, node->lchild->rsibling->rsibling->lchild->value);
                return NULL;
            }
            return type_name;
        }
        else if (!strcmp(node->lchild->rsibling->name, "DOT"))  // 访问结构体
        {

        }
        else  // 处理操作符
        {
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
            else if (type1->kind == ARRAY)    // 如果两个都是ARRAY类型
            {
                type1 = get_array_type(type1);
                type2 = get_array_type(type2);
                if (type1->kind != type2->kind)
                {
                    printf("Error type 7 at Line %d: Type mismatched for operands\n", node->lineno);
                    return NULL;
                }
                else if (type1->kind == BASIC)
                {
                    if (type1->u.basic != type2->u.basic)
                    {
                        printf("Error type 7 at Line %d: Type mismatched for operands\n", node->lineno);
                        return NULL;
                    }
                }
                else if (type1->kind == STRUCTURE)
                {
                    // 如果是结构体数组，暂时不做处理
                }
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
            }
            else
            {
                return type;
            }
        }
        else   // 说明这是个函数名
        {
            Type type = check(SymbolTable, node->lchild->value, PARAMETER);
            if(!type)
            {
                printf("Error type 2 at Line %d: Undefined function \"%s\"\n", node->lineno, node->lchild->value);
            }
            else
            {
                // 对函数参数暂时不做处理，回头再来
                return type;
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