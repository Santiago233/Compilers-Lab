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
Program:   ExtDefList{strncpy(root->name,"pragram",20);root->line = 1;root->childnode = stact[1];root->rnode = NULL;} 
		;

ExtDefList: ExtDef  ExtDefList	{Node* p = newNode("ExtDefList",*(int*)&@1);insert(2,p); }
		|   {Node* pp = newNode_("NULL",0,0);scount++;stact[scount] = pp;}/* empty */  {Node* p = newNode("ExtDefList",0);insert(1,p);}
		;

ExtDef: Specifier   ExtDecList  SEMI {Node* p = newNode("ExtDef",*(int*)&@1);insert(3,p); }
		| 	Specifier   SEMI  {Node* p = newNode("ExtDef",*(int*)&@1);insert(2,p); }
		|   Specifier   FunDec  CompSt  {Node* p = newNode("ExtDef",*(int*)&@1);insert(3,p); }
		|   error   SEMI    { printf("Error type B at Line %d: Missing variable definition\n", @2); }
        |   error   FunDec  CompSt  { printf("Error type B at Line %d: Missing function definition\n", @2); }
        |   Specifier   ExtDecList  error   StrangeDef  { printf("Error type B at Line %d: Missing semicolon for global variable;\n", @2); }
        |   Specifier   ExtDecList  error   SEMI    { printf("Error type B at Line %d: Missing semicolon for global variable;\n", @2); }
		;

ExtDecList: VarDec       {Node* p = newNode("ExtDecList",*(int*)&@1);insert(1,p); }
		|	VarDec  COMMA   ExtDecList   {Node* p = newNode("ExtDecList",*(int*)&@1);insert(3,p); }
		;

Specifier:  TYPE       {Node* p = newNode("Specifier",*(int*)&@1);insert(1,p); }
		|	StructSpecifier  {Node* p = newNode("Specifier",*(int*)&@1);insert(1,p); }
		;

StructSpecifier:    STRUCT  OptTag  LC  DefList RC  {Node* p = newNode("StrcutSpecifier",*(int*)&@1);insert(5,p); }
		|	STRUCT  Tag	{Node* p = newNode("StructSpecifier",*(int*)&@1);insert(2,p); }
		|   STRUCT  error   LC  DefList RC  { printf("Error type B at Line %d: Illegal name for structure\n", @1); }
        |   STRUCT  error   SEMI    { printf("Error type B at Line %d: Illegal name for structure definition\n", @1); }
		;

OptTag: ID{Node* p = newNode("OptTag",*(int*)&@1);insert(1,p); }
		|{Node* pp = newNode_("NULL",0,0);scount++;stact[scount] = pp;}{Node* p = newNode("OptTag",0);insert(1,p);}
	    ;

Tag:    ID{Node* p = newNode("Tag",*(int*)&@1);insert(1,p); }
		;

VarDec:	ID  {Node* p = newNode("VarDec",*(int*)&@1);insert(1,p); }
		|	VarDec	LB	INT  RB{Node* p = newNode("VarDec",*(int*)&@1);insert(4,p); }
        |   VarDec  LB  error   RB  { printf("Error type B at Line %d: Error for array subscript\n", @1); }
        |   VarDec  LB  INT error   RB  { printf("Error type B at Line %d: Lost for ']'\n", @3); }
        |   VarDec  LB  INT error   SEMI    { printf("Error type B at Line %d: Lost for ']'\n", @3); }
		;

FunDec:	ID  LP  VarList RP  {Node* p = newNode("FunDec",*(int*)&@1);insert(4,p); }
		|	ID  LP  RP{Node* p = newNode("FunDec",*(int*)&@1);insert(3,p); }
		;				

VarList:    ParamDec COMMA VarList{Node* p = newNode("VarList",*(int*)&@1);insert(3,p); }
		|	ParamDec{Node* p = newNode("VarList",*(int*)&@1);insert(1,p); }
		|	ParamDec	error	VarList	{ printf("Error type B at Line %d: Error for parameters of the function\n", @1); }
		;

ParamDec:   Specifier VarDec{Node* p = newNode("ParamDec",*(int*)&@1);insert(2,p); }
		|	error	VarDec	{ printf("Error type B at Line %d: Missing  type definitions of variables\n", @1); }
		;

CompSt: LC DefList StmtList RC{Node* p = newNode("CompSt",*(int*)&@1);insert(4,p); }
		;

StmtList:   Stmt StmtList{Node* p = newNode("StmtList",*(int*)&@1);insert(2,p); }
		|{Node* pp = newNode_("NULL",0,0);scount++;stact[scount] = pp;}{Node* p = newNode("StmtList",0);insert(1,p);}
		;

Stmt:   Exp SEMI{Node* p = newNode("Stmt",*(int*)&@1);insert(2,p); }
		|	Exp	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	Exp	error	RC	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	CompSt  {Node* p = newNode("Stmt",*(int*)&@1);insert(1,p); }
		|	RETURN Exp SEMI  {Node* p = newNode("Stmt",*(int*)&@1);insert(3,p); }
		|	RETURN Exp	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon of 'return'\n", @2); }
		|	RETURN Exp	error	{ printf("Error type B at Line %d: Missing semicolon of 'return'\n", @2); }
		| 	IF  LP Exp RP Stmt  %prec LOWER_THAN_ELSE   {Node* p = newNode("Stmt",*(int*)&@1);insert(5,p); }
		|	IF  LP Exp RP Stmt ELSE Stmt	{Node* p = newNode("Stmt",*(int*)&@1);insert(7,p); }	
		|	IF	LP Exp RP Stmt ELSE	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	IF	LP Exp RP Stmt ELSE	error	RC	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	WHILE LP Exp RP Stmt {Node* p = newNode("Stmt",*(int*)&@1);insert(5,p); }
		;		

DefList:    Def DefList{Node* p = newNode("DefList",*(int*)&@1);insert(2,p); }
		|{Node* pp = newNode_("NULL",0,0);scount++;stact[scount] = pp;}{Node* p = newNode("DefList",0);insert(1,p);}
		;

Def:    Specifier DecList SEMI{Node* p = newNode("Def",*(int*)&@1);insert(3,p); }
		|	Specifier DecList error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for definitions of Local variables\n", @2); }
		;

DecList:    Dec{Node* p = newNode("DecList",*(int*)&@1);insert(1,p); }
		|	Dec COMMA DecList  {Node* p = newNode("DecList",*(int*)&@1);insert(3,p); }
		;

Dec:    VarDec    {Node* p = newNode("Dec",*(int*)&@1);insert(1,p); }
		|	VarDec ASSIGNOP Exp   {Node* p = newNode("Dec",*(int*)&@1);insert(3,p); }
		;

Exp:    Exp ASSIGNOP    Exp   {Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
	    |	Exp	ASSIGNOP	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
        |   Exp AND Exp   {Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
	    |	Exp	AND	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp OR  Exp  {Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp	OR	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp RELOP   Exp{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp	RELOP	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		| 	Exp PLUS  	Exp{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp	PLUS	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp MINUS	Exp{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp	MINUS	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp	STAR 	Exp{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp	STAR	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		| 	Exp DIV Exp{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp	DIV	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	LP  Exp RP{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	MINUS   Exp{Node* p = newNode("Exp",*(int*)&@1);insert(2,p); }
		|	NOT Exp{Node* p = newNode("Exp",*(int*)&@1);insert(2,p); }
		|	ID  LP  Args    RP{Node* p = newNode("Exp",*(int*)&@1);insert(4,p); }
		|	ID  LP  RP{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	Exp LB  Exp RB{Node* p = newNode("Exp",*(int*)&@1);insert(4,p); }
		|	Exp	LB	Exp	error		RB	{ printf("Error type B at Line %d: Lost for ']'\n", @3); }
		|	Exp	LB	Exp	error	SEMI	{ printf("Error type B at Line %d: Lost for ']'\n", @3); }
		|	Exp DOT ID{Node* p = newNode("Exp",*(int*)&@1);insert(3,p); }
		|	ID{Node* p = newNode("Exp",*(int*)&@1);insert(1,p); }
		|	INT{Node* p = newNode("Exp",*(int*)&@1);insert(1,p); }
		|	FLOAT{Node* p = newNode("Exp",*(int*)&@1);insert(1,p); }
        ;

Args:   Exp COMMA   Args{Node* p = newNode("Args",*(int*)&@1);insert(3,p); }
		|	Exp{Node* p = newNode("Args",*(int*)&@1);insert(1,p); }
		;

StrangeDef: FunDec  CompSt{Node* p = newNode("StrangeDef",*(int*)&@1);insert(2,p); }
        ;

%%
yyerror(char* msg){
	flag = 2;
    //fprintf(stderr, "error: %s\n", msg);
}