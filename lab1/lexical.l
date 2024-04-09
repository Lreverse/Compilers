%option yylineno
%{
    #include "syntax.tab.h"
    #include "Tree.h"
    extern int Error_flag;

    int yycolumn = 1;
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}
digit  [0-9]
letter [a-zA-Z]
relop  >|<|>=|<=|==|!=
decimal  0|[1-9]{digit}*
octal  0[0-7]+
hex  0[xX][0-9a-fA-F]+
exponent_form  ({digit}*\.?{digit}+|{digit}+\.)[Ee][+-]?{digit}+

%%
int|float  { yylval.type_Tnode = creatTnode(0, "TYPE", yytext); return TYPE; }
struct  { yylval.type_Tnode = creatTnode(0, "STRUCT", yytext); return STRUCT; }
return  { yylval.type_Tnode = creatTnode(0, "RETURN", yytext); return RETURN; }
if  { yylval.type_Tnode = creatTnode(0, "IF", yytext); return IF; }
else  { yylval.type_Tnode = creatTnode(0, "ELSE", yytext); return ELSE; }
while  { yylval.type_Tnode = creatTnode(0, "WHILE", yytext); return WHILE; }
{decimal}|{octal}|{hex} {
    char str[32] = "";
    int num;
    if (yytext[0] == '0' && yytext[1] != '\0') {
        if (yytext[1] == 'x' | yytext[1] == 'X')
            sscanf(yytext, "%x", &num);   // 16进制字符串转成10进制数
        else
            sscanf(yytext, "%o", &num);   // 8进制字符串转成10进制数
        sprintf(str, "%d", num);
        yylval.type_Tnode = creatTnode(0, "INT", str);
    }
    else
        yylval.type_Tnode = creatTnode(0, "INT", yytext);
    return INT; 
}
{digit}+\.{digit}+|{exponent_form}  { 
    char str[32] = "";
    double num;
    sscanf(yytext, "%lf", &num);    // 以double的格式输入
    sprintf(str, "%lf", num);
    yylval.type_Tnode = creatTnode(0, "FLOAT", str); 
    return FLOAT; 
}
[_a-zA-Z][_a-zA-Z0-9]*  { yylval.type_Tnode = creatTnode(0, "ID", yytext); return ID; }
" "|\t  { ;}
;  { yylval.type_Tnode = creatTnode(0, "SEMI", yytext); return SEMI; }
,  { yylval.type_Tnode = creatTnode(0, "COMMA", yytext); return COMMA; }
=  { yylval.type_Tnode = creatTnode(0, "ASSIGNOP", yytext); return ASSIGNOP; }
{relop}  { yylval.type_Tnode = creatTnode(0, "RELOP", yytext); return RELOP; }
"+"  { yylval.type_Tnode = creatTnode(0, "PLUS", yytext); return PLUS; }
"-"  { yylval.type_Tnode = creatTnode(0, "MINUS", yytext); return MINUS; }
"*"  { yylval.type_Tnode = creatTnode(0, "STAR", yytext); return STAR; }
"/"  { yylval.type_Tnode = creatTnode(0, "DIV", yytext); return DIV; }
"&&"  { yylval.type_Tnode = creatTnode(0, "AND", yytext); return AND; }
"||"  { yylval.type_Tnode = creatTnode(0, "OR", yytext); return OR; }
"."  { yylval.type_Tnode = creatTnode(0, "DOT", yytext); return DOT; }
!  { yylval.type_Tnode = creatTnode(0, "NOT", yytext); return NOT; }
"("  { yylval.type_Tnode = creatTnode(0, "LP", yytext); return LP; }
")"  { yylval.type_Tnode = creatTnode(0, "RP", yytext); return RP; }
"["  { yylval.type_Tnode = creatTnode(0, "LB", yytext); return LB; }
"]"  { yylval.type_Tnode = creatTnode(0, "RB", yytext); return RB; }
"{"  { yylval.type_Tnode = creatTnode(0, "LC", yytext); return LC; }
"}"  { yylval.type_Tnode = creatTnode(0, "RC", yytext); return RC; }
.  {
    Error_flag = 1;
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
\n  { yycolumn = 1; }

%%
