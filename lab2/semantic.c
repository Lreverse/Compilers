#include "semantic.h"


void Program(Tnode *node)
{
    ExcDefList(node->lchild);
}

void ExcDefList(Tnode *node)
{
    if (!node)    // 空产生式
        return;
    ExtDef(node->lchild);
    ExcDefList(node->lchild->rsibling);
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
            HashNode Hnode = createHnode(node->lchild->value, type);
            insertHnode(SymbolTable, Hnode);
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
    if (!node)   // 空产生式
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
}

/* 函数定义 */
void FunDec(Tnode *node, Type rtnType)
{
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = FUNCTION;
    type->u.function.rtnType = rtnType;
    type->u.function.argc = 0;
    type->u.function.argv = NULL;
    if(!strcmp(node->lchild->rsibling->rsibling->name, "VarList"))
    {
        VarList(node->lchild->rsibling->rsibling, type);
    }
    HashNode Hnode = createHnode(node->lchild->value, type);
    insertHnode(SymbolTable, Hnode);
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