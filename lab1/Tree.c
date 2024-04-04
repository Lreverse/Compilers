#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tree.h"


Tnode* creatTnode(int type, char* name, char* value)
{
    Tnode *p = (Tnode*)malloc(sizeof(Tnode));
    strcpy_s(p->name, sizeof(p->name), name);
    strcpy_s(p->value, sizeof(p->value), value);
    p->lineno = yylineno;
    p->type = type;
    p->lchild = NULL;
    p->rsibling = NULL;
    return p;
}


