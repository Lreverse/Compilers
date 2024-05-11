#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
#include "symbol_table.h"
#include "semantic.h"
#include "IR.h"

Tnode *root = NULL; // 语法树的根节点
int Error_flag = 0; // 判断源文件是否出现语法错误

extern FILE *yyin;
extern int yyparse();
extern void yyrestart(FILE *);
extern InterCodes IR_Head;
extern Var_node var_map;
void get_file_name(char *path, char *file_name);

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }
    yyrestart(yyin);
    // yydebug = 1;
    yyparse();
    if (!Error_flag)
    {
        printParseTree(root);
        printf("\n");

        // 生成符号表
        SymbolTable = (symbol_Table)malloc(sizeof(symbol_Table_));
        initHashT(SymbolTable);
        Program(root);
        printHashT(SymbolTable);
        printf("\n");

        // 生成中间代码
        IR_Head = (InterCodes)malloc(sizeof(InterCodes_));  // 生成头节点
        initList(IR_Head);

        var_map = (Var_node)malloc(sizeof(Var_node_));
        var_map->next = NULL;

        translate_Program(root);

        printf("Intermediate code:\n");
        char path[32] = "./ir/";
        char file_name[32];
        get_file_name(argv[1], file_name);
        strcat(path, file_name);
        strcat(path, ".ir");
        printf("stored in %s\n", path);
        freopen(path, "w", stdout);
        print_IR(IR_Head);
    }
        
    return 0;
}

void get_file_name(char *path, char *file_name)
{
    char *p = NULL, *pr = NULL;
    p = strrchr(path, '/');
    if (p == NULL)
        return;
    pr = strrchr(p+1, '.');
    if (pr != NULL)
        strncpy(file_name, p+1, strlen(p+1)-strlen(pr));
    else
        strcpy(file_name, p+1);
}