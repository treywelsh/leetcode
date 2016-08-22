#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

/*
 * O(n) time
 * O(n) space
 *
 */

/* Stack */
#define STACK_LEN 100
struct stack {
    const struct TreeNode* val[STACK_LEN];
    size_t last;
};

#define stack_init(s) \
    (s)->last = 0; \

#define stack_is_empty(s) \
    ((s)->last == 0)

#define stack_size(s) \
    ((s)->last)

#define stack_push(s, e) \
    (s)->val[((s)->last)++] = (e)

#define stack_pop(s) \
    ((s)->val[--((s)->last)])


/**
 *  Return an array of size *returnSize.
 *  Note: The returned array must be malloced, assume caller calls free().
 */
int*
inorderTraversal(const struct TreeNode* root, int* returnSize) {
    const struct TreeNode* node;
    struct stack stck;
    struct stack temp_ret;
    size_t i;
    int * ret;

    if (root == NULL || returnSize == NULL) {
        return NULL;
    }

    stack_init(&stck);
    stack_init(&temp_ret);

    node = root;
    do {
        if (node != NULL) {
            stack_push(&stck, node);
            node = node->left;
            continue;
        }
        if (!stack_is_empty(&stck)) {
            node = stack_pop(&stck);
            stack_push(&temp_ret, node);
        }
        node = node->right;
    } while (node != NULL || !stack_is_empty(&stck));

    *returnSize = stack_size(&temp_ret);
    ret = malloc(sizeof(int) * stack_size(&temp_ret));
    if (ret == NULL) {
        *returnSize = 0;
        return NULL;
    }
    for (i = stack_size(&temp_ret) ; i > 0 ; i--) {
        ret[i - 1] = (stack_pop(&temp_ret))->val;
    }
    return ret;
}

int
main(void) {
    int *ret;
    int ret_len;
    int i;
    struct TreeNode t1 = { .val = 1, .left = NULL, .right = NULL };
    struct TreeNode t2 = { .val = 2, .left = NULL, .right = NULL };
    struct TreeNode t3 = { .val = 3, .left = NULL, .right = NULL };
    struct TreeNode t4 = { .val = 4, .left = NULL, .right = NULL };
    struct TreeNode t5 = { .val = 5, .left = NULL, .right = NULL };
    struct TreeNode t6 = { .val = 6, .left = NULL, .right = NULL };
    t1.left = &t2 ; t1.right = &t3;
    t2.left = &t4 ; t2.right = &t5;
    t3.left = &t6 ;

    ret = inorderTraversal(&t1, &ret_len);
    for (i = 0 ; i < ret_len ; i++) {
        printf("%d ", ret[i]);
    }
    printf("\n");
    free(ret);
}
