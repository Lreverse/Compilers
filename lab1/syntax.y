%locations
%{
    #include "lex.yy.c"
    #include "Tree.h"
    void yyerror(char* msg);
    Tnode *root = NULL;  // 语法树的根节点
    Error_flag = 0;  // 判断源文件是否出现语法错误
%}

%union {
  int type_int;
  float type_float;
  double type_double;
  Tnode* type_Tnode;
}

%token <type_Tnode> TYPE
%token <type_Tnode> STRUCT
%token <type_Tnode> RETURN
%token <type_Tnode> IF ELSE
%token <type_Tnode> WHILE
%token <type_Tnode> INT
%token <type_Tnode> FLOAT
%token <type_Tnode> ID
%token <type_Tnode> SEMI COMMA
%token <type_Tnode> ASSIGNOP RELOP
%token <type_Tnode> PLUS MINUS STAR DIV
%token <type_Tnode> AND OR NOT
%token <type_Tnode> DOT
%token <type_Tnode> LP RP LB RB LC RC

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT


%%
/* High-level Definitions */
Program : ExtDefList  { $$=creatTnode(1, "Program", ""); appendTnode($$, 1, $1); root=$$}
  ;
ExtDefList : ExtDef ExtDefList  { $$=creatTnode(1, "ExtDefList", ""); appendTnode($$, 2, $1, $2); }
  |  { $$=NULL; }
  ;
ExtDef : Specifier ExtDecList SEMI  { $$=creatTnode(1, "ExtDef", ""); appendTnode($$, 3, $1, $2, $3); }
  | Specifier SEMI  { $$=creatTnode(1, "ExtDefList", ""); appendTnode($$, 2, $1, $2); }
  | Specifier FunDec CompSt  { $$=creatTnode(1, "ExtDefList", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
ExtDecList : VarDec  { $$=creatTnode(1, "ExtDecList", ""); appendTnode($$, 1, $1); }
  | VarDec COMMA ExtDecList  { $$=creatTnode(1, "ExtDecList", ""); appendTnode($$, 3, $1, $2, $3); }
  ;

/* Specifiers */
Specifier : TYPE  { $$=creatTnode(1, "Specifier", ""); appendTnode($$, 1, $1); }
  | StructSpecifier  { $$=creatTnode(1, "Specifier", ""); appendTnode($$, 1, $1); }
  ;
StructSpecifier : STRUCT OptTag LC DefList RC  { $$=creatTnode(1, "StructSpecifier", ""); appendTnode($$, 5, $1, $2, $3, $4, $5); }
  | STRUCT Tag  { $$=creatTnode(1, "StructSpecifier", ""); appendTnode($$, 2, $1, $2); }
  ;
OptTag : ID  { $$=creatTnode(1, "OptTag", ""); appendTnode($$, 1, $1); }
  |  { $$=NULL; }
  ;
Tag : ID  { $$=creatTnode(1, "Tag", ""); appendTnode($$, 1, $1); }
  ;

/* Declarators */
VarDec : ID  { $$=creatTnode(1, "VarDec", ""); appendTnode($$, 1, $1); }
  | VarDec LB INT RB  { $$=creatTnode(1, "VarDec", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  ;
FunDec : ID LP VarList RP  { $$=creatTnode(1, "FunDec", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  | ID LP RP  { $$=creatTnode(1, "FunDec", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
VarList : ParamDec COMMA VarList  { $$=creatTnode(1, "VarList", ""); appendTnode($$, 3, $1, $2, $3); }
  | ParamDec  { $$=creatTnode(1, "VarList", ""); appendTnode($$, 1, $1); }
  ;
ParamDec : Specifier VarDec  { $$=creatTnode(1, "ParamDec", ""); appendTnode($$, 2, $1, $2); }
  ;

/* Statements */
CompSt : LC DefList StmtList RC  { $$=creatTnode(1, "CompSt", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  ;
StmtList : Stmt StmtList  { $$=creatTnode(1, "StmtList", ""); appendTnode($$, 2, $1, $2); }
  |  { $$=NULL; }
  ;
Stmt : Exp SEMI  { $$=creatTnode(1, "Stmt", ""); appendTnode($$, 2, $1, $2); }
  | CompSt  { $$=creatTnode(1, "Stmt", ""); appendTnode($$, 1, $1); }
  | RETURN Exp SEMI  { $$=creatTnode(1, "Stmt", ""); appendTnode($$, 3, $1, $2, $3); }
  | IF LP Exp RP Stmt  %prec LOWER_THAN_ELSE  { $$=creatTnode(1, "Stmt", ""); appendTnode($$, 5, $1, $2, $3, $4, $5); }
  | IF LP Exp RP Stmt ELSE Stmt  { $$=creatTnode(1, "Stmt", ""); appendTnode($$, 7, $1, $2, $3, $4, $5, $6, $7); }
  | WHILE LP Exp RP Stmt  { $$=creatTnode(1, "Stmt", ""); appendTnode($$, 5, $1, $2, $3, $4, $5); }
  ;

/* Local Definitions */
DefList : Def DefList  { $$=creatTnode(1, "DefList", ""); appendTnode($$, 2, $1, $2); }
  |  { $$=NULL; }
  ;
Def : Specifier DecList SEMI  { $$=creatTnode(1, "Def", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
DecList : Dec  { $$=creatTnode(1, "DecList", ""); appendTnode($$, 1, $1); }
  | Dec COMMA DecList  { $$=creatTnode(1, "DecList", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
Dec : VarDec  { $$=creatTnode(1, "Dec", ""); appendTnode($$, 1, $1); }
  | VarDec ASSIGNOP Exp  { $$=creatTnode(1, "Dec", ""); appendTnode($$, 3, $1, $2, $3); }
  ;

/* Expressions */
Exp : Exp ASSIGNOP Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp AND Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp OR Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp RELOP Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp PLUS Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp MINUS Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp STAR Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp DIV Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | LP Exp RP  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | MINUS Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 2, $1, $2); }
  | NOT Exp  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 2, $1, $2); }
  | ID LP Args RP  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  | ID LP RP  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp LB Exp RB  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  | Exp DOT ID  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | ID  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 1, $1); }
  | INT  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 1, $1); }
  | FLOAT  { $$=creatTnode(1, "Exp", ""); appendTnode($$, 1, $1); }
  ;
Args : Exp COMMA Args  { $$=creatTnode(1, "Args", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp  { $$=creatTnode(1, "Args", ""); appendTnode($$, 1, $1); }
  ;

/* Comments */

/* error */
Stmt : error SEMI
  ;
CompSt : error RC
  ;
VarDec : error RB
  ;
FunDec : error RP
  ;
ExtDef : error SEMI
  ;

%%
void yyerror(char* msg)
{
    Error_flag = 1;
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}