%option yylineno
%{
    #include <stdio.h>
    #include <math.h>
	#include<string.h>
	#include <stdlib.h>
	#include "syntax.tab.h"
	#include"node.h"
	char* mystr[1024];
	int mystrc;
	int scount;
	int lineflag;
	Node* stact[1024];
	int yycolumn = 1;
	#define YY_USER_ACTION \
		if(lineflag){yylineno = 1;lineflag = 0;}\
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
%}
digit [0-9]
letter [_a-zA-Z]

%%
([1-9]{digit}*)|0 {
		//printf("INT_10, decimal integer value %d\n", atoi(yytext));
		//printf("%s\n",yytext);
		Node* p = newNode_("INT",(YYSTYPE)atoi(yytext),yylineno);
		scount++;
		//printf("%s\n",p->name);
		stack[scount] = p;
		return INT;
	}
0[xX][0-9a-fA-F]+ {
        int int_yytext = _16to10(yytext);
        //printf("INT_16, hexadecimal integer value %d\n", int_yytext);
	Node* p = newNode_("INT",(YYSTYPE)int_yytext,yylineno);
		scount++;
		stack[scount] = p;

		return INT;
    }
0[0-7]+ {
        int int_yytext = _8to10(yytext);
        //printf("INT_8, octal integer value %d\n", int_yytext);
	Node* p = newNode_("INT",(YYSTYPE)int_yytext,yylineno);
		scount++;
		stack[scount] = p;
		return INT;
    }
0[xX][0-9a-fA-F]*[g-zG-Z]+[0-9a-zA-Z]* {
		printf("Error type A at Line %d:Wrong Hexadecimal Number \'%s\'\n", yylineno, yytext);
	}
0[0-7]*[89]+[0-9]* {
		printf("Error type A at Line %d:Wrong Octal Number \'%s\'\n", yylineno, yytext);
	}
(([1-9]{digit}*)|0)"."{digit}+ {
		//printf("Float value %f\n", atof(yytext));
		Node* p = newNode_("FLOAT",(YYSTYPE)(float)atof(yytext),yylineno);
		scount++;
		//printf("%f\n",*(float*)&p->val);
		stack[scount] = p;
		return FLOAT;
	}
0{digit}*"."{digit}* {
		printf("Error type A at Line %d:Wrong Float value \'%s\'\n", yylineno, yytext);
	}
";" {
		//printf();
		Node* p = newNode("SEMI",yylineno);
		scount++;
		stack[scount] = p;
		return SEMI;
	}
"," {
		//printf("COMMA\n");
		Node* p = newNode("COMMA",yylineno);
		scount++;
		stack[scount] = p;
		return COMMA;
	}
"=" {
		//printf("ASSIGNOP\n");
		Node* p = newNode("ASSIGNOP",yylineno);
		scount++;
		stack[scount] = p;
		return ASSIGNOP;
	}
">" {
		//printf("RELOP_DAYU\n");
		Node* p = newNode("RELOP",yylineno);
		scount++;
		stack[scount] = p;
		return RELOP;
	}
"<" {
		//printf("RELOP_XIAOYU\n");
		Node* p = newNode("LELOP_XIAOYU",yylineno);
		scount++;
		stack[scount] = p;
		return RELOP;
	}
">=" {
		//printf("RELOP_DAYUDENGYU\n");
		Node* p = newNode("RELOP_DAYUDENGYU",yylineno);
		scount++;
		stack[scount] = p;
		return RELOP;
	}
"<=" {
		//printf("RELOP_XIAOYUDENGYU\n");
		Node* p = newNode("RELOP_XIAOYUDENGYU",yylineno);
		scount++;
		stack[scount] = p;
		return RELOP;
	}
"==" {
		Node* p = newNode("RELOP_LIANDENG",yylineno);
		scount++;
		stack[scount] = p;
		//printf("RELOP_LIANDENG\n");
		return RELOP;
	}
"!=" {
		//printf("RELOP_BUDENGYU\n");
		Node* p = newNode("RELOP_BUDENGYU",yylineno);
		scount++;
		stack[scount] = p;
		return RELOP;
	}
"+" {
		Node* p = newNode("PLUS",yylineno);
		scount++;
		stack[scount] = p;
		//printf("PLUS\n");
		
		return PLUS;
	}
"-" {
		Node* p = newNode("MINUS",yylineno);
		scount++;
		stack[scount] = p;
		//printf("MINUS\n");
		return MINUS;
	}
"*" {
		Node* p = newNode("STAR",yylineno);
		scount++;
		stack[scount] = p;
		//printf("STAR\n");
		return STAR;
	}
"/" {
		Node* p = newNode("DIV",yylineno);
		scount++;
		stack[scount] = p;
		//printf("DIV\n");
		return DIV;
	}
"&&" {
		Node* p = newNode("AND",yylineno);
		scount++;
		stack[scount] = p;
		//printf("AND\n");
		return AND;
	}
"||" {
		Node* p = newNode("OR",yylineno);
		scount++;
		stack[scount] = p;
		//printf("OR\n");
		return OR;
	}
"." {
		Node* p = newNode("DOT",yylineno);
		scount++;
		stack[scount] = p;
		//printf("DOT\n");
		return DOT;
	}
"!" {
		Node* p = newNode("NOT",yylineno);
		scount++;
		stack[scount] = p;
		//printf("NOT\n");
		return NOT;
	}
"(" {
		Node* p = newNode("LP",yylineno);
		scount++;
		stack[scount] = p;
		//printf("LP\n");
		return LP;
	}
")" {
		Node* p = newNode("RP",yylineno);
		scount++;
		stack[scount] = p;
		//printf("RP\n");
		return RP;
	}
"[" {
		Node* p = newNode("LB",yylineno);
		scount++;
		stack[scount] = p;
		//printf("LB\n");
		return LB;
	}
"]" {
		Node* p = newNode("RB",yylineno);
		scount++;
		stack[scount] = p;
		//printf("RB\n");
		return RB;
	}
"{" {
		Node* p = newNode("LC",yylineno);
		scount++;
		stack[scount] = p;
		//printf("LC\n");
		return LC;
	}
"}" {
		Node* p = newNode("RC",yylineno);
		scount++;
		stack[scount] = p;
		//printf("RC\n");
		return RC;
	}
"int" {
		Node* p = newNode_("TYPE: INT",(YYSTYPE)"int",yylineno);
		scount++;
		stack[scount] = p;
		//printf("TYPE_INT\n");
		return TYPE;
	}
"float" {
		Node* p = newNode_("TYPE: FLOAT",(YYSTYPE)"float",yylineno);
		scount++;
		stack[scount] = p;
		//printf("TYPE_FLOAT\n");
		return TYPE;
	}
"struct" {
		Node* p = newNode("STRUCT",yylineno);
		scount++;
		stack[scount] = p;
		//printf("STRCUT\n");
		return STRUCT;
	}
"return" {
		Node* p = newNode("RETURN",yylineno);
		scount++;
		stack[scount] = p;
		//printf("RETURN\n");
		return RETURN;
	}
"if" {
		Node* p = newNode("IF",yylineno);
		scount++;
		stack[scount] = p;
		//printf("IF\n");
		return IF;
	}
"else" {
		Node* p = newNode("RC",yylineno);
		scount++;
		stack[scount] = p;
		//printf("ELSE\n");
		return ELSE;
	}
"while" {
		Node* p = newNode("RC",yylineno);
		scount++;
		stack[scount] = p;
		//printf("WHILE\n");
		return WHILE;
	}
" " {
		//printf("SPACE\n");
	}
\t {
		//printf("TAB\n");
	}
\n {
		//printf("NL\n");
		yycolumn = 1;
	}
\r {
		//printf("CR\n");
	}
{letter}({letter}|{digit})* {
		Node* p = newNode_id("ID",yytext,yylineno);
		mystr[mystrc++] = yytext;
		scount++;
		stack[scount] = p;
		//printf("ID %s\n ", yytext);
		
		return ID;
	}
{digit}+{letter}+({letter}|{digit})* {
		printf("Error type A at Line %d:Wrong ID \'%s\'\n", yylineno, yytext);
	}
. {
		printf("Error type A at Line %d:Mysterious characters \'%s\'\n", yylineno, yytext);
	}

%%
int _8to10(const char* strHex) {
	int i = 0, len = 0, temp = 0;
	int nDec = 0;
	len = strlen(strHex);
	int j;
	int power = 1;
	for (j = 1; j < len; j++)
		power *= 8;
	for (i = 0; strHex[i] != '\0'; ++i)
	{
		 temp = strHex[i] - '0'; 
		nDec += (temp*power);
		power = power / 8;
	}
	return nDec;
}
int _16to10(const char* strHex){
	int i = 0, len = 0, temp = 0;
	int nDec = 0;
	int j;
	int power = 1;
	len = strlen(strHex);
	for (j = 3; j < len; j++)
		power *= 16;
	for (i = 2; strHex[i] != '\0'; ++i)
	{
		switch (strHex[i])
		{
		case 'A': temp = 10; break;
		case 'B': temp = 11; break;
		case 'C': temp = 12; break;
		case 'D': temp = 13; break;
		case 'E': temp = 14; break;
		case 'F': temp = 15; break;
		case 'a': temp = 10; break;
		case 'b': temp = 11; break;
		case 'c': temp = 12; break;
		case 'd': temp = 13; break;
		case 'e': temp = 14; break;
		case 'f': temp = 15; break;
		default: temp = strHex[i] - '0'; break;
		}
		nDec += (temp*power);
		power = power / 16;
	}
	return nDec;
	
}
