#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Table_Size 0x3fff
enum BASIC_TYPE { BASIC_INT, BASIC_FLOAT };
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct HashNode_* HashNode;
typedef struct symbol_Table_* symbol_Table;

extern symbol_Table SymbolTable;

/* 定义类型结构 */
typedef struct Type_
{
    enum { BASIC, ARRAY, STRUCTURE, FUNCTION } kind;
    union
    {
        int basic;  // 基本类型
        struct { Type elem; int size; } array;  // 数组类型包括元素类型与数组大小
        FieldList structure;  // 结构体类型是一个链表
        struct { int argc; FieldList argv; Type rtnType; } function;  // 函数类型(参数个数、参数类型、返回类型)
    } u;
} Type_;

typedef struct FieldList_
{
    char* name;  // 域的名字
    Type type;   // 域的类型
    FieldList tail;  // 下一个域
} FieldList_;

/* 哈希表结点 */
typedef struct HashNode_
{
    char *name;  // 结点名字
    Type type;   // 结点类型
    HashNode next;   // 发生冲突时，指向下一个结点
} HashNode_;

/* 符号表 */
typedef struct symbol_Table_
{
    HashNode HashT[Table_Size];
} symbol_Table_;

/* 哈希相关函数 */
unsigned int hash_pjw(char *name);
void initHashT(symbol_Table Table);
HashNode createHnode(char *name, Type type);
void insertHnode(symbol_Table Table, HashNode node);
void printHashT(symbol_Table Table);

#endif
