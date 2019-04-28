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
		int pflag; //define 0 or 1 for int or float;
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

typedef struct ArrayList{
	char* name;
	int line;
	Type type;
	myArray* myarray;
	struct ArrayList* next;
}ArrayList;

typedef struct FuncList{
	char* name;
	Type_ type;
	VarDecList* FuncVarList; 
}FuncList;

extern Node* stack[1024];
extern int scount ;
extern myArray* MyArrayList[N];

extern Node* root;
extern char* mystr[1024];
extern int mystrc ;
extern VarList MyVarList[N];	//print for check name
extern FuncList MyFuncList[N];
extern struct Type_ TYPE_MYINT;
extern struct Type_ TYPE_MYFLOAT;
static void Treefather(Node*);
static void AllInsert(VarList MyVarlist[], FuncList MyFuncList[], int array, Node* root);
static void AllCheck(VarList[], FuncList[], int, Node*);
static void AllSetpflag(Node*); 
static void AllInsert_(VarList MyVarList[], FuncList MyFuncList[], int array, Node* root);
static void AllInsert_array(Node* root);
static void MyVarInsert(VarList[], unsigned int, char*, int, int);
static void MyFuncInsert(FuncList[], unsigned int, char*, int, VarDecList*, int);
static unsigned int hash_pjw(char*);
static int JudgeMyFuncVar(VarList[], VarDecList*);
static void MyVarDecListAdd(VarDecList* MyVarDecList,  Node* trace);
static void MyVarDecListAdd_(VarDecList* MyVarDecList,  Node* trace);
static void MyVarDecListAdd__(VarDecList*, Node*, VarList[]);
static void MyVarDecListAdd_array(Node* trace);
static void MyFuncVarListAdd(VarDecList*, Node*);
static void MyArrayInsert(ArrayList* point_, unsigned int val, char* name, int TODO_, int line);
static void NodepflagAdd(VarList[], FuncList[], Node*);
static void MyFuncCompare(FuncList[], VarDecList*, int);
static void FuncCheckRe(Node*);
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
			}else if(!strcmp(temp,"TYPE: INT") || !strcmp(temp,"TYPE: FLOAT")){
				printf("\n");//pflag = 1;
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
	while(point1 != NULL && point1->line != N){
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
						i++;
					}
					else {
							/* code */
							Type temp1 = head->mytype;
							head->mytype = p;
							p->u.array.elem = temp1;
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
				int line = point1->line;
				int val = hash_pjw(head->name);//hashnode
				printf("%s %d\n",head->name,val);
					if(MyVarList[val].name != NULL){
						if(!strcmp(MyVarList[val].name , head->name)){
							if(!strcmp(MyVarList[val].name , head->name)){printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name); 
								return;}
						}
					if(MyFuncList[val].name != NULL){
							if(!strcmp(MyFuncList[val].name , head->name))
								{printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name); 
								return;}
						}
					if(!strcmp(MyArrayList[val]->name,head->name)){
							//printf("%s %s\n",name,MyArrayList[val]->name);
							printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name); 
								return;
						}
						MyArrayList[val] = head;
					}
					else
					{
						while (MyVarList[val].name != NULL)
						{
							if(!strcmp(MyVarList[val].name,head->name)){
								if(!strcmp(MyVarList[val].name , head->name)){printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name); 
								return;}
							}
							if(MyFuncList[val].name != NULL){
								if(!strcmp(MyFuncList[val].name , head->name))
									{printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name); 
									return;}
							}
							if(!strcmp(MyArrayList[val]->name,head->name)){
							//printf("%s %s\n",name,MyArrayList[val]->name);
							printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name); 
								return;
							}
							else{
								int val_ = (val + 1) % N;
								while(val_ != val){
									if(!strcmp(MyVarList[val].name , head->name)||!strcmp(MyFuncList[val].name , head->name)||!strcmp(MyArrayList[val]->name,head->name)){
										printf("Error type 3 at Line %d: Redefined array \"%s\".\n", line, head->name);
										return;
									}
									if(MyArrayList[val_] == NULL){
										MyArrayList[val_] = head;
										return;
									}
									val_ = (val + 1) % N;
								}
							}
							printf("Error: No Space for new variable!\n");
							return;
						}
					}
				}
		
		}
		Arraygenerate(point1->childnode, number + 2);
		point1 = point1->rnode;
		}
}

void Treefather(Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		point->pflag = -1;
		Node* p = point->childnode;
		while(p != NULL && p->line != 0){
			p->fathernode = point;
			p = p->rnode;
		}
		
				
		Treefather(point->childnode);
			
		point = point->rnode;
	}
}

static void AllInsert(VarList MyVarList[],FuncList MyFuncList[], int array, Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		
		if(!strcmp(point->name,"ExtDef") ){
			int TODO_ = 1;	//default type is "int"&& !strcmp(point->childnode->rnode->childnode->rnode->name , "ID"))
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
			//printf("addlist %s\n",ExtDef->name);
			if(!strcmp(ExtDef->name, "ExtDecList")){
				if( strcmp(point->childnode->childnode->name , "ID"))return;				
				if(ExtDef->line == N)return;
				Node* trace = ExtDef->childnode;
				//printf("addlist %s\n",trace->name);
				VarDecList* MyVarDecList = (struct VarDecList*)malloc(sizeof(struct VarDecList));
				MyVarDecList->name = NULL;
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
				//printf("%s\n", name);
				int line = ExtDef->childnode->line; 
				unsigned int val = hash_pjw(name);

				VarDecList* MyFuncVarList = (struct VarDecList*)malloc(sizeof(struct VarDecList));	//define for copying
				MyFuncVarList->name = NULL;
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
				/*VarDecList* pp = MyFuncVarList;
				int number_ = 0;
				while(pp){number_ += 1; pp = pp->next;}
				printf("%s %d\n", name, number_);*/
				
				if(flag == 1){	//judge whether variables are repeated
					
					MyFuncInsert(MyFuncList, val, name, TODO_, MyFuncVarList, line);
					
				}
			}
			point = point->rnode;
			continue;
		}
		AllInsert(MyVarList, MyFuncList, 0, point->childnode);
		point = point->rnode;
		
	}
}

static void AllInsert_(VarList MyVarList[], FuncList MyFuncList[], int array, Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		if(!strcmp(point->name , "Def") ){
			if( strcmp(point->childnode->rnode->childnode->childnode->childnode->name , "ID"))return;
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
			
			VarDecList* MyVarDecList = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			MyVarDecList->name = NULL;
			MyVarDecListAdd_(MyVarDecList, trace);
			while(trace->rnode != NULL){
				trace = trace->rnode->rnode->childnode; 
				MyVarDecListAdd_(MyVarDecList, trace);
			}
			VarDecList* point_ = MyVarDecList;
			while(point_ != NULL){
				char* name = point_->name;
				int line = point_->line;
				//printf("addlist %s\n",name);
				unsigned int val = hash_pjw(name);
				//printf("%d%s\n", val,name); 
				MyVarInsert(MyVarList, val, name, TODO_, line);
				point_ = point_->next;
			}
			point = point->rnode;
			continue;
		}
		AllInsert_(MyVarList, MyFuncList, 0, point->childnode);
		point = point->rnode;
	}
}

/*static void AllInsert_array_(Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		if(!strcmp(point->name , "Def") ){
			if( !strcmp(point->childnode->rnode->childnode->childnode->childnode->name , "ID"))return;
			Node* DecDef = point->childnode->rnode;
			
			Node* trace = DecDef->childnode;
			
			ArrayList* MyArrayNodeList = (struct ArrayList*)malloc(sizeof(struct ArrayList));
			MyArrayNodeList->name = NULL;
			MyArrayNodeListAdd_(MyArrayNodeList, trace);  //trace:  Dec
			while(trace->rnode != NULL){
				trace = trace->rnode->rnode->childnode; 
				MyArrayNodeListAdd_(MyArrayNodeList, trace);
			}
			ArrayList* point_ = MyArrayNodeList;
			while(point_ != NULL){
				char* name = point_->name;
				int line = point_->line;
				printf("addlist %s\n",name);
				unsigned int val = hash_pjw(name);
				//printf("%d%s\n", val,name); 
				MyArrayInsert(point_, val, name, TODO_, line);
				point_ = point_->next;
			}
			point = point->rnode;
			continue;
		}
		AllInsert_array(point->childnode);
		point = point->rnode;
	}
}


static void MyArrayInsert_(ArrayList* point_, unsigned int val, char* name, int TODO_, int line){
	if(MyVarList[val].name == NULL){
		
		MyArrayList[val] = point_->myarray;
		
		//printf("%d\n", val);
	}else{
	//printf("%s %s\n", MyVarList[val].name, name);
		if(!strcmp(MyArrayList[val].name , name)){
			printf("Error type 3 at Line %d: Redefined Array \"%s\".\n", line, name); 
			return;
		}else{
			int val_ = (val + 1) % N;
			while(val_ != val){
				if(!strcmp(MyArrayList[val_].name , name)){
					printf("Error type 3 at Line %d: Redefined Array \"%s\".\n", line, name);
					return;
				}
				if(MyArrayList[val_] == NULL){
					MyArrayList[val_] = point_->myarray;
					return;
				}
				val_ = (val + 1) % N;
			}
		}
		printf("Error: No Space for new variable!\n");
	}
}
*/

static void AllCheck(VarList MyVarList[], FuncList MyFuncList[], int array, Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		//printf("%s\n", point->name);
		if(!strcmp(point->name, "Exp")){
			NodepflagAdd(MyVarList, MyFuncList, point);
			point = point->rnode;
			continue;
		}
		if(point->childnode != NULL){
			Node* point_ = point->childnode;
			AllCheck(MyVarList, MyFuncList, array, point_); 
		} 
		point = point->rnode;
	}
}

static void MyVarInsert(VarList MyVarList[], unsigned int val, char* name, int TODO_, int line){
	if(MyVarList[val].name == NULL){
			//printf("%s %d\n", name ,val);
			if(MyFuncList[val].name != NULL){
				
				if(!strcmp(MyFuncList[val].name , name))
				{printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
					return;}
					}
			if(MyArrayList[val] !=NULL){
				
				if(!strcmp(MyArrayList[val]->name,name)){
					//printf("%s %s\n",name,MyArrayList[val]->name);
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
								return;
							}}
		MyVarList[val].name = name;
		if(TODO_ == 1){
			MyVarList[val].type = TYPE_MYINT;
		}else{
			MyVarList[val].type = TYPE_MYFLOAT;
		}
		//printf("%d\n", val);
	}else{
		if(MyFuncList[val].name != NULL){
			
				if(!strcmp(MyFuncList[val].name , name))
				{printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
					return;}
					}
				if(MyArrayList[val] !=NULL){
					
				if(!strcmp(MyArrayList[val]->name,name)){
					//printf("%s %s\n",name,MyArrayList[val]->name);
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
								return;
							}}
			if(!strcmp(MyVarList[val].name , name)){
			printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
			return;
		}else{
			int val_ = (val + 1) % N;
			while(val_ != val){
				if(MyFuncList[val].name != NULL){
				if(!strcmp(MyFuncList[val].name , name))
				{printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
					return;}
					}
				if(MyArrayList[val] !=NULL){
				if(!strcmp(MyArrayList[val]->name,name)){
					//printf("%s %s\n",name,MyArrayList[val]->name);
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
								return;
							}}
				if(!strcmp(MyVarList[val].name , name)){
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name);
					return;
				}
				if(MyVarList[val_].name == NULL){
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
		//printf("%s\n", name);
		if(MyVarList[val].name!=NULL){
			if(!strcmp(MyVarList[val].name , name)){
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
				return;
			}
		}
		if(MyArrayList[val] !=NULL){
			if(!strcmp(MyFuncList[val].name , name)){
				//printf("%s %s\n",name,MyArrayList[val]->name);
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
				return;
			}
		}
		MyFuncList[val].name = name;
		if(TODO_ == 1){
			MyFuncList[val].type = TYPE_MYINT;
		}else{
			MyFuncList[val].type = TYPE_MYFLOAT; 
		}
		MyFuncList[val].FuncVarList = (struct VarDecList*)malloc(sizeof(struct VarDecList)); 
		VarDecList* p = MyFuncVarList;
		//int number = 0;
		while(p){	//may go wrong for the reason that MyFuncVarList is empty
			if(MyFuncList[val].FuncVarList->name == NULL){				
				MyFuncList[val].FuncVarList->name = p->name;
				MyFuncList[val].FuncVarList->type = p->type;
				MyFuncList[val].FuncVarList->next = NULL;
			}else{
				
				VarDecList* point = MyFuncList[val].FuncVarList;
				while(point->next != NULL){
					point = point->next;
				} 
				VarDecList* q = (struct VarDecList*)malloc(sizeof(struct VarDecList));
				q->name = p->name;
				q->type = p->type;
				q->next = NULL;
				point->next = q;
			}
			p = p->next;
			//number += 1;
		}
		//printf("number: %d\n", number);
	}else{
		if(MyVarList[val].name!=NULL){
				if(!strcmp(MyVarList[val].name , name))
				{printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
					return;}
					}
				if(MyArrayList[val] !=NULL){
				if(!strcmp(MyFuncList[val].name , name)){
					//printf("%s %s\n",name,MyArrayList[val]->name);
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name); 
								return;
							}}
		if(!strcmp(MyFuncList[val].name , name)){
			printf("Error type 4 at Line %d: Redefined function \"%s\".\n", line, name);
			return;
		}else{
			int val_ = (val + 1) % N;
			while(val_ != val){
				if(!strcmp(MyVarList[val].name , name)||!strcmp(MyFuncList[val].name , name)||!strcmp(MyArrayList[val]->name,name)){
					printf("Error type 4 at Line %d: Redefined function \"%s\".\n", line, name);
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
			if(MyVarList[i].name != NULL && !strcmp(p->name , MyVarList[i].name)){
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", p->line, p->name);
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

static void MyVarDecListAdd(VarDecList* MyVarDecList, Node* trace){	//define for all 
	if(!strcmp(trace->childnode->name , "ID")){
		Node* ID = trace->childnode;
		//printf("ID: %s\n", ID->val2);
		if(MyVarDecList->name ==NULL){
			MyVarDecList->name = ID->val2;
			MyVarDecList->line = ID->line;
			MyVarDecList->next = NULL;
		}else{
			VarDecList* point = MyVarDecList;
			while(point->next != NULL){
				point = point->next;
			}
			VarDecList* p = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			p->name = ID->val2;
			p->line = ID->line;
			point->next = p;
			p->next = NULL;
		}
	}else{
		}
	
}

static void MyVarDecListAdd_(VarDecList* MyVarDecList,Node* trace){	//define for function
	if(!strcmp(trace->childnode->childnode->name , "ID")){
		
		Node* ID = trace->childnode->childnode;
		if(MyVarDecList->name == NULL){

			MyVarDecList->name = ID->val2;
			MyVarDecList->line = ID->line;
			MyVarDecList->next = NULL;
		}else{
			VarDecList* point = MyVarDecList;
			while(point->next != NULL){
				point = point->next;
			}
			VarDecList* p = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			p->name = ID->val2;
			p->line = ID->line;
			p->next = NULL;
			point->next = p;
		}
	}else{
		//printf("%s\n",trace->childnode->childnode->name);
		//Arraygenerate(trace->childnode, 0);
	}
}

static void MyVarDecListAdd__(VarDecList* MyVarDecList, Node* trace, VarList MyVarList[]){	//test variables of used function
	if(!strcmp(trace->childnode->name, "INT") || !strcmp(trace->childnode->name, "FLOAT")){
		Node* p = trace->childnode;
		if(MyVarDecList->name == NULL){
			char* name = "nuMber";
			MyVarDecList->name = name;
			MyVarDecList->line = p->line;
			if(!strcmp(p->name, "INT")){
				MyVarDecList->type = TYPE_MYINT;
			}else{
				MyVarDecList->type = TYPE_MYFLOAT;
			}
			MyVarDecList->next = NULL;
			//printf("%s\n", MyVarDecList->name);
		}else{
			VarDecList* point = MyVarDecList;
			while(point->next != NULL){
				point = point->next;
			}
			VarDecList* q = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			q->name = "nuMber";
			q->line = p->line;
			if(!strcmp(p->name, "INT")){
				q->type = TYPE_MYINT;
			}else{
				q->type = TYPE_MYFLOAT;
			}
			q->next = NULL;
			point->next = q;
		}
	}else{	//I don't think that there will be functions as variables in the function
		Node* p = trace->childnode; 
		int i;
		int flag = 0;
		Type_ type_;
		for(i=0; i<N; i++){	//remember to compare with arraylist and structlist
			if(!strcmp(p->val2, MyVarList[i].name)){
				flag = 1;
				type_ = MyVarList[i].type;
			}
		}
		if(flag == 0){
			printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", p->line, p->val2);
			return;
		}
		if(MyVarDecList->name == NULL){
			MyVarDecList->name = p->val2;
			MyVarDecList->line = p->line;
			MyVarDecList->type = type_;
			MyVarDecList->next = NULL;
		}else{
			VarDecList* point = MyVarDecList;
			while(point->next != NULL){
				point = point->next;
			}
			VarDecList* q = (struct VarDecList*)malloc(sizeof(struct VarDecList));
			q->name = p->val2;
			q->line = p->line;
			q->type = type_;
			q->next = NULL;
			point->next = q;
		}
	}
}

static void MyFuncVarListAdd(VarDecList* MyFuncVarList, Node* trace_){	//without thinking about struct and so on
	Node* ParamDec = trace_;
	Node* type = ParamDec->childnode->childnode;
	Node* Var = ParamDec->childnode->rnode->childnode;
	int TODO = 0;
	//printf("%d\n", TODO);
	if(!strcmp(type->name , "TYPE: INT")){
		TODO = 0; 
	}else if(!strcmp(type->name , "TYPE :FLOAT")){
		TODO = 1;
	}else{
		TODO = 2;	//type of struct
	}
	if(MyFuncVarList->name == NULL){
		//printf("%s\n", Var->val2);
		MyFuncVarList->name = Var->val2;
		if(TODO == 0){
			MyFuncVarList->type = TYPE_MYINT;
		}else if(TODO == 1){
			MyFuncVarList->type = TYPE_MYFLOAT;
		}
		MyFuncVarList->line = Var->line;
		MyFuncVarList->next = NULL;
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
		p->next = NULL;
		point->next = p;
	}
}

static void NodepflagAdd(VarList MyVarList[], FuncList MyFuncList[], Node* point){
	Node* point_ = point;
	//printf("pflagAdd: %s\n", point_->name);
	while(point_ != NULL && point_->line != 0){
		if(!strcmp(point_->name, "ID")){
			//printf("%s\n", point_->val2);
			/*Node* w = point_;
			while(w){
				printf("%s\n", w->name);
				w = w->rnode;
			}*/
			if(point_->rnode == NULL){
				int Flag_ID = 0;
				if(point_->fathernode->rnode != NULL && !strcmp(point_->fathernode->rnode->name, "LB")){	//array
					
				}else if(point_->fathernode->rnode != NULL && !strcmp(point_->fathernode->rnode->name, "DOT")){	//structure name
					
				}else if(point_->fathernode->childnode != NULL && !strcmp(point_->fathernode->childnode->name, "Exp")){	//structure field name
					
				}else{
					int i;
					for(i=0; i<N; i++){
						if(MyVarList[i].name != NULL && !strcmp(point_->val2, MyVarList[i].name)){
							Flag_ID = 1;
							if(MyVarList[i].type.u.basic == 1){
								point_->pflag = 0;
							}else{
								point_->pflag = 1; 
							}
							break;
						}
					}
				}
				//printf("%d\n", point_->pflag);
				if(Flag_ID != 1){
					printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", point_->line, point_->val2);
				}
			}else{
				int i;
				int Flag_ID = 0;
				for(i=0; i<N; i++){
					if(MyFuncList[i].name != NULL && !strcmp(point_->val2, MyFuncList[i].name)){
						//printf("%s\n", point_->val2);
						Flag_ID = 1;
						if(!strcmp(point_->rnode->rnode->name, "Args")){
							Node* trace = point_->rnode->rnode->childnode;
							VarDecList* MyVarDecList = (struct VarDecList*)malloc(sizeof(struct VarDecList));
							MyVarDecList->name = NULL;
							MyVarDecListAdd__(MyVarDecList, trace, MyVarList);
							while(trace->rnode != NULL) {
								trace = trace->rnode->rnode->childnode;
								MyVarDecListAdd__(MyVarDecList, trace, MyVarList);
							}
							MyFuncCompare(MyFuncList, MyVarDecList, i);
						}
						if(MyFuncList[i].type.u.basic == 1){
							point_->pflag = 0;
						}else{
							point_->pflag = 1;
						}
					}
				}
				if(Flag_ID != 1){
					printf("Error type 2 at Line %d: Undefined function \"%s\".\n", point_->line, point_->val2);
				}
			}
			return;
		}else if(!strcmp(point_->name, "INT")){
			//printf("int-pflag\n");
			point_->pflag = 0;
		}else if(!strcmp(point_->name, "FLOAT")){
			//printf("float-pflag\n");
			point_->pflag = 1;
		}
		if(point_->childnode != NULL){
			Node* point__ = point_->childnode;
			NodepflagAdd(MyVarList, MyFuncList, point__);
		}
		point_ = point_->rnode;
	}
}

void MyFuncCompare(FuncList MyFuncList[], VarDecList* MyVarDecList, int value){
	/*VarDecList* pp = MyFuncList[value].FuncVarList;
	int number_ = 0;
	while(pp){number_ += 1; pp = pp->next;}
	printf("number: %d\n", number_);*/
	VarDecList* point = MyFuncList[value].FuncVarList;
	//printf("%s\n", MyFuncList[value].name);
	int number1 = 0;
	int number2 = 0;
	VarDecList* p1 = point;
	VarDecList* p2 = MyVarDecList;
	while(p1){
		if(p1->type.u.basic == 0){	//I don't know why the number of variables increases...
			p1 = p1->next;
		}else{
			number1 += 1;
			p1 = p1->next;
		}
	} 
	while(p2){
		number2 += 1;
		p2 = p2->next;
	}
	int flag = 0;
	//printf("%d %d\n", number1, number2);
	if(number1 != number2){
		flag = 1;
	}else{
		VarDecList* p3 = point;
		VarDecList* p4 = MyVarDecList;
		while(p3){
			if(p3->type.u.basic != p4->type.u.basic){
				flag = 1;
			}
			p3 = p3->next;
			p4 = p4->next;
		}
	}
	if(flag == 1){
		printf("Error type 9 at Line %d: Function \"%s(", MyVarDecList->line, MyFuncList[value].name);
		VarDecList* p3 = point;
		while(p3){
			if(p3->type.u.basic == 0){
				p3 = p3->next;
			}else{
				if(p3->type.u.basic == 1){
					printf("int");
				}else if(p3->type.u.basic == 2){
					printf("float");
				}
				if(p3->next != NULL){
					printf(", ");
				}
				p3 = p3->next;
			}
		}
		printf(")\" is not applicable for arguments \"(");
		VarDecList* p4 = MyVarDecList;
		while(p4){
			if(p4->type.u.basic == 1){
				printf("int");
			}else if(p4->type.u.basic == 2){
				printf("float");
			}
			if(p4->next != NULL){
				printf(", ");
			}
			p4 = p4->next;
		} 
		printf(")\".\n");
	}
}

static void AllSetpflag(Node* root){
	Node* point = root;
	//printf("root: %s\n", point->name);
	while(point != NULL && point->line != 0){
		if(!strcmp(point->name, "Exp")){
			if(point->pflag != -1){
				return;
			}
			Node* p =point->childnode;
			//printf("%s %d\n", p->name, p->pflag);
			if(p->name == "Exp" && p->rnode->name == "LB"){
				if(p->pflag != -1){
					point->pflag = p->pflag;
				}else{
					AllSetpflag(p);
				}
				return;
			}else if(p->name == "Exp" && p->rnode->name == "DOT"){
				point->pflag = p->rnode->rnode->pflag; 
				return;
			}else if(p->name == "ID" || p->name == "INT" || p->name == "FLOAT"){
				point->pflag = p->pflag;
				return;
			}else{
				Node* q = p;
				int pflag = -1;
				int equalflag = 0;	//decide whether to own "ASSIGNOP"
				while(q){
					//printf("%s\n", q->name);
					if(q->pflag == -1 && !strcmp(q->name, "Exp")){
						AllSetpflag(q);
						//printf("%d\n", q->pflag);
						if(pflag == -1){
							pflag = q->pflag;
						}else{
							if(q->pflag != pflag){
								if(equalflag == 1){
									printf("Error type 5 at Line %d: Type mismatched for assignment.\n", q->line);
								}else{
									printf("Error type 7 at Line %d: Type mismatched for operands.\n", q->line);
								}
							}
						}
					}else if(q->pflag == -1 && !strcmp(q->name, "ASSIGNOP")){
						equalflag = 1;
					}else if(q->pflag != -1){
						//printf("pflag: %d", pflag);
						if(pflag == -1){
							pflag = q->pflag;
						}else{
							if(q->pflag != pflag){
								printf("%d %d\n", q->pflag, pflag);
								if(equalflag == 1){
									printf("Error type 5 at Line %d: Type mismatched for assignment.\n", q->line);
								}else{
									printf("Error type 7 at Line %d: Type mismatched for operands.\n", q->line);
								}
							}
						}
					}
					q = q->rnode;
				} 
				point->pflag = pflag;	//without thinking more about more mistakes to types of Vars
				return;
			}
		}
		if(point->childnode != NULL){
			Node* point_ = point->childnode;
			AllSetpflag(point_);
		}
		point = point->rnode;
	} 
} 

static void FuncCheckRe(Node* root){
	Node* point = root;
	while(point != NULL && point->line != 0){
		if(!strcmp(point->name, "RETURN")){
			int pflag = point->rnode->pflag;
			//printf("%d\n", pflag);
			int pflag_ = -1;
			Node* point__ = point;
			while(strcmp(point__->fathernode->name, "ExtDef")){
				//printf("p: %s\n", point__->name);
				point__ = point__->fathernode;
			}
			if(!strcmp(point__->fathernode->childnode->childnode->name, "TYPE: INT")){
				pflag_ = 0;
			}else{
				pflag_ = 1;
			}
			if(pflag != pflag_){
				printf("Error type 8 at Line %d: Type mismatched for return.\n", point->line);
			}
			return;
		}
		if(point->childnode != NULL){
			Node* point_ = point->childnode;
			FuncCheckRe(point_);
		}
		point = point->rnode;
	}
}

#endif
