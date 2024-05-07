#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
#include "symbol_table.h"
#include "semantic.h"

Tnode *root = NULL; // 语法树的根节点
int Error_flag = 0; // 判断源文件是否出现语法错误

extern FILE *yyin;
extern int yyparse();
extern void yyrestart(FILE *);

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
        SymbolTable = (symbol_Table)malloc(sizeof(symbol_Table_));
        initHashT(SymbolTable);
        Program(root);
        printHashT(SymbolTable);
    }
        
    return 0;
}