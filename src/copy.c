#include "param.h"
#include <stdlib.h>
#include <string.h>

#ifdef DBG_ALL
#define DBG_CP
#endif

#ifdef DBG_CP
#include <stdio.h>
#endif

Int *copyInt(Int *);
Bool *copyBool(Bool *);
Clsr *copyClsr(Clsr *);
ClsrRec *copyClsrRec(ClsrRec *);
ValList *copyValList(ValList *);
Val *copyVal(Val *);
Var *copyVar(Var *);
Op *copyOp(Op *);
If *copyIf(If *);
Let *copyLet(Let *);
Fun *copyFun(Fun *);
App *copyApp(App *);
LetRec *copyLetRec(LetRec *);
Exp *copyExp(Exp *);


Int *copyInt(Int *sample){
#ifdef DBG_CP
    printf("copyInt start\n");
#endif
    Int *ob = (Int *)malloc(sizeof(Int));
    ob->i = sample->i;
    return ob;
}

Bool *copyBool(Bool *sample){
#ifdef DBG_CP
    printf("copyBool start\n");
#endif
    Bool *ob = (Bool *)malloc(sizeof(Bool));
    ob->b = sample->b;
    return ob;
}

Clsr *copyClsr(Clsr *sample){
#ifdef DBG_CP
    printf("copyClsr start\n");
#endif
    Clsr *ob = (Clsr *)malloc(sizeof(Clsr));
    ob->vallist_ = copyValList(sample->vallist_);
    ob->exp_ = copyExp(sample->exp_);
    return ob;
}

ClsrRec *copyClsrRec(ClsrRec *sample){
#ifdef DBG_CP
    printf("copyClsrRec start\n");
#endif
    ClsrRec *ob = (ClsrRec *)malloc(sizeof(ClsrRec));
    ob->vallist_ = copyValList(sample->vallist_);
    ob->exp_ = copyExp(sample->exp_);
    return ob;
}

ValList *copyValList(ValList *sample){
    if(sample==NULL)return NULL;
#ifdef DBG_CP
    printf("copyValList start\n");
#endif
    ValList *ob = (ValList *)malloc(sizeof(ValList));
    ob->val_ = copyVal(sample->val_);
    ob->prev = copyValList(sample->prev);
    return ob;
}

Val *copyVal(Val *sample){
#ifdef DBG_CP
    printf("copyVal start\n");
#endif
    Val *ob = (Val *)malloc(sizeof(Val));
    ob->val_type = sample->val_type;
    if(ob->val_type==INT_){
        ob->u.int_ = copyInt(sample->u.int_);
    }else if(ob->val_type==BOOL_){
        ob->u.bool_ = copyBool(sample->u.bool_);
    }else if(ob->val_type==CLSR){
        ob->u.clsr_ = copyClsr(sample->u.clsr_);
    }else{
        ob->u.clsrrec_ = copyClsrRec(sample->u.clsrrec_);
    }
    return ob;
}

Var *copyVar(Var *sample){
#ifdef DBG_CP
    printf("copyVar start\n");
#endif
    Var *ob = (Var *)malloc(sizeof(Var));
    ob->n = sample->n;
    return ob;
};

Op *copyOp(Op *sample){
#ifdef DBG_CP
    printf("copyOp start\n");
#endif
    Op *ob = (Op *)malloc(sizeof(Op));
    ob->op_type = sample->op_type;
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

If *copyIf(If *sample){
#ifdef DBG_CP
    printf("copyIf start\n");
#endif
    If *ob = (If *)malloc(sizeof(If));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    ob->exp3_ = copyExp(sample->exp3_);
    return ob;
}

Let *copyLet(Let *sample){
#ifdef DBG_CP
    printf("copyLet start\n");
#endif
    Let *ob = (Let *)malloc(sizeof(Let));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

Fun *copyFun(Fun *sample){
#ifdef DBG_CP
    printf("copyFun start\n");
#endif
    Fun *ob = (Fun *)malloc(sizeof(Fun));
    ob->exp_ = copyExp(sample->exp_);
    return ob;
}

App *copyApp(App *sample){
#ifdef DBG_CP
    printf("copyApp start\n");
#endif
    App *ob = (App *)malloc(sizeof(App));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

LetRec *copyLetRec(LetRec *sample){
#ifdef DBG_CP
    printf("copyLetRec start\n");
#endif
    LetRec *ob = (LetRec *)malloc(sizeof(LetRec));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

Exp *copyExp(Exp *sample){
#ifdef DBG_CP
    printf("copyExp start\n");
#endif
    Exp *ob = (Exp *)malloc(sizeof(Exp));
    ob->exp_type = sample->exp_type;
    if(ob->exp_type==INT)ob->u.int_ = copyInt(sample->u.int_);
    else if(ob->exp_type==BOOL)ob->u.bool_ = copyBool(sample->u.bool_);
    else if(ob->exp_type==VAR)ob->u.var_ = copyVar(sample->u.var_);
    else if(ob->exp_type==OP)ob->u.op_ = copyOp(sample->u.op_);
    else if(ob->exp_type==IF)ob->u.if_ = copyIf(sample->u.if_);
    else if(ob->exp_type==LET)ob->u.let_ = copyLet(sample->u.let_);
    else if(ob->exp_type==FUN)ob->u.fun_ = copyFun(sample->u.fun_);
    else if(ob->exp_type==APP)ob->u.app_ = copyApp(sample->u.app_);
    else ob->u.letrec_ = copyLetRec(sample->u.letrec_);
#ifdef DBG_CP
    printf("copyExp end\n");
#endif
    return ob;
}
