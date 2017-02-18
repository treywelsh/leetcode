/* https://leetcode.com/problems/circular-array-loop
*
* O(n) in time
* O(1) in space
*
*/

#include <stdio.h>
#include <stdbool.h>

#define next(x, arr) (((x) + (arr)[(x)]) % numsSize) /* /!\ depends of numsSize */

bool circularArrayLoop(int* nums, int numsSize) {
    int slw, fst, fst_step;
    int i;

    for ( i = 0 ; i < numsSize ; i++ ) {
        slw = i;
        fst = i;
        fst_step = next(fst, nums);
        /* XXX multiplication can result in an integer overflow.
         * Just one sign is needed */
        while (nums[fst] * nums[fst_step] > 0 && nums[fst_step] * nums[next(fst_step, nums)] > 0) {
            fst = next(fst_step, nums);
            if (fst_step == fst) {
                break;
            }

            if (slw == fst) {
                return true;
            }

            slw = next(slw, nums);
            fst_step = next(fst, nums);
        }
        nums[i] = 0;

        /* XXX Here we can re-run elements and set them to zero */
    }
    return false;
}


void
_test_case(int * array, int array_len) {
    int i;

    /* print array */
    printf("array : ");
    for ( i = 0 ; i < array_len ; i++ ) {
        printf("%d ", array[i]);
    }
    printf("\n");

    if (circularArrayLoop(array, array_len)) {
        printf(" --> loop !\n");
    }
}

#define test_case(array) _test_case((array), sizeof((array))/sizeof((array)[0]))

int
main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    int array[] = { 2, -1, 1, 2, 2 };
    int array2[] = { -1, 2 };
    int array3[] = { -2, 1, -1, -2, -2 };
    int array4[] = { 1, -1, 1, 1 };
    int array5[] = {3, 2, 1};

    test_case(array);
    test_case(array2);
    test_case(array3);
    test_case(array4);
    test_case(array5);

    return 0;
}
