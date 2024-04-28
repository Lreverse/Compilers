#include "symbol_table.h"

symbol_Table SymbolTable = NULL;

/* 哈希函数 */
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

/* 初始化哈希表 */
void initHashT(symbol_Table Table)
{
    for(int i = 0; i < Table_Size; i++)
    {
        Table->HashT[i] = NULL;
    }
}

/*
 *  flag：表示参数name是变量名还是函数名
 * 
 *  该函数既可以用来检查是否重复定义，又可以检查使用之前是否已经定义
 *  1. 重复定义：返回0代表该符号重复定义
 *  2. 使用定义：返回0代表该符号已经定义
 *  ！判断错误时要注意返回值
 */
int check(symbol_Table Table, char *name, enum VarDec_flag flag)
{
    unsigned int val = hash_pjw(name);
    HashNode p = Table->HashT[val];
    while(p)
    {
        if(flag == VARIABLE)
        {
            if(!strcmp(p->name, name) && p->type->kind != FUNCTION)
            {
                return 0;
            }
        }
        else if (flag == PARAMETER)
        {
            if(!strcmp(p->name, name) && p->type->kind == FUNCTION)
            {
                return 0;
            }
        }
        p = p->next;
    }
    return 1;
}

/* 创建结点 */
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

/* 插入结点 */
void insertHnode(symbol_Table Table, HashNode node)
{
    unsigned int val = hash_pjw(node->name);
    // 采用头插法
    node->next = Table->HashT[val];
    Table->HashT[val] = node;
}

/* 打印符号表 */
void printHashT(symbol_Table Table)
{
    HashNode p = NULL;
    int j = 1;
    printf("\nsymbol table:\n");
    for(int i = 0; i < Table_Size; i++)
    {
        p = Table->HashT[i];
        while(p)
        {
            int kind = p->type->kind;
            printf("%d:  %s  kind(%d)  ", j, p->name, kind);

            // 类型0：basic
            if (kind == 0)
                printf("basic=%d\n", p->type->u.basic);
            // 类型1：array
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
            // 类型3：function
            else if (kind == 3)
            {
                int argc = p->type->u.function.argc;
                FieldList argv = p->type->u.function.argv;
                printf("%s(", p->name);
                while(argv)
                {
                    // 判断参数类型
                    int param_kind = argv->type->kind;
                    // 形参为basic
                    if (param_kind == 0)
                    {
                        if (argv->type->u.basic == BASIC_INT)
                            printf("int ");
                        else if (argv->type->u.basic == BASIC_FLOAT)
                            printf("float ");
                        printf("%s", argv->name); 
                    }
                    // 形参为array
                    else if (param_kind == 1)
                    {
                        int temp[32] = {0};
                        Type elem = argv->type->u.array.elem;
                        temp[0] = argv->type->u.array.size;   
                        // temp用来保存数组每个维度的值，因为类型在链表末尾，为确保输出顺序，需要先存储维度，最后再打印
                        // printf("[%d]", argv->type->u.array.size);
                        int i = 1;
                        while (elem)
                        {
                            if (elem->kind == 0)
                            {
                                if (elem->u.basic == BASIC_INT)
                                    printf("int ");
                                else if (elem->u.basic == BASIC_FLOAT)
                                    printf("float ");
                                break;
                            }
                            else if (elem->kind == 1)
                            {
                                // printf("[%d]", elem->u.array.size);
                                temp[i] = elem->u.array.size;
                                i++;
                                elem = elem->u.array.elem;
                            }
                        }
                        printf("%s", argv->name); 
                        for (int j = 0; j < i; j++)
                            printf("[%d]", temp[j]);
                    }
                    argv = argv->tail;
                    if (argv)
                        printf(", ");
                }
                printf(")\n");
            }
            p = p->next;
            j++;
        }
    }
}