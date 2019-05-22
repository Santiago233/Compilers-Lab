#ifndef _INTERCODE_H
#define _INTERCODE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;
typedef struct ARG_LIST_* ARG_LIST;
typedef struct Operand_{
	enum {VARIABLE,CONSTANT,TEMPORARY,ADDRESS,FUNCTION_NAME,LABLE_,RELOP_}kind;
	union{
		int var_no;
		int value;
		char* funcname;
		char* relop;
	}u;
}Operand_;


typedef struct InterCode_{
	enum{ASSIGN,ADD,SUB,MUL,DIVIDE,FUNCTION,LABLE,ADDR_TO,VALUE_TO,TO_VALUE,GOTO,IF_GOTO,RETURN_,DEC,ARG,CALL,PARAM,READ,WRITE}kind;
	union{
		struct{Operand op1,op2,op3,op;}if_goto;	//if goto
		struct{Operand right,left;}assign;	//useless
		struct{Operand result, op1, op2;}binop;	//binocular operation
		struct{Operand result,op;}sinop;	//monocular operation && assign
		struct{Operand result;}noop;	//nop operation
	}u;
}InterCode_;   

typedef struct InterCodes_ { InterCode code; InterCodes prev, next;}InterCodes_;
typedef struct ARG_LIST_ { Operand arg;int num;ARG_LIST next;}ARG_LIST_;



//Varibles
extern Node* stack[1024];
extern int scount ;
extern myArray* MyArrayList[N];
extern myStruct* MyStructList[N];
extern Node* root;
extern char* mystr[1024];
extern int mystrc ;
extern VarList MyVarList[N];	//print for check name
extern FuncList MyFuncList[N];
extern struct Type_ TYPE_MYINT;
extern struct Type_ TYPE_MYFLOAT;
int tcount;
int lcount;
int vcount;


//FUNCTIONs

Operand new_operand();
Operand new_temp();
Operand new_lable();
Operand get_relop(Node*);
InterCodes new_intercodes();
InterCodes new_noop(InterCodes , Operand);
InterCodes new_sinop(InterCodes , Operand, Operand);
InterCodes new_binop(InterCodes , Operand, Operand, Operand);
InterCodes new_if_go();
InterCodes translate_Exp(Node*, Operand);
InterCodes translate_Stmt(Node*);
InterCodes translate_Cond(Node*, Operand, Operand);
//InterCodes translate_Exp();
InterCodes translate_Args(Node*, ARG_LIST); 
InterCodes combine(InterCodes, InterCodes);
InterCodes into_lable(Operand);
void codeoutput();
void opeoutput();

InterCodes new_intercodes(){
	InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
	InterCode p1 = (InterCode)malloc(sizeof(struct InterCode_));
	p->code = p1;
	p->prev = NULL;
	p->next = NULL;
	return p;
}
Operand new_temp(){
	tcount ++;
	Operand op = new_operand();
	op->u.var_no = tcount;
	op->kind = TEMPORARY;
	return op;
}
Operand new_lable(){
	lcount ++;
	Operand op = new_operand();
	op->u.var_no = lcount;
	op->kind = LABLE_;
	return op;
}
Operand new_operand(){
	Operand p = (Operand)malloc(sizeof(struct Operand_));
	return p;
}
Operand get_relop(Node* relop){
	Operand p = new_operand();
	p->kind = RELOP_;
	switch(relop->kind){       //find it in node.h
		case RELOP_DAYU:{
			p->u.relop = ">";
			break;
		}
		case RELOP_XIAOYU:{
			p->u.relop = "<";
			break;
		}
		case RELOP_DAYUDENGYU:{
			p->u.relop = ">=";
			break;
		}
		case RELOP_XIAOYUDENGYU:{
			p->u.relop = "<=";
			break;
		}
		case RELOP_LIANDENG:{
			p->u.relop = "==";
			break;
		}
		case RELOP_BUDENGYU:{
			p->u.relop = "!=";
			break;
		}
	}
	return p;
}

InterCodes translate_Exp(Node* Exp, Operand place){
	Node* p = Exp->childnode;
	if(!strcmp(p->name, "INT")){
		InterCodes expcode = new_intercodes();
		expcode->code->kind = ASSIGN;
		expcode->code->u.sinop.result = new_operand();
		expcode->code->u.sinop.op = new_operand();
		expcode->code->u.sinop.result = place;
		expcode->code->u.sinop.op->kind = CONSTANT;
		expcode->code->u.sinop.op->u.value = *(int*)&p->val;
		return expcode;
	}else if(!strcmp(p->name, "ID")){
		InterCodes expcode = new_intercodes();
		expcode->code->kind = ASSIGN;
		expcode->code->u.sinop.result = new_operand();
		expcode->code->u.sinop.op = new_operand();
		expcode->code->u.sinop.result = place;
		expcode->code->u.sinop.op->kind = VARIABLE;
		expcode->code->u.sinop.op->u.var_no = vcount;
		vcount ++;
		return expcode;
	}else if(!strcmp(p->name, "Exp") && !strcmp(p->rnode->name, "ASSIGNOP")){
		Operand tmp = new_temp();
		InterCodes expcode1 = translate_Exp(p->rnode->rnode, tmp);
		InterCodes expcode2 = new_intercodes();
		expcode2->code->kind = ASSIGN;
		expcode2->code->u.sinop.result = new_operand();
		expcode2->code->u.sinop.op = new_operand();
		expcode2->code->u.sinop.result->kind = VARIABLE;
		expcode2->code->u.sinop.result->u.var_no = vcount;
		vcount ++;
		expcode2->code->u.sinop.op = tmp;
		InterCodes expcode3 = new_intercodes();
		expcode3->code->kind = ASSIGN;
		expcode3->code->u.sinop.result = new_operand();
		expcode3->code->u.sinop.op = new_operand();
		expcode3->code->u.sinop.result = place;
		expcode3->code->u.sinop.op->kind = VARIABLE;
		expcode3->code->u.sinop.op->u.var_no = vcount;
		vcount ++;
		expcode2 = combine(expcode2, expcode3);
		expcode1 = combine(expcode1, expcode2);
		return expcode1;
	}else if(!strcmp(p->name, "Exp") && (!strcmp(p->rnode->name, "PLUS") || !strcmp(p->rnode->name, "MINUS") || !strcmp(p->rnode->name, "STAR") || !strcmp(p->rnode->name, "DIV"))){
		Operand tmp1 = new_temp();
		Operand tmp2 = new_temp();
		InterCodes expcode1 = translate_Exp(p, tmp1);
		InterCodes expcode2 = translate_Exp(p->rnode->rnode, tmp2);
		InterCodes expcode3 = new_intercodes();
		if(!strcmp(p->rnode->name, "PLUS")){
			expcode3->code->kind = ADD;
		}else if(!strcmp(p->rnode->name, "MINUS")){
			expcode3->code->kind = SUB;
		}else if(!strcmp(p->rnode->name, "STAR")){
			expcode3->code->kind = MUL;
		}else{
			expcode3->code->kind = DIVIDE;
		}
		expcode3->code->u.sinop.result = new_operand();
		expcode3->code->u.sinop.op1 = new_operand();
		expcode3->code->u.sinop.op2 = new_operand();
		expcode3->code->u.binop.result = place;
		expcode3->code->u.binop.op1 = tmp1;
		expcode3->code->u.binop.op2 = tmp2;
		expcode1 = combine(expcode1, expcode2);
		expcode1 = combine(expcode1, expcode3);
		return expcode1;
	}else if(){}
}

InterCodes translate_Stmt(Node* Stmt){


}
InterCodes translate_Cond(Node* Exp, Operand lable_true, Operand lable_false){


}
//InterCodes translate_Exp();
InterCodes translate_Args(Node* Args, ARG_LIST arglist){
	Node* Exp = Args->childnode;
	if(Exp->rnode == NULL){
		Operand t1 = new_temp();
		InterCodes code1 = translate_Exp(Exp, t1);
		ARG_LIST templist = (ARG_LIST)malloc(sizeof(struct ARG_LIST_));
		templist->arg = t1;
		templist->next = arglist;
		return code1;
	}
	else if(!strcmp(Exp->rnode->name , "COMMA")){
		Operand t1 = new_temp();
		InterCodes code1 = translate_Exp(Exp ,t1);
		ARG_LIST templist = (ARG_LIST)malloc(sizeof(struct ARG_LIST_));
		templist->arg = t1;
		templist->next = arglist;
		InterCodes code2 = translate_Args(Exp->rnode->rnode, templist);
		return combine(code1,code2);
	}
	else{
		printf("error in translate_Args\n");
	}

}

InterCodes combine(InterCodes code1, InterCodes code2){   //like as insert one code2;
	InterCodes temp1 = code1->next;
	//InterCodes temp2 = code2->prev;
	code1->next = code2;
	code2->prev = code1;
	code2->next = temp1;
	temp1->prev = code2;
	return code1;
}

InterCodes into_lable(Operand lable){
	InterCodes code = new_intercodes();
	code->code->kind = LABLE;
	code->code->u.noop.result = lable;
	return code;
}

InterCodes new_noop(InterCodes code, Operand result){
	InterCodes p = code;
	p->u.noop.result = result;
	return p;
}
InterCodes new_sinop(InterCodes code, Operand result, Operand op){
	InterCodes p = code;
	p->u.sinop.result = result;
	p->u.sinop.op = op;
	return p;
}
InterCodes new_binop(InterCodes code, Operand result, Operand op1, Operand op2){
	InterCodes p = code;
	p->u.binop.result = result;
	p->u.binop.op1 = op1;
	p->u.binop.op2 = op2;
	return p;
}


void codeoutput(InterCodes srccode){
	//enum{ASSIGN,ADD,SUB,MUL,DIVIDE,FUNCTION,LABLE,ADDR_TO,VALUE_TO,TO_VALUE,GOTO,IF_GOTO,RETURN_,DEC,ARG,CALL,PARAM,READ,WRITE}kind;
	InterCodes tcode = srccode;
	while(tcode != NULL){
		switch (tcode->code->kind){
			case FUNCTION:{
				printf("FUNCTION ");
				opeoutput(tcode->code->u.noop.result);
				printf(" :\n");
				break;
			}
			case LABLE:{
				printf("LABLE ");
				opeoutput(tcode->code->u.noop.result);
				printf(" :\n");
				break;
			}
			case RETURN_:{
				printf("RETURN ");
				opeoutput(tcode->code->u.noop.result);
				printf("\n");
				break;
			}
			case ASSIGN:{
				opeoutput(tcode->code->u.sinop.result);
				printf(" := ");
				opeoutput(tcode->code->u.sinop.op);
				printf("\n");
				break;
			}
			case SUB:{
				opeoutput(tcode->code->u.binop.result);
				printf(" := ");
				opeoutput(tcode->code->u.binop.op1);
				printf(" - ");
				opeoutput(tcode->code->u.binop.op2);
				printf("\n");
				break;
			}
			case MUL:{
				opeoutput(tcode->code->u.binop.result);
				printf(" := ");
				opeoutput(tcode->code->u.binop.op1);
				printf(" * ");
				opeoutput(tcode->code->u.binop.op2);
				printf("\n");
				break;
			}
			case DIVIDE:{
				opeoutput(tcode->code->u.binop.result);
				printf(" := ");
				opeoutput(tcode->code->u.binop.op1);
				printf(" / ");
				opeoutput(tcode->code->u.binop.op2);
				printf("\n");
				break;
			}
			case ADD:{
				opeoutput(tcode->code->u.binop.result);
				printf(" := ");
				opeoutput(tcode->code->u.binop.op1);
				printf(" + ");
				opeoutput(tcode->code->u.binop.op2);
				printf("\n");
				break;
			}
			case ADDR_TO:{
				opeoutput(tcode->code->u.sinop.result);
				printf(" := &");
				opeoutput(tcode->code->u.sinop.op);
				printf("\n");
				break;
			}
			case VALUE_TO:{
				opeoutput(tcode->code->u.sinop.result);
				printf(" := *");
				opeoutput(tcode->code->u.sinop.op);
				printf("\n");
				break;
			}
			case TO_VALUE:{
				printf("*");
				opeoutput(tcode->code->u.sinop.result);
				printf(" := ");
				opeoutput(tcode->code->u.sinop.op);
				printf("\n");
				break;
			}
			case GOTO:{
				printf("GOTO ");
				opeoutput(tcode->code->u.noop.result);
				printf("\n");
				break;
			}
			case IF_GOTO:{
				printf("IF ");
				opeoutput(tcode->code->u.if_goto.op1);
				printf(" ");
				opeoutput(tcode->code->u.if_goto.op);
				//todo:relop
				printf(" ");
				opeoutput(tcode->code->u.if_goto.op2);
				printf(" GOTO ");
				opeoutput(tcode->code->u.if_goto.op3);
				printf("\n");
				break;
			}
			case DEC:{
				//TODO: array.size;
				//x = tcode->code->u.noop.result;
				int size = 4;//   * x;   //not true
				printf("DEC ");
				opeoutput(tcode->code->u.noop.result);
				printf("%d\n", size);
				break;
			}
			case ARG:{
				printf("ARG ");
				opeoutput(tcode->code->u.noop.result);
				printf("\n");
				break;
			}
			case CALL:{
				opeoutput(tcode->code->u.sinop.result);
				printf(" := CALL ");
				opeoutput(tcode->code->u.sinop.op);
				printf("\n");
				break;
			}
			case PARAM:{
				printf("PARAM ");
				opeoutput(tcode->code->u.noop.result);
				printf("\n");
				break;
			}
			case READ:{     //noop
				printf("READ ");
				opeoutput(tcode->code->u.noop.result);
				printf("\n");
				break;
			}
			case WRITE:{    //noop
				printf("WRITE ");
				opeoutput(tcode->code->u.noop.result);
				printf("\n");
				break;
			}

		}
		tcode = tcode->next;

	}
}

void opeoutput(Operand op){
	//enum {VARIABLE,CONSTANT,TEMPORARY,ADDRESS,FUNCTION_NAME,LABLE_}kind;
	switch(op->kind){
		case FUNCTION_NAME:{
			printf("%s", op->u.funcname);
			break;
		}
		case VARIABLE:{
			printf("v%d", op->u.var_no);
			break;
		}
		case CONSTANT:{
			printf("#%d", op->u.value);
			break;
		}
		case LABLE_:{
			printf("lable%d", op->u.var_no);
			break;
		}
		case TEMPORARY:{
			printf("t%d", op->u.var_no);
			break;
		}
		case ADDRESS:{
			//???

			//printf("%d", op->u.var_no);
			break;
		}
		case RELOP_:{
			printf("%s", op->u.relop);
			break;
		}
	}
}

#endif