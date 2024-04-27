#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Tree.h"

Tnode *creatTnode(int type, char *name, char *value)
{
    Tnode *p = (Tnode *)malloc(sizeof(Tnode));
    memcpy(p->name, name, sizeof(p->name));
    memcpy(p->value, value, sizeof(p->value));
    // strcpy(p->name, name);
    // strcpy(p->value, value);
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
            parent->lineno = p->lineno;
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
    static int blank_2 = 0; // 需要缩进2个空格的个数
    int type = node->type;
    char *name = node->name;
    char *value = node->value;
    for (int i = 0; i < blank_2; i++) // 缩进（打印空格）
    {
        printf("  ");
    }

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
    blank_2++;
    Tnode *p = node->lchild;
    while (p != NULL)
    {
        printParseTree(p);
        p = p->rsibling;
    }
    blank_2--;
}