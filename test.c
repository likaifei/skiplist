#include "skiplist.h"
#include <stdio.h>
#include "iter.h"

skiplist *create(void){
    skiplist *list;
    list = sklCreate();
    sklInsert(list, (double)10, "nihao");
    sklInsert(list, (double)15, "nihao");
    sklInsert(list, (double)20, "nihao");
    sklInsert(list, (double)50, "shijie");
    sklInsert(list, (double)51, "sss");
    sklInsert(list, (double)55, "shijie");
    sklInsert(list, (double)100, "sss");
    sklInsert(list, (double)150, "shijie");
    sklInsert(list, (double)234, "sss");
    return list;
}

void insert_delete_find(void){
    skiplist *list;
    skiplistnode *x;
    list = create();
    x = sklFindByScore(list, (double)15);
    if(x)printf(x->ele);
    sklDelete(list, (double)234, "sss", NULL);
    sklDeleteByScore(list, (double)50, NULL);
    x = sklFindByScore(list, (double)11);
    if(x)printf(x->ele);
    x = sklFindByMin(list, (double)21);
    if(x)printf(x->ele);
    x = sklFindByMin(list, (double)20);
    if(x)printf(x->ele);
    x = sklFindByMax(list, (double)61);
    if(x)printf(x->ele);
    x = sklFindByMax(list, (double)52);
    if(x)printf(x->ele);
    sklFree(list);

}


void test_iter(void){
    skiplist *list;
    skiplistnode *x;
    sklIter *desc, *asc;
    list = create();
    x = sklFindByMax(list, (double)120);
    desc = sklIterCreate(list, x, DESC);
    while(x){
        printf(x->ele);
        x = sklIterNext(desc);
    }
    sklIterFree(desc);
    x = sklFindByMin(list, (double)11);
    asc = sklIterCreate(list, x, ASC);
    while(x){
        printf(x->ele);
        x = sklIterNext(asc);
    }
    sklIterFree(asc);
}

void main(void){
//    insert_delete_find();
    test_iter();   
}

