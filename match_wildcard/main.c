#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * O(nm) in time
 * O(nm) in size
 *
 * Not optimal
 */
#define LEN_MAX 4000
bool dyn[LEN_MAX][LEN_MAX];

bool isMatch(const char* string, const char* pattern) {
    size_t i, j;
    size_t pattern_len, string_len;

    pattern_len = strlen(pattern);
    string_len = strlen(string);

    /* Initialize */
    dyn[0][0] = true;
    for (j = 0 ; j < pattern_len ; j++) {
        if (pattern[j] == '*' && dyn[0][j] == true) {
            dyn[0][j + 1] = true;
            continue;
        }
        dyn[0][j + 1] = false;
    }

    /* Fill the array */
    for (i = 0 ; i < string_len; i++) {
        dyn[i + 1][0]   = false;
        for (j = 0 ; j < pattern_len; j++) {
            if (pattern[j] == string[i]
                    || pattern[j] == '?') {
                dyn[i + 1][j + 1] = dyn[i][j];
            }
            else if (pattern[j] == '*'
                    && (dyn[i + 1][j] || dyn[i][j + 1])) {
                dyn[i + 1][j + 1] = true;
            } else {
                dyn[i + 1][j + 1] = false;
            }
        }
    }

    return dyn[string_len][pattern_len];
}

void
test_case(const char * string, const char * pattern) {
    printf("==============================\n");
    printf("str=%s pat=%s match=%d\n", string, pattern, isMatch(string, pattern));
}


    int
main(int argc, char ** argv)
{
    test_case("", "");
    test_case("", "*");
    test_case("a", "*");
    test_case("t", "*t");
    test_case("t", "t*");
    test_case("abc", "*");
    test_case("a", "?");
    test_case("aa", "a");
    test_case("atubic", "a*b?c");
    test_case("atubic", "*");
    test_case("atubic", "a*?c");
    test_case("atubicdkdal", "a*bkjfalsdkf?casdlfja");
    test_case("ho", "**ho");
    test_case("ho", "t**ho");
    return 0;
}
