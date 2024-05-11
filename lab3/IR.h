#ifndef __IR_H
#define __IR_H

#include "symbol_table.h"
#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;
typedef struct Var_node_* Var_node;
typedef struct arg_list_node_* arg_list_node;

typedef struct Operand_ 
{
    enum { OP_VARIABLE, OP_TEMP, OP_CONSTANT, OP_LABEL, OP_RELOP, OP_ADDRESS, OP_FUNCTION } kind;
    union {
        int no;
        char value[32];
    } u;
} Operand_;

typedef struct InterCode_
{
    enum IR_TYPE { 
            IR_FUNCTION, IR_PARAM, IR_RETURN,
            IR_LABEL, IR_GOTO, IR_IF, 
            IR_ASSIGN, IR_ADD, IR_SUB, IR_MUL, IR_DIV,
            IR_READ, IR_WRITE, 
            IR_FUNC_CALL, IR_ARG
        } kind;
    union {
        struct { Operand op; } one;   // 只有一个操作数
        struct { Operand op1, op2; } two;   // 有两个操作数
        struct { Operand op1, op2, op3; } three;
        struct { Operand op1, op2, op3, op4; } four;
        // struct { Operand left, right; } assign;
    } u;
} InterCode_;

/* 双向链表 */
typedef struct InterCodes_ {
    InterCode_ code;
    struct InterCodes_ *prev, *next;
} InterCodes_;

typedef struct Var_node_{
    Operand op;
    char *name;
    struct Var_node_ *next;
} Var_node_;

typedef struct arg_list_node_{
    Operand op;
    struct arg_list_node_ *next;
} arg_list_node_;

/* 链表操作 */
void initList(InterCodes head);
void insertList(InterCodes head, InterCodes node);

void insertVarMap(Var_node head, Operand op, char *name);
Operand get_op_from_var_map(Var_node head, char *name);

/* 翻译函数 */
void print_OP(Operand op);
void print_IR(InterCodes IR);
void translate_Program(Tnode *node);
void translate_ExtDefList(Tnode *node);
void translate_ExtDef(Tnode *node);
void translate_FunDec(Tnode *node);
void translate_VarList(Tnode *node);
void translate_ParamDec(Tnode *node);
void translate_VarDec(Tnode *node, Operand op);
void translate_CompSt(Tnode *node);
void translate_DefList(Tnode *node);
void translate_Def(Tnode *node);
void translate_DecList(Tnode *node);
void translate_Dec(Tnode *node);
void translate_StmtList(Tnode *node);
void translate_Stmt(Tnode *node);
void translate_Exp(Tnode *node, Operand place);
void translate_Cond(Tnode *node, Operand label1, Operand label2);
void translate_Args(Tnode *node, arg_list_node arg_list);

Operand new_var(void);
Operand new_temp(void);
Operand new_label(void);
Operand new_constant(char *value);
Operand new_relop(char *value);

InterCodes new_IR(enum IR_TYPE kind, int num, ...);

#endif