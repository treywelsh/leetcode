#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *
 * O(n) time
 * O(n) space
 *
 */

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

#define	LIST_NEXT(elm, field)		((elm)->field.le_next)

struct helt {
    LIST_ENTRY(helt) next;

    int key;
    int value;
};

#define HEADS_MAX (1 << 10) /* 1024 */
#define ELT_MAX (1 << 14)

#define HEADS_MASK (HEADS_MAX - 1)

struct htable {
    struct helt pool[ELT_MAX];
    size_t pool_sz;

    /* Linked list heads */
    LIST_HEAD(, helt) heads[HEADS_MAX];
};

/* global hash table */
struct htable ht;

static inline void
htable_init() {
    int i;

    ht.pool_sz = 0;

    for (i = 0; i < HEADS_MAX ; i++) {
        LIST_INIT(&ht.heads[i]);
    }
}

static inline int *
htable_get(int key) {
    struct helt * elm;

    LIST_FOREACH(elm, &ht.heads[key & HEADS_MASK], next) {
        if (key == elm->key) {
            return &elm->value;
        }
    }
    return NULL;
}

static inline int 
htable_set(int key, int value) {
    struct helt * elm;

    LIST_FOREACH(elm, &ht.heads[key & HEADS_MASK], next) {
        if (key == elm->key) {
            return 1;
        }
    }

    /* Insert element if pool not empty */
    if (ht.pool_sz == ELT_MAX) {
        return -1;
    }
    ht.pool[ht.pool_sz].key = key;
    ht.pool[ht.pool_sz].value = value;
    LIST_INSERT_HEAD(&ht.heads[key & HEADS_MASK], &ht.pool[ht.pool_sz], next);
    ht.pool_sz++;

    return 0;
}

/*
 * Note: The returned array must be malloced, assume caller calls free().
 */
int*
twoSum(int* nums, int numsSize, int target) {
    int i;
    int * stored_index;
    int * sol;

    htable_init();
        
    /* Only one solution can be supposed,
     * so we do insertion and search in only one loop.
     */
    sol = NULL;
    for (i = 0 ; i < numsSize ; i++) {
        /* insert (target - nums) values in hash*/
        htable_set(target - nums[i], i);

        /* look for nums values in hash */
        stored_index = htable_get(nums[i]);
        if (stored_index == NULL
                || *stored_index == i) {
            continue;
        }

        /* if solution found, return it */
        sol = malloc(2 * sizeof(*sol));
        if (sol == NULL) {
            return NULL;
        }

        if (i < *stored_index) {
            sol[0] = i;
            sol[1] = *stored_index;
            break;
        } 

        sol[0] = *stored_index;
        sol[1] = i;
        break;
    }

    return sol;
}

int
main(void) {
    int array[] = {-1, -2, -3, -4, -5};
    int target = -8;
    int * ret;

    ret = twoSum(array, 5, target);
    if (ret == NULL) {
        return 0;
    }
    printf("(%d, %d) = %d\n", ret[0], ret[1], target);
    free(ret);

    return 0;
}
