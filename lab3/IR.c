#include "IR.h"

int var_no = 1;              // 变量标号
int temp_no = 1;             // 临时变量标号
int label_no = 1;            // label标号
InterCodes IR_Head = NULL;   // 链表头节点


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
    node->next->prev = node;
    head->next = node;
}

/* 打印操作数 */
void print_OP(Operand op)
{
    switch(op->kind)
    {
        case OP_VARIABLE:
            printf("v%d", op->u.no);
            break;
        case OP_TEMP:
            printf("t%d", op->u.no);
            break;
        case OP_CONSTANT:
            printf("#%d", atoi(op->u.value));
            break;
        case OP_LABEL:
            printf("label%d", op->u.no);
            break;
        case OP_RELOP:
            printf("%s", op->u.value);
            break;
        default:
            printf("\nprint_OP: no match\n");
            break;
    }
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
            case IR_FUNCTION:
                printf("FUNCTION %s :\n", IR->code.u.one.op->u.value);
                break;
            case IR_PARAM:
                printf("PARAM v%d\n", IR->code.u.one.op->u.no);
                break;
            case IR_RETURN:
                printf("RETURN ");
                print_OP(IR->code.u.one.op);
                printf("\n");
                break;
            case IR_LABEL:
                printf("LABEL ");
                print_OP(IR->code.u.one.op);
                printf(" :\n");
                break;
            case IR_GOTO:
                printf("GOTO ");
                print_OP(IR->code.u.one.op);
                printf("\n");
                break;
            case IR_IF:
                printf("IF ");
                print_OP(IR->code.u.four.op1);
                printf(" ");
                print_OP(IR->code.u.four.op2);
                printf(" ");
                print_OP(IR->code.u.four.op3);
                printf(" GOTO ");
                print_OP(IR->code.u.four.op4);
                printf("\n");
                break;
            // case IR_ASSIGN:
            //     print_OP(IR->code.u.two.op1);
            //     printf(" := ");
            //     print_OP(IR->code.u.two.op2);
            //     printf("\n");
            //     break;
            // case IR_ADD:
            //     print_OP(IR->code.u.three.op1);
            //     printf(" := ");
            //     print_OP(IR->code.u.three.op2);
            //     printf(" + ");
            //     print_OP(IR->code.u.three.op3);
            //     printf("\n");
            //     break;
            // case IR_SUB:
            //     print_OP(IR->code.u.three.op1);
            //     printf(" := ");
            //     print_OP(IR->code.u.three.op2);
            //     printf(" - ");
            //     print_OP(IR->code.u.three.op3);
            //     printf("\n");
            //     break;
            // case IR_MUL:
            //     print_OP(IR->code.u.three.op1);
            //     printf(" := ");
            //     print_OP(IR->code.u.three.op2);
            //     printf(" * ");
            //     print_OP(IR->code.u.three.op3);
            //     printf("\n");
            //     break;
            // case IR_DIV:
            //     print_OP(IR->code.u.three.op1);
            //     printf(" := ");
            //     print_OP(IR->code.u.three.op2);
            //     printf(" / ");
            //     print_OP(IR->code.u.three.op3);
            //     printf("\n");
            //     break;
            default:
                printf("default\n");
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

/* ExtDef */
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

/* FunDec */
void translate_FunDec(Tnode *node)
{
    // 生成操作符
    Operand op = (Operand)malloc(sizeof(Operand_));
    op->kind = OP_VARIABLE;
    strcpy(op->u.value, node->lchild->value);

    // 生成中间代码
    InterCodes ir = new_IR(IR_FUNCTION, 1, op);
    insertList(IR_Head, ir);

    if (!strcmp(node->lchild->rsibling->rsibling->name, "VarList"))
    {
        translate_VarList(node->lchild->rsibling->rsibling);
    }
}

/* VarList */
void translate_VarList(Tnode *node)
{
    translate_ParamDec(node->lchild);
    if (node->lchild->rsibling)
        translate_VarList(node->lchild->rsibling->rsibling);
}

/* ParamDec */
void translate_ParamDec(Tnode *node)
{
    // 生成操作符
    Operand op = new_var();

    // 生成中间代码
    InterCodes ir = new_IR(IR_PARAM, 1, op);
    insertList(IR_Head, ir);
}

/* CompSt */
void translate_CompSt(Tnode *node)
{
    if (!strcmp(node->lchild->rsibling->name, "RC"))
        return;
    if (!strcmp(node->lchild->rsibling->rsibling->name, "RC"))
    {
        if (!strcmp(node->lchild->rsibling->name, "DefList")) {}
            // DefList(node->lchild->rsibling);
        else if (!strcmp(node->lchild->rsibling->name, "StmtList"))
            translate_StmtList(node->lchild->rsibling);
        return;
    }
    // DefList(node->lchild->rsibling);
    translate_StmtList(node->lchild->rsibling->rsibling);
    // 因为DefList和StmtList都可以为空产生式，所以如果这里不进行比较，会导致传入错误结点，导致后续分析出错，出现段错误
}

/* StmtList */
void translate_StmtList(Tnode *node)
{
    if (node == NULL)  // 空产生式
        return;
    translate_Stmt(node->lchild);
    translate_StmtList(node->lchild->rsibling);
}

void translate_Stmt(Tnode *node)
{
    if (!strcmp(node->lchild->name, "Exp"))
    {
        translate_Exp(node->lchild, NULL);
    }
    else if (!strcmp(node->lchild->name, "CompSt"))
    {
        translate_CompSt(node->lchild );
    }
    else if (!strcmp(node->lchild->name, "RETURN"))
    {
        Operand var_temp = new_temp();
        translate_Exp(node->lchild->rsibling, var_temp);
        
        InterCodes ir = new_IR(IR_RETURN, 1, var_temp);
        insertList(IR_Head, ir);
    }
    else if (!strcmp(node->lchild->name, "IF"))
    {
        Operand label1 = new_label(), label2 = new_label();

        translate_Cond(node->lchild->rsibling->rsibling, label1, label2);   // code1

        InterCodes ir1 = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir1);

        translate_Stmt(node->lchild->rsibling->rsibling->rsibling->rsibling); // code2
        InterCodes ir2 = new_IR(IR_LABEL, 1, label2);

        if (node->lchild->rsibling->rsibling->rsibling->rsibling->rsibling == NULL) 
        {
            insertList(IR_Head, ir2);
        }
        else   // 遇到"ELSE"
        {
            Operand label3 = new_label();

            InterCodes ir_goto = new_IR(IR_GOTO, 1, label3);
            insertList(IR_Head, ir_goto);
            insertList(IR_Head, ir2);

            translate_Stmt(node->lchild->rsibling->rsibling->rsibling->rsibling->rsibling->rsibling);  // code3

            InterCodes ir3 = new_IR(IR_LABEL, 1, label3);
            insertList(IR_Head, ir3);
        }
    }
    else if(!strcmp(node->lchild->name, "WHILE"))
    {

    }
}

/* 判断条件，进行跳转 */
void translate_Cond(Tnode *node, Operand label_true, Operand label_false)
{
    if (!strcmp(node->lchild->rsibling->name, "RELOP"))
    {
        Operand t1 = new_temp(), t2 = new_temp();
        translate_Exp(node->lchild, t1);
        translate_Exp(node->lchild->rsibling->rsibling, t2);

        Operand op = get_relop(node->lchild->rsibling);
        InterCodes ir = new_IR(IR_IF, 4, t1, op, t2, label_true);
        insertList(IR_Head, ir);
        InterCodes ir_goto = new_IR(IR_GOTO, 1, label_false);
        insertList(IR_Head, ir_goto);
    }
    else if (!strcmp(node->lchild->name, "NOT"))
    {
        translate_Cond(node->lchild->rsibling, label_false, label_true);
    }
    else if (!strcmp(node->lchild->rsibling->name, "AND"))
    {
        Operand label1 = new_label();
        translate_Cond(node->lchild, label1, label_false);
        InterCodes ir = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir);
        translate_Cond(node->lchild->rsibling->rsibling, label_true, label_false);
    }
    else if (!strcmp(node->lchild->rsibling->name, "OR"))
    {
        Operand label1 = new_label();
        translate_Cond(node->lchild, label_true, label1);
        InterCodes ir = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir);
        translate_Cond(node->lchild->rsibling->rsibling, label_true, label_false);
    }
    else
    {
        Operand t1 = new_temp();
        translate_Exp(node, t1);

        Operand op1 = (Operand)malloc(sizeof(Operand_));
        op1->kind = OP_RELOP;
        strcpy(op1->u.value, "!=");
        Operand op2 =(Operand)malloc(sizeof(Operand_));
        op2->kind = OP_CONSTANT;
        strcpy(op2->u.value, "0");

        InterCodes ir_if = new_IR(IR_IF, 4, t1, op1, op2, label_true);
        insertList(IR_Head, ir_if);

        InterCodes ir_goto = new_IR(IR_GOTO, 1, label_false);
        insertList(IR_Head, ir_goto);
    }
}

/* Exp */
void translate_Exp(Tnode *node, Operand place)
{
    if (!strcmp(node->lchild->name, "INT"))
    {
        
    }
    else if (!strcmp(node->lchild->name, "ID"))
    {
        
    }
}

/* 获取新的变量 */
Operand new_var(void)
{
    Operand var = (Operand)malloc(sizeof(Operand_));
    var->kind = OP_VARIABLE;
    var->u.no = var_no;
    var_no++;
    return var;
}

/* 获取新的临时变量 */
Operand new_temp(void)
{
    Operand temp = (Operand)malloc(sizeof(Operand_));
    temp->kind = OP_TEMP;
    temp->u.no = temp_no;
    temp_no++;
    return temp;
}

/* 获取新的标签号 */
Operand new_label(void)
{
    Operand label = (Operand)malloc(sizeof(Operand_));
    label->kind = OP_LABEL;
    label->u.no = label_no;
    label_no++;
    return label;
}

/* 获取判断符号 */
Operand get_relop(Tnode *node)
{
    Operand op = (Operand)malloc(sizeof(Operand_));
    op->kind = OP_RELOP;
    strcpy(op->u.value, node->value);
    return op;
}

/* 生成新的IR */
InterCodes new_IR(enum IR_TYPE kind, int num, ...)
{
    InterCodes ir = (InterCodes)malloc(sizeof(InterCodes_));
    ir->next = ir->prev = NULL;
    ir->code.kind = kind;

    va_list args;
    va_start(args, num);
    switch(num)
    {
        case 1:
            ir->code.u.one.op = va_arg(args, Operand);
            break;
        case 2:
            ir->code.u.two.op1 = va_arg(args, Operand);
            ir->code.u.two.op2 = va_arg(args, Operand);
            break;
        case 3:
            ir->code.u.three.op1 = va_arg(args, Operand);
            ir->code.u.three.op2 = va_arg(args, Operand);
            ir->code.u.three.op3 = va_arg(args, Operand);
            break;
        case 4:
            ir->code.u.four.op1 = va_arg(args, Operand);
            ir->code.u.four.op2 = va_arg(args, Operand);
            ir->code.u.four.op3 = va_arg(args, Operand);
            ir->code.u.four.op4 = va_arg(args, Operand);
            break;
        default:
            printf("new_IR: not match\n");
            break;
    }
    va_end(args);
}