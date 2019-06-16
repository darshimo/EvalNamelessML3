#include "param.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef DBG_ALL
#define DBG_READ
#endif

#ifdef DBG_READ
#include <stdio.h>
void ind(int d);
#endif


void error(char *);

Int *readInt(char *);
Bool *readBool(char *);
Clsr *readClsr(char *, char *);
ClsrRec *readClsrRec(char *, char *);
ValList *readValList(char *);
Val *readVal(char *);
Var *readVar(char *);
If *readIf(char *);
Op *readOp(char *);
Let *readLet(char *);
Fun *readFun(char *);
App *readApp(char *);
LetRec *readLetRec(char *);
Exp *readExp(char *);
Infr *readInfr(char *);
Eval *readEval(char *);
Cncl *readCncl(char *);


Int *readInt(char *str){
#ifdef DBG_READ
    printf("int : %s\n",str);
#endif
    Int *ob = (Int *)malloc(sizeof(Int));
    ob->i = atoi(str);
    return ob;
}


Bool *readBool(char *str){
#ifdef DBG_READ
    printf("bool : %s\n",str);
#endif
    Bool *ob = (Bool *)malloc(sizeof(Bool));
    if(str[0]=='t')ob->b = 1;
    else ob->b = 0;
    return ob;
}


Clsr *readClsr(char *str1, char *str2){
#ifdef DBG_READ
    printf("clsr : (%s)[%s]\n",str1,str2);
#endif
    Clsr *ob = (Clsr *)malloc(sizeof(Clsr));
    ob->vallist_ = readValList(str1);
    for(int i=0;i<3;i++){
        str2 += strcspn(str2," ");
        str2 += strspn(str2," ");
    }
    ob->exp_ = readExp(str2);
    return ob;
}


ClsrRec *readClsrRec(char *str1, char *str2){
#ifdef DBG_READ
    printf("clserec : (%s)[%s]\n",str1,str2);
#endif
    ClsrRec *ob = (ClsrRec *)malloc(sizeof(ClsrRec));
    ob->vallist_ = readValList(str1);
    char *tmp1, *tmp2;
    for(int i=0;i<6;i++){
        str2 += strcspn(str2," ");
        str2 += strspn(str2," ");
    }
    ob->exp_ = readExp(str2);

    return ob;
}


ValList *readValList(char *str){
    if(*str=='\0')return NULL;

    ValList *ob = (ValList *)malloc(sizeof(ValList));

    ValList *vallist_tmp = NULL;

    char *tmp1 = str;
    str = NULL;
    char *tmp2 = tmp1;

    int count = 0;
    while(1){
        tmp2 += strcspn(tmp2,"(),");
        if(*tmp2=='('){
            count++;
        }else if(*tmp2==')'){
            count--;
        }else if(*tmp2==','){
            if(count==0){
                str = tmp2;
            }
        }else{
            break;
        }
        tmp2++;
    }

    if(str==NULL){
        str = tmp1;
    }else{
        *str = '\0';
        str++;
        str+=strspn(str," ");
        vallist_tmp = readValList(tmp1);
    }

#ifdef DBG_READ
    printf("vallist : %s\n",str);
#endif

    ob->val_ = readVal(str);
    ob->prev = vallist_tmp;

    return ob;
}


Val *readVal(char* str){
#ifdef DBG_READ
    printf("val : %s\n",str);
#endif
    Val *ob = (Val *)malloc(sizeof(Val));

    if(str[0]=='t'||str[0]=='f'){
        ob->val_type = BOOL_;
        ob->u.bool_ = readBool(str);
    }else if(47<str[0]&&str[0]<58){
        ob->val_type = INT_;
        ob->u.int_ = readInt(str);
    }else if(str[0]=='('){
        char *str1, *str2;
        str++;
        str1 = str;
        int count = 0;
        while(1){
            str+=strcspn(str,"()");
            if(*str=='('){
                count++;
            }else if(*str==')'){
                if(count==0){
                    break;
                }
                count--;
            }else{
                error("1mismatch parentheses.");
            }
            str++;
        }
        *str = '\0';
        str+=2;
        str+=strspn(str," ");
        str2 = str;
        *strrchr(str2,']') = '\0';
        if(str2[0]=='f'){
            ob->val_type = CLSR;
            ob->u.clsr_ = readClsr(str1,str2);
        }else if(str2[0]=='r'){
            ob->val_type = CLSRREC;
            ob->u.clsrrec_ = readClsrRec(str1,str2);
        }else{
            error("val is not correct\n");
        }
    }else{
        error("val is not correct\n");
    }

    return ob;
}


Var *readVar(char *str){
#ifdef DBG_READ
    printf("var : %s\n",str);
#endif
    Var *ob = (Var *)malloc(sizeof(Var));
    str++;
    *(str+strcspn(str," "))='\0';
    ob->n = atoi(str);
    return ob;
}


If *readIf(char *str){
#ifdef DBG_READ
    printf("if  : %s\n",str);
#endif
    If *ob = (If *)malloc(sizeof(If));

    int count = 0;
    char *str1,*str2,*str3;
    str+=strcspn(str," ");
    str+=strspn(str," ");
    str1 = str;
    while(*str!='\0'){
        if(strncmp(str-1," if ", 4)==0){
            count++;
        }else if(strncmp(str-1," then ",6)==0){
            if(count){
                count--;
            }else{
                *str = '\0';
                str++;
                break;
            }
        }
        str+=strcspn(str," ");
        str+=strspn(str," ");
    }
    str+=strcspn(str," ");
    str+=strspn(str," ");
    str2 = str;
    while(*str!='\0'){
        if(strncmp(str-1," if ",4)==0){
            count++;
        }else if(strncmp(str-1," else ",6)==0){
            if(count){
                count--;
            }else{
                *str = '\0';
                str++;
                break;
            }
        }
        str+=strcspn(str," ");
        str+=strspn(str," ");
    }
    str+=strcspn(str," ");
    str+=strspn(str," ");
    str3 = str;

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);
    ob->exp3_ = readExp(str3);

    return ob;
}


Op *readOp(char* str){
#ifdef DBG_READ
    printf("op  : %s\n",str);
#endif
    Op *ob = (Op *)malloc(sizeof(Op));

    int count = 0;
    ob->op_type = TIMES;
    char *str1, *str2;
    str1 = str;
    str2 = NULL;
    while(1){
        str += strcspn(str,"(+-*<)il");
        if(str[0]=='('){
            count++;
        }else if(str[0]==')'){
            count--;
        }else if(count==0){
            if(strncmp(str-1," if ",4)*strncmp(str-1," let ",5)==0){
                break;
            }else if(strncmp(str-1," + ",3)==0){
                ob->op_type=PLUS;
                str2 = str;
            }else if(strncmp(str-1," - ",3)==0){
                if(str==str1){
                    str++;
                    continue;
                }
                ob->op_type=MINUS;
                str2 = str;
            }else if(strncmp(str-1," < ",3)==0){
                ob->op_type=LT;
                str2 = str;
                break;
            }else if(strncmp(str-1," * ",3)==0){
                if(ob->op_type==TIMES){
                    str2 = str;
                }
            }else if(*str=='\0'){
                break;
            }
        }
        str++;
    }

    if(str2==NULL)error("invalid.");

    *str2 = '\0';
    str2++;
    str2 += strspn(str2," ");
    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);

    return ob;
}


Let *readLet(char *str){
#ifdef DBG_READ
    printf("let : %s\n",str);
#endif
    Let *ob = (Let *)malloc(sizeof(Let));

    char *str1, *str2;

    str1 = str;
    for(int i=0;i<3;i++){
        str1 += strcspn(str1," ");
        str1 += strspn(str1," ");
    }
    str2 = str1;

    int count = 0;
    while(1){
        str2+=strcspn(str2,"li");
        if(strncmp(str2-1," let ",5)==0){
            count++;
        }else if(strncmp(str2-1," in ",4)==0){
            if(count==0){
                break;
            }
            count--;
        }else if(*str2=='\0'){
            error("2mismatch let and in.");
        }
        str2++;
    }

    *str2 = '\0';
    str2++;
    str2 += strcspn(str2," ");
    str2 += strspn(str2," ");

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);

    return ob;
}


Fun *readFun(char *str){
#ifdef DBG_READ
    printf("fun : %s\n",str);
#endif
    Fun *ob = (Fun *)malloc(sizeof(Fun));
    for(int i=0;i<3;i++){
        str += strcspn(str," ");
        str += strspn(str," ");
    }

    ob->exp_ = readExp(str);

    return ob;
}


App *readApp(char *str){
#ifdef DBG_READ
    printf("app : %s\n",str);
#endif
    App *ob = (App *)malloc(sizeof(App));
    char *str1 = str;
    char *str2 = NULL;
    int count;

    while(1){
        if(*str=='('){
            count = 0;
            str++;
            while(1){
                str+=strcspn(str,"()");
                if(*str=='('){
                    count++;
                }else if(*str==')'){
                    if(count==0){
                        break;
                    }
                    count--;
                }else{
                    error("3mismatch parentheses.");
                }
                str++;
            }
            str++;
        }else{
            str+=strcspn(str," ");
        }
        str+=strspn(str," ");
        if(*str=='\0'){
            break;
        }else{
            str2 = str;
        }
    }

    if(str2==NULL)error("sole app.");

    *(str2-1)='\0';

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);
    return ob;
}


LetRec *readLetRec(char *str){
#ifdef DBG_READ
    printf("letrec : %s\n",str);
#endif
    LetRec *ob = (LetRec *)malloc(sizeof(LetRec));
    char *str1, *str2;
    str1 = str;
    for(int i=0;i<7;i++){
        str1 += strcspn(str1," ");
        str1 += strspn(str1," ");
    }
    str2 = str1;

    int count = 0;
    while(1){
        str2+=strcspn(str2,"li");
        if(strncmp(str2-1," let ",5)==0){
            count++;
        }else if(strncmp(str2-1," in ",4)==0){
            if(count==0){
                break;
            }
            count--;
        }else if(*str2=='\0'){
            error("4mismatch let and in.");
        }
        str2++;
    }
    *str2 = '\0';
    str2+=2;
    str2 += strspn(str2," ");

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);

    return ob;
}


Exp *readExp(char* str){

    if(str[0]=='('){
        int count = 0;
        char *tmp1, *tmp2;
        tmp1 = str+1;
        while(1){
            tmp1+=strcspn(tmp1,"()");
            if(*tmp1=='('){
                count++;
            }else if(*tmp1==')'){
                if(count==0){
                    break;
                }
                count--;
            }else{
                error("5mismatch parentheses.");
            }
            tmp1++;
        }
        if(*tmp1=='\0')error("6mismatch parentheses.");
        tmp2=tmp1 + 1;
        if(*(tmp2+strspn(tmp2," "))=='\0'){
            str++;
            *tmp1 = '\0';
            return readExp(str);
        }
    }

#ifdef DBG_READ
    //printf("exp : %s\n",str);
#endif

    Exp *ob = (Exp *)malloc(sizeof(Exp));

    if(strncmp(str,"let rec ",8)==0){//when exp is letrec
        ob->exp_type = LETREC;
        ob->u.letrec_ = readLetRec(str);
    }else if(strncmp(str,"let ",4)==0){//when exp is let
        ob->exp_type = LET;
        ob->u.let_ = readLet(str);
    }else if(strncmp(str,"fun ",4)==0){//when exp is fun
        ob->exp_type = FUN;
        ob->u.fun_ = readFun(str);
    }else if(strncmp(str,"if ",3)==0){//when exp is if
        ob->exp_type = IF;
        ob->u.if_ = readIf(str);
    }else if(strncmp(str,"true",4)*strncmp(str,"false",5)==0){//when exp is bool
        ob->exp_type = BOOL;
        ob->u.bool_ = readBool(str);
    }else{

        char *tmp;
        tmp = str;
        tmp += strspn(tmp,"0123456789");
        tmp += strspn(tmp," ");
        if(*tmp=='\0'){//when exp is int
            ob->exp_type = INT;
            ob->u.int_ = readInt(str);
        }else{

            tmp = str;
            tmp += strcspn(tmp," ()+-*<");
            tmp += strspn(tmp," ");
            if(*tmp=='\0'){//when exp is var
                ob->exp_type = VAR;
                ob->u.var_ = readVar(str);
            }else{

                tmp = str;
                int count = 0;
                while(1){
                    tmp+=strcspn(tmp,"()+-*<");
                    if(*tmp=='('){
                        count++;
                    }else if(*tmp==')'){
                        count--;
                    }else if(*tmp!='\0'){
                        if(count==0)break;
                    }else{
                        break;
                    }
                    tmp++;
                }
                if(*tmp=='\0'){//when exp is app
                    ob->exp_type = APP;
                    ob->u.app_ = readApp(str);
                }else{//when exp is op
                    ob->exp_type = OP;
                    ob->u.op_ = readOp(str);
                }
            }
        }
    }

    return ob;
}


Eval *readEval(char* str){
#ifdef DBG_READ
    printf("eval: %s\n",str);
#endif
    Eval *ob = (Eval *)malloc(sizeof(Eval));

    char *str1, *str2, *str3;
    str1 = str;
    str2 = strstr(str1,"|-");
    *str2 = '\0';
    str2+=2;
    str2+=strspn(str2," ");
    str3 = strstr(str2," evalto ");
    *str3 = '\0';
    str3+=8;
    str3+=strspn(str3," ");

    ob->vallist_ = readValList(str1);
    ob->exp_ = readExp(str2);
    ob->val_ = readVal(str3);

    return ob;
}


Infr *readInfr(char* str){
#ifdef DBG_READ
    printf("infr: %s\n",str);
#endif
    Infr *ob = (Infr *)malloc(sizeof(Infr));

    char *tp;

    tp = strtok(str," ");
    ob->int1 = atoi(tp);

    tp = strtok(NULL," ");
    if(strcmp(tp,"plus")==0){
        ob->infr_type = PLUS;
    }else if(strcmp(tp,"minus")==0){
        ob->infr_type = MINUS;
    }else if(strcmp(tp,"times")==0){
        ob->infr_type = TIMES;
    }else{
        ob->infr_type = LT;
        tp = strtok(NULL," ");
    }

    tp = strtok(NULL," ");
    ob->int2 = atoi(tp);

    tp = strtok(NULL," ");

    tp = strtok(NULL," ");
    ob->val_ = readVal(tp);

    return ob;
}


Cncl *readCncl(char* str){
#ifdef DBG_READ
    printf("cncl: %s\n",str);
#endif
    Cncl* ob = (Cncl *)malloc(sizeof(Cncl));

    str += strspn(str," ");
    if(strstr(str," is ")==NULL){
        ob->cncl_type = EVAL;
        ob->u.eval_ = readEval(str);
    }else{
        ob->cncl_type = INFR;
        ob->u.infr_ = readInfr(str);
    }

    return ob;
}
