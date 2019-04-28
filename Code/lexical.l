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
		YYSTYPE F = (YYSTYPE)p;
		yylval = F;
		return INT;
	}
0[xX][0-9a-fA-F]+ {
        int int_yytext = _16to10(yytext);
        //printf("INT_16, hexadecimal integer value %d\n", int_yytext);
	Node* p = newNode_("INT",(YYSTYPE)int_yytext,yylineno);
		YYSTYPE F = (YYSTYPE)p;
		yylval = F;

		return INT;
    }
0[0-7]+ {
        	int int_yytext = _8to10(yytext);
        //printf("INT_8, octal integer value %d\n", int_yytext);
		Node* p = newNode_("INT",(YYSTYPE)int_yytext,yylineno);
		YYSTYPE F = (YYSTYPE)p;
		yylval = F;
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
		YYSTYPE F = (YYSTYPE)p;
		yylval = F;
		return FLOAT;
	}
0{digit}*"."{digit}* {
		printf("Error type A at Line %d:Wrong Float value \'%s\'\n", yylineno, yytext);
	}
";" {
		Node* p = newNode("SEMI",yylineno);
		
		yylval = (YYSTYPE)p;
		return SEMI;
	}
"," {
		//printf("COMMA\n");
		Node* p = newNode("COMMA",yylineno);
		yylval = (YYSTYPE)p;
		return COMMA;
	}
"=" {
		//printf("ASSIGNOP\n");
		Node* p = newNode("ASSIGNOP",yylineno);
		yylval = (YYSTYPE)p;
		return ASSIGNOP;
	}
">" {
		//printf("RELOP_DAYU\n");
		Node* p = newNode("RELOP_DAYU",yylineno);
		yylval = (YYSTYPE)p;
		return RELOP;
	}
"<" {
		//printf("RELOP_XIAOYU\n");
		Node* p = newNode("RELOP_XIAOYU",yylineno);
		yylval = (YYSTYPE)p;
		return RELOP;
	}
">=" {
		//printf("RELOP_DAYUDENGYU\n");
		Node* p = newNode("RELOP_DAYUDENGYU",yylineno);
		yylval = (YYSTYPE)p;
		return RELOP;
	}
"<=" {
		//printf("RELOP_XIAOYUDENGYU\n");
		Node* p = newNode("RELOP_XIAOYUDENGYU",yylineno);
		yylval = (YYSTYPE)p;
		return RELOP;
	}
"==" {
		Node* p = newNode("RELOP_LIANDENG",yylineno);
		yylval = (YYSTYPE)p;
		return RELOP;
	}
"!=" {
		//printf("RELOP_BUDENGYU\n");
		Node* p = newNode("RELOP_BUDENGYU",yylineno);
		yylval = (YYSTYPE)p;
		return RELOP;
	}
"+" {
		
		Node* p = newNode("PLUS",yylineno);
		yylval = (YYSTYPE)p;
		
		return PLUS;
	}
"-" {

		Node* p = newNode("MINUS",yylineno);
		yylval = (YYSTYPE)p;
		return MINUS;
	}
"*" {
		Node* p = newNode("STAR",yylineno);
		yylval = (YYSTYPE)p;
		return STAR;
	}
"/" {
		Node* p = newNode("DIV",yylineno);
		yylval = (YYSTYPE)p;
		return DIV;
	}
"&&" {
		Node* p = newNode("AND",yylineno);
		yylval = (YYSTYPE)p;
		return AND;
	}
"||" {
		Node* p = newNode("OR",yylineno);
		yylval = (YYSTYPE)p;
		return OR;
	}
"." {
		Node* p = newNode("DOT",yylineno);
		yylval = (YYSTYPE)p;
		return DOT;
	}
"!" {
		Node* p = newNode("NOT",yylineno);
		yylval = (YYSTYPE)p;
		return NOT;
	}
"(" {
		Node* p = newNode("LP",yylineno);
		yylval = (YYSTYPE)p;

		return LP;
	}
")" {
		Node* p = newNode("RP",yylineno);
		yylval = (YYSTYPE)p;
		return RP;
	}
"[" {
		Node* p = newNode("LB",yylineno);
		yylval = (YYSTYPE)p;
		return LB;
	}
"]" {
		Node* p = newNode("RB",yylineno);
		yylval = (YYSTYPE)p;
		return RB;
	}
"{" {
		Node* p = newNode("LC",yylineno);
		yylval = (YYSTYPE)p;
		return LC;
	}
"}" {
		Node* p = newNode("RC",yylineno);
		yylval = (YYSTYPE)p;
		return RC;
	}
"int" {		
		Node* p = newNode_id("TYPE","int",yylineno);
		yylval = (YYSTYPE)p;
		return TYPE;
		//YYSTYPE F = (YYSTYPE)yytext;
		//printf("%s\n",*(char**)&F);
		
	}
"float" {
		Node* p = newNode_id("TYPE","float",yylineno);
		yylval = (YYSTYPE)p;
		return TYPE;
		
	}
"struct" {
		Node* p = newNode("STRUCT",yylineno);
		yylval = (YYSTYPE)p;
		return STRUCT;
	}
"return" {
		Node* p = newNode("RETURN",yylineno);
		yylval = (YYSTYPE)p;
		return RETURN;
	}
"if" {
		Node* p = newNode("IF",yylineno);
		yylval = (YYSTYPE)p;
		return IF;
	}
"else" {
		Node* p = newNode("ELSE",yylineno);
		yylval = (YYSTYPE)p;
		return ELSE;
	}
"while" {
		Node* p = newNode("WHILE",yylineno);
		yylval = (YYSTYPE)p;
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
		
		//scount++;
		//stack[scount] = p;
		//name_d = new char[20];
		char *name_d = yytext;
		
		
		Node* p = newNode_id("ID",name_d,yylineno);
		YYSTYPE F = (YYSTYPE)p;
		yylval = F;
		//printf("%s\n",*(char**)&(*(Node**)&yylval)->val);
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
