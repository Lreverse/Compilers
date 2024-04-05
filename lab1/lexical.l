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
hex  0[xX]([0-9a-f]+|[0-9A-F]+)
exponent_form  ({digit}*\.?{digit}+|{digit}+\.)[Ee][+-]?{digit}+

%%
int|float  { yylval.type_Tnode = creatTnode(0, "TYPE", yytext); return TYPE; }
struct  { yylval.type_Tnode = creatTnode(0, "STRUCT", yytext); return STRUCT; }
return  { yylval.type_Tnode = creatTnode(0, "RETURN", yytext); return RETURN; }
if  { yylval.type_Tnode = creatTnode(0, "IF", yytext); return IF; }
else  { yylval.type_Tnode = creatTnode(0, "ELSE", yytext); return ELSE; }
while  { yylval.type_Tnode = creatTnode(0, "WHILE", yytext); return WHILE; }
{decimal}|{octal}|{hex} { yylval.type_Tnode = creatTnode(0, "INT", yytext); return INT; }
{digit}*\.{digit}+|{exponent_form}  { yylval.type_Tnode = creatTnode(0, "FLOAT", yytext); return FLOAT; }
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