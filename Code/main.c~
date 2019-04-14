#include <stdio.h>
#include<string.h>
#include "node.h"
int flag = 0;
int scount = 0;
Node* stack[1024] = {NULL};
Node* root = NULL;
char* mystr[1024];
int mystrc = 0;

int main(int argc, char ** argv){
int i;
	extern struct Node* root ;
	root = (struct Node*)malloc(sizeof(struct Node));
    
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
	//printf("error is here %d\n",scount);
	root->childnode = stack[1];
	mystrc = 0;
	//printf("%s  %d\n",stack[1]->childnode->rnode->name,stack[1]->childnode->rnode->line);
	generate(root,1);

	//printf("%s  %d\n",root->name,root->line);
	//printf("%s  %d\n",root->childnode->name,root->childnode->line);
	//for(i = 1;stack[i] != NULL;i++){
	//	printf("%s  %d\n",stack[i]->name,stack[i]->line);
	//}
    return 0;
}
