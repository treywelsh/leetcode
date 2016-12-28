/* https://leetcode.com/problems/two-sum
 *
 * O(n) time
 * O(n) space
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/queue.h"


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
test_case(int * nums, int numsSize, int target) {
    int * ret;
    int i;

    for (i = 0 ; i < numsSize ; i++) {
        printf("%*d ", 2, nums[i]);
    }
    printf("\n\t");

    ret = twoSum(nums, numsSize, target);
    if (ret == NULL) {
        printf("No solution for target %*d\n", 2 ,target);
        return 0;
    }

    printf("%*d + %*d = %*d ", 2, nums[ret[0]], 2, nums[ret[1]], 2, target);
    if (nums[ret[0]] + nums[ret[1]] == target) {
        printf("PASSED\n");
    } else {
        printf("FAILED : ");



        printf("\n");
    }
    free(ret);
}

int
main(void) {
    int array1[] = {-1, -2, -3, -4, -5};
    int array2[] = { 1, -1,  2,  3,  4, 5};
    int array3[] = {1, 2, 3, 8, 9, 3};
    int array4[] = {3, 5, 1, 0, 3, 5, 20, 10, 6, 7, 9, 7, 8};
    int array5[] = {8, 8};

    test_case(array1, 5, -8);
    test_case(array2, 6, 8);
    test_case(array3, 6, 6);
    test_case(array4, 13, 12);
    test_case(array5, 2, 1);

    return 0;
}
