#include "skiplist.h"
#include <stdio.h>

void main(void){
    skiplist *list;
    skiplistnode *x;
    list = sklCreate();
    sklInsert(list, (double)10, "nihao");
    sklInsert(list, (double)15, "nihao");
    sklInsert(list, (double)20, "nihao");
    sklInsert(list, (double)50, "shijie");
    sklInsert(list, (double)51, "sss");
    sklInsert(list, (double)52, "shijie");
    sklInsert(list, (double)100, "sss");
    sklInsert(list, (double)150, "shijie");
    sklInsert(list, (double)234, "sss");

    x = sklFindByScore((double)15);
    printf(x->ele);
    x = sklFindByScore((double)11);
    printf(x->ele);
    x = sklFindByMin((double)21);
    printf(x->ele);
    x = sklFindByMin((double)20);
    printf(x->ele);
    x = sklFindByMax((double)61);
    printf(x->ele);
    x = sklFindByMax((double)52);
    printf(x->ele);
    sklDelete(list, (double)234, "sss");
    sklDeleteByScore(list, (double)50);
    sklFree(list);

}