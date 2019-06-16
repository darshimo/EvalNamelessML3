#include "param.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DBG_ALL
#define DBG_DRV
#endif

Int *copyInt(Int *);
Bool *copyBool(Bool *);
Exp *copyExp(Exp *);
ValList *copyValList(ValList *);
Val *copyVal(Val *);
Val *getVal(ValList *, int);

#ifdef DBG_DRV
void writeInt(Int *);
void writeBool(Bool *);
void writeClsr(Clsr *);
void writeClsrRec(ClsrRec *);
void writeValList(ValList *);
void writeVal(Val *);
void writeFun(Fun *);
void writeApp(App *);
void writeLetRec(LetRec *);
void writeExp(Exp *);
void ind(int);
#endif

void derivation(Cncl *, int);


void B_Plus(Cncl *cncl_ob, int d){
    int i1 = cncl_ob->u.infr_->int1;
    int i2 = cncl_ob->u.infr_->int2;
#ifdef DBG_DRV
    ind(d);
    printf("B-Plus: ");
    printf("%d plus %d",i1,i2);
    printf("\n");
#endif
    cncl_ob->rule_type = B_PLUS;

    Asmp *asmp_ob = NULL;

    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = INT_;
    val_ob->u.int_ = (Int *)malloc(sizeof(Int));
    val_ob->u.int_->i = i1 + i2;

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.infr_->val_ = val_ob;
    return;
}

void B_Minus(Cncl *cncl_ob, int d){
    int i1 = cncl_ob->u.infr_->int1;
    int i2 = cncl_ob->u.infr_->int2;
#ifdef DBG_DRV
    ind(d);
    printf("B-Minus: ");
    printf("%d minus %d",i1,i2);
    printf("\n");
#endif
    cncl_ob->rule_type = B_MINUS;

    Asmp *asmp_ob = NULL;

    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = INT_;
    val_ob->u.int_ = (Int *)malloc(sizeof(Int));
    val_ob->u.int_->i = i1 - i2;

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.infr_->val_ = val_ob;
    return;
}

void B_Times(Cncl *cncl_ob, int d){
    int i1 = cncl_ob->u.infr_->int1;
    int i2 = cncl_ob->u.infr_->int2;
#ifdef DBG_DRV
    ind(d);
    printf("B-Times: ");
    printf("%d times %d",i1,i2);
    printf("\n");
#endif
    cncl_ob->rule_type = B_TIMES;

    Asmp *asmp_ob = NULL;

    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = INT_;
    val_ob->u.int_ = (Int *)malloc(sizeof(Int));
    val_ob->u.int_->i = i1 * i2;

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.infr_->val_ = val_ob;
    return;
}

void B_Lt(Cncl *cncl_ob, int d){
    int i1 = cncl_ob->u.infr_->int1;
    int i2 = cncl_ob->u.infr_->int2;
#ifdef DBG_DRV
    ind(d);
    printf("B-Lt: ");
    printf("%d less than %d",i1,i2);
    printf("\n");
#endif
    cncl_ob->rule_type = B_LT;

    Asmp *asmp_ob = NULL;

    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = BOOL_;
    val_ob->u.bool_ = (Bool *)malloc(sizeof(Bool));
    val_ob->u.bool_->b = i1 < i2;

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.infr_->val_ = val_ob;
    return;
}

void E_Int(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-Int: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = E_INT;

    Int *i = cncl_ob->u.eval_->exp_->u.int_;

    Asmp *asmp_ob = NULL;

    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = INT_;
    val_ob->u.int_ = copyInt(i);

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_Bool(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-Bool: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = E_BOOL;

    Bool *b = cncl_ob->u.eval_->exp_->u.bool_;

    Asmp *asmp_ob = NULL;

    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = BOOL_;
    val_ob->u.bool_ = copyBool(b);

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}


void E_Var(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-Var: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = E_VAR;

    ValList *ups = cncl_ob->u.eval_->vallist_;
    int n = cncl_ob->u.eval_->exp_->u.var_->n;

    Asmp *asmp_ob = NULL;
    Val *val_ob = copyVal(getVal(ups,n));

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_Op(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-Op: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    ValList *ups = cncl_ob->u.eval_->vallist_;
    Exp *d1 = cncl_ob->u.eval_->exp_->u.op_->exp1_;
    Exp *d2 = cncl_ob->u.eval_->exp_->u.op_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->cncl_type = EVAL;
    asmp_ob->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->cncl_->u.eval_->vallist_ = copyValList(ups);
    asmp_ob->cncl_->u.eval_->exp_ = copyExp(d1);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->cncl_type = EVAL;
    asmp_ob->next->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->next->cncl_->u.eval_->vallist_ = copyValList(ups);
    asmp_ob->next->cncl_->u.eval_->exp_ = copyExp(d2);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->next->cncl_->cncl_type = INFR;
    asmp_ob->next->next->cncl_->u.infr_ = (Infr *)malloc(sizeof(Infr));

    InfrOpType tmp = cncl_ob->u.eval_->exp_->u.op_->op_type;
    if(tmp == PLUS){
        cncl_ob->rule_type = E_PLUS;
        asmp_ob->next->next->cncl_->u.infr_->infr_type = PLUS;
    }else if(tmp == MINUS){
        cncl_ob->rule_type = E_MINUS;
        asmp_ob->next->next->cncl_->u.infr_->infr_type = MINUS;
    }else if(tmp == TIMES){
        cncl_ob->rule_type = E_TIMES;
        asmp_ob->next->next->cncl_->u.infr_->infr_type = TIMES;
    }else{
        cncl_ob->rule_type = E_LT;
        asmp_ob->next->next->cncl_->u.infr_->infr_type = LT;
    }

    asmp_ob->next->next->cncl_->u.infr_->int1 = asmp_ob->cncl_->u.eval_->val_->u.int_->i;
    asmp_ob->next->next->cncl_->u.infr_->int2 = asmp_ob->next->cncl_->u.eval_->val_->u.int_->i;
    derivation(asmp_ob->next->next->cncl_,d+1);
    asmp_ob->next->next->next = NULL;

    Val *val_ob = copyVal(asmp_ob->next->next->cncl_->u.infr_->val_);

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_If(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-If: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    ValList *ups = cncl_ob->u.eval_->vallist_;
    Exp *d1 = cncl_ob->u.eval_->exp_->u.if_->exp1_;
    Exp *d2 = cncl_ob->u.eval_->exp_->u.if_->exp2_;
    Exp *d3 = cncl_ob->u.eval_->exp_->u.if_->exp3_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->cncl_type = EVAL;
    asmp_ob->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->cncl_->u.eval_->vallist_ = copyValList(ups);
    asmp_ob->cncl_->u.eval_->exp_ = copyExp(d1);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->cncl_type = EVAL;
    asmp_ob->next->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->next->cncl_->u.eval_->vallist_ = copyValList(ups);

    if(asmp_ob->cncl_->u.eval_->val_->u.bool_->b){
    cncl_ob->rule_type = E_IFT;
        asmp_ob->next->cncl_->u.eval_->exp_ = copyExp(d2);
    }else{
    cncl_ob->rule_type = E_IFF;
        asmp_ob->next->cncl_->u.eval_->exp_ = copyExp(d3);
    }
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = NULL;

    Val *val_ob = copyVal(asmp_ob->next->cncl_->u.eval_->val_);
    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_Let(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-Let: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = E_LET;

    ValList *ups = cncl_ob->u.eval_->vallist_;
    Exp *d1 = cncl_ob->u.eval_->exp_->u.let_->exp1_;
    Exp *d2 = cncl_ob->u.eval_->exp_->u.let_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->cncl_type = EVAL;
    asmp_ob->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->cncl_->u.eval_->vallist_ = copyValList(ups);
    asmp_ob->cncl_->u.eval_->exp_ = copyExp(d1);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->cncl_type = EVAL;
    asmp_ob->next->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->next->cncl_->u.eval_->vallist_ = (ValList *)malloc(sizeof(ValList));
    asmp_ob->next->cncl_->u.eval_->vallist_->prev = copyValList(ups);
    asmp_ob->next->cncl_->u.eval_->vallist_->val_ = copyVal(asmp_ob->cncl_->u.eval_->val_);
    asmp_ob->next->cncl_->u.eval_->exp_ = copyExp(d2);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = NULL;

    Val *val_ob = copyVal(asmp_ob->next->cncl_->u.eval_->val_);
    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_Fun(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-Fun: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = E_FUN;

    ValList *ups = cncl_ob->u.eval_->vallist_;
    Exp *d1 = cncl_ob->u.eval_->exp_->u.fun_->exp_;

    Asmp *asmp_ob = NULL;
    Val *val_ob = (Val *)malloc(sizeof(Val));
    val_ob->val_type = CLSR;
    val_ob->u.clsr_ = (Clsr *)malloc(sizeof(Clsr));
    val_ob->u.clsr_->vallist_ = copyValList(ups);
    val_ob->u.clsr_->exp_ = copyExp(d1);

    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_App(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-App: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    ValList *ups = cncl_ob->u.eval_->vallist_;
    Exp *d1 = cncl_ob->u.eval_->exp_->u.app_->exp1_;
    Exp *d2 = cncl_ob->u.eval_->exp_->u.app_->exp2_;
    ValList *ups2;
    Val *w2;
    Exp *d0;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->cncl_type = EVAL;
    asmp_ob->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->cncl_->u.eval_->vallist_ = copyValList(ups);
    asmp_ob->cncl_->u.eval_->exp_ = copyExp(d1);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->cncl_type = EVAL;
    asmp_ob->next->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->next->cncl_->u.eval_->vallist_ = copyValList(ups);
    asmp_ob->next->cncl_->u.eval_->exp_ = copyExp(d2);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->next->cncl_->cncl_type = EVAL;
    asmp_ob->next->next->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));

    if(asmp_ob->cncl_->u.eval_->val_->val_type==CLSR){
        cncl_ob->rule_type = E_APP;

        ups2 = asmp_ob->cncl_->u.eval_->val_->u.clsr_->vallist_;
        w2 = asmp_ob->next->cncl_->u.eval_->val_;
        d0 = asmp_ob->cncl_->u.eval_->val_->u.clsr_->exp_;

        asmp_ob->next->next->cncl_->u.eval_->vallist_ = (ValList *)malloc(sizeof(ValList));
        asmp_ob->next->next->cncl_->u.eval_->vallist_->prev = copyValList(ups2);
        asmp_ob->next->next->cncl_->u.eval_->vallist_->val_ = copyVal(w2);
        asmp_ob->next->next->cncl_->u.eval_->exp_ = copyExp(d0);
    }else{
        cncl_ob->rule_type = E_APPREC;

        ups2 = asmp_ob->cncl_->u.eval_->val_->u.clsrrec_->vallist_;
        w2 = asmp_ob->next->cncl_->u.eval_->val_;
        d0 = asmp_ob->cncl_->u.eval_->val_->u.clsrrec_->exp_;

        asmp_ob->next->next->cncl_->u.eval_->vallist_ = (ValList *)malloc(sizeof(ValList));
        asmp_ob->next->next->cncl_->u.eval_->vallist_->prev = (ValList *)malloc(sizeof(ValList));
        asmp_ob->next->next->cncl_->u.eval_->vallist_->prev->prev = copyValList(ups2);
        asmp_ob->next->next->cncl_->u.eval_->vallist_->prev->val_ = copyVal(asmp_ob->cncl_->u.eval_->val_);
        asmp_ob->next->next->cncl_->u.eval_->vallist_->val_ = copyVal(w2);
        asmp_ob->next->next->cncl_->u.eval_->exp_ = copyExp(d0);
    }

    derivation(asmp_ob->next->next->cncl_,d+1);
    asmp_ob->next->next->next = NULL;

    Val *val_ob = copyVal(asmp_ob->next->next->cncl_->u.eval_->val_);
    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void E_LetRec(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    ind(d);
    printf("E-LetRec: ");
    writeValList(cncl_ob->u.eval_->vallist_);
    printf(" |- ");
    writeExp(cncl_ob->u.eval_->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = E_LETREC;

    ValList *ups = cncl_ob->u.eval_->vallist_;
    Exp *d1 = cncl_ob->u.eval_->exp_->u.letrec_->exp1_;
    Exp *d2 = cncl_ob->u.eval_->exp_->u.letrec_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->cncl_type = EVAL;
    asmp_ob->cncl_->u.eval_ = (Eval *)malloc(sizeof(Eval));
    asmp_ob->cncl_->u.eval_->vallist_ = (ValList *)malloc(sizeof(ValList));
    asmp_ob->cncl_->u.eval_->vallist_->prev = copyValList(ups);
    asmp_ob->cncl_->u.eval_->vallist_->val_ = (Val *)malloc(sizeof(Var));
    asmp_ob->cncl_->u.eval_->vallist_->val_->val_type = CLSRREC;
    asmp_ob->cncl_->u.eval_->vallist_->val_->u.clsrrec_ = (ClsrRec *)malloc(sizeof(ClsrRec));
    asmp_ob->cncl_->u.eval_->vallist_->val_->u.clsrrec_->vallist_ = copyValList(ups);
    asmp_ob->cncl_->u.eval_->vallist_->val_->u.clsrrec_->exp_ = copyExp(d1);
    asmp_ob->cncl_->u.eval_->exp_ = copyExp(d2);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = NULL;

    Val *val_ob = copyVal(asmp_ob->cncl_->u.eval_->val_);
    cncl_ob->asmp_ = asmp_ob;
    cncl_ob->u.eval_->val_ = val_ob;
    return;
}

void derivation(Cncl *cncl_ob, int d){
    if(cncl_ob->cncl_type == INFR){
        InfrOpType tmp = cncl_ob->u.infr_->infr_type;
        if(tmp==PLUS) B_Plus(cncl_ob,d);
        else if(tmp==MINUS) B_Minus(cncl_ob,d);
        else if(tmp==TIMES) B_Times(cncl_ob,d);
        else B_Lt(cncl_ob,d);
    }else{
        ExpType tmp = cncl_ob->u.eval_->exp_->exp_type;
        if(tmp == INT) E_Int(cncl_ob,d);
        else if(tmp == BOOL) E_Bool(cncl_ob,d);
        else if(tmp == VAR) E_Var(cncl_ob,d);
        else if(tmp == OP) E_Op(cncl_ob, d);
        else if(tmp == IF) E_If(cncl_ob, d);
        else if(tmp == LET) E_Let(cncl_ob,d);
        else if(tmp == FUN) E_Fun(cncl_ob,d);
        else if(tmp == APP) E_App(cncl_ob,d);
        else E_LetRec(cncl_ob,d);
    }
    return;
}
