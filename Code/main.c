#include <stdio.h>
#include<string.h>
#include "node.h"
#include "intercode.h"
#define N 0x3fff
int flag = 0;
int scount = 0;
Node* stack[1024] = {NULL};
Node* root = NULL;
char* mystr[1024];
myArray* MyArrayList[N] = {NULL};
VarList MyVarList[N] = {NULL};	//print for check name
FuncList MyFuncList[N] = {NULL};
myStruct* MyStructList[N] = {NULL};
int mystrc = 0;
int lineflag = 0;
struct Type_ TYPE_MYINT = {
	BASIC , 1
};

struct Type_ TYPE_MYFLOAT = {
	BASIC , 2
};

void Translate(Node*, char*);

int main(int argc, char** argv){
/*	
	int i;
    if(argc < 2){
        return 1;
    }
    for(i = 1; i < argc; i++){
	extern struct Node* root ;
	root = (struct Node*)malloc(sizeof(struct Node));
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
		
		//root->childnode = stack[0];
		
		//generate(root,0);
		
		Treefather(root); 
		Arraygenerate(root,0);	
		lineflag = 1;
		//printf("%s\n",MyArrayList[97]->name);
	
		AllInsert(MyVarList, MyFuncList, 0, root);
		//printf("error there\n");	//0 for the loss of array
		AllInsert_(MyVarList, MyFuncList, 0, root);

		Structgenerate(root,0);
		AllCheck(MyVarList, MyFuncList, 0, root);
		//printf("error there\n");
		AllSetpflag(root);
		FuncCheckRe(root);
	}
	flag = 0;
	int j;
	for(j = 0; j < 1024; j++){
		Node* p = stack[j];
		free(p);
		stack[j] = NULL;
	}
	scount = 0;
	//printf("%d\n",i);
    }
*/

	if(argc < 3){
        return 1;
    }
    extern struct Node* root ;
	root = (struct Node*)malloc(sizeof(struct Node));
    FILE *f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    fclose(f);
	if(flag == 1){
		printf("Lexical analysis of one file has gone wrong!\n");
	}else if(flag == 2){
		printf("Gramma analysis of one file has gone wrong!\n");
	}else{
		printf("Lexical analysis and Gramma analysis of one file compelted!\n");
		//Treefather(root); 
		//Arraygenerate(root,0);	
		lineflag = 1;	
		//AllInsert(MyVarList, MyFuncList, 0, root);
		//AllInsert_(MyVarList, MyFuncList, 0, root);
		//Structgenerate(root,0);
		//AllCheck(MyVarList, MyFuncList, 0, root);
		//AllSetpflag(root);
		//FuncCheckRe(root);
		printf("Translate compelted!\n");
		Translate(root, argv[2]);
	}
	flag = 0;
	int j;
	for(j = 0; j < 1024; j++){
		Node* p = stack[j];
		free(p);
		stack[j] = NULL;
	}
	scount = 0;
	return 0;
}

void Translate(Node* root, char* name){
	InterCodes result = translate_Program(root);
	if(freopen(name,"w",stdout)==NULL)
        printf("freopen error\n");
	codeoutput(result);
	fclose(stdout);
}