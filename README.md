# Compilers-Lab
Lab for Lesson 'Compilers'

Lexical Analysis:
(1)flex lexical.l
(2)gcc main.c lex.yy.c -lfl -o scanner
(3)./scanner test.cmm

Gramma Analysis:
(1)bison -d syntax.y
(2)flex lexical.l
(3)gcc main.c syntax.tab.c -lfl -ly -o parser
(4)./parser test.cmm