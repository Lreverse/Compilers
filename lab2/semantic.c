#include "semantic.h"


void Program(Tnode *node)
{
    ExcDecList(node->lchild);
}

void ExcDecList(Tnode *node)
{
    if (!node)    // 空产生式
        return;
    ExtDef(node->lchild);
    ExcDecList(node->lchild->rsibling);
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
    VarDec(node->lchild, type);
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
void VarDec(Tnode *node, Type type)
{
    if (!strcmp(node->lchild->name, "ID"))
    {
        HashNode Hnode = createHnode(node->lchild->value, type);
        insertHnode(SymbolTable, Hnode);
    }
    else if (!strcmp(node->lchild->name, "VarDec"))
    {
        Type type_array = (Type)malloc(sizeof(Type_));
        type_array->kind = ARRAY;
        type_array->u.array.size = atoi(node->lchild->rsibling->rsibling->value);
        type_array->u.array.elem = type;
        VarDec(node->lchild, type_array);
    }
    /* int a[10][3] */
}

