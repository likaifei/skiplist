#include "skiplist.h"
#include <stdio.h>

void main(void){
    skiplist *list;
    skiplistnode *x;
    list = sklCreate();
    sklInsert(list, (double)10, "nihao");
    return ;
    sklInsert(list, (double)15, "nihao");
    sklInsert(list, (double)20, "nihao");
    sklInsert(list, (double)50, "shijie");
    sklInsert(list, (double)51, "sss");
    sklInsert(list, (double)52, "shijie");
    sklInsert(list, (double)100, "sss");
    sklInsert(list, (double)150, "shijie");
    sklInsert(list, (double)234, "sss");

    x = sklFindByScore(list, (double)15);
    printf(x->ele);
    x = sklFindByScore(list, (double)11);
    printf(x->ele);
    x = sklFindByMin(list, (double)21);
    printf(x->ele);
    x = sklFindByMin(list, (double)20);
    printf(x->ele);
    x = sklFindByMax(list, (double)61);
    printf(x->ele);
    x = sklFindByMax(list, (double)52);
    printf(x->ele);
    sklDelete(list, (double)234, "sss", NULL);
    sklDeleteByScore(list, (double)50, NULL);
    sklFree(list);

}