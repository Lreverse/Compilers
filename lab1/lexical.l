%option yylineno
%{
    #include "syntax.tab.h"

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

int|float  { return TYPE; }
struct  { return STRUCT; }
return  { return RETURN; }
if  { return IF; }
else  { return ELSE; }
while  { return WHILE; }
{decimal}|{octal}|{hex} { return INT; }
{digit}*\.{digit}+|{exponent_form}  { return FLOAT; }
[_a-zA-Z][_a-zA-Z0-9]*  { return ID; }
" "|\t  { ;}
;  { return SEMI; }
,  { return COMMA; }
=  { return ASSIGNOP; }
{relop}  { return RELOP; }
"+"  { return PLUS; }
"-"  { return MINUS; }
"*"  { return STAR; }
"/"  { return DIV; }
"&&"  { return AND; }
"||"  { return OR; }
"."  { return DOT; }
!  { return NOT; }
"("  { return LP; }
")"  { return RP; }
"["  { return LB; }
"]"  { return RB; }
"{"  { return LC; }
"}"  { return RC; }
.  {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
\n  { yycolumn = 1; }

%%