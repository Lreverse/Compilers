#include "IR.h"

/* 打印操作数 */
void print_OP(Operand op)
{

}

/* 打印中间代码 */
void print_IR(InterCodes IR)
{
    while(IR)
    {
        switch(IR->code->kind)
        {
            case IR_LABEL:
                printf("LABEL %s :\n", IR->code.u.one.op.value);
                break;
            case IR_FUNCTION:
                printf("FUNCTION %s :\n", IR->code.u.one.op.value);
                break;
            case IR_ASSIGN:
                print_OP(IR->code.u.assign.left);
                printf(" := ");
                print_OP(IR->code.assign.right);
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
        IR = IR->next;
    }
}


void translate_Exp(Tnode *node)
{
    if (!strcmp(node->lchild->name, "INT"))
    {
        
    }
    else if (!strcmp(node->lchild->name, "ID"))
    {
        
    }
}


