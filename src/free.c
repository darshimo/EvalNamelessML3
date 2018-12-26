#include "param.h"
#include <stdlib.h>

//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

void freeInt(Int *);
void freeBool(Bool *);
void freeClsr(Clsr *);
void freeClsrRec(ClsrRec *);
void freeValList(ValList *);
void freeVal(Val *);
void freeVar(Var *);
void freeOp(Op *);
void freeIf(If *);
void freeLet(Let *);
void freeFun(Fun *);
void freeApp(App *);
void freeLetRec(LetRec *);
void freeExp(Exp *);
void freeAsmp(Asmp *);
void freeInfr(Infr *);
void freeEval(Eval *);
void freeCncl(Cncl *);


void freeInt(Int *ob){
#ifdef DEBUG
    printf("free int\n");
#endif
    free(ob);
    return;
}

void freeBool(Bool *ob){
#ifdef DEBUG
    printf("free bool\n");
#endif
    free(ob);
    return;
}

void freeClsr(Clsr *ob){
#ifdef DEBUG
    printf("free clsr\n");
#endif
    freeValList(ob->vallist_);
    freeExp(ob->exp_);
    free(ob);
    return;
}

void freeClsrRec(ClsrRec *ob){
#ifdef DEBUG
    printf("free clsrrec\n");
#endif
    freeValList(ob->vallist_);
    freeExp(ob->exp_);
    free(ob);
    return;
}

void freeValList(ValList *ob){
    if(ob==NULL)return;
#ifdef DEBUG
    printf("free vallist\n");
#endif
    freeVal(ob->val_);
    freeValList(ob->prev);
    free(ob);
    return;
}

void freeVal(Val *ob){
#ifdef DEBUG
    printf("free val\n");
#endif
    if(ob->val_type==INT_)freeInt(ob->u.int_);
    else if(ob->val_type==BOOL_)freeBool(ob->u.bool_);
    else if(ob->val_type==CLSR)freeClsr(ob->u.clsr_);
    else freeClsrRec(ob->u.clsrrec_);
    free(ob);
    return;
}

void freeVar(Var *ob){
#ifdef DEBUG
    printf("free var\n");
#endif
    free(ob);
    return;
}

void freeOp(Op *ob){
#ifdef DEBUG
    printf("free op\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    free(ob);
    return;
}

void freeIf(If *ob){
#ifdef DEBUG
    printf("free if\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    freeExp(ob->exp3_);
    free(ob);
    return;
}

void freeLet(Let *ob){
#ifdef DEBUG
    printf("free let\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    free(ob);
    return;
}

void freeFun(Fun *ob){
#ifdef DEBUG
    printf("free fun\n");
#endif
    freeExp(ob->exp_);
    free(ob);
    return;
}

void freeApp(App *ob){
#ifdef DEBUG
    printf("free app\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    free(ob);
    return;
}

void freeLetRec(LetRec *ob){
#ifdef DEBUG
    printf("free letrec\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    return;
}

void freeExp(Exp *ob){
#ifdef DEBUG
    printf("free exp\n");
#endif
    if(ob->exp_type==INT)freeInt(ob->u.int_);
    else if(ob->exp_type==BOOL)freeBool(ob->u.bool_);
    else if(ob->exp_type==VAR)freeVar(ob->u.var_);
    else if(ob->exp_type==OP)freeOp(ob->u.op_);
    else if(ob->exp_type==IF)freeIf(ob->u.if_);
    else if(ob->exp_type==LET)freeLet(ob->u.let_);
    else if(ob->exp_type==FUN)freeFun(ob->u.fun_);
    else if(ob->exp_type==APP)freeApp(ob->u.app_);
    else freeLetRec(ob->u.letrec_);
    free(ob);
    return;
}

void freeAsmp(Asmp *ob){
    if(ob==NULL)return;
#ifdef DEBUG
    printf("free asmp\n");
#endif
    freeCncl(ob->cncl_);
    freeAsmp(ob->next);
    free(ob);
    return;
}

void freeInfr(Infr *ob){
#ifdef DEBUG
    printf("free infr\n");
#endif
    freeVal(ob->val_);
    free(ob);
    return;
}

void freeEval(Eval *ob){
#ifdef DEBUG
    printf("free eval\n");
#endif
    freeValList(ob->vallist_);
    freeExp(ob->exp_);
    freeVal(ob->val_);
    free(ob);
    return;
}

void freeCncl(Cncl *ob){
#ifdef DEBUG
    printf("free cncl\n");
#endif
    freeAsmp(ob->asmp_);
    if(ob->cncl_type==INFR)freeInfr(ob->u.infr_);
    else freeEval(ob->u.eval_);
    free(ob);
    return;
}
