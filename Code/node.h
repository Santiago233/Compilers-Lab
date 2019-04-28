#ifndef _NODE_H
#define _NODE_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"syntax.tab.h"
#define N 0x3fff
typedef struct Type_* Type;
typedef struct FieldList*  FieldList;
typedef struct Node{
		int line;
		char name[20];
		YYSTYPE val;
		char val2[20];
		struct Node* rnode;
		struct Node* childnode;
		struct Node* fathernode; 
		int pflag;
	}Node;

typedef struct Type_
{
	enum {
		BASIC, ARRAY, STRUCTURE
	} kind;
	union {
		int basic;	//1 for int, 2 for float
		struct {
			Type elem;
			int size;
		} array;	//array
		FieldList structure;	//struct
	} u;
}Type_;


typedef struct FieldList_
{
	char* name;
	Type type;
	FieldList tail;
}FieldList_;

typedef struct myArray
{
	Type mytype;
	char* name;
	/* data */
}myArray;
typedef struct VarList{
	char* name;
	Type_ type; 
}VarList;

typedef struct VarDecList{
	char* name;
	int line;
	Type_ type;
	struct VarDecList* next;
}VarDecList;

typedef struct FuncList{
	char* name;
	Type_ type;
	VarDecList* FuncVarList; 
}FuncList;

extern Node* stack[1024];
extern int scount ;
extern myArray* Arraylist[1024];

extern Node* root;
extern char* mystr[1024];
extern int mystrc ;
extern struct Type_ TYPE_MYINT;
extern struct Type_ TYPE_MYFLOAT;
static void Treefather(Node*);
static void AllInsert(VarList MyVarlist[], FuncList MyFunctionlst[], int array, Node* root);
static int AllCheck(VarList[], FuncList[], int, Node*);
static void MyVarInsert(VarList[], unsigned int, char*, int, int);
static void MyFuncInsert(FuncList[], unsigned int, char*, int, VarDecList*, int);
static unsigned int hash_pjw(char*);
static int JudgeMyFuncVar(VarList[], VarDecList*);
static void MyVarDecListAdd(VarDecList*, Node*);
static void MyVarDecListAdd_(VarDecList*, Node*);
static void MyFuncVarListAdd(VarDecList*, Node*);
static int MyVarCheck(VarList MyVarList[], Node* point);
//static void add_node(YYSTYPE p);




static void add_node(struct Node* p)
{
	//Node* tree = *(Node**)&p;
	//printf("%d  %s\n",p->line,p->name);
	stack[scount++] = p;
}	
static void insert(Node* p, int num)
{   
	
		int i;
		scount--;
		for(i = 0; i< num-1;i++){
			//printf("error\n");
			stack[scount - 1]->rnode = stack[scount];
			scount--;
			}
		//printf("%s\n",p->name);
		p->childnode = stack[scount];	
		//printf("%s\n",p->name);
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


static struct Node* newNode_id (char* myname,char* val, int lineno)
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
		strncpy(p->val2,val,20);
		//printf("%s\n",p->val);
		p->rnode = NULL;
		p->childnode = NULL;
		//scount++;
		//stack[scount] = p;
    		return p;
}


static void generate(Node* root, int number){
	Node* point1 = root; 
	while(point1 != NULL && point1->line != 0){
		char* temp;
		temp = point1->name;
		//printf("%s\n",temp);	
		if(point1->line == N ){
				generate(point1->childnode, number + 2);
				point1 = point1->rnode;}
		else{
			int i;
			for(i = 1; i <= number; i=i+1){
				printf(" ");
			} 
			printf("%s", point1->name);
			//int pflag = 0;
			if(!strcmp(temp,"ID")){
				//printf("\n");
				printf(": %s\n",point1->val2);
			}else if(!strcmp(temp,"INT")){
				printf(": %d\n", *(int*)&point1->val);//pflag = 1;
			}else if(!strcmp(temp,"FLOAT")){
				printf(": %f\n", *(float*)&point1->val);//pflag = 1;
			}else if(!strcmp(temp,"TYPE")){
				printf(": %s\n",point1->val2);//pflag = 1;
			}else{
				printf(" (%d)\n", point1->line);
			}
			//printf("%s\n",point1->name);
			if(!strcmp(temp,"StmtList")){
				//printf("%s\n",point1->rnode->name);
			}
			generate(point1->childnode, number + 2);
			point1 = point1->rnode;}
			
			
	}


}

static void Arraygenerate(Node* root, int number) {
	Node* point1 = root;
	while (point1 != NULL && point1->line != 0) {
		char* temp;
		temp = point1->name;
		int i = 0;
		int a;
		if (!strcmp(temp, "VarDec")) {
			point1 = point1->childnode;
			temp = point1->name;
			struct myArray* head = (struct myArray*)malloc(sizeof(struct myArray));
			head->mytype = NULL;
			//Type temp = head->mytype;
			while (!strcmp(temp, "VarDec")) {
				if(point1->rnode!=NULL){
				a = *(int*)&point1->rnode->rnode->val;
				Type p = (Type)malloc(sizeof(struct Type_));

				
				p->kind = ARRAY;
				p->u.array.elem = NULL;
				p->u.array.size = a;
				
				if (head->mytype == NULL) {
					head->mytype = p;
					int x = 0;//hashnode
					if(Arraylist[x] == NULL)
						Arraylist[x] = head;
					else
					{
						x++;
						while (Arraylist[x] != NULL)
						{
							if(Arraylist[x]->name == head->name)
								/* code */
								break;
							else
								x++;
						}
						Arraylist[x] = head;
					}
					i++;

				}
				
				else {
					{
						/* code */
						Type temp1 = head->mytype;
						head->mytype = p;
						p->u.array.elem = temp1;

					}
				}
				point1 = point1->childnode;
				temp = point1->name;
				//printf("%d \n", a);
			}
				else
				{
					point1=point1->childnode;
					temp = point1->name;
				}
			}
			if (i > 0) {
				head->name = point1->val2;
				//printf("%s\n", point1->val2);


			}
			}

		


		if (point1->childnode != NULL) {
			Node* point2 = point1->childnode;
			Arraygenerate(point2, number + 2);
		}
		point1 = point1->rnode;
	}
}

void Treefather(Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		point->pflag = 0;
		Node* p = point->childnode;
		while(p != NULL && p->line != 0){
			p->fathernode = point;
			p = p->rnode;
		}
		
				
		Treefather(point->childnode);
			
		point = point->rnode;
	}
}

static void AllInsert(VarList MyVarList[], FuncList MyFuncList[], int array, Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		
		if(!strcmp(point->name,"ExtDef")){
			point->pflag = 1;
			int TODO_ = 1;	//default type is "int"
			char* type_name = point->childnode->childnode->name;
			//printf("%s\n",type_name);
			if(!strcmp(type_name , "TYPE: INT")) {
				TODO_ = 1;
			}else if(!strcmp( type_name, "TYPE: FLOAT")){
				TODO_ = 2;
			}else{
				TODO_ = 3;	//type of struct
			}
			Node* ExtDef = point->childnode->rnode;
			
			if(!strcmp(ExtDef->name, "ExtDecList")){
				
				Node* trace = ExtDef->childnode;
				printf("addlist %s\n",trace->name);
				VarDecList* MyVarDecList = (struct VarDecList*)malloc(sizeof(struct VarDecList));
				MyVarDecListAdd(MyVarDecList, trace);	//need to modify for ArrayList-1
				
				while(trace->rnode != NULL){
					trace = trace->rnode->rnode->childnode; 
					MyVarDecListAdd(MyVarDecList, trace);	//need to modify for ArrayList-2
				}
				VarDecList* point_ = MyVarDecList;
				while(point_ != NULL){
					char* name = point_->name;
					int line = point_->line;
					unsigned int val = hash_pjw(name);
					MyVarInsert(MyVarList, val, name, TODO_, line);
					point_ = point_->next;
				}
			}else if(!strcmp(ExtDef->name , "FunDec")){
				char* name = ExtDef->childnode->val2;
				int line = ExtDef->childnode->line; 
				unsigned int val = hash_pjw(name);

				VarDecList* MyFuncVarList = (struct VarDecList*)malloc(sizeof(struct VarDecList));	//define for copying
				int flag = 0;
				Node* trace = ExtDef->childnode->rnode->rnode;
				
				if(!strcmp(trace->name , "RP")){
					flag = 1;
				}else{
					Node* trace_ = trace->childnode;
					MyFuncVarListAdd(MyFuncVarList, trace_);	//need to modify for ArrayList-3
					while(trace_->rnode != NULL){
						trace_ = trace_->rnode->rnode->childnode;
						MyFuncVarListAdd(MyFuncVarList, trace_);	//need to modify for ArrayList-4
					}
					flag = JudgeMyFuncVar(MyVarList, MyFuncVarList);
				}
				if(flag == 1){	//judge whether variables are repeated
					
					MyFuncInsert(MyFuncList, val, name, TODO_, MyFuncVarList, line);
					
				}
			}
			//point = point->rnode;
			//continue;
		}
		if(!strcmp(point->name , "Def")){
			point->pflag = 1;
			int TODO_ = 1;	//default type is "int"
			char* type_name = point->childnode->childnode->name;
			
			if(!strcmp(type_name,"TYPE: INT")) {
				TODO_ = 1;
			}else if(!strcmp(type_name , "TYPE: FLOAT")){
				TODO_ = 2;
			}else{
				TODO_ = 3;	//type of struct
			}
			Node* DecDef = point->childnode->rnode;
			
			Node* trace = DecDef->childnode;
			printf("sdsdasd%s\n",point->childnode->name);
			VarDecList* MyVarDecList = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			MyVarDecListAdd_(MyVarDecList, trace);
			while(trace->rnode != NULL){
				trace = trace->rnode->rnode->childnode; 
				MyVarDecListAdd_(MyVarDecList, trace);
			}
			VarDecList* point_ = MyVarDecList;
			while(point_ != NULL){
				char* name = point_->name;
				int line = point_->line;
				unsigned int val = hash_pjw(name);
				printf("%d%s\n", val,name); 
				MyVarInsert(MyVarList, val, name, TODO_, line);
				point_ = point_->next;
			}
			//point = point->rnode;
			//continue;
		}
		
		printf("%s\n",point->name);
		AllInsert(MyVarList, MyFuncList, 0, point->childnode);
		point = point->rnode;
	}
}

static int AllCheck(VarList MyVarList[], FuncList MyFuncList[], int array, Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0 && point->pflag == 0){
		if(!strcmp(point->name , "ID")){
			if(point->rnode == NULL){
				return MyVarCheck(MyVarList, point);
			}
		}
		if(!strcmp(point->name , "Exp")){
			point->pflag = 1;
			return AllCheck(MyVarList, MyFuncList, array, point);
		}
		point = point->rnode;
	}
}

static void MyVarInsert(VarList MyVarList[], unsigned int val, char* name, int TODO_, int line){
	if(MyVarList[val].name == '\0'){
		printf("%d%s\n",val,name);
		MyVarList[val].name = name;
		if(TODO_ == 1){
			MyVarList[val].type = TYPE_MYINT;
		}else{
			MyVarList[val].type = TYPE_MYFLOAT;
		}
		//printf("%d\n", val);
	}else{
		if(MyVarList[val].name == name){
			printf("Error type 3 at Line %d: Redefined variable \"%s\"\n", line, name); 
			return;
		}else{
			int val_ = (val + 1) % N;
			while(val_ != val){
				if(!strcmp(MyVarList[val_].name , name)){
					printf("Error type 3 at Line %d: Redefined variable \"%s\"\n", line, name);
					return;
				}
				if(!strcmp(MyVarList[val_].name , '\0')){
					MyVarList[val_].name = name;
					if(TODO_ == 1){
						MyVarList[val_].type = TYPE_MYINT;
					}else{
						MyVarList[val_].type = TYPE_MYFLOAT;
					}
					//printf("%d\n", val_); 
					return;
				}
				val_ = (val + 1) % N;
			}
		}
		printf("Error: No Space for new variable!\n");
	}
}

static void MyFuncInsert(FuncList MyFuncList[], unsigned int val, char* name, int TODO_, VarDecList* MyFuncVarList, int line){
		
	if(MyFuncList[val].name == NULL){
		MyFuncList[val].name = name;
		if(TODO_ == 1){
			MyFuncList[val].type = TYPE_MYINT;
		}else{
			MyFuncList[val].type = TYPE_MYFLOAT; 
		}
		MyFuncList[val].FuncVarList = (struct VarDecList*)malloc(sizeof(struct VarDecList)); 
		VarDecList* p = MyFuncVarList;
		while(p){	//may go wrong for the reason that MyFuncVarList is empty
			if(MyFuncList[val].FuncVarList->name == "\0"){
				
				MyFuncList[val].FuncVarList->name = p->name;
				MyFuncList[val].FuncVarList->type = p->type;
			}else{
				
				VarDecList* point = MyFuncList[val].FuncVarList;
				while(point->next != NULL){
					point = point->next;
				} 
				VarDecList* q = (struct VarDecList*)malloc(sizeof(struct VarDecList));
				q->name = p->name;
				q->type = p->type;
				point->next = q;
			}
			p = p->next;
		}
	}else{
		if(!strcmp(MyFuncList[val].name , name)){
			printf("Error type 4 at Line %d: Redefined function \"%s\"\n", line, name);
			return;
		}else{
			int val_ = (val + 1) % N;
			while(val_ != val){
				if(!strcmp(MyFuncList[val_].name , name)){
					printf("Error type 4 at Line %d: Redefined function \"%s\"\n", line, name);
					return;
				}
				if(MyFuncList[val_].name == NULL){
					MyFuncList[val_].name = name;
					if(TODO_ == 1){
						MyFuncList[val_].type = TYPE_MYINT;
					}else{
						MyFuncList[val_].type = TYPE_MYFLOAT; 
					}
					MyFuncList[val_].FuncVarList = (struct VarDecList*)malloc(sizeof(struct VarDecList)); 
					VarDecList* p = MyFuncVarList;
					while(p){
						if(MyFuncList[val_].FuncVarList->name == NULL){
							MyFuncList[val_].FuncVarList->name = p->name;
							MyFuncList[val_].FuncVarList->type = p->type;
						}else{
							VarDecList* point = MyFuncList[val_].FuncVarList;
							while(point->next != NULL){
								point = point->next;
							} 
							VarDecList* q = (struct VarDecList*)malloc(sizeof(struct VarDecList));
							q->name = p->name;
							q->type = p->type;
							point->next = q;
						}
						p = p->next;
					}
					return;
				}
				val_ = (val_ + 1) % N;
			}
		}
		printf("Error: No Space for new function!\n");
	}
}

static unsigned int hash_pjw(char* name){
	unsigned int val =0, i;
	for(; *name; name++){
		val = (val << 2) + *name;
		if(i = val & ~0x3fff){
			val = (val ^ (i >> 12)) & 0x3fff;
		}
	}
	return val;
}

static int JudgeMyFuncVar(VarList MyVarList[], VarDecList* MyFuncVarList){
	VarDecList * p =MyFuncVarList;
	int flag_ = 1;
	while(p){
		int i;
		for(i=0; i<N; i++){
			if(!strcmp(p->name , MyVarList[i].name)){
				printf("Error type 3 at Line %d: Redefined variable \"%s\"\n", p->line, p->name);
				flag_ = 0;
				break;
			}
		}
		p = p->next;
	}
	if(flag_ == 0){
		return 0;
	}else{
		return 1;
	}
}

static void MyVarDecListAdd(VarDecList* MyVarDecList, Node* trace){	
	if(!strcmp(trace->childnode->name , "ID")){
		Node* ID = trace->childnode;
		
		if(MyVarDecList->name ==NULL){
			MyVarDecList->name = ID->val2;
			MyVarDecList->line = ID->line;
		}else{
			VarDecList* point = MyVarDecList;
			while(point->next != NULL){
				point = point->next;
			}
			VarDecList* p = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			p->name = ID->val2;
			p->line = ID->line;
			point->next = p;
		}
	}else{
	}
}

static void MyVarDecListAdd_(VarDecList* MyVarDecList, Node* trace){
	if(!strcmp(trace->childnode->childnode->name , "ID")){
		Node* ID = trace->childnode;
		if(MyVarDecList->name == NULL){
			MyVarDecList->name = ID->val2;
			MyVarDecList->line = ID->line;
		}else{
			VarDecList* point = MyVarDecList;
			while(point->next != NULL){
				point = point->next;
			}
			VarDecList* p = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			p->name = ID->val2;
			p->line = ID->line;
			point->next = p;
		}
	}else{
	}
}

static void MyFuncVarListAdd(VarDecList* MyFuncVarList, Node* trace_){	//without thinking about struct and so on
	Node* ParamDec = trace_;
	Node* type = ParamDec->childnode->childnode;
	Node* Var = ParamDec->childnode->rnode->childnode;
	int TODO = 0;
	if(!strcmp(type->name , "TYPE: INT")){
		TODO = 0; 
	}else if(!strcmp(type->name , "TYPE :FLOAT")){
		TODO = 1;
	}else{
		TODO = 2;	//type of struct
	}
	if(MyFuncVarList->name == NULL){
		MyFuncVarList->name = Var->val2;
		if(TODO == 0){
			MyFuncVarList->type = TYPE_MYINT;
		}else if(TODO == 1){
			MyFuncVarList->type = TYPE_MYFLOAT;
		}
		MyFuncVarList->line = Var->line;
	}else{
		VarDecList* point = MyFuncVarList;
		while(point->next != NULL){
			point = point->next;
		} 
		VarDecList* p = (struct VarDecList*)malloc(sizeof(struct VarDecList));
		p->name = Var->val2;
		if(TODO == 0){
			p->type = TYPE_MYINT;
		}else if(TODO == 1){
			p->type = TYPE_MYFLOAT;
		}
		p->line = Var->line;
		point->next = p;
	}
}

static int MyVarCheck(VarList MyVarList[], Node* point){
		return 0;	   //to modify
		int i;
	for(i=0; i<N; i++){
		if(/*MyVarList[i] == point->val2*/1){
			if(!strcmp(MyVarList[i].name , "TYPE_MYINT")){
				return 1;
			}else{
				return 2;
			}
		}
	}
	printf("Error type 1 at Line %d: Undefined variable \"%s\"", point->line, point->val2);
	return 0;
}
#endif




