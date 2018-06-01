//一个帮助库, 帮助向前还是向后遍历 skiplist
#ifndef __ITER_H
#define __ITER_H
#include "skiplist.h"
#define ASC 1
#define DESC 0   

typedef struct sklIter{
    skiplist *list;
    skiplistnode *node;
    int asc;
} sklIter;

skiplistnode *sklIterNext(sklIter *iter);
sklIter *sklIterCreate(skiplist *list, skiplistnode *node, int type);
void sklIterFree(sklIter *iter);
#endif
