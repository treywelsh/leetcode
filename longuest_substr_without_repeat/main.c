#include <stdio.h>
#include <string.h>

/* Longuest substring without repeting character
 * O(n) in time O(1) in space */
size_t
longuest_substring(const char * s, size_t len) {
    size_t i, j;
    size_t dmax = 1;
    size_t chars[256];
    if (s == NULL) {
        return 0;
    }
    if (len < 2) {
        return len;
    }
    for (i = 0 ; i < 256 ; i++)
        chars[i] = -1;
    for (i = 0, j = 0 ; j < len ; j++) {
        /* Last value keep the previous position of the char */
        if (i <= chars[s[j]]) {
            i = chars[s[j]] + 1;
        }
        if (dmax < j - i + 1) {
            dmax = j - i + 1;
        }
        chars[s[j]] = j;
    }
    return dmax;
}

void
test_case(const char * s) {
    printf("===============\n");
    printf("%s\n%lu\n", s, longuest_substring(s, strlen(s)));
}

    int
main(int argc, char ** argv)
{
    test_case("");
    test_case("aa");
    test_case("aaaaaaaaaaaaaaaaa");
    test_case("ab");
    test_case("abc");
    test_case("aba");
    test_case("abca");
    test_case("abcd abcd abdc");
    test_case("123456789");
    return 0;
}
