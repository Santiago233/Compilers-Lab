%locations

%{
    #include <stdio.h>
    #include "lex.yy.c"
    #include "node.h"
    int flag;
%}

%union{
	int type_int;
	struct Node* type_node;
	float type_float;
	double type_double;
	char* type_string;
	
}

/* declared tokens */
%token <type_node>INT
%token <type_node>FLOAT
%token <type_node>ID
%token <type_node>SEMI
%token <type_node>COMMA
%token <type_node>ASSIGNOP
%token <type_node>RELOP
%token <type_node>PLUS
%token <type_node>MINUS  
%token <type_node>STAR
%token <type_node>DIV
%token <type_node>AND
%token <type_node>OR
%token <type_node>DOT
%token <type_node>NOT
%token <type_node>LP
%token <type_node>RP
%token <type_node>LB
%token <type_node>RB
%token <type_node>LC
%token <type_node>RC
%token <type_node>TYPE
%token <type_node>STRUCT
%token <type_node>RETURN
%token <type_node>IF
%token <type_node>ELSE
%token <type_node>WHILE
/*declared non-terminals*/
%type <type_node>Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Dec Exp Args StrangeDef 
%type <type_node>Def DecList
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
Program:   ExtDefList{add_node($1);strncpy(root->name,"Program",20);insert(root,1);root->line = 1;root->rnode = NULL;} 
		;

ExtDefList: ExtDef  ExtDefList	{add_node($1);add_node($2);Node* p = newNode("ExtDefList",*(int*)&@1); $$ = p;insert(p,2);}
		|   {Node* p = newNode_id("ExtDefList","NULL",N);$$ = p;}
		;

ExtDef: Specifier   ExtDecList  SEMI {add_node($1);add_node($2);add_node($3);Node* p = newNode("ExtDef",*(int*)&@1); $$ = p;insert(p,3);}
		| 	Specifier   SEMI  {add_node($1);add_node($2);Node* p = newNode("ExtDef",*(int*)&@1); $$ = p;insert(p,2);}
		|   Specifier   FunDec  CompSt  {add_node($1);add_node($2);add_node($3);Node* p = newNode("ExtDef",*(int*)&@1);$$ = p;insert(p,3); }
		|   error   SEMI    { printf("Error type B at Line %d: Missing variable definition\n", @2); }
        |   error   FunDec  CompSt  { printf("Error type B at Line %d: Missing function definition\n", @2); }
        |   Specifier   ExtDecList  error   StrangeDef  { printf("Error type B at Line %d: Missing semicolon for global variable;\n", @2); }
        |   Specifier   ExtDecList  error   SEMI    { printf("Error type B at Line %d: Missing semicolon for global variable;\n", @2); }
		;

ExtDecList: VarDec       {add_node($1);Node* p = newNode("ExtDecList",*(int*)&@1);$$ = p;insert(p,1); }
		|	VarDec  COMMA   ExtDecList   {add_node($1);add_node($2);add_node($3);Node* p = newNode("ExtDecList",*(int*)&@1); $$ = p;insert(p,3);}
		;

Specifier:  TYPE      {add_node($1);Node* p = newNode("Specifier",*(int*)&@1); $$ = p;insert(p,1);}
		|	StructSpecifier  {add_node($1);Node* p = newNode("Specifier",*(int*)&@1); $$ = p;insert(p,1);}
		;

StructSpecifier:    STRUCT  OptTag  LC DefList RC  {add_node($1);add_node($2);add_node($3);add_node($4);add_node($5);Node* p = newNode("StrcutSpecifier",*(int*)&@1); $$ = p;insert(p,5);}
		|	STRUCT  Tag	{add_node($1);add_node($2);Node* p = newNode("StructSpecifier",*(int*)&@1);$$ = p;insert(p,2); }
		|   STRUCT  error   LC  DefList RC  { printf("Error type B at Line %d: Illegal name for structure\n", @1); }
        |   STRUCT  error   SEMI    { printf("Error type B at Line %d: Illegal name for structure definition\n", @1); }
		;

OptTag: ID{add_node($1);Node* p = newNode("OptTag",*(int*)&@1);$$ = p;$$ = p;insert(p,1);}
		|{Node* p = newNode_id("OptTag","NULL",N);$$ = p;}
	    ;

Tag:    ID{add_node($1);Node* p = newNode("Tag",*(int*)&@1); $$ = p;insert(p,1);}
		;

VarDec:	ID  {add_node($1);Node* p = newNode("VarDec",*(int*)&@1);$$ = p;insert(p,1);}
		|	VarDec	LB	INT  RB{add_node($1);add_node($2);add_node($3);add_node($4);   Node* p = newNode("VarDec",*(int*)&@1);$$ = p;insert(p,4); }
        |   VarDec  LB  error   RB  { printf("Error type B at Line %d: Error for array subscript\n", @1); }
        |   VarDec  LB  INT error   RB  { printf("Error type B at Line %d: Lost for ']'\n", @3); }
        |   VarDec  LB  INT error   SEMI    { printf("Error type B at Line %d: Lost for ']'\n", @3); }
		;

FunDec:	ID  LP  VarList RP  {add_node($1);add_node($2);add_node($3);add_node($4);Node* p = newNode("FunDec",*(int*)&@1);$$ = p;insert(p,4); }
		|	ID   LP  RP{add_node($1);add_node($2);add_node($3);Node* p = newNode("FunDec",*(int*)&@1); $$ = p;insert(p,3);}
		;				

VarList:    ParamDec COMMA VarList{add_node($1);add_node($2);add_node($3);Node* p = newNode("VarList",*(int*)&@1);$$ = p;insert(p,3); }
		|	ParamDec{add_node($1);Node* p = newNode("VarList",*(int*)&@1);$$ = p;insert(p,1); }
		|	ParamDec	error	VarList	{ printf("Error type B at Line %d: Error for parameters of the function\n", @1); }
		;

ParamDec:   Specifier VarDec{add_node($1);add_node($2);Node* p = newNode("ParamDec",*(int*)&@1);$$ = p;insert(p,2);}
		|	error	VarDec	{ printf("Error type B at Line %d: Missing  type definitions of variables\n", @1); }
		;

CompSt: LC DefList StmtList RC{add_node($1);add_node($2);add_node($3);add_node($4);Node* p = newNode("CompSt",*(int*)&@1);$$ = p;insert(p,4); }
		;

StmtList:   Stmt StmtList{add_node($1);add_node($2);Node* p = newNode("StmtList",*(int*)&@1);$$ = p;insert(p,2); }
		|{Node* p = newNode_id("StmtList","NULL",N);$$ = p;}
		;

Stmt:   Exp SEMI{add_node($1);add_node($2);Node* p = newNode("Stmt",*(int*)&@1); $$ = p;insert(p,2);}
		|	Exp	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	Exp	error	RC	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	CompSt  {add_node($1);Node* p = newNode("Stmt",*(int*)&@1);$$ = p;insert(p,1);}
		|	RETURN Exp SEMI  {add_node($1);add_node($2);add_node($3);Node* p = newNode("Stmt",*(int*)&@1);$$ = p;insert(p,3); }
		|	RETURN Exp	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon of 'return'\n", @2); }
		|	RETURN Exp	error	{ printf("Error type B at Line %d: Missing semicolon of 'return'\n", @2); }
		| 	IF  LP Exp RP Stmt  %prec LOWER_THAN_ELSE   {add_node($1);add_node($2);add_node($3);add_node($4);add_node($5);Node* p = newNode("Stmt",*(int*)&@1);$$ = p;insert(p,5); }
		|	IF  LP Exp RP Stmt ELSE Stmt	{add_node($1);add_node($2);add_node($3);add_node($4);add_node($5);add_node($6);add_node($7);Node* p = newNode("Stmt",*(int*)&@1);$$ = p;insert(p,7); }	
		|	IF	LP Exp RP Stmt ELSE	error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	IF	LP Exp RP Stmt ELSE	error	RC	{ printf("Error type B at Line %d: Missing semicolon for sentence\n", @1); }
		|	WHILE LP Exp RP Stmt {add_node($1);add_node($2);add_node($3);add_node($4);add_node($5);Node* p = newNode("Stmt",*(int*)&@1);$$ = p;insert(p,5); }
		;		

DefList:    Def DefList{add_node($1);add_node($2);Node* p = newNode("DefList",*(int*)&@1); $$ = p;insert(p,2);}
		|{Node* p = newNode_id("DefList","NULL",N);$$ = p;}
		;

Def:    Specifier DecList SEMI{add_node($1);add_node($2);add_node($3);}{Node* p = newNode("Def",*(int*)&@1);$$ = p;insert(p,3); }
		|	Specifier DecList error	SEMI	{ printf("Error type B at Line %d: Missing semicolon for definitions of Local variables\n", @2); }
		;

DecList:    Dec{add_node($1);Node* p = newNode("DecList",*(int*)&@1); $$ = p;insert(p,1);}
		|	Dec COMMA DecList  {add_node($1);add_node($2);add_node($3);Node* p = newNode("DecList",*(int*)&@1);$$ = p;insert(p,3); }
		;

Dec:    VarDec    {add_node($1);Node* p = newNode("Dec",*(int*)&@1); $$ = p;insert(p,1);}
		|	VarDec ASSIGNOP Exp   {add_node($1);add_node($2);add_node($3);Node* p = newNode("Dec",*(int*)&@1);$$ = p;insert(p,3); }
		;

Exp:    Exp ASSIGNOP    Exp   {add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,3);}
	    |	Exp	ASSIGNOP	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
        |   Exp AND Exp   {add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,3);}
	    |	Exp	AND	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp OR  Exp  {add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,3); }
		|	Exp	OR	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp RELOP   Exp{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,3);}
		|	Exp	RELOP	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		| 	Exp PLUS  	Exp{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,3); }
		|	Exp	PLUS	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp MINUS	Exp{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,3);}
		|	Exp	MINUS	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	Exp	STAR 	Exp{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,3);}
		|	Exp	STAR	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		| 	Exp DIV Exp{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,3);}
		|	Exp	DIV	error	SEMI	{ printf("Error type B at Line %d: The number of operands is wrong;\n", @2); }
		|	LP  Exp RP{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,3); }
		|	MINUS   Exp{add_node($1);add_node($2);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,2); }
		|	NOT Exp{add_node($1);add_node($2);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,2);}
		|	ID  LP  Args    RP{add_node($1);add_node($2);add_node($3);add_node($4);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,4);}
		|	ID  LP  RP{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,3);}
		|	Exp LB  Exp RB{add_node($1);add_node($2);add_node($3);add_node($4);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,4);}
		|	Exp	LB	Exp	error		RB	{ printf("Error type B at Line %d: Lost for ']'\n", @3); }
		|	Exp	LB	Exp	error	SEMI	{ printf("Error type B at Line %d: Lost for ']'\n", @3); }
		|	Exp DOT ID{add_node($1);add_node($2);add_node($3);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,3); }
		|	ID{add_node($1);Node* p = newNode("Exp",*(int*)&@1);$$ = p;insert(p,1); }
		|	INT{add_node($1);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,1);}
		|	FLOAT{add_node($1);Node* p = newNode("Exp",*(int*)&@1); $$ = p;insert(p,1);}
        ;

Args:   Exp COMMA   Args{add_node($1);add_node($2);add_node($3);Node* p = newNode("Args",*(int*)&@1);$$ = p;insert(p,3);}
		|	Exp{add_node($1);Node* p = newNode("Args",*(int*)&@1);$$ = p;insert(p,1);}
		;

StrangeDef: FunDec  CompSt{add_node($1);add_node($2);Node* p = newNode("StrangeDef",*(int*)&@1); $$ = p;insert(p,2);}
        ;

%%
yyerror(char* msg){
	flag = 2;
    //fprintf(stderr, "error: %s\n", msg);
}
