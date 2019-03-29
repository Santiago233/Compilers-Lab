#include <stdio.h>
#include "node.h"

int main(int argc, char ** argv){
    int i;
    /*if (argc < 2){
        yylex();
        printf("Lexical analysis of one file completed!\n");
        return 0;
    }*/
    if(argc < 2){
        return 1;
    }
    for(i = 1; i < argc; i++){
        FILE *f = fopen(argv[i], "r");
        if (!f){
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        //yylex();
        yyparse();
        fclose(f);
        //printf("Lexical analysis of one file completed!\n");
        //printf("Gramma analysis of one file completed!\n");
	if(flag == 1){
		printf("Lexical analysis of one file has gone wrong!\n");
	}else if(flag == 2){
		printf("Gramma analysis of one file has gone wrong!\n");
	}else{
		//printf("%d\n",flag);
		printf("Lexical analysis and Gramma analysis of one file compelted!\n");
	}
	flag = 0;
    }
    return 0;
}
