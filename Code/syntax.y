%locations

%{
    #include <stdio.h>
    #include "lex.yy.c"
    int flag;
%}

/* declared tokens */
%token INT
%token FLOAT
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
%token LP
%token RP
%token LB
%token RB
%token LC
%token RC
%token TYPE
%token STRUCT
%token RETURN
%token IF
%token ELSE
%token WHILE
%token ID

/* precedence and associativity of Operators */
/* MINUS for one unit lost */
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

/* actions to conflicts between shift and reduce */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program:    ExtDefList
		;

ExtDefList: ExtDef  ExtDefList
		|   /* empty */
		;

ExtDef: Specifier   ExtDecList  SEMI
		| 	Specifier   SEMI
		|   Specifier   FunDec  CompSt
		|   error   SEMI    { printf("Error type B at Line %d: Missing variable definition\n", @2); }
        |   error   FunDec  CompSt  { printf("Error type B at Line %d: Missing function definition\n", @2); }
        |   Specifier   ExtDecList  error   StrangeDef  { printf("Error type B at Line %d: Missing semicolon for global variable;\n", @2); }
        |   Specifier   ExtDecList  error   SEMI    { printf("Error type B at Line %d: Missing semicolon for global variable;\n", @2); }
		;

ExtDecList: VarDec
		|	VarDec  COMMA   ExtDecList
		;

Specifier:  TYPE
		|	StructSpecifier
		;

StructSpecifier:    STRUCT  OptTag  LC  DefList RC
		|	STRUCT  Tag
		|   STRUCT  error   LC  DefList RC  { printf("Error type B at Line %d: Illegal name for structure\n", @1); }
        |   STRUCT  error   SEMI    { printf("Error type B at Line %d: Illegal name for structure definition\n", @1); }
		;

OptTag: ID
		|
	    ;

Tag:    ID
		;

VarDec:	ID
		|	VarDec	LB	INT  RB
        |   VarDec  LB  error   RB  { printf("Error type B at Line %d: Error for array subscript\n", @1); }
        |   VarDec  LB  INT error   RB  { printf("Error type B at Line %d: Lost for ']'\n", @3); }
        |   VarDec  LB  INT error   SEMI    { printf("Error type B at Line %d: Lost for ']'\n", @3); }
		;

FunDec:	ID  LP  VarList RP
		|	ID  LP  RP
		;				

VarList:    ParamDec COMMA VarList
		|	ParamDec
		|	ParamDec	error	VarList	{ printf("Error type B at Line %d: Error for parameters of the function\n", @1); }
		;

ParamDec:   Specifier VarDec
		|	error	VarDec	{ printf("Error type B at Line %d: Missing  type definitions of variables\n", @1); }
		;

CompSt: LC DefList StmtList RC
		;

StmtList:   Stmt StmtList
		|
		;

Stmt:   Exp SEMI
		|	Exp	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	Exp	error	RC	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	CompSt
		|	RETURN Exp SEMI
		|	RETURN Exp	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon of 'return'\n", @2); }
		|	RETURN Exp	error	{ printf("Error type B at Line %d: Missing semicolon of 'return'\n", @2); }
		| 	IF  LP Exp RP Stmt  %prec LOWER_THAN_ELSE
		|	IF  LP Exp RP Stmt ELSE Stmt		
		|	IF	LP Exp RP Stmt ELSE	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	IF	LP Exp RP Stmt ELSE	error	RC	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	WHILE LP Exp RP Stmt
		;		

DefList:    Def DefList
		|
		;

Def:    Specifier DecList SEMI
		|	Specifier DecList error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for definitions of Local variables\n", @2); }
		;

DecList:    Dec
		|	Dec COMMA DecList
		;

Dec:    VarDec
		|	VarDec ASSIGNOP Exp
		;

Exp:    Exp ASSIGNOP    Exp
	    |	Exp	ASSIGNOP	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
        |   Exp AND Exp
	    |	Exp	AND	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp OR  Exp
		|	Exp	OR	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp RELOP   Exp
		|	Exp	RELOP	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		| 	Exp PLUS  	Exp
		|	Exp	PLUS	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp MINUS	Exp
		|	Exp	MINUS	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp	STAR 	Exp
		|	Exp	STAR	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		| 	Exp DIV Exp
		|	Exp	DIV	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	LP  Exp RP
		|	MINUS   Exp
		|	NOT Exp
		|	ID  LP  Args    RP
		|	ID  LP  RP
		|	Exp LB  Exp RB
		|	Exp	LB	Exp	error		RB	{ printf("Error type B at Line %d: Lost for ']'\n", @3); }
		|	Exp	LB	Exp	error	SEMI	{ printf("Error type B at Line %d: Lost for ']'\n", @3); }
		|	Exp DOT ID
		|	ID
		|	INT
		|	FLOAT
        ;

Args:   Exp COMMA   Args
		|	Exp
		;

StrangeDef: FunDec  CompSt
        ;

%%
yyerror(char* msg){
	flag = 2;
    //fprintf(stderr, "error: %s\n", msg);
}
