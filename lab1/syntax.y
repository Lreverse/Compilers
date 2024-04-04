%locations
%{
    #include "lex.yy.c"
    void yyerror(char* msg);
%}

%token TYPE
%token STRUCT
%token RETURN
%token IF ELSE
%token WHILE
%token INT
%token FLOAT
%token ID
%token SEMI COMMA
%token ASSIGNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR NOT
%token DOT
%token LP RP LB RB LC RC

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

/* Specifiers */
Specifier : TYPE
  | StructSpecifier
  ;
StructSpecifier : STRUCT OptTag LC DefList RC
  | STRUCT Tag
  ;
OptTag : ID
  |
  ;
Tag : ID
  ;

/* Declarators */
VarDec : ID
  | VarDec LB INT RB
  ;
FunDec : ID LP VarList RP
  | ID LP RP
  ;
VarList : ParamDec COMMA VarList
  | ParamDec
  ;
ParamDec : Specifier VarDec
  ;

/* Statements */
CompSt : LC DefList StmtList RC
  ;
StmtList : Stmt StmtList
  |
  ;
Stmt : Exp SEMI
  | CompSt
  | RETURN Exp SEMI
  | IF LP Exp RP Stmt  %prec LOWER_THAN_ELSE
  | IF LP Exp RP Stmt ELSE Stmt
  | WHILE LP Exp RP Stmt
  ;

/* Local Definitions */
DefList : Def DefList
  | 
  ;
Def : Specifier DecList SEMI
  ;
DecList : Dec
  | Dec COMMA DecList
  ;
Dec : VarDec
  | VarDec ASSIGNOP Exp
  ;

/* Expressions */
Exp : Exp ASSIGNOP Exp
  | Exp AND Exp
  | Exp OR Exp
  | Exp RELOP Exp
  | Exp PLUS Exp
  | Exp MINUS Exp
  | Exp STAR Exp
  | Exp DIV Exp
  | LP Exp RP
  | MINUS Exp
  | NOT Exp
  | ID LP Args RP
  | ID LP RP
  | Exp LB Exp RB
  | Exp DOT ID
  | ID
  | INT
  | FLOAT
  ;
Args : Exp COMMA Args
  | Exp
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
void yyerror(char* msg) {
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}