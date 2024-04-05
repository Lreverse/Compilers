#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Tree.h"

Tnode *creatTnode(int type, char *name, char *value)
{
    Tnode *p = (Tnode *)malloc(sizeof(Tnode));
    strcpy_s(p->name, sizeof(p->name), name);
    strcpy_s(p->value, sizeof(p->value), value);
    p->lineno = yylineno;
    p->type = type;
    p->lchild = NULL;
    p->rsibling = NULL;
    return p;
}

void appendTnode(Tnode *parent, int num, ...)
{
    va_list args;
    va_start(args, num);
    Tnode *pr = NULL;

    for (int i = 0; i < num; i++)
    {
        Tnode *p = va_arg(args, Tnode *);
        if (parent->lchild == NULL)
        {
            parent->lchild = p;
        }
        else
        {
            pr = parent->lchild;
            while (pr->rsibling != NULL)
                pr = pr->rsibling;
            pr->rsibling = p;
        }
    }
    va_end(args);
}

void printParseTree(Tnode *root)
{
    
}