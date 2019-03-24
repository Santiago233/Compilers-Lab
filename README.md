# Compilers-Lab
Lab for Lesson 'Compilers'

lexical analysis：
-flex lexical.l
-gcc main.c lex.yy.c -lfl -o scanner
-./scanner test.cmm