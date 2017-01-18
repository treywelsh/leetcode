#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/queue.h"

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
