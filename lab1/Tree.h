#ifndef __TREE_H
#define __TREE_H

extern int yylineno;

// 采用二叉树表示法（孩子兄弟表示法）
typedef struct Tnode{
    int lineno;  // 行号
    int type;    // 0表示词法单元，1表示语法单元
    char name[32];   // 节点名称
    char value[32];   // 词法单元对应的属性值
    struct Tnode *lchild;
    struct Tnode *rsibling;
} Tnode;

// 创建节点
Tnode* creatTnode(int type, char* name, char* value);


#endif