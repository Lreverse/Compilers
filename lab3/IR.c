#include "IR.h"

int var_no = 1;              // 变量标号
int temp_no = 1;             // 临时变量标号
int label_no = 1;            // label标号
InterCodes IR_Head = NULL;   // 链表头指针，头节点在主函数生成
Var_node var_map = NULL;     // 变量映射表头指针，头节点在主函数生成


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

/* 存储变量映射结点 */
void insertVarMap(Var_node head, Operand op, char *value)
{
    Var_node node = (Var_node)malloc(sizeof(Var_node_));
    node->next = NULL;
    node->op = op;
    node->name = value;

    // 头插法
    node->next = head->next;
    head->next = node;
}

/* 根据ID名字获取变量op */
Operand get_op_from_var_map(Var_node head, char *name)
{
    Var_node p = head->next;   // 跳过第一个头结点
    while(p)
    {
        if (!strcmp(p->name, name))
        {
            return p->op;
        }
        p = p->next;
    }
    return NULL;
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
        case OP_FUNCTION:
            printf("%s", op->u.value);
            break;
        case OP_ADDRESS:
            if (op->u.other.type == ADDRESS_VAR)
                printf("&v%d", op->u.other.no);
            else
                printf("&t%d", op->u.other.no);
            break;
        case OP_REFER:
            if (op->u.other.type == ADDRESS_VAR)
                printf("*v%d", op->u.other.no);
            else
                printf("*t%d", op->u.other.no);
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
                printf("FUNCTION ");
                print_OP(IR->code.u.one.op);
                printf(" :\n");
                break;
            case IR_PARAM:
                printf("PARAM ");
                print_OP(IR->code.u.one.op);
                printf("\n");
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
            case IR_ASSIGN:
                print_OP(IR->code.u.two.op1);
                printf(" := ");
                print_OP(IR->code.u.two.op2);
                printf("\n");
                break;
            case IR_ADD:
                print_OP(IR->code.u.three.op1);
                printf(" := ");
                print_OP(IR->code.u.three.op2);
                printf(" + ");
                print_OP(IR->code.u.three.op3);
                printf("\n");
                break;
            case IR_SUB:
                print_OP(IR->code.u.three.op1);
                printf(" := ");
                print_OP(IR->code.u.three.op2);
                printf(" - ");
                print_OP(IR->code.u.three.op3);
                printf("\n");
                break;
            case IR_MUL:
                print_OP(IR->code.u.three.op1);
                printf(" := ");
                print_OP(IR->code.u.three.op2);
                printf(" * ");
                print_OP(IR->code.u.three.op3);
                printf("\n");
                break;
            case IR_DIV:
                print_OP(IR->code.u.three.op1);
                printf(" := ");
                print_OP(IR->code.u.three.op2);
                printf(" / ");
                print_OP(IR->code.u.three.op3);
                printf("\n");
                break;
            case IR_READ:
                printf("READ ");
                print_OP(IR->code.u.one.op);
                printf("\n");
                break;
            case IR_WRITE:
                printf("WRITE ");
                print_OP(IR->code.u.one.op);
                printf("\n");
                break;
            case IR_FUNC_CALL:
                print_OP(IR->code.u.two.op1);
                printf(" := CALL ");
                print_OP(IR->code.u.two.op2);
                printf("\n");
                break;
            case IR_ARG:
                printf("ARG ");
                print_OP(IR->code.u.one.op);
                printf("\n");
                break;
            case IR_ARRAY:
                printf("DEC ");
                print_OP(IR->code.u.two.op1);
                printf(" %d\n", atoi(IR->code.u.two.op2->u.value) * 4);
                break;
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
    op->kind = OP_FUNCTION;
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

    translate_VarDec(node->lchild->rsibling, op);
}

/* VarDec */
void translate_VarDec(Tnode *node, Operand op)
{
    if (!strcmp(node->lchild->name, "ID"))
    {
        insertVarMap(var_map, op, node->lchild->value);
    }
    else   // 数组（只处理一维数组）
    {
        Operand num = new_constant(node->lchild->rsibling->rsibling->value);
        InterCodes ir = new_IR(IR_ARRAY, 2, op, num);
        insertList(IR_Head, ir);
        translate_VarDec(node->lchild, op);
    }
}

/* CompSt */
void translate_CompSt(Tnode *node)
{
    if (!strcmp(node->lchild->rsibling->name, "RC"))
        return;
    if (!strcmp(node->lchild->rsibling->rsibling->name, "RC"))
    {
        if (!strcmp(node->lchild->rsibling->name, "DefList"))
            translate_DefList(node->lchild->rsibling);
        else if (!strcmp(node->lchild->rsibling->name, "StmtList"))
            translate_StmtList(node->lchild->rsibling);
        return;
    }
    translate_DefList(node->lchild->rsibling);
    translate_StmtList(node->lchild->rsibling->rsibling);
    // 因为DefList和StmtList都可以为空产生式，所以如果这里不进行比较，会导致传入错误结点，导致后续分析出错，出现段错误
}

/* DefList */
void translate_DefList(Tnode *node)
{
    if (node == NULL)   // 空产生式
        return;
    translate_Def(node->lchild);
    translate_DefList(node->lchild->rsibling);
}

/* Def */
void translate_Def(Tnode *node)
{
    translate_DecList(node->lchild->rsibling);
}

/* DecList */
void translate_DecList(Tnode *node)
{
    translate_Dec(node->lchild);
    if (node->lchild->rsibling)
    {
        // 对于逗号，进行递归调用
        translate_DecList(node->lchild->rsibling->rsibling);
    }
}

/* Dec */
void translate_Dec(Tnode *node)
{
    Operand op = new_var();
    translate_VarDec(node->lchild, op);
    if (node->lchild->rsibling != NULL)
    {
        translate_Exp(node->lchild->rsibling->rsibling, op);
    }
}

/* StmtList */
void translate_StmtList(Tnode *node)
{
    if (node == NULL)  // 空产生式
        return;
    translate_Stmt(node->lchild);
    translate_StmtList(node->lchild->rsibling);
}

/* Stmt */
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
        Operand label1 = new_label(), label2 = new_label(), label3 = new_label();
        InterCodes ir1 = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir1);
        translate_Cond(node->lchild->rsibling->rsibling, label2, label3);  // code1
        InterCodes ir2 = new_IR(IR_LABEL, 1, label2);
        insertList(IR_Head, ir2);
        translate_Stmt(node->lchild->rsibling->rsibling->rsibling->rsibling);  // code2
        InterCodes ir3 = new_IR(IR_GOTO, 1, label1);
        insertList(IR_Head, ir3);
        InterCodes ir4 = new_IR(IR_LABEL, 1, label3);
        insertList(IR_Head, ir4);
    }
}

/* 判断条件，进行跳转 */
void translate_Cond(Tnode *node, Operand label_true, Operand label_false)
{
    if (node->lchild->rsibling != NULL)
    {
        if (!strcmp(node->lchild->rsibling->name, "RELOP"))
        {
            Operand t1 = new_temp(), t2 = new_temp();
            translate_Exp(node->lchild, t1);
            translate_Exp(node->lchild->rsibling->rsibling, t2);

            Operand op = new_relop(node->lchild->rsibling->value);
            InterCodes ir = new_IR(IR_IF, 4, t1, op, t2, label_true);
            insertList(IR_Head, ir);
            InterCodes ir_goto = new_IR(IR_GOTO, 1, label_false);
            insertList(IR_Head, ir_goto);
            return;
        }
        else if (!strcmp(node->lchild->name, "NOT"))
        {
            translate_Cond(node->lchild->rsibling, label_false, label_true);
            return;
        }
        else if (!strcmp(node->lchild->rsibling->name, "AND"))
        {
            Operand label1 = new_label();
            translate_Cond(node->lchild, label1, label_false);
            InterCodes ir = new_IR(IR_LABEL, 1, label1);
            insertList(IR_Head, ir);
            translate_Cond(node->lchild->rsibling->rsibling, label_true, label_false);
            return;
        }
        else if (!strcmp(node->lchild->rsibling->name, "OR"))
        {
            Operand label1 = new_label();
            translate_Cond(node->lchild, label_true, label1);
            InterCodes ir = new_IR(IR_LABEL, 1, label1);
            insertList(IR_Head, ir);
            translate_Cond(node->lchild->rsibling->rsibling, label_true, label_false);
            return;
        }
    }
    Operand t1 = new_temp();
    translate_Exp(node, t1);

    Operand op1 = new_relop("!=");       
    Operand op2 = new_constant("0");

    InterCodes ir_if = new_IR(IR_IF, 4, t1, op1, op2, label_true);
    insertList(IR_Head, ir_if);

    InterCodes ir_goto = new_IR(IR_GOTO, 1, label_false);
    insertList(IR_Head, ir_goto);
}

/* Exp */
void translate_Exp(Tnode *node, Operand place)
{
    if (!strcmp(node->lchild->name, "INT"))
    {
        Operand op = new_constant(node->lchild->value);
        InterCodes ir = new_IR(IR_ASSIGN, 2, place, op);
        insertList(IR_Head, ir);
    }
    else if (!strcmp(node->lchild->name, "FLOAT"))
    {
        printf("float is not permitted\n");
    }
    else if (!strcmp(node->lchild->name, "ID"))
    {
        if (node->lchild->rsibling == NULL)   // ID
        {
            Operand op = get_op_from_var_map(var_map, node->lchild->value);
            // if (op == NULL)
            // {
            //     printf("op is null\n");
            // }
            InterCodes ir = new_IR(IR_ASSIGN, 2, place, op);
            insertList(IR_Head, ir);
        }
        else if (node->lchild->rsibling->rsibling->rsibling == NULL)  // ID LP RP
        {
            if (!strcmp(node->lchild->value, "read"))
            {
                InterCodes ir = new_IR(IR_READ, 1, place);
                insertList(IR_Head, ir);
            }
            else
            {
                Operand op = (Operand)malloc(sizeof(Operand_));
                op->kind = OP_FUNCTION;
                strcpy(op->u.value, node->lchild->value);
                InterCodes ir = new_IR(IR_FUNC_CALL, 2, place, op);
                insertList(IR_Head, ir);
            }
        }
        else    // ID LP Args RP
        {
            arg_list_node arg_list = (arg_list_node)malloc(sizeof(arg_list_node_));
            arg_list->next = NULL;
            translate_Args(node->lchild->rsibling->rsibling, arg_list);
            if (!strcmp(node->lchild->value, "write"))
            {
                InterCodes ir_write = new_IR(IR_WRITE, 1, arg_list->next->op);
                insertList(IR_Head, ir_write);
                if (place != NULL)
                {
                    Operand op_0 = new_constant("0");
                    InterCodes ir_0 = new_IR(IR_ASSIGN, 2, place, op_0);
                    insertList(IR_Head, ir_0);
                }
            }
            else
            {
                arg_list_node p = arg_list->next;
                while (p)
                {
                    InterCodes ir = new_IR(IR_ARG, 1, p->op);
                    insertList(IR_Head, ir);
                    p = p->next;
                }
                Operand op = (Operand)malloc(sizeof(Operand_));
                op->kind = OP_FUNCTION;
                strcpy(op->u.value, node->lchild->value);
                if (place == NULL)
                {
                    place = new_temp();
                }
                InterCodes ir_call = new_IR(IR_FUNC_CALL, 2, place, op);
                insertList(IR_Head, ir_call);
            }
        }
    }
    else if (!strcmp(node->lchild->rsibling->name, "ASSIGNOP"))
    {
        if (!strcmp(node->lchild->lchild->name, "ID") && node->lchild->lchild->rsibling == NULL)  // 左值为单个变量的情况
        {
            Operand t1 = new_temp();
            Operand v = get_op_from_var_map(var_map, node->lchild->lchild->value);
            // if (v == NULL)
            // {
            //     printf("op is null\n");
            // }
            translate_Exp(node->lchild->rsibling->rsibling, t1);
            InterCodes ir = new_IR(IR_ASSIGN, 2, v, t1);
            insertList(IR_Head, ir);
            if (place != NULL)
            {
                InterCodes ir_option = new_IR(IR_ASSIGN, 2, place, v);
                insertList(IR_Head, ir_option);
            }
        }
        else if (!strcmp(node->lchild->lchild->rsibling->name, "LB"))  // 数组
        {
            Operand t1 = new_temp(), t2 = new_temp();
            translate_Exp(node->lchild, t1);
            translate_Exp(node->lchild->rsibling->rsibling, t2);

            Operand t_refer = (Operand)malloc(sizeof(Operand_));
            t_refer->kind = OP_REFER;
            t_refer->u.other.type = ADDRESS_TEMP;
            t_refer->u.other.no = t1->u.no;
            InterCodes ir = new_IR(IR_ASSIGN, 2, t_refer, t2);
            insertList(IR_Head, ir);
        }
        else // 结构体
        {
            printf("left value is not a single variable\n");
        }
    }
    else if (!strcmp(node->lchild->rsibling->name, "PLUS"))
    {
        Operand t1 = new_temp(), t2 = new_temp();
        translate_Exp(node->lchild, t1);
        translate_Exp(node->lchild->rsibling->rsibling, t2);
        InterCodes ir = new_IR(IR_ADD, 3, place, t1, t2);
        insertList(IR_Head, ir);
    }
    else if (!strcmp(node->lchild->rsibling->name, "MINUS"))
    {
        Operand t1 = new_temp(), t2 = new_temp();
        translate_Exp(node->lchild, t1);
        translate_Exp(node->lchild->rsibling->rsibling, t2);
        InterCodes ir = new_IR(IR_SUB, 3, place, t1, t2);
        insertList(IR_Head, ir);
    }
    else if (!strcmp(node->lchild->rsibling->name, "STAR"))
    {
        Operand t1 = new_temp(), t2 = new_temp();
        translate_Exp(node->lchild, t1);
        translate_Exp(node->lchild->rsibling->rsibling, t2);
        InterCodes ir = new_IR(IR_MUL, 3, place, t1, t2);
        insertList(IR_Head, ir);
    }
    else if (!strcmp(node->lchild->rsibling->name, "DIV"))
    {
        Operand t1 = new_temp(), t2 = new_temp();
        translate_Exp(node->lchild, t1);
        translate_Exp(node->lchild->rsibling->rsibling, t2);
        InterCodes ir = new_IR(IR_DIV, 3, place, t1, t2);
        insertList(IR_Head, ir);
    }
    else if (!strcmp(node->lchild->name, "MINUS"))
    {
        Operand t1 = new_temp();
        translate_Exp(node->lchild->rsibling, t1);
        if (place != NULL)
        {
            Operand t2 = new_constant("0");
            InterCodes ir = new_IR(IR_SUB, 3, place, t2, t1);
            insertList(IR_Head, ir);
        }
    }
    else if (!strcmp(node->lchild->rsibling->name, "RELOP"))
    {
        if (place == NULL)
            printf("Line %d: place is NULL\n", node->lineno);
        Operand label1 = new_label(), label2 = new_label();
        Operand t_0 = new_constant("0");
        Operand t_1 = new_constant("1");

        InterCodes ir = new_IR(IR_ASSIGN, 2, place, t_0);
        insertList(IR_Head, ir);
        translate_Cond(node, label1, label2);  // code1
        InterCodes ir1 = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir1);
        InterCodes ir2 = new_IR(IR_ASSIGN, 2, place, t_1);
        insertList(IR_Head, ir2);
        InterCodes ir3 = new_IR(IR_LABEL, 1, label2);
        insertList(IR_Head, ir3);
    }
    else if (!strcmp(node->lchild->rsibling->name, "AND"))
    {
        if (place == NULL)
            printf("Line %d: place is NULL\n", node->lineno);
        Operand label1 = new_label(), label2 = new_label();
        Operand t_0 = new_constant("0");
        Operand t_1 = new_constant("1");

        InterCodes ir = new_IR(IR_ASSIGN, 2, place, t_0);
        insertList(IR_Head, ir);
        translate_Cond(node, label1, label2);  // code1
        InterCodes ir1 = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir1);
        InterCodes ir2 = new_IR(IR_ASSIGN, 2, place, t_1);
        insertList(IR_Head, ir2);
        InterCodes ir3 = new_IR(IR_LABEL, 1, label2);
        insertList(IR_Head, ir3);
    }
    else if (!strcmp(node->lchild->rsibling->name, "OR"))
    {
        if (place == NULL)
            printf("Line %d: place is NULL\n", node->lineno);
        Operand label1 = new_label(), label2 = new_label();
        Operand t_0 = new_constant("0");
        Operand t_1 = new_constant("1");

        InterCodes ir = new_IR(IR_ASSIGN, 2, place, t_0);
        insertList(IR_Head, ir);
        translate_Cond(node, label1, label2);  // code1
        InterCodes ir1 = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir1);
        InterCodes ir2 = new_IR(IR_ASSIGN, 2, place, t_1);
        insertList(IR_Head, ir2);
        InterCodes ir3 = new_IR(IR_LABEL, 1, label2);
        insertList(IR_Head, ir3);
    }
    else if (!strcmp(node->lchild->name, "NOT"))
    {
        if (place == NULL)
            printf("Line %d: place is NULL\n", node->lineno);
        Operand label1 = new_label(), label2 = new_label();
        Operand t_0 = new_constant("0");
        Operand t_1 = new_constant("1");

        InterCodes ir = new_IR(IR_ASSIGN, 2, place, t_0);
        insertList(IR_Head, ir);
        translate_Cond(node, label1, label2);  // code1
        InterCodes ir1 = new_IR(IR_LABEL, 1, label1);
        insertList(IR_Head, ir1);
        InterCodes ir2 = new_IR(IR_ASSIGN, 2, place, t_1);
        insertList(IR_Head, ir2);
        InterCodes ir3 = new_IR(IR_LABEL, 1, label2);
        insertList(IR_Head, ir3);
    }
    else if (!strcmp(node->lchild->name, "LP"))
    {
        translate_Exp(node->lchild->rsibling, place);
    }
    else if (!strcmp(node->lchild->rsibling->name, "LB"))
    {
        if (!strcmp(node->lchild->lchild->name, "ID"))   // 一维数组
        {
            Operand t1 = new_temp(), t2 = new_temp();
            Operand con_4 = new_constant("4");
            translate_Exp(node->lchild->rsibling->rsibling, t2);
            InterCodes ir = new_IR(IR_MUL, 3, t1, t2, con_4);
            insertList(IR_Head, ir);

            Operand op = get_op_from_var_map(var_map, node->lchild->lchild->value);
            Operand t_address = (Operand)malloc(sizeof(Operand_));
            t_address->kind = OP_ADDRESS;
            t_address->u.other.type = ADDRESS_VAR;
            t_address->u.other.no = op->u.no;
            InterCodes ir_adrress = new_IR(IR_ADD, 3, place, t_address, t1);
            insertList(IR_Head, ir_adrress);
        }
    }
    else
    {
        printf("not done yet!\n");
    }
}

/* Args */
void translate_Args(Tnode *node, arg_list_node arg_list)
{
    // 头插法
    Operand t1 = new_temp();
    translate_Exp(node->lchild, t1);
    arg_list_node p = (arg_list_node)malloc(sizeof(arg_list_node_));
    p->op = t1;
    p->next = arg_list->next;
    arg_list->next = p;   
    if (node->lchild->rsibling != NULL)
    {
        translate_Args(node->lchild->rsibling->rsibling, arg_list);
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

/* 生成常数 */
Operand new_constant(char *value)
{
    Operand op = (Operand)malloc(sizeof(Operand_));
    op->kind = OP_CONSTANT;
    strcpy(op->u.value, value);
    return op;
}

/* 获取判断符号 */
Operand new_relop(char *value)
{
    Operand op = (Operand)malloc(sizeof(Operand_));
    op->kind = OP_RELOP;
    strcpy(op->u.value, value);
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