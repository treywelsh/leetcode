 /* https://leetcode.com/problems/search-in-rotated-sorted-array
 * 
 * O(log(n)) in time
 * O(1) in space
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int search(int* nums, int numsSize, int target) {
    int lo, mid, hi;

    lo = 0;
    hi = numsSize - 1;

    while (lo < hi) {
        mid = (lo + hi) / 2;

        if (nums[lo] <= nums[mid]) {
            /* first part in ascending order*/
            if (target >= nums[lo] && target <= nums[mid]) {
                /* target in first part*/
                hi = mid;
            } else {
                lo = mid + 1;
            }
        } else {
            /* second part in ascending order*/
            if (target > nums[mid] && target <= nums[hi]) {
                /* target in second part*/
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
    }
    if (nums[lo] == target) {
        return lo;
    }
    return -1;
}

int
rotate_array(int * nums, int len) {
    int i, temp, nxt;

    nxt = nums[0];
    for (i = 0 ; i < len - 1 ; i++) {
        temp = nums[(i + 1) % len];
        nums[(i + 1) % len] = nxt;
        nxt = temp;
    }
    nums[0] = nxt;
}

int
test_case(int * nums, int len, int target) {
    int i, j, ret;
    
    for (i = 0 ; i < len ; i++) {
        rotate_array(nums, len);
        printf("search %d in ", target);
        for (j = 0 ; j < len ; j++) {
            printf("%*d ", 2, nums[j]); 
        }
        ret = search(nums, len, target);
        if (ret == -1) {
            printf("not found\n");
        } else {
            printf(", found at %d\n", search(nums, len, target));
        }
    }

}

int
main(void) {
    int ret;
    int i;
    int array1[] = { 0, 1, 2, 3, 4, 5 };
    int array2[] = { 1 };
    int array3[] = { 3, 1 };

    //test_case(array1, 6, 5);
    //test_case(array2, 1, 0);
    test_case(array3, 2, 1);

    return 0;
}
