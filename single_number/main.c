/* https://leetcode.com/problems/single-number
 *
 * O(n) in time
 * O(1) in space
 *
 */

#include <stdio.h>

int singleNumber(int* nums, int numsSize) {
    int i;
    int res;

    for (i = 0, res = 0 ; i < numsSize ; i++) {
        res ^= nums[i];
    }

    return res;
}

    
int
main(int argc, char ** argv)
{
    int array[] = { 1, 4, 5, 6, 3, 6, 7, 4, 1, 3, 5 };
    int result = 7;
    int i, ret;

    printf("Array : { ");  
    for ( i = 0 ; i < sizeof(array)/sizeof(int) ; i ++) {
        printf("%d, ", array[i]);
    }
    printf(" }\n");

    ret = singleNumber(array, sizeof(array)/sizeof(int));
    if (ret == result) {
        printf("Sucess\n");
    } else {
        printf("Failed\n");
    }
    printf("returned single number : %d\n", ret);
    return 0;
}
