%option yylineno
%{
    #include <stdio.h>
    #include <math.h>
	#include <stdlib.h>
	#include "syntax.tab.h"
	int yycolumn = 1;
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
%}
digit [0-9]
letter [_a-zA-Z]

%%
([1-9]{digit}*)|0 {
		printf("INT_10, decimal integer value %d\n", atoi(yytext));
		return INT;
	}
0[xX][0-9a-fA-F]+ {
        int int_yytext = _16to10(yytext);
        printf("INT_16, hexadecimal integer value %d\n", int_yytext);
		return INT;
    }
0[0-7]+ {
        int int_yytext = _8to10(yytext);
        printf("INT_8, octal integer value %d\n", int_yytext);
		return INT;
    }
0[xX][0-9a-fA-F]*[g-zG-Z]+[0-9a-zA-Z]* {
		printf("Error type A at Line %d:Wrong Hexadecimal Number \'%s\'\n", yylineno, yytext);
	}
0[0-7]*[89]+[0-9]* {
		printf("Error type A at Line %d:Wrong Octal Number \'%s\'\n", yylineno, yytext);
	}
(([1-9]{digit}*)|0)"."{digit}+ {
		printf("Float value %f\n", atof(yytext));
		return FLOAT;
	}
0{digit}*"."{digit}* {
		printf("Error type A at Line %d:Wrong Float value \'%s\'\n", yylineno, yytext);
	}
";" {
		printf("SEMI\n");
		return SEMI;
	}
"," {
		printf("COMMA\n");
		return COMMA;
	}
"=" {
		printf("ASSIGNOP\n");
		return ASSIGNOP;
	}
">" {
		printf("RELOP_DAYU\n");
		return RELOP;
	}
"<" {
		printf("RELOP_XIAOYU\n");
		return RELOP;
	}
">=" {
		printf("RELOP_DAYUDENGYU\n");
		return RELOP;
	}
"<=" {
		printf("RELOP_XIAOYUDENGYU\n");
		return RELOP;
	}
"==" {
		printf("RELOP_LIANDENG\n");
		return RELOP;
	}
"!=" {
		printf("RELOP_BUDENGYU\n");
		return RELOP;
	}
"+" {
		printf("PLUS\n");
		return PLUS;
	}
"-" {
		printf("MINUS\n");
		return MINUS;
	}
"*" {
		printf("STAR\n");
		return STAR;
	}
"/" {
		printf("DIV\n");
		return DIV;
	}
"&&" {
		printf("AND\n");
		return AND;
	}
"||" {
		printf("OR\n");
		return OR;
	}
"." {
		printf("DOT\n");
		return DOT;
	}
"!" {
		printf("NOT\n");
		return NOT;
	}
"(" {
		printf("LP\n");
		return LP;
	}
")" {
		printf("RP\n");
		return RP;
	}
"[" {
		printf("LB\n");
		return LB;
	}
"]" {
		printf("RB\n");
		return RB;
	}
"{" {
		printf("LC\n");
		return LC;
	}
"}" {
		printf("RC\n");
		return RC;
	}
"int" {
		printf("TYPE_INT\n");
		return TYPE;
	}
"float" {
		printf("TYPE_FLOAT\n");
		return TYPE;
	}
"struct" {
		printf("STRCUT\n");
		return STRUCT;
	}
"return" {
		printf("RETURN\n");
		return RETURN;
	}
"if" {
		printf("IF\n");
		return IF;
	}
"else" {
		printf("ELSE\n");
		return ELSE;
	}
"while" {
		printf("WHILE\n");
		return WHILE;
	}
" " {
		printf("SPACE\n");
	}
\t {
		printf("TAB\n");
	}
\n {
		printf("NL\n");
		yycolumn = 1;
	}
\r {
		printf("CR\n");
	}
{letter}({letter}|{digit})* {
		printf("ID %s\n", yytext);
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