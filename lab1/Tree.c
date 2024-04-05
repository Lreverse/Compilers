#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Tree.h"

Tnode *creatTnode(int type, char *name, char *value)
{
    Tnode *p = (Tnode *)malloc(sizeof(Tnode));
    // strcpy_s(p->name, sizeof(p->name), name);
    // strcpy_s(p->value, sizeof(p->value), value);
    strcpy(p->name, name);
    strcpy(p->value, value);
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

void printParseTree(Tnode *node)
{
    if (node == NULL)
        return;
    int type = node->type;
    char *name = node->name;
    char *value = node->value;
    if (type) // 语法单元
    {
        printf("%s (%d)\n", name, node->lineno);
    }
    else // 词法单元
    {
        if (strcmp(name, "ID") && strcmp(name, "TYPE") && strcmp(name, "INT") && strcmp(name, "FLOAT"))
            printf("%s\n", name);
        else
            printf("%s: %s\n", name, value);
    }

    // 先序遍历
    Tnode *p = node->lchild;
    while (p != NULL)
    {
        printParseTree(p);
        p = p->rsibling;
    }
    
}

/*
 1  Program (1)
 2    ExtDefList (1)
 3      ExtDef (1)
 4        Specifier (1)
 5          TYPE: int
 6        FunDec (1) 
 7          ID: inc 
 8          LP 
 9          RP 
10        CompSt (2) 
11          LC 
12          DefList (3) 
13            Def (3) 
14              Specifier (3) 
15                TYPE: int 
16              DecList (3) 
17                Dec (3) 
18                  VarDec (3) 
19                    ID: i 
20              SEMI 
21          StmtList (4) 
22            Stmt (4) 
23              Exp (4) 
24                Exp (4) 
25                  ID: i 
26                ASSIGNOP 
27                Exp (4) 
28                  Exp (4) 
29                    ID: i 
30                  PLUS 
31                  Exp (4) 
32                    INT: 1 
33              SEMI 
34          RC
*/