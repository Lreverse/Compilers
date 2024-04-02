%option yylineno
%{
    #include <stdio.h>
%}
digit  [0-9]
letter [a-zA-Z]
relop  >|<|>=|<=|==|!=
decimal  0|[1-9]{digit}*
octal  0[0-7]+
hex  0[xX]([0-9a-f]+|[0-9A-F]+)
exponent_form  ({digit}*\.?{digit}+|{digit}+\.)[Ee][+-]?{digit}+

%%

int|float  { printf("TYPE\n"); }
struct  { printf("STRUCT\n"); }
return  { printf("RETURN\n"); }
if  { printf("IF\n"); }
else  { printf("ELSE\n"); }
while  { printf("WHILE\n"); }
{decimal}|{octal}|{hex} { printf("INT\n"); }
{digit}*\.{digit}+|{exponent_form}  { printf("FLOAT\n"); }
[_a-zA-Z][_a-zA-Z0-9]*  { printf("ID\n"); }
" "|\t  { ;}
;  { printf("SEMI\n"); }
,  { printf("COMMA\n"); }
=  { printf("ASSIGNOP\n"); }
{relop}  { printf("RELOP\n"); }
"+"  { printf("PLUS\n"); }
"-"  { printf("MINUS\n"); }
"*"  { printf("STAR\n"); }
"/"  { printf("DIV\n"); }
"&&"  { printf("AND\n"); }
"||"  { printf("OR\n"); }
"."  { printf("DOT\n"); }
!  { printf("NOT\n"); }
"("  { printf("LR\n"); }
")"  { printf("RP\n"); }
"["  { printf("LB\n"); }
"]"  { printf("RB\n"); }
"{"  { printf("LC\n"); }
"}"  { printf("RC\n"); }
.  {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}

%%
int main(int argc, char**argv) {
    if (argc > 1) {
        if ( !(yyin = fopen(argv[1], "r")) ) {
            perror(argv[1]);
            return 1;
        }
    }
    yylex();
    return 0;
}