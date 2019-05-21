# Compilers-Lab
Lab for Lesson 'Compilers: Principles, Techniques and Tools'

Lexical Analysis:
(1)flex lexical.l
(2)gcc main.c lex.yy.c -lfl -o scanner
(3)./scanner test.cmm

Gramma Analysis:
(1)bison -d syntax.y
(2)flex lexical.l
(3)gcc main.c syntax.tab.c -lfl -ly -o parser
(4)./parser test.cmm

Semantic Analysis:
(1)bison -d syntax.y
(2)flex lexical.l
(3)gcc main.c syntax.tab.c -lfl -ly -o parser
(4)./parser test.cmm


Tips:
(1)Due to the terrible grammar of C--, some right instructions can be reported as Lexical errors
(2)some Gramma errors cannot be recovered by the authors
(3)some Semantic errors might be reported twice in different ways due to the execution order in function 'Main' && the storage of structures