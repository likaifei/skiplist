#include <math.h>
#include <malloc.h>
#include "skiplist.h"
#include <string.h>

//创建一个节点
skiplistnode *sklCreateNode(int level, double score, char *ele){
    skiplistnode *node = malloc(sizeof(*node) + level*sizeof(skiplistlevel));
    node->score = score;
    node->ele = ele;
    return node;
}

//创建一个列表
skiplist *sklCreate(void){
    skiplist *list = malloc(sizeof(*list));
    int j;
    list->level = 1;
    list->length = 0;
    list->head = sklCreateNode(SKIPLIST_MAXLEVEL, 0, NULL);
    for(j = 0; j < SKIPLIST_MAXLEVEL; j++){
        list->head->level[j].forward = NULL;
        list->head->level[j].span = 0;
    }
    list->head->backward = NULL;
    return list;
}
//销毁节点
void sklFreeNode(skiplistnode *node){
    free(node->ele);
    free(node);
}
//销毁列表
void sklFree(skiplist *list){
    skiplistnode *node = list->head->level[0].forward, *next;
    free(list->head);
    while(node){
        next = node->level[0].forward;
        sklFreeNode(node);
        node = next;
    }
    free(list);
}
//随机一个层数
int zslRandomLevel(void){
    int level = 1;
    while((random() & 0xFFFF) < (SKIPLIST_P * random())){
        level += 1;
    }
    return level > SKIPLIST_MAXLEVEL? SKIPLIST_MAXLEVEL: level;
}

skiplistnode *sklInsert(skiplist *list, double score, char *ele){
    skiplistnode *update[SKIPLIST_MAXLEVEL], *x;
    unsigned int rank[SKIPLIST_MAXLEVEL];
    int i, level;
    x = list->head;
    //从最高级别往下遍历, 找到每个级别合适的位置
    for(i = list->level - 1; i > 0; i--){
        //初始化每层跨度
        rank[i] = i == (list->level - 1) ? 0 : rank[i + 1];
        while(x->level[i].forward &&//条件是节点存在
                (x->level[i].forward->score < score ||//并且 分值小于要插入的分值
                (x->level[i].forward->score = score && //或者 分值相等但是字符串小于要插入的字符串
                strcmp(x->level[i].forward->ele, ele) < 0))){//继续往前找, 知道条件失败 就是要插入的位置
                    rank[i] += x->level[i].span;//更新每层的跨度
                    x = x->level[i].forward;
                }
        //保存每一级的插入位置,等待更新
        update[i] = x;
    }
    //随机出一个层级
    level = zslRandomLevel();
    if(level > list->level){//超过了目前列表中的最大层级
        for(i = list->level; i < level; i++){
            //初始化层级信息
            rank[i] = 0;
            update[i] = list->head;
            update[i]->level[i].span = list->length;
        }
        //更新最大层级
        list->level = level;
    }
    x = sklCreateNode(level, score, ele);
    for(i = 0; i < level; i++){
        //插入
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;
        //更新跨度
        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    //未受影响的高层更新跨度
    for(i = level; i < list->level; i++){
        update[i]->level[i].span ++;
    }
    //更新后退指针
    x->backward = (update[0] == list->head)? NULL : update[0];
    if(x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        list->tail = x;
    
    list->length ++;
    return x;
}

//帮助删除函数
void sklDeleteNode(skiplist *list, skiplistnode *node, skiplistnode **update){
    int i;
    for(i = 0; i < list->level; i++){
        if(update[i]->level[i].forward == node){
            update[i]->level[i].forward = node->level[i].forward;
            update[i]->level[i].span =+ node->level[i].span - 1;
        }else{
            update[i]->level[i].span --;
        }
    }
    if(node->level[0].forward){
        node->level[0].forward->backward = node->backward;
    }else{
        list->tail = node->backward;
    }
    while(list->level > 1 && list->head->level[list->level - 1].forward == NULL)
        list->level--;
    list->length --;
}

//根据键值加分数删除一个元素
int sklDelete(skiplist *list, double score, char *ele, skiplistnode **node){
    skiplistnode *update[SKIPLIST_MAXLEVEL], *x;
    int i;
    x = list->head;
    //从最高级别往下遍历, 找到每个级别合适的位置
    for(i = list->level - 1; i > 0; i--){
        while(x->level[i].forward &&//条件是节点存在
                (x->level[i].forward->score < score ||//并且 分值小于要插入的分值
                (x->level[i].forward->score = score && //或者 分值相等但是字符串小于要插入的字符串
                strcmp(x->level[i].forward->ele, ele) < 0))){//继续往前找, 知道条件失败 就是要插入的位置
                    x = x->level[i].forward;
                }
        //保存每一级的插入位置,等待更新
        update[i] = x;
    }
    x = x->level[0].forward;
    //判断是否相等
    if(x && score == x->score && strcmp(x->ele, ele) == 0){
        sklDeleteNode(list, x, update);
        //是否返回删除的元素
        if(!node){
            sklFreeNode(x);
        }else{
            *node = x;
        }
        return 1;
    }
    return 0;
}

int sklDeleteByScore(skiplist *list, double score, char *ele, skiplistnode **node){
    skiplistnode *update[SKIPLIST_MAXLEVEL], *x;
    int i;
    x = list->head;
    //从最高级别往下遍历, 找到每个级别合适的位置
    for(i = list->level - 1; i > 0; i--){
        while(x->level[i].forward &&//条件是节点存在
                x->level[i].forward->score < score{//只比较分数
                    x = x->level[i].forward;
                }
        //保存每一级的插入位置,等待更新
        update[i] = x;
    }
    x = x->level[0].forward;
    //判断是否相等
    if(x && score == x->score){//只比较分数
        sklDeleteNode(list, x, update);
        //是否返回删除的元素
        if(!node){
            sklFreeNode(x);
        }else{
            *node = x;
        }
        return 1;
    }
    return 0;
}

skiplistnode *sklFindByScore(skiplist *list, double score){
    int i;
    skiplistnode *x;
    x = list->head;
    for(i = list->level - 1; i > 0; i--){
        while(x->level[i].forward &&//条件是节点存在
                x->level[i].forward->score < score //并且 分值小于要插入的分值
                ){//继续往前找, 知道条件失败 就是要插入的位置
                    x = x->level[i].forward;
                }
    }
    x = x->level[0].forward;
    if(x && score == x->score) return x;
    return NULL;
}

skiplistnode *sklFindByMin(skiplist *list, double score){
    int i;
    skiplistnode *x;
    x = list->head;
    for(i = list->level - 1; i > 0; i--){
        while(x->level[i].forward &&//条件是节点存在
                x->level[i].forward->score < score //并且 分值小于要插入的分值
                ){//继续往前找, 知道条件失败 就是要插入的位置
                    x = x->level[i].forward;
                }
    }
    x = x->level[0].forward;
    if(x && x->score >= score) return x;
    return NULL;
}

skiplistnode *sklFindByMax(skiplist *list, double score){
    int i;
    skiplistnode *x;
    x = list->head;
    for(i = list->level - 1; i > 0; i--){
        while(x->level[i].forward &&//条件是节点存在
                x->level[i].forward->score < score //并且 分值小于要插入的分值
                ){//继续往前找, 知道条件失败 就是要插入的位置
                    x = x->level[i].forward;
                }
    }
    //往前试探一下看看 是不是等于 max 如果等于就返回这个就对了
    if(x->level[0].forward && x->level[0].forward->score == score) return x->level[0].forward;
    if(x) return x;
    return NULL;
}
