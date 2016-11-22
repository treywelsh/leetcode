#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* some choosen elements coming from sys/queue.h, I didn't want to rewrite this part */

#define LIST_ENTRY(type)                        \
    struct {                                \
        struct type *le_next;   /* next element */          \
        struct type **le_prev;  /* address of previous next element */  \
    }


#define LIST_HEAD(name, type)                       \
    struct name {                               \
        struct type *lh_first;  /* first element */         \
    }



#define LIST_INIT(head) do {                        \
    (head)->lh_first = NULL;                    \
} while (/*CONSTCOND*/0)

#define LIST_FOREACH(var, head, field)                  \
    for ((var) = ((head)->lh_first);                \
            (var);                          \
            (var) = ((var)->field.le_next))

#define LIST_REMOVE(elm, field) do {                    \
    if ((elm)->field.le_next != NULL)               \
    (elm)->field.le_next->field.le_prev =           \
    (elm)->field.le_prev;               \
    *(elm)->field.le_prev = (elm)->field.le_next;           \
} while (/*CONSTCOND*/0)



#define LIST_INSERT_HEAD(head, elm, field) do {             \
    if (((elm)->field.le_next = (head)->lh_first) != NULL)      \
    (head)->lh_first->field.le_prev = &(elm)->field.le_next;\
    (head)->lh_first = (elm);                   \
    (elm)->field.le_prev = &(head)->lh_first;           \
} while (/*CONSTCOND*/0)



#define _TAILQ_ENTRY(type, qual)                    \
    struct {                                \
        qual type *tqe_next;        /* next element */      \
        qual type *qual *tqe_prev;  /* address of previous next element */\
    }

#define TAILQ_ENTRY(type)   _TAILQ_ENTRY(struct type,)

#define _TAILQ_HEAD(name, type, qual)                   \
    struct name {                               \
        qual type *tqh_first;       /* first element */     \
        qual type *qual *tqh_last;  /* addr of last next element */ \
    }
#define TAILQ_HEAD(name, type)  _TAILQ_HEAD(name, struct type,)

#define TAILQ_INIT(head) do {                       \
    (head)->tqh_first = NULL;                   \
    (head)->tqh_last = &(head)->tqh_first;              \
} while (/*CONSTCOND*/0)

#define TAILQ_INSERT_HEAD(head, elm, field) do {            \
    if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)    \
    (head)->tqh_first->field.tqe_prev =         \
    &(elm)->field.tqe_next;             \
    else                                \
    (head)->tqh_last = &(elm)->field.tqe_next;      \
    (head)->tqh_first = (elm);                  \
    (elm)->field.tqe_prev = &(head)->tqh_first;         \
} while (/*CONSTCOND*/0)



#define TAILQ_INSERT_TAIL(head, elm, field) do {            \
    (elm)->field.tqe_next = NULL;                   \
    (elm)->field.tqe_prev = (head)->tqh_last;           \
    *(head)->tqh_last = (elm);                  \
    (head)->tqh_last = &(elm)->field.tqe_next;          \
} while (/*CONSTCOND*/0)



#define TAILQ_REMOVE(head, elm, field) do {             \
    if (((elm)->field.tqe_next) != NULL)                \
    (elm)->field.tqe_next->field.tqe_prev =         \
    (elm)->field.tqe_prev;              \
    else                                \
    (head)->tqh_last = (elm)->field.tqe_prev;       \
    *(elm)->field.tqe_prev = (elm)->field.tqe_next;         \
} while (/*CONSTCOND*/0)

#define TAILQ_FOREACH(var, head, field)                 \
    for ((var) = ((head)->tqh_first);               \
            (var);                          \
            (var) = ((var)->field.tqe_next))

#define TAILQ_FIRST(head)       ((head)->tqh_first)

struct cache_lru_elt {
    LIST_ENTRY(cache_lru_elt) next;
    TAILQ_ENTRY(cache_lru_elt) next_lru;

    int key;
    int value;
};

#define HEADS_MAX (1 << 10) /* 1024 */
#define HEADS_MASK (HEADS_MAX - 1)
struct cache_lru {
    struct cache_lru_elt * pool;
    size_t pool_max;

    /* Hash */
    LIST_HEAD(, cache_lru_elt) heads[HEADS_MAX];

    /* LRU */
    TAILQ_HEAD(, cache_lru_elt) lru_head;
};

/* global cache */
struct cache_lru cache;

void lruCacheInit(int capacity) {
    struct cache_lru_elt * elm;
    int i;

    cache.pool_max = capacity;
    cache.pool = malloc(cache.pool_max * sizeof(*elm));
    if (cache.pool == NULL) {
        return;
    }
    memset(cache.pool, -1, cache.pool_max * sizeof(*elm));

    for (i = 0; i < HEADS_MAX ; i++) {
        LIST_INIT(&cache.heads[i]);
    }

    TAILQ_INIT(&cache.lru_head);

    /* Fill LRU with pool elements */
    for (i = 0 ; i < cache.pool_max ; i++) {
        TAILQ_INSERT_HEAD(&cache.lru_head, &cache.pool[i], next_lru);
    }

}

void lruCacheFree() {
    free(cache.pool);
}

int lruCacheGet(int key) {
    struct cache_lru_elt * elm;

    LIST_FOREACH(elm, &cache.heads[key & HEADS_MASK], next) {
        if (key == elm->key) {
            /* LRU update */
            TAILQ_REMOVE(&cache.lru_head, elm, next_lru);
            TAILQ_INSERT_TAIL(&cache.lru_head, elm, next_lru);
            return elm->value;
        }
    }
    return -1;
}

void lruCacheSet(int key, int value) {
    struct cache_lru_elt * elm;
    struct cache_lru_elt * elm2;

    LIST_FOREACH(elm, &cache.heads[key & HEADS_MASK], next) {
        if (key == elm->key) {
            elm->value = value;
            /* LRU update */
            TAILQ_REMOVE(&cache.lru_head, elm, next_lru);
            TAILQ_INSERT_TAIL(&cache.lru_head, elm, next_lru);
            return;
        }
    }

    /* Recycle element with LRU here */
    elm = TAILQ_FIRST(&cache.lru_head);

    /* LRU update */
    TAILQ_REMOVE(&cache.lru_head, elm, next_lru);
    TAILQ_INSERT_TAIL(&cache.lru_head, elm, next_lru);

    /* remove from hash */
    LIST_FOREACH(elm2, &cache.heads[elm->key & HEADS_MASK], next) {
        if (elm->key == elm2->key) {
            LIST_REMOVE(elm, next);
            break;
        }
    }

    elm->key = key;
    elm->value = value;

    /* reinsert in hash */
    LIST_INSERT_HEAD(&cache.heads[key & HEADS_MASK], elm, next);
}

void main(void) {
    int i;

    lruCacheInit(3);

    for (i = 0 ; i < 5 ; i++ ) {
        lruCacheSet(i, i + 1);
        printf("%d\n", lruCacheGet(i));
    }
    printf("\n");
    for (i = 0 ; i < 5 ; i++ ) {
        printf("%d\n", lruCacheGet(i));
    }

    lruCacheFree();
}
