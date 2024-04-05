
### 词法分析

主要任务：将输入文件中的字符流组织成为词法单元流，在某些字符不符合程序设计语言词法规范时它也要有能力报告相应的错误

#### GNU Flex

基于Unix环境的词法分析程序生成工具，由Vern Paxson使用C语言重写Lex得到的。

#### 常用函数(或变量)

|函数名(或变量名)|说明|
|:--:|:--:|
|`yylex()`|执行词法分析|
|`yystart()`|重新开始词法分析器的输入|
|`yytext`|`char*`，保存了当前词法单元所对应的词素|
|`yylval`|表示当前词法单元所对应的属性值，类型为`YYSTYPE`，可通过`.y`文件中`%union`修改|
|`yylloc`|表示当前词法单元所对应的位置信息|


#### 编写Flex源代码

一般放在`.l`后缀的文件

```lexical
{definitions}
%%
{rules}
%%
{user subroutines}
```

示例：文字统计程序
```
%{
    #include <stdio.h>
    int chars = 0;
    int words = 0;
    int lines = 0;
%}
letter [a-zA-Z]
%%
{letter}+ { words++; chars += yyleng; }
\n { chars++; lines++; }
. { chars++; }
%%
int main(int argc, char**argv) {
    if (argc > 1) {
        if ( !(yyin = fopen(argv[1], "r")) ) {
            perror(argv[1]);
            return 1;
        }
    }
    yylex();
    printf("%8d%8d%8d\n", lines, words, chars);
    return 0;
}
```

#### 生成词法分析器

- 使用Flex对源代码进行编译
```bash
flex lexical.l
```
编译好的结果会保存在当前目录下的`lex.yy.c`文件中

- 使用gcc编译程序
```bash
gcc lex.yy.c -lfl -o scanner
```

- 运行程序

使用词法分析器scanner分析测试文件`test.cmm`
```bash
./scanner test.cmm
```



### 语法分析

主要任务：读入词法单元流，判断输入程序是否匹配程序设计语言的语法规范，并在匹配规范的情况下构建起输入程序的静态结构

#### GNU Bison

采用自底向上的LALR(1)分析技术，前身为基于Unix的Yacc。

#### 常用函数或变量

|函数名(或变量名)|说明|
|:--:|:--:|
|`yyparse()`|对输入文件进行语法分析，如果分析成功没有错误则返回0，否则返回非0|
|`yyerror()`|报告错误<br>（重新定义时，需重新声明）`void yyerror(char* msg);`|
|`yyerrok()`|恢复错误状态（该实验没有使用到）|


#### 编写Flex源代码

一般放在`.y`后缀的文件，编写框架与`.l`文件一致

```bison
{definitions}
%%
{rules}
%%
{user subroutines}
```

#### 生成语法分析器

- 使用Bison对源代码进行编译
```bash
bison -d syntax.y
```

- 修改Flex源代码`lexical.l`，然后重新编译
```bash
flex lexical.l
```

- 使用gcc编译
```bash
gcc main.c syntax.tab.c -lfl -ly -o parser
```

- 运行程序

使用语法分析器parser分析测试文件`test.cmm`
```bash
./parser test.cmm
```