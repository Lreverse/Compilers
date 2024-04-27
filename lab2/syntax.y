%locations
%{
    #include "lex.yy.c"
    #include "Tree.h"
    void yyerror(char* msg);
    extern Tnode* root;
    extern int Error_flag;
%}

%union {
    struct Tnode* type_Tnode;
}


/* 定义终结符和其对应的类型 */
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

/* 定义非终结符的类型 */
%type <type_Tnode> Program ExtDefList ExtDef ExtDecList
%type <type_Tnode> Specifier StructSpecifier OptTag Tag
%type <type_Tnode> VarDec FunDec VarList ParamDec
%type <type_Tnode> CompSt StmtList Stmt
%type <type_Tnode> DefList Def DecList Dec
%type <type_Tnode> Exp Args

/* 优先级和结合性 */
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
Program : ExtDefList  { $$=createTnode(1, "Program", ""); appendTnode($$, 1, $1); root=$$; }
  ;
ExtDefList : ExtDef ExtDefList  { $$=createTnode(1, "ExtDefList", ""); appendTnode($$, 2, $1, $2); }
  |  { $$=NULL; }
  ;
ExtDef : Specifier ExtDecList SEMI  { $$=createTnode(1, "ExtDef", ""); appendTnode($$, 3, $1, $2, $3); }
  | Specifier SEMI  { $$=createTnode(1, "ExtDef", ""); appendTnode($$, 2, $1, $2); }
  | Specifier FunDec CompSt  { $$=createTnode(1, "ExtDef", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
ExtDecList : VarDec  { $$=createTnode(1, "ExtDecList", ""); appendTnode($$, 1, $1); }
  | VarDec COMMA ExtDecList  { $$=createTnode(1, "ExtDecList", ""); appendTnode($$, 3, $1, $2, $3); }
  ;

/* Specifiers */
Specifier : TYPE  { $$=createTnode(1, "Specifier", ""); appendTnode($$, 1, $1); }
  | StructSpecifier  { $$=createTnode(1, "Specifier", ""); appendTnode($$, 1, $1); }
  ;
StructSpecifier : STRUCT OptTag LC DefList RC  { $$=createTnode(1, "StructSpecifier", ""); appendTnode($$, 5, $1, $2, $3, $4, $5); }
  | STRUCT Tag  { $$=createTnode(1, "StructSpecifier", ""); appendTnode($$, 2, $1, $2); }
  ;
OptTag : ID  { $$=createTnode(1, "OptTag", ""); appendTnode($$, 1, $1); }
  |  { $$=NULL; }
  ;
Tag : ID  { $$=createTnode(1, "Tag", ""); appendTnode($$, 1, $1); }
  ;

/* Declarators */
VarDec : ID  { $$=createTnode(1, "VarDec", ""); appendTnode($$, 1, $1); }
  | VarDec LB INT RB  { $$=createTnode(1, "VarDec", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  ;
FunDec : ID LP VarList RP  { $$=createTnode(1, "FunDec", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  | ID LP RP  { $$=createTnode(1, "FunDec", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
VarList : ParamDec COMMA VarList  { $$=createTnode(1, "VarList", ""); appendTnode($$, 3, $1, $2, $3); }
  | ParamDec  { $$=createTnode(1, "VarList", ""); appendTnode($$, 1, $1); }
  ;
ParamDec : Specifier VarDec  { $$=createTnode(1, "ParamDec", ""); appendTnode($$, 2, $1, $2); }
  ;

/* Statements */
CompSt : LC DefList StmtList RC  { $$=createTnode(1, "CompSt", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  ;
StmtList : Stmt StmtList  { $$=createTnode(1, "StmtList", ""); appendTnode($$, 2, $1, $2); }
  |  { $$=NULL; }
  ;
Stmt : Exp SEMI  { $$=createTnode(1, "Stmt", ""); appendTnode($$, 2, $1, $2); }
  | CompSt  { $$=createTnode(1, "Stmt", ""); appendTnode($$, 1, $1); }
  | RETURN Exp SEMI  { $$=createTnode(1, "Stmt", ""); appendTnode($$, 3, $1, $2, $3); }
  | IF LP Exp RP Stmt  %prec LOWER_THAN_ELSE  { $$=createTnode(1, "Stmt", ""); appendTnode($$, 5, $1, $2, $3, $4, $5); }
  | IF LP Exp RP Stmt ELSE Stmt  { $$=createTnode(1, "Stmt", ""); appendTnode($$, 7, $1, $2, $3, $4, $5, $6, $7); }
  | WHILE LP Exp RP Stmt  { $$=createTnode(1, "Stmt", ""); appendTnode($$, 5, $1, $2, $3, $4, $5); }
  ;

/* Local Definitions */
DefList : Def DefList  { $$=createTnode(1, "DefList", ""); appendTnode($$, 2, $1, $2); }
  |  { $$=NULL; }
  ;
Def : Specifier DecList SEMI  { $$=createTnode(1, "Def", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
DecList : Dec  { $$=createTnode(1, "DecList", ""); appendTnode($$, 1, $1); }
  | Dec COMMA DecList  { $$=createTnode(1, "DecList", ""); appendTnode($$, 3, $1, $2, $3); }
  ;
Dec : VarDec  { $$=createTnode(1, "Dec", ""); appendTnode($$, 1, $1); }
  | VarDec ASSIGNOP Exp  { $$=createTnode(1, "Dec", ""); appendTnode($$, 3, $1, $2, $3); }
  ;

/* Expressions */
Exp : Exp ASSIGNOP Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp AND Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp OR Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp RELOP Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp PLUS Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp MINUS Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp STAR Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp DIV Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | LP Exp RP  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | MINUS Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 2, $1, $2); }
  | NOT Exp  { $$=createTnode(1, "Exp", ""); appendTnode($$, 2, $1, $2); }
  | ID LP Args RP  { $$=createTnode(1, "Exp", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  | ID LP RP  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp LB Exp RB  { $$=createTnode(1, "Exp", ""); appendTnode($$, 4, $1, $2, $3, $4); }
  | Exp DOT ID  { $$=createTnode(1, "Exp", ""); appendTnode($$, 3, $1, $2, $3); }
  | ID  { $$=createTnode(1, "Exp", ""); appendTnode($$, 1, $1); }
  | INT  { $$=createTnode(1, "Exp", ""); appendTnode($$, 1, $1); }
  | FLOAT  { $$=createTnode(1, "Exp", ""); appendTnode($$, 1, $1); }
  ;
Args : Exp COMMA Args  { $$=createTnode(1, "Args", ""); appendTnode($$, 3, $1, $2, $3); }
  | Exp  { $$=createTnode(1, "Args", ""); appendTnode($$, 1, $1); }
  ;

/* Comments */

/* error */
Stmt : error SEMI  {}
  ;
CompSt : error RC  {}
  ;
VarDec : error RB  {}
  ;
FunDec : error RP  {}
  ;
ExtDef : error SEMI  {}
  ;


%%
void yyerror(char* msg)
{
    Error_flag = 1;
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}