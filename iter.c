#include <malloc.h>
#include "iter.h"
sklIter *sklIterCreate(skiplist *list, skiplistnode *node, int type){
    sklIter *iter = malloc(sizeof(*iter));
    iter->list = list;
    iter->node = node;
    iter->asc = type;
    return iter;
}

skiplistnode *sklIterNext(sklIter *iter){
    skiplistnode *next;
    if(iter->asc == ASC){
        next = iter->node->level[0].forward;
    }else{
        next = iter->node->backward;
    }
    if(next && next != iter->list->head){
        iter->node = next;
        return next;
    }
    return NULL;
}

void sklIterFree(sklIter *iter){
    free(iter);
}
