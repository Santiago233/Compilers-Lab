%locations
%{
    #include <stdio.h>
    #include "lex.yy.c"
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
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT MINUS
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
        |   error   SEMI    { printf("%d Error type B: 变量缺少类型定义， 如直接a;\n", @1); }
        |   error   FunDec  CompSt  { printf("%d Error type B: 函数缺少类型定义， 如直接a;\n", @1); }
        |   Specifier   error   SEMI    { printf("%d Error type B: 缺少与类型相关的变量定义， 如直接int int;\n", @1); }
        |   Specifier   error   CompSt  { printf("%d Error type B: 缺少与类型相关的函数名定义， 如直接int int;\n", @1); }
        |   Specifier   ExtDecList  error   StrangeDef  { printf("%d Error type B: 分号缺失;\n", @2); }
        |   Specifier   error   StrangeDef  { printf("%d Error type B: 分号缺失;\n", @1); }
        |   Specifier   ExtDecList  error   SEMI    { printf("%d Error type B: 分号缺失;\n", @2); }
        |   Specifier   error   SEMI    { printf("%d Error type B: 分号缺失;\n", @1); }
		;

ExtDecList: VarDec
		|	VarDec  COMMA   ExtDecList
        |   VarDec  error   ExtDecList  { printf("%d Error type B: 全局变量定义缺少‘，’，如a.b\n", @1); }
		;

Specifier:  TYPE
		|	StructSpecifier
		;

StructSpecifier:    STRUCT  OptTag  LC  DefList RC
		|	STRUCT  Tag
        |   STRUCT  error   LC  DefList RC  { printf("%d Error type B: 结构类型名非法\n", @1); }
        |   STRUCT  OptTag  error   RC  { printf("%d Error type B: 结构体定义的左括号缺失\n", @2); }
        |   STRUCT  OptTag  LC  DefList error   StrangeDef  { printf("%d Error type B: 结构体定义的右括号缺失\n", @4); }
        |   STRUCT  OptTag  LC  DefList error   SEMI    { printf("%d Error type B: 结构体定义的右括号缺失\n", @4); }
        |   STRUCT  error   SEMI    { printf("%d Error type B: 初始化新结构变量类型名非法\n", @1); }
		;

OptTag: ID
		|
	    ;

Tag:    ID
		;

VarDec:	ID
		|	VarDec	LB	INT  RB
		;

FunDec:	ID  LP  VarList RP
		|	ID  LP  RP
		;				

VarList:    ParamDec COMMA VarList
		|	ParamDec
		;

ParamDec:   Specifier VarDec
		;

CompSt: LC DefList StmtList RC
		;

StmtList:   Stmt StmtList
		|
		;

Stmt:   Exp SEMI
		|	CompSt
		|	RETURN Exp SEMI
		| 	IF  LP Exp RP Stmt  %prec LOWER_THAN_ELSE
		|	IF  LP Exp RP Stmt ELSE Stmt
		|	WHILE LP Exp RP Stmt
		;		

DefList:    Def DefList
		|
		;

Def:    Specifier DecList SEMI
		;

DecList:    Dec
		|	Dec COMMA DecList
		;

Dec:    VarDec
		|	VarDec ASSIGNOP Exp
		;

Exp:    Exp ASSIGNOP    Exp
        |   Exp AND Exp
		|	Exp OR  Exp
		|	Exp RELOP   Exp
		| 	Exp PLUS  	Exp
		|	Exp MINUS	Exp
		|	Exp	STAR 	Exp
		| 	Exp DIV Exp
		|	LP  Exp RP
		|	MINUS   Exp
		|	NOT Exp
		|	ID  LP  Args    RP
		|	ID  LP  RP
		|	Exp LB  Exp RB
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
    fprintf(stderr, "error: %s\n", msg);
}