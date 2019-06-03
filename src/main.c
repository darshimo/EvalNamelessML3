#include "param.h"
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG
#ifdef DEBUG
void debugCncl(Cncl *,int);
#endif

void error(char*);
Cncl* readCncl(char*);
Val *copyVal(Val *);
int cmpVal(Val*,Val*);
void derivation(Cncl*, int);
void writeCncl(Cncl *, int);
void freeCncl(Cncl *);


int main(int argc, char *argv[]){
    if(argc!=2)error("arg is not correct.\n");


    char *str = (char *)malloc(sizeof(char)*500);
    char *filename = (char *)malloc(sizeof(char)*20);
    sprintf(filename,"problem/%s",argv[1]);
    FILE *fp;
    if((fp = fopen(filename,"r"))==NULL){
        printf("fopen error.");
        exit(EXIT_FAILURE);
    }
    fgets(str,500,fp);


#ifdef DEBUG
    printf("read start.\n");
#endif
    Cncl *cncl_ob = readCncl(str);
#ifdef DEBUG
    printf("read complete.\n\n");
#endif

    Val *result;
    if(cncl_ob->cncl_type == INFR){
        result = copyVal(cncl_ob->u.infr_->val_);
    }else{
        result = copyVal(cncl_ob->u.eval_->val_);
    }

#ifdef DEBUG
    printf("debug start.\n");
    debugCncl(cncl_ob,0);
    printf("debug complete.\n\n");
#endif


#ifdef DEBUG
    printf("derivation start.\n");
#endif
    derivation(cncl_ob,0);
#ifdef DEBUG
    printf("derivation complete.\n\n");
#endif


    if(cncl_ob->cncl_type == INFR){
        if(cmpVal(cncl_ob->u.infr_->val_,result)){
            error("result is not correct.\n");
        }
    }else{
        if(cmpVal(cncl_ob->u.eval_->val_,result)){
            error("result is not correct.\n");
        }
    }


#ifdef DEBUG
    printf("write start.\n");
#endif
    writeCncl(cncl_ob,0);
#ifdef DEBUG
    printf("write complete.\n");
#endif


#ifdef DEBUG
    printf("free start.\n");
#endif
    freeCncl(cncl_ob);
#ifdef DEBUG
    printf("free complete.\n\n");
#endif

    return 0;
}
