%{
    #include "lex.yy.c"



%}

%token TYPE
%token STRUCT
%token RETURN
%token IF
%token ELSE
%token WHILE
%token INT
%token FLOAT
%token ID
%token SEMI
%token COMMA
%token ASSIGNOP
%token RELOP
%token PLUS
%token MINUS
%token STAR
%token DIV
%token AND
%token OR
%token DOT
%token NOT
%token LR
%token RP
%token LB
%token RB
%token LC
%token RC


%%
Program : ExtDefList
  ;
ExtDefList : ExtDef ExtDefList
  |
  ;
ExtDef : Specifier ExtDecList SEMI
  | Specifier SEMI
  | Specifier FunDec CompSt
  ;
ExtDecList : VarDec
  | VarDec COMMA ExtDecList
  ;


