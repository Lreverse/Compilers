#ifndef __IR_H
#define __IR_H

#include <stdio.h>
#include <string.h>
#include "Tree.h"
#include "symbol_table.h"

typedef Operand_* Operand;
typedef InterCode_* InterCode;
typedef InterCodes_* InterCodes;


typedef struct Operand_ 
{
    enum { OP_VARIABLE, OP_CONSTANT, OP_ADDRESS } kind;
    union {
        int var_no;
        char value[32];
    } u;
} Operand_;

typedef struct InterCode_
{
    enum { 
            IR_LABEL, IR_FUNCTION, IR_ASSIGN, 
            IR_ADD, IR_SUB, IR_MUL, IR_DIV 
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

void translate_Stmt();
void translate_Exp(Tnode *node);
void translate_CompSt();


#endif