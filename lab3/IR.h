#ifndef __IR_H
#define __IR_H

#include "symbol_table.h"
#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;


typedef struct Operand_ 
{
    enum { OP_VARIABLE, OP_TEMP, OP_CONSTANT, OP_LABEL, OP_ADDRESS } kind;
    union {
        int no;
        char value[32];
    } u;
} Operand_;

typedef struct InterCode_
{
    enum { 
            IR_FUNCTION, IR_PARAM, IR_RETURN,
            IR_LABEL, IR_GOTO, 
            IR_ASSIGN, IR_ADD, IR_SUB, IR_MUL, IR_DIV 
        } kind;
    union {
        struct { Operand op; } one;   // 只有一个操作数
        struct { Operand op1, op2; } two;   // 有两个操作数
        struct { Operand result, op1, op2; } three;
        struct { Operand left, right; } assign;
    } u;
} InterCode_;

/* 双向链表 */
typedef struct InterCodes_ {
    InterCode_ code;
    struct InterCodes_ *prev, *next;
} InterCodes_;

/* 链表操作 */
void initList(InterCodes head);
void insertList(InterCodes head, InterCodes node);

/* 翻译函数 */
void print_OP(Operand op);
void print_IR(InterCodes IR);
void translate_Program(Tnode *node);
void translate_ExtDefList(Tnode *node);
void translate_ExtDef(Tnode *node);
void translate_FunDec(Tnode *node);
void translate_VarList(Tnode *node);
void translate_ParamDec(Tnode *node);
void translate_CompSt(Tnode *node);
void translate_StmtList(Tnode *node);
void translate_Stmt(Tnode *node);
void translate_Exp(Tnode *node, Operand place);
void translate_Cond(Tnode *node, Operand label1, Operand label2);

Operand new_var(void);
Operand new_temp(void);
Operand new_label(void);

#endif