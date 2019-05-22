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
struct Operand_{
	enum {VARIABLE,CONSTANT,TEMPORARY,ADDRESS,FUNCTION_NAME,LABLE_,RELOP_}kind;
	union{
		int var_no;
		int value;
		char* funcname;
		char* relop;
	}u;
};


typedef struct InterCode_{
	enum{ASSIGN,ADD,SUB,MUL,DIVIDE,FUNCTION,LABLE,ADDR_TO,VALUE_TO,TO_VALUE,GOTO,IF_GOTO,RETURN_,DEC,ARG,CALL,PARAM,READ,WRITE}kind;
	union{
		struct{Operand op1,op2,op3,op;}if_goto;
		struct{Operand right,left;}assign;
		struct 
		{
			Operand result, op1, op2;
		}binop; //
		struct{Operand result,op;}sinop;
		struct{Operand result;}noop;
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


//FUNCTIONs

Operand new_operand();
Operand new_temp();
Operand new_lable();
Operand get_relop(Node*);
InterCodes new_intercodes();
InterCodes translate_Exp(Node*, Operand);
InterCodes translate_Stmt(Node*);
InterCodes translate_Cond(Node*, Operand, Operand);
//InterCodes translate_Exp();
InterCodes translate_Args(Node*, ARG_LIST); 
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
	switch(relop->name){
		case "RELOP_DAYU":{
			p->u.relop = ">";
			break;
		}
		case "RELOP_XIAOYU":{
			p->u.relop = "<";
			break;
		}
		case "RELOP_DAYUDENGYU":{
			p->u.relop = ">=";
			break;
		}
		case "RELOP_XIAOYUDENGYU":{
			p->u.relop = "<=";
			break;
		}
		case "RELOP_LIANDENG":{
			p->u.relop = "==";
			break;
		}
		case "RELOP_BUDENGYU":{
			p->u.relop = "!=";
			break;
		}
	}
	return p;
}

InterCodes translate_Exp(Node* Exp, Operand place){

}

InterCodes translate_Stmt(Node* Stmt){


}
InterCodes translate_Cond(Node* Exp, Operand lable_true, Operand lable_false){


}
//InterCodes translate_Exp();
InterCodes translate_Args(Node* ARGS, ARG_LIST arglist){


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