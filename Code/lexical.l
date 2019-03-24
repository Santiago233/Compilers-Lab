%option yylineno
%{
    #include <stdio.h>
    int _16to10(char* strHex);
    int _8to10(char* strHex);
%}
digit [0-9]
letter [_a-zA-Z]

%%
([1-9]digit*)|0 { printf("INT_10, decimal integer value %d\n", atoi(yytext)); }
0[xX][0-9a-fA-F]+ {
        int int_yytext = _16to10(yytext);
        printf("INT_16, hexadecimal integer value %d\n", int_yytext);
    }
0[0-7]+ {
        int int_yytext = _8to10(yytext);
        printf("INT_8, octal integer value %s\n", int_yytext);
    }
([1-9]digit*)|0.digit* { printf("Float value %f\n", yytext); }
";" { printf("SEMI\n"); }
"," { printf("COMMA\n"); }
"=" { printf("ASSIGNOP\n"); }
">" { printf("RELOP_DAYU\n"); }
"<" { printf("RELOP_XIAOYU\n"); }
">=" { printf("RELOP_DAYUDENGYU\n"); }
"<=" { printf("RELOP_XIAOYUDENGYU\n"); }
"==" { printf("RELOP_LIANDENG\n"); }
"!=" { printf("RELOP_BUDENGYU\n"); }
"+" { printf("PLUS\n"); }
"-" { printf("MINUS\n"); }
"*" { printf("STAR\n"); }
"/" { printf("DIV\n"); }
"&&" { printf("AND\n"); }
"||" { printf("OR\n"); }
"." { printf("DOT\n"); }
"!" { printf("NOT\n"); }
"(" { printf("LP\n"); }
")" { printf("RP\n"); }
"[" { printf("LB\n"); }
"]" { printf("RB\n"); }
"{" { printf("LC\n"); }
"}" { printf("RC\n"); }
"int" { printf("TYPE_INT\n"); }
"float" { printf("TYPE_FLOAT\n"); }
"struct" { printf("STRCUT\n"); }
"return" { printf("RETURN\n"); }
"if" { printf("IF\n"); }
"else" { printf("ELSE\n"); }
"while" { printf("WHILE\n"); }
letter(letter|digit)* { printf("ID %f\n", yytext) }
. { printf("Error type A at Line %d:Mysterious characters \'%s\'\n", yylineno, yytext) }

%%
int _8to10(char* strHex) {
	int i = 0, len = 0, temp = 0;
	//len = str.length();
	int nDec = 0;
	len = strlen(strHex);
	for (i = 0; strHex[i] != '\0'; ++i)
	{
		temp = strHex[i] - '0'; 
		nDec += (temp*pow(8, len - 1 - i));
	}
	return nDec;


}
int _16to10(char* strHex){
	//string str = strHex;
	int i = 0, len = 0, temp = 0;
	//len = str.length();
	int nDec = 0;

	len = strlen(strHex);
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
		nDec += (temp*pow(16,len - 1 - i));
	}
	return nDec;
	
}