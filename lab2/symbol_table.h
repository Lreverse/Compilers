#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#define Table_Size 0x3fff

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct HashNode_* HashNode;
typedef struct symbol_Table_* symbol_Table;

/* 定义类型结构 */
struct Type_
{
    enum { BASIC, ARRAY, STRUCTURE, FUNCTION } kind;
    union
    {
        int basic;  // 基本类型
        struct { Type elem; int size; } array;  // 数组类型包括元素类型与数组大小
        FieldList structure;  // 结构体类型是一个链表
        struct { int argc; FieldList argv; Type rtnType } function;  // 函数类型(参数个数、参数类型、返回类型)
    } u;
};

struct FieldList_
{
    char* name;  // 域的名字
    Type type;   // 域的类型
    FieldList tail;  // 下一个域
};

/* 哈希表结点 */
struct HashNode_
{
    char *name;  // 结点名字
    Type type;   // 结点类型
    HashNode next;   // 发生冲突时，指向下一个结点
};

/* 符号表 */
struct symbol_Table_
{
    HashNode HashT[Table_Size];
};

/* 哈希相关函数 */
unsigned int hash_pjw(char *name);
void InitHashT(symbol_Table Table);
HashNode CreateNode(char *name, Type type);
void InsertNode(symbol_Table Table, HashNode node);

#endif
