#include <stdio.h>
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

void Insert(HashNode hashT, char *name)
{
    unsigned int val = hash_pjw(name);
    
}