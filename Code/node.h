#ifndef _NODE_H
#define _NODE_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"syntax.tab.h"

typedef struct Node{
		int line;
		char name[20];
		YYSTYPE val;
		struct Node* rnode;
		struct Node* childnode	;
	}Node;
extern Node* stack[1024];
extern int scount ;

extern Node* root;
extern char* mystr[1024];
extern int mystrc ;

	
static void insert(int num,Node* p)
{   
	
		int i;
	
		for(i = 0; i< num-1;i++){
			//printf("error\n");
			stack[scount - 1]->rnode = stack[scount];
			scount--;
			}
		//printf("%s\n",p->name);
		p->childnode = stack[scount];	
		stack[scount] = p;	
	
}

static struct Node* newNode (char* myname,int lineno)
{	
	
	if(root == NULL){
		root = (struct Node*)malloc(sizeof(struct Node));
	}
		
		struct Node *p=(struct Node*)malloc(sizeof(struct Node));
		if (p==NULL)
		{
			printf("Error:out of memory.\n");
			exit(1);
		}
   	 	strncpy(p->name,myname,20);

		p->line = lineno;
		p->val = (YYSTYPE)0;
		p->rnode = NULL;
		p->childnode = NULL;
		//scount++;
		//stack[scount] = p;
    		return p;
}


static struct Node* newNode_ (char* myname,YYSTYPE myval, int lineno)
	{
		//printf("Error is not here.\n");
		struct Node *p=(struct Node*)malloc(sizeof(struct Node));
		
		if (p==NULL)
		{
			printf("Error:out of memory.\n");
			exit(1);
		}
   	 	strncpy(p->name,myname,20);
		//printf("Error is not here.\n");
		p->line = lineno;
		p->val = myval;
		//printf("%s\n",p->val);
		p->rnode = NULL;
		p->childnode = NULL;
		//scount++;
		//stack[scount] = p;
    		return p;
}

static void generate(Node* root, int number){
	Node* point1 = root; 
//	if(point1->line != 0){
		while(point1 != NULL){
		if(point1->line != 0){
		
		
		int i;
		for(i = 1; i <= number; i=i+1){
			printf(" ");
		} 
		char* temp;
		temp = point1->name;
		printf("%s", point1->name);
		int pflag = 0;
		if(!strcmp(temp,"ID")){
			printf("\n");
			//printf(": %s\n",point1->val);pflag = 1;
		}else if(!strcmp(temp,"INT")){
			printf(": %d\n", *(int*)&point1->val);pflag = 1;
		}else if(!strcmp(temp,"FLOAT")){
			printf(": %f\n", *(float*)&point1->val);pflag = 1;
		}else if(!strcmp(temp,"TYPE: INT")||!strcmp(temp,"TYPE: FLOAT")){
			printf("\n");pflag = 1;
		}else{
			printf(" (%d)\n", point1->line);
			;
		}
			//judge whether end or not
		if(point1->childnode != NULL){
			Node* point2 = point1->childnode;
			
			generate(point2, number + 2);
			
		}
		point1 = point1->rnode;
	}
	}
		

		
//	}

/*	if(root->line != 0){
		int i;
		for(i = 1; i <= number; i=i+1){
			printf(" ");
		} 
		char* temp;
		temp = root->name;
		printf("%s", root->name);
		int pflag = 0;
		if(!strcmp(temp,"ID")){
			//printf(": %s\n", root->val);pflag = 1;
			printf(": %s\n",root->val);pflag = 1;
		}else if(!strcmp(temp,"INT")){
			printf(": %d\n", *(int*)&root->val);pflag = 1;
		}else if(!strcmp(temp,"FLOAT")){
			printf(": %f\n", *(float*)&root->val);pflag = 1;
		}else if(!strcmp(temp,"TYPE: INT")||!strcmp(temp,"TYPE: FLOAT")){
			printf("\n");pflag = 1;
		}else{
			printf(" (%d)\n", root->line);
			;
		}
			//judge whether end or not
		if(root->childnode != NULL){
			Node* point = root->childnode;
			
			generate(point, number + 2);
			
		}
		if(root->rnode != NULL){
			Node* point = root->rnode;
			
			generate(point, number + 1);
				
		}
		

		
	}
*/
}


#endif




