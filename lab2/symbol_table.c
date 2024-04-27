#include "symbol_table.h"

symbol_Table SymbolTable = NULL;

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

void initHashT(symbol_Table Table)
{
    for(int i = 0; i < Table_Size; i++)
    {
        Table->HashT[i] = NULL;
    }
}

HashNode createHnode(char *name, Type type)
{
    HashNode p = (HashNode)malloc(sizeof(HashNode_));
    if(p == NULL)
    {
        printf("Not enough space to allocate memory!\n");
        exit(0);
    }
    p->name = name;
    p->type = type;
    p->next = NULL;
    return p;
}

void insertHnode(symbol_Table Table, HashNode node)
{
    unsigned int val = hash_pjw(node->name);
    // 采用头插法
    node->next = Table->HashT[val];
    Table->HashT[val] = node;
}

void printHashT(symbol_Table Table)
{
    HashNode p = NULL;
    int j = 1;
    for(int i = 0; i < Table_Size; i++)
    {
        p = Table->HashT[i];
        while(p)
        {
            int kind = p->type->kind;
            printf("%d:  %s  kind(%d)  ", j, p->name, kind);
            if (kind == 0)
                printf("basic=%d\n", p->type->u.basic);
            else if (kind == 1)
            {
                Type elem = p->type->u.array.elem;
                printf("array=[%d]", p->type->u.array.size);
                while (elem && elem->kind == 1)
                {
                    printf("[%d]", elem->u.array.size);
                    elem = elem->u.array.elem;
                }
                printf("\n");
            }
            p = p->next;
            j++;
        }
    }
}