#ifndef _NODE_H
#define _NODE_H
#include<stdio.h>
#include<string.h>
#include"syntax.tab.h"

typedef struct Node{
		int line;
		char* name;
		YYSTYPE val;
		struct Node* rnode;
		struct Node* childnode	;
	}Node;
static Node* stack[1024];
static int scount;
extern int flag = 0;
static Node* root;

	
static void insert(char* name,YYSTYPE val,int num)
{   
	if(root == NULL){
		root = (struct Node*)malloc(sizeof(struct Node));
	}
	struct Node *p;
	
	if (stack[scount] == NULL)
		return;
	else{
		int i;
	
		for(i = 0; i< num-1;i++){
			stack[scount] = stack[scount - 1]->rnode;
			scount--;
			}
		stack[scount] = stack[scount -1]->childnode;		
	}
}

static struct Node* newNode (char* myname,int lineno)
{
		struct Node *p=(struct Node*)malloc(sizeof(struct Node));
		if (p==NULL)
		{
			printf("Error:out of memory.\n");
			exit(1);
		}
   	 	strncpy(p->name,myname,sizeof(myname));

		p->line = lineno;
		p->val = (YYSTYPE)0;
		p->rnode = NULL;
		p->childnode = NULL;
    		return p;
}


static struct Node* newNode_ (char* myname,YYSTYPE myval, int lineno)
	{
		struct Node *p=(struct Node*)malloc(sizeof(struct Node));
		if (p==NULL)
		{
			printf("Error:out of memory.\n");
			exit(1);
		}
   	 	strncpy(p->name,myname,sizeof(myname));
		p->line = lineno;
		p->val = myval;
		p->rnode = NULL;
		p->childnode = NULL;
    		return p;
}


#endif




