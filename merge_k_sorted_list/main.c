#include <stdio.h>
#include <stdlib.h>

/* The elements are of type int */
struct heap {
    int* val;
    size_t len;
    size_t last; /* index of last inserted value */
};

int
heap_init(struct heap* h, size_t max_len) {
    h->val = malloc(sizeof(*(h->val)) * (max_len + 1));
    if (h->val == NULL) {
        return 1;
    }
    h->val[0] = 0; /* index 0 is excluded. Correspond to empty heap. */
    h->len = max_len;
    h->last = 0;
    return 0;
}

void
heap_clean(struct heap* h) {
   free(h->val);
}

static inline void
heap_up(struct heap* h, size_t idx) {
    size_t i, parent;
    int node;

    i = idx;
    node = h->val[idx];
    for ( parent = i >> 1 ; parent >= 1 ; parent = i >> 1 ) {
        if (node > h->val[parent]) { /* heap min comparison */
            /* insert node */
            break;
        }
        h->val[i] = h->val[parent];
        i = parent;
    }
    h->val[i] = node;
}

static inline void
heap_down(struct heap* h, size_t idx) {
    size_t i, left, right, max_son;
    int node;

    if (idx >= h->last) {
        return;
    }

    i = idx;
    node = h->val[idx];
    for ( left = i << 1 ; left <= h->last ; left = i << 1) {
        right = left + 1;
        max_son = left;
        if (right <= h->last && h->val[right] < h->val[left]) { /* heap min comparison */
           max_son = right;
        }
        if (node < h->val[max_son]) { /* heap min comparison */
            /* insert node */
            break;
        }
        /* value at i index go down */
        h->val[i] = h->val[max_son];
        i = max_son;
    }
    h->val[i] = node;
}

int
heap_push(struct heap* h, int elm) {
    /* heap full */
    if (h->last >= h->len) {
       return 1;
    }

    (h->last)++;
    h->val[h->last] = elm;
    heap_up(h, h->last);

    return 0;
}

int
heap_pop(struct heap* h, int *elm) {
    /* if heap is empty */
    if (h->last == 0) {
        *elm = h->val[0];
        return 1;
    }

    /* keep first value, swap it with last value, call down, and return it*/
    *elm = h->val[1];
    h->val[1] = h->val[h->last];
    (h->last)--;
    heap_down(h, 1);

    return 0;
}


/**
 *
 * Definition for singly-linked list.
 *
 * struct ListNode {
 *      int val;
 *      struct ListNode *next;
 * };
 *
 * Heap based solution :
 * O(nk) in space
 * O(nk*log(nk)) in time ?
 *
 */
struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    int i;
    struct ListNode ret;
    struct ListNode* elm;
    struct ListNode** last;
    struct heap hp;

    if (listsSize < 1 || lists == NULL) {
        return NULL;
    } else if (listsSize == 1) {
        return lists[0];
    }

    ret.next = NULL;
    last = &ret.next;

    heap_init(&hp, 15000);

    /* add list values into the heap (memory limit is not exceeded...),
     * and construct output linked list
     */
    i = 0;
    while (i < listsSize) {
        elm = lists[i];
        while (elm != NULL){
            heap_push(&hp, elm->val);
            *last = elm;
            last = &elm->next;
            elm = elm->next;
        }
        i++;
    }

    /* copy values into nodes */
    elm = ret.next;
    while (elm != NULL) {
        heap_pop(&hp, &elm->val);
        elm = elm->next;
    }

    return ret.next;
}
