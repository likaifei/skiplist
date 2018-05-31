
#define SKIPLIST_MAXLEVEL 32
#define SKIPLIST_P 0.25

typedef struct skiplistnode{
    char *ele;
    double score;
    struct skiplistnode *backward;
    struct skiplistlevel{
        struct skiplistnode *forward;
        unsigned int span;
    } level[];
} skiplistnode;

typedef struct skiplist{
    struct skiplistnode *head, *tail;
    unsigned long length;
    int level;
} skiplist;

skiplist *sklCreate(void);
skiplistnode *sklFindByScore(skiplist *list, double score);
skiplistnode *sklFindByMin(skiplist *list, double min);
skiplistnode *sklFindByMax(skiplist *list, double max);
skiplistnode *sklInsert(skiplist *list, double score, char *ele);
int sklDelete(skiplist *list, double score, char *ele);
int sklDeleteByScore(skiplist *list, double score);
