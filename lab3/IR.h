#ifndef __IR_H
#define __IR_H

typedef Operand_* Operand;

typedef struct Operand_ 
{
    enum { VARIABLE, CONSTANT, ADDRESS } kind;
    union {
        int var_no;
        int value;
    } u;
} Operand_;

struct InterCode
{
    enum { ASSIGN, ADD, SUB, MUL } kind;
    union {
        struct { Operand right, left; } assign;
        struct { Operand result, op1, op2; } binop;
    } u;
};

#endif