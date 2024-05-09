#include "IR.h"

InterCodes IR_Head = NULL;

/* 初始化双循环链表 */
void initList(InterCodes head)
{
    head->prev = head;
    head->next = head;
}

/* 将中间代码结点插入链表 */
void insertList(InterCodes head, InterCodes node)
{
    // 头插法（打印时向前遍历）
    node->prev = head;
    node->next = head->next;
    head->next = node;
    if (head->prev == head)
        head->prev = node;
}

/* 打印操作数 */
void print_OP(Operand op)
{

}

/* 打印中间代码 */
void print_IR(InterCodes head)
{
    InterCodes IR = head->prev;
    if (IR == head)
    {
        printf("list is empty!\n");
        return;
    }
    while(IR != head)
    {
        switch(IR->code.kind)
        {
            case IR_LABEL:
                printf("LABEL %s :\n", IR->code.u.one.op->u.value);
                break;
            case IR_FUNCTION:
                printf("FUNCTION %s :\n", IR->code.u.one.op->u.value);
                break;
            case IR_ASSIGN:
                print_OP(IR->code.u.assign.left);
                printf(" := ");
                print_OP(IR->code.u.assign.right);
                printf("\n");
                break;
            case IR_ADD:
                print_OP(IR->code.u.three.result);
                printf(" := ");
                print_OP(IR->code.u.three.op1);
                printf(" + ");
                print_OP(IR->code.u.three.op2);
                printf("\n");
                break;
            case IR_SUB:
                print_OP(IR->code.u.three.result);
                printf(" := ");
                print_OP(IR->code.u.three.op1);
                printf(" - ");
                print_OP(IR->code.u.three.op2);
                printf("\n");
                break;
            case IR_MUL:
                print_OP(IR->code.u.three.result);
                printf(" := ");
                print_OP(IR->code.u.three.op1);
                printf(" * ");
                print_OP(IR->code.u.three.op2);
                printf("\n");
                break;
            case IR_DIV:
                print_OP(IR->code.u.three.result);
                printf(" := ");
                print_OP(IR->code.u.three.op1);
                printf(" / ");
                print_OP(IR->code.u.three.op2);
                printf("\n");
                break;
            default:
                printf("\n");
                break;
        }
        IR = IR->prev;
    }
}

/* Program */
void translate_Program(Tnode *node)
{
    translate_ExtDefList(node->lchild);
}

/* ExtDefList */
void translate_ExtDefList(Tnode *node)
{
    if (node == NULL)    // 空产生式
        return;
    translate_ExtDef(node->lchild);
    translate_ExtDefList(node->lchild->rsibling);
}

/* 全局定义 */
void translate_ExtDef(Tnode *node)
{
    char Node_name[32];
    strcpy(Node_name, node->lchild->rsibling->name);
    if(!strcmp(Node_name, "ExtDecList"))
    {
        // translate_ExtDecList(node->lchild->rsibling);
    }
    else if(!strcmp(Node_name, "SEMI"))
    {
        return;
    }
    else if(!strcmp(Node_name, "FunDec"))
    {
        translate_FunDec(node->lchild->rsibling);
        translate_CompSt(node->lchild->rsibling->rsibling);
    }
}

void translate_FunDec(Tnode *node)
{
    // 生成操作符
    Operand op = (Operand)malloc(sizeof(Operand_));
    op->kind = OP_VARIABLE;
    strcpy(op->u.value, node->lchild->value);

    // 生成中间代码
    InterCodes ir = (InterCodes)malloc(sizeof(InterCodes_));
    ir->code.kind = IR_FUNCTION;
    ir->code.u.one.op = op;
    insertList(IR_Head, ir);
    if (!strcmp(node->lchild->rsibling->rsibling->name, "VarList"))
    {
        translate_VarList(node->lchild->rsibling->rsibling);
    }
}

void translate_VarList(Tnode *node)
{

}

void translate_CompSt(Tnode *node)
{

}

/* Exp */
void translate_Exp(Tnode *node)
{
    if (!strcmp(node->lchild->name, "INT"))
    {
        
    }
    else if (!strcmp(node->lchild->name, "ID"))
    {
        
    }
}