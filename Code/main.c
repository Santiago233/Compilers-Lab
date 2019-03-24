#include <stdio.h>

int main(int argc, char ** argv){
    int i;
    if (argc < 2){
        yylex();
        printf("Lexical analysis of one file completed!\n");
        return 0;
    }
    for(i = 1; i < argc; i++){
        FILE *f = fopen(argv[i], "r");
        if (!f){
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yylex();
        fclose(f);
        printf("Lexical analysis of one file completed!\n");
    }
    return 0;
}