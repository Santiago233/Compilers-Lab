#ifndef _INTERCODE_H
#define _INTERCODE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#define N 0x3fff

typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;
typedef struct ARG_LIST_* ARG_LIST;
typedef struct member_* member;
//typedef struct func_* func;

typedef struct Operand_{
	enum {VARIABLE,CONSTANT,TEMPORARY,ADDRESS,FUNCTION_NAME,LABEL_,RELOP_}kind;
	union{
		int var_no;
		int value;
		char* funcname;
		char* relop;
	}u;
}Operand_;


typedef struct InterCode_{
	enum{ASSIGN,ADD,SUB,MUL,DIVIDE,FUNCTION,LABEL,ADDR_TO,VALUE_TO,TO_VALUE,GOTO,IF_GOTO,RETURN_,DEC,ARG,CALL,PARAM,READ,WRITE,EMPTY}kind;
	union{
		struct{Operand op1,op2,op3,op;}if_goto;	//if goto
		struct{Operand right,left;}assign;	//useless
		struct{Operand result, op1, op2;}binop;	//binocular operation
		struct{Operand result,op;}sinop;	//monocular operation && assign
		struct{Operand result;}noop;	//nop operation
	}u;
}InterCode_;

typedef struct member_{
	char* name;
	int vcount;
	member next;
}member_;

/*typedef struct func_{
	char* name;
	struct func next;
}func_;*/

member vlist = NULL;

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
int tcount = 1;
int lcount = 1;
int vcount = 1;
int lookup(char*);


//FUNCTIONs

Operand new_operand();
Operand new_temp();
Operand new_label();
Operand get_relop(Node*);
InterCodes new_intercodes();
InterCodes new_noop(InterCodes , Operand);
InterCodes new_sinop(InterCodes , Operand, Operand);
InterCodes new_binop(InterCodes , Operand, Operand, Operand);
InterCodes new_if_go(Operand,Operand,Operand,Operand);
InterCodes translate_Exp(Node*, Operand);
InterCodes translate_Stmt(Node*);
InterCodes translate_Cond(Node*, Operand, Operand);
//InterCodes translate_Exp();
InterCodes translate_Args(Node*, ARG_LIST); 
InterCodes combine(InterCodes, InterCodes);
InterCodes into_label(Operand);
void codeoutput();
void opeoutput();


InterCodes translate_CompSt(Node*);
InterCodes translate_StmtList(Node*);
InterCodes translate_FunDec(Node*);
InterCodes translate_VarList(Node*);
InterCodes translate_ParamDec(Node*);
InterCodes translate_VarDec(Node*,Node*);
InterCodes translate_DefList(Node*);
InterCodes translate_Def(Node*);
InterCodes translate_DecList(Node*,Node*);
InterCodes translate_Dec(Node*,Node*);
InterCodes translate_Program(Node*);
InterCodes translate_ExtDefList(Node*);
InterCodes translate_ExtDef(Node*);


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

Operand new_label(){
	lcount ++;
	Operand op = new_operand();
	op->u.var_no = lcount;
	op->kind = LABEL_;
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
		//expcode->code->u.sinop.result = new_operand();
		expcode->code->u.sinop.op = new_operand();
		expcode->code->u.sinop.result = place;
		expcode->code->u.sinop.op->kind = CONSTANT;
		expcode->code->u.sinop.op->u.value = *(int*)&p->val;
		if(place == NULL){
			expcode->code->kind = EMPTY;
		}
		return expcode;
	}else if(!strcmp(p->name, "ID") && p->rnode == NULL){
		InterCodes expcode = new_intercodes();
		expcode->code->kind = ASSIGN;
		//expcode->code->u.sinop.result = new_operand();
		expcode->code->u.sinop.op = new_operand();
		expcode->code->u.sinop.result = place;
		expcode->code->u.sinop.op->kind = VARIABLE;
		expcode->code->u.sinop.op->u.var_no = lookup(p->val2);
		if(place == NULL){
			expcode->code->kind = EMPTY;
		}
		return expcode;
	}else if(!strcmp(p->name, "Exp") && !strcmp(p->rnode->name, "ASSIGNOP")){
		Operand tmp = new_temp();
		InterCodes expcode1 = translate_Exp(p->rnode->rnode, tmp);
		InterCodes expcode2 = new_intercodes();
		expcode2->code->kind = ASSIGN;
		expcode2->code->u.sinop.result = new_operand();
		//expcode2->code->u.sinop.op = new_operand();
		expcode2->code->u.sinop.result->kind = VARIABLE;
		expcode2->code->u.sinop.result->u.var_no = lookup(p->childnode->val2);
		//printf("%s\n", p->val2);
		expcode2->code->u.sinop.op = tmp;
		InterCodes expcode3 = new_intercodes();
		expcode3->code->kind = ASSIGN;
		//expcode3->code->u.sinop.result = new_operand();
		expcode3->code->u.sinop.op = new_operand();
		expcode3->code->u.sinop.result = place;
		expcode3->code->u.sinop.op->kind = VARIABLE;
		expcode3->code->u.sinop.op->u.var_no = lookup(p->childnode->val2);
		if(place == NULL){
			expcode3->code->kind = EMPTY;
		}
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
		//expcode3->code->u.binop.result = new_operand();
		//expcode3->code->u.binop.op1 = new_operand();
		//expcode3->code->u.binop.op2 = new_operand();
		expcode3->code->u.binop.result = place;
		expcode3->code->u.binop.op1 = tmp1;
		expcode3->code->u.binop.op2 = tmp2;
		if(place == NULL){
			expcode3->code->kind = EMPTY;
		}
		expcode1 = combine(expcode1, expcode2);
		expcode1 = combine(expcode1, expcode3);
		return expcode1;
	}else if(!strcmp(p->name, "Exp") || !strcmp(p->name, "NOT")){
		Operand label1 = new_label();
		Operand label2 = new_label();
		Operand op1 = new_operand();
		op1->kind = CONSTANT;
		op1->u.value = 0;
		InterCodes expcode0 = new_intercodes();
		expcode0 = new_sinop(expcode0, place, op1);
		expcode0->code->kind = ASSIGN;
		InterCodes expcode1 = translate_Cond(p, label1, label2);
		InterCodes expcode2_1 = into_label(label1);
		Operand op2 = new_operand();
		op2->kind = CONSTANT;
		op2->u.value = 1;
		InterCodes expcode2_2 = new_intercodes();
		expcode2_2 = new_sinop(expcode2_2, place, op2);
		expcode2_2->code->kind = ASSIGN;
		InterCodes expcode2 = combine(expcode2_1, expcode2_2);
		InterCodes expcode3 = into_label(label2);
		if(place == NULL){
			expcode0->code->kind = EMPTY;
		}
		if(place == NULL){
			expcode2_2->code->kind = EMPTY;
		}
		expcode0 = combine(combine(combine(expcode0, expcode1), expcode2), expcode3);
		return expcode0;
	}else if(!strcmp(p->name, "ID") && !strcmp(p->rnode->rnode->name, "RP")){
		char* name = p->val2;
		InterCodes expcode = new_intercodes();
		if(!strcmp(name, "read")){
			expcode = new_noop(expcode, place);
			expcode->code->kind = READ;
			return expcode;
		}else{
			Operand op = new_operand();
			op->kind = FUNCTION;
			op->u.funcname = name;
			expcode = new_sinop(expcode, place, op);
			expcode->code->kind = CALL;

			return expcode;
		}
	}else if(!strcmp(p->name, "ID") && !strcmp(p->rnode->rnode->name, "Args")){
		char* name = p->val2;
		ARG_LIST arglist = NULL;
		InterCodes expcode1 = translate_Args(p->rnode->rnode, arglist);
		if(!strcmp(name, "write")){
			Operand op = new_operand();
			op->kind = VARIABLE;
			op->u.var_no = arglist->arg->u.var_no;
			InterCodes expcode2 = new_intercodes();
			expcode2 = new_noop(expcode2, op);
			expcode2->code->kind = WRITE;
			return combine(expcode1, expcode2);
		}else{
			ARG_LIST q = arglist;
			Operand op = new_operand();
			op->kind = VARIABLE;
			op->u.var_no = q->arg->u.var_no;
			InterCodes expcode2 = new_intercodes();
			expcode2 = new_noop(expcode2, op);
			expcode2->code->kind = ARG;
			q = q->next;
			while(q){
				Operand op_ = new_operand();
				op_->kind = VARIABLE;
				op->u.var_no = q->arg->u.var_no;
				expcode2 = new_noop(expcode2, op);
				expcode2->code->kind = ARG;
				q = q->next;
			}
			Operand op_ = new_operand();
			op_->kind = FUNCTION;
			op_->u.funcname = name;
			InterCodes expcode3 = new_intercodes();
			expcode3 = new_sinop(expcode3, place, op_);
			expcode3->code->kind = CALL;
			return combine(combine(expcode1, expcode2), expcode3);
		}
	}else{
		printf("error in translate_Exp\n");
	}
}

InterCodes translate_Stmt(Node* Stmt){
	Node* p = Stmt->childnode;
	if(!strcmp(p->name, "Exp")){
		//printf("000000\n");
		return translate_Exp(p, NULL);
	}else if(!strcmp(p->name, "CompSt")){
		return translate_CompSt(p);
	}else if(!strcmp(p->name, "RETURN")){
		Operand tmp = new_temp();
		InterCodes expcode1 = translate_Exp(p->rnode, tmp);
		InterCodes expcode2 = new_intercodes();
		expcode2 = new_noop(expcode2, tmp);
		expcode2->code->kind = RETURN_;
		return combine(expcode1, expcode2);
	}else if(!strcmp(p->name, "IF")){
		if(p->rnode->rnode->rnode->rnode->rnode == NULL){
			Operand label1 = new_label();
			Operand label2 = new_label();
			InterCodes expcode1 = translate_Cond(p->rnode->rnode, label1, label2);
			InterCodes expcode2 = translate_Stmt(p->rnode->rnode->rnode->rnode);
			InterCodes expcode_1 = into_label(label1);
			InterCodes expcode_2 = into_label(label2);
			return combine(combine(combine(expcode1, expcode_1), expcode2), expcode_2);
		}else{
			Operand label1 = new_label();
			Operand label2 = new_label();
			Operand label3 = new_label();
			InterCodes expcode1 = translate_Cond(p->rnode->rnode, label1, label2);
			InterCodes expcode2 = translate_Stmt(p->rnode->rnode->rnode->rnode);
			InterCodes expcode3 = translate_Stmt(p->rnode->rnode->rnode->rnode->rnode->rnode);
			InterCodes exp1 = into_label(label1);
			InterCodes exp2 = new_intercodes();
			exp2 = new_noop(exp2, label3);
			exp2->code->kind = GOTO;
			InterCodes exp3 = into_label(label2);
			InterCodes exp4 = into_label(label3);
			return combine(combine(combine(combine(combine(combine(expcode1, exp1), expcode2), exp2), exp3), expcode3), exp4);
		}
	}else if(!strcmp(p->name, "WHILE")){
		Operand label1 = new_label();
		Operand label2 = new_label();
		Operand label3 = new_label();
		InterCodes expcode1 = translate_Cond(p->rnode->rnode, label2, label3);
		InterCodes expcode2 = translate_Stmt(p->rnode->rnode->rnode->rnode);
		InterCodes exp1 = into_label(label1);
		InterCodes exp2 = into_label(label2);
		InterCodes exp3 = new_noop(exp3, label1);
		exp3->code->kind = GOTO;
		InterCodes exp4 = into_label(label3);
		return combine(combine(combine(combine(combine(exp1, expcode1), exp2), expcode2), exp3), exp4);
	}
}

InterCodes translate_Cond(Node* Exp, Operand label_true, Operand label_false){
	Node* point = Exp->childnode;
	if(!strcmp(point->name ,"EXP") && strstr(point->rnode->name,"RELOP")){
		Operand t1,t2,op;
		t1 = new_temp();
		t2 = new_temp();
		InterCodes code1,code2,code3;
		code1 = translate_Exp(point, t1);
		code2 = translate_Exp(point->rnode->rnode,t2);
		op = get_relop(point->rnode);
		code3 = new_if_go(t1,t2,label_true,op);
		InterCodes code4 = new_intercodes();
		code4->code->kind = GOTO;
		//Operand label_false = new_label();
		code4->code->u.noop.result = label_false;
		code1 = combine(code1,code2);
		code1 = combine(code1,code3);
		code1 = combine(code1,code4);
		return code1;
	}
	else if(!strcmp(point->name,"NOT") && !strcmp(point->rnode->name,"EXP")){
		return translate_Cond(point->rnode,label_false,label_true);
	}
	else if(!strcmp(point->name,"EXP") && !strcmp(point->rnode->name,"AND")){
		Operand label1 = new_label();
		InterCodes code1,code2,code3;
		code1 = translate_Cond(point,label1,label_false);
		code2 = translate_Cond(point,label_true,label_false);
		code3 = new_intercodes();
		code3->code->kind = LABEL;
		code3 = new_noop(code3,label1);
		code1 = combine(code1,code3);
		code1 = combine(code1,code2);
		return code1;

	}
	else if(!strcmp(point->name,"EXP") && !strcmp(point->rnode->name,"OR")){
		Operand label1 = new_label();
		InterCodes code1,code2,code3;
		code1 = translate_Cond(point,label_true,label1);
		code2 = translate_Cond(point,label_true,label_false);
		code3 = new_intercodes();
		code3->code->kind = LABEL;
		code3 = new_noop(code3,label1);
		code1 = combine(code1,code3);
		code1 = combine(code1,code2);
		return code1;

	}
	else{
		Operand t1,t2,op;
		t1 = new_temp();
		InterCodes code1,code2,code3;
		code1 = translate_Exp(Exp,t1);
		t2 = new_operand();
		t2->kind = CONSTANT;
		t2->u.value = 0;
		op = new_operand();
		op->kind = RELOP_;
		op->u.relop = "!=";
		code2 = new_if_go(t1,t2,label_true,op);
		code3 = new_intercodes();
		code3->code->kind = GOTO;
		code3 = new_noop(code3,label_false);
		code1 = combine(code1,code2);
		code1 = combine(code1,code3);
		return code1;
	}
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
		arglist = templist;
		return code1;
	}
	else if(!strcmp(Exp->rnode->name , "COMMA")){
		Operand t1 = new_temp();
		InterCodes code1 = translate_Exp(Exp ,t1);
		ARG_LIST templist = (ARG_LIST)malloc(sizeof(struct ARG_LIST_));
		templist->arg = t1;
		templist->next = arglist;
		arglist = templist;
		InterCodes code2 = translate_Args(Exp->rnode->rnode, arglist);
		return combine(code1,code2);
	}
	else{
		printf("error in translate_Args\n");
	}
}


InterCodes translate_CompSt(Node* CompSt){
	Node* point = CompSt->childnode;
	InterCodes code1 = translate_DefList(point->rnode);
	InterCodes code2 = translate_StmtList(point->rnode->rnode);
	//codeoutput(code2);
	return combine(code1,code2);

}
InterCodes translate_StmtList(Node* StmtList){
	Node* point = StmtList->childnode;
	if(point == NULL ||point->line == N){

		InterCodes code1 = new_intercodes();
		code1->code->kind = EMPTY;
		return code1;
	}
	else if(!strcmp(point->name,"Stmt")){
		InterCodes code1,code2;
		code1 = translate_Stmt(point);
		//codeoutput(code1);
		code2 = translate_StmtList(point->rnode);//codeoutput(code2);
		return combine(code1,code2);
	}

}
InterCodes translate_FunDec(Node* FunDec){
	Node* point = FunDec->childnode;

	if(!strcmp(point->rnode->rnode->name,"RP")){
		//printf("%s\n",point->rnode->rnode->name);
		InterCodes code1 = new_intercodes();
		code1->code->kind = FUNCTION;
		Operand result = new_operand();
		result->kind = FUNCTION_NAME;
		result->u.funcname = point->val2;
		code1->code->u.noop.result = result;
		//codeoutput(code1);
		return code1;
	}
	else{
		InterCodes code2;
		InterCodes code1 = new_intercodes();
		code1->code->kind = FUNCTION;
		Operand result = new_operand();
		result->kind = FUNCTION_NAME;
		result->u.funcname = point->val2;
		code1->code->u.noop.result = result;
		code2 = translate_VarList(point->rnode->rnode);
		return combine(code1,code2);

	}

}
InterCodes translate_VarList(Node* VarList){
	Node* point = VarList->childnode;
    if(point == NULL || point->rnode->rnode == NULL){
        InterCodes code1 = translate_ParamDec(point);
    }
    else if(point->rnode->rnode != NULL){
        InterCodes code1,code2;
        code1 = translate_ParamDec(point);
        code2 = translate_VarList(point->rnode->rnode);
        return combine(code1,code2);
    }
}
InterCodes translate_ParamDec(Node* ParamDec){
	Node* point = ParamDec->childnode;
    if(!strcmp(point->childnode->name,"TYPE")){
        Operand result = new_operand();
        result->kind = VARIABLE;
        if(strcmp(point->rnode->childnode->name,"ID"))
        {
        		printf("Cannot translate: code contains variables of multi-dementional array type or parameters of array type\n");
        		exit(0);
        }
        result->u.var_no = lookup(point->rnode->childnode->name);
        InterCodes code1 = new_intercodes();
        code1->code->kind = PARAM;
        code1->code->u.noop.result = result;
        return code1;

    }
    else{
    		Operand result = new_operand();
        	result->kind = VARIABLE;
        	result->u.var_no = lookup(point->rnode->childnode->name);
        	InterCodes code1 = new_intercodes();
       	code1->code->kind = PARAM;
        	code1->code->u.noop.result = result;
        	return code1;
    }
}
InterCodes translate_VarDec(Node* s,Node* VarDec){
	Node* point = VarDec->childnode;
	

}
InterCodes translate_DefList(Node* DefList){
	Node* point = DefList->childnode;
	if(point == NULL ||point->line == N){
		//printf("0001\n");
		InterCodes code1 = new_intercodes();
		code1->code->kind = EMPTY;
		return code1;
	}
	else if(!strcmp(point->name,"Def")){
		InterCodes code1,code2;
		code1 = translate_Def(point);//codeoutput(code2);
		//printf("0000\n");
		code2 = translate_DefList(point->rnode);
		
		return combine(code1,code2);
	}
}
InterCodes translate_Def(Node* Def){
	Node* point = Def->childnode;
	if(!strcmp(point->name,"Specifier")){
		InterCodes code1 = translate_DecList(point,point->rnode);
		//printf("00000\n");
		
		return code1;
	}
}
InterCodes translate_DecList(Node* s,Node* DecList){
	Node* point = DecList->childnode;
	if(point->rnode == NULL){
		return translate_Dec(s,point);
	}
	else{
		InterCodes code1,code2;

		code1 = translate_Dec(s,point);
		code2 = translate_DecList(s,point->rnode->rnode);
		//codeoutput(code1);
		return combine(code1,code2);
	}
}
InterCodes translate_Dec(Node* s,Node* Dec){
	Node* point = Dec->childnode;
	if(point->rnode != NULL){

		InterCodes code1,code2;
		Operand t1 = new_temp();
		code1 = translate_Exp(point->rnode->rnode,t1);
		Operand result = new_operand();
		code2 = new_intercodes();
		code2->code->kind = ASSIGN;
		//Operand result = new_operand();
		result->kind = VARIABLE;
		result->u.var_no = lookup(point->childnode->name);
		//printf("%s\n", point->childnode->name);
		code2->code->u.sinop.result = result;
		code2->code->u.sinop.op = t1;
		//codeoutput(combine(code1,code2));
		return combine(code1,code2);

	}
	else{
		InterCodes code1 = new_intercodes();
		code1->code->kind = EMPTY;
		return code1;
	}

}
InterCodes translate_Program(Node* Program){
	Node* point = Program->childnode;
	InterCodes code1 = translate_ExtDefList(point);
	return code1;
}
InterCodes translate_ExtDefList(Node* ExtDefList){
	Node* point = ExtDefList->childnode;
	if(point == NULL || point->line == N){
		return NULL;
	}
	else if(!strcmp(point->name,"ExtDef")){
		InterCodes code1,code2;
		code1 = translate_ExtDef(point);
		code2 = translate_ExtDefList(point->rnode);
		return combine(code1,code2);
	}
}
InterCodes translate_ExtDef(Node* ExtDef){
	Node* point = ExtDef->childnode;
	if(!strcmp(point->rnode->name,"FunDec")){
		InterCodes code1,code2;
		code1 = translate_FunDec(point->rnode);
		code2 = translate_CompSt(point->rnode->rnode);
		return combine(code1,code2);
	}

}


InterCodes combine(InterCodes code1, InterCodes code2){   //like as insert one code2;
	//InterCodes temp1 = code1->next;
	//InterCodes temp2 = code2->prev;
	if(code2 == NULL)
		return code1;
	/*code1->next = code2;
	code2->prev = code1;
	code2->next = temp1;
	temp1->prev = code2;
	return code1;*/
	InterCodes p = code1;
	InterCodes p1 = code2;
	//codeoutput(code1);
	//codeoutput(code2);
	while(p->next){p = p->next;}
	/*if(p1->prev){
		p1 = p1->prev;
		p1->next = code1;
		p->next = code2;
		code1->prev = p1;
		code2->prev = p;
		InterCodes q = code2;
		while(q->prev){q = q->prev;}
		return q;
	}else{*/
		p->next = code2;
		code2->prev = p;
		return code1;
	//}
}

InterCodes into_label(Operand label){
	InterCodes code = new_intercodes();
	code->code->kind = LABEL;
	code->code->u.noop.result = label;
	return code;
}

InterCodes new_noop(InterCodes code, Operand result){
	InterCodes p = code;
	p->code->u.noop.result = result;
	return p;
}

InterCodes new_sinop(InterCodes code, Operand result, Operand op){
	InterCodes p = code;
	p->code->u.sinop.result = result;
	p->code->u.sinop.op = op;
	return p;
}

InterCodes new_binop(InterCodes code, Operand result, Operand op1, Operand op2){
	InterCodes p = code;
	p->code->u.binop.result = result;
	p->code->u.binop.op1 = op1;
	p->code->u.binop.op2 = op2;
	return p;
}

InterCodes new_if_go(Operand op1,Operand op2,Operand op3,Operand op){
	InterCodes p = new_intercodes();
	p->code->kind = IF_GOTO;
	p->code->u.if_goto.op1 = op1;
	p->code->u.if_goto.op2 = op2;
	p->code->u.if_goto.op3 = op3;
	p->code->u.if_goto.op = op;
}

void codeoutput(InterCodes srccode){
	//enum{ASSIGN,ADD,SUB,MUL,DIVIDE,FUNCTION,LABEL,ADDR_TO,VALUE_TO,TO_VALUE,GOTO,IF_GOTO,RETURN_,DEC,ARG,CALL,PARAM,READ,WRITE}kind;

	InterCodes tcode = srccode;
	while(tcode != NULL){
		switch (tcode->code->kind){
			case FUNCTION:{
				printf("FUNCTION ");
				opeoutput(tcode->code->u.noop.result);
				printf(" :\n");
				break;
			}
			case LABEL:{
				printf("label ");
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
			case EMPTY:{
				break;
			}

		}
		tcode = tcode->next;

	}
}

void opeoutput(Operand op){
	//enum {VARIABLE,CONSTANT,TEMPORARY,ADDRESS,FUNCTION_NAME,label_}kind;
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
		case LABEL_:{
			printf("label%d", op->u.var_no);
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

int lookup(char* val){	//variable
	if(vlist == NULL){
		vlist = (member)malloc(sizeof(struct member_));
		vlist->name = val;
		vlist->vcount = vcount;
		vcount += 1;
		vlist->next = NULL;
		return vlist->vcount;
	}else{
		member p = vlist;
		while(p->next != NULL){
			if(!strcmp(p->name, val)){return p->vcount;}
			else{p = p->next;}
		}
		if(!strcmp(p->name, val)){return p->vcount;}
		else{
			member q = (member)malloc(sizeof(struct member_));
			q->name = val;
			q->vcount = vcount;
			vcount += 1;
			q->next = NULL;
			p->next = q;
			return q->vcount;
		}
	}
}

#endif