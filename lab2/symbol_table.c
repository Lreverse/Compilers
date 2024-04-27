#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"


unsigned int hash_pjw(char *name)
{
    unsigned int val = 0, i;
    for(; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & Table_Size)
            val = (val ^ (i >> 12)) & Table_Size;
    }
    return val;
}

void InitHashT(symbol_Table Table)
{
    for(int i = 0; i < Table_Size; i++)
    {
        Table->HashT[i] = NULL;
    }
}

HashNode CreateNode(char *name, Type type)
{
    HashNode p = (HashNode)malloc(sizeof(struct HashNode_));
    if(p == NULL)
    {
        printf("Not enough space to allocate memory!\n");
        exit(0);
    }
    strcpy(p->name, name);
    p->type = type;
    p->next = NULL;
    return p;
}

void InsertNode(symbol_Table Table, HashNode node)
{
    unsigned int val = hash_pjw(node->name);
    // 采用头插法
    node->next = Table->HashT[val];
    Table->HashT[val] = node;
}