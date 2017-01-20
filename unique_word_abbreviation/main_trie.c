/* https://leetcode.com/problems/unique-word-abbreviation
 *
 * In time (mean case, what about worst case ?) :
 *  - O(l * n) insertion with l as mean size of abbreviations, and n the count
 *  - O(l) search with l as mean size of abbreviations
 * O(LEN * l * n) in space
 *
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 100
#define ASCII_SHIFT(x) (x) - '0'

/* prefix tree containing abbreviations */
struct ValidWordAbbrNode {
    struct ValidWordAbbrNode* chars[LEN];
    char * word; /* keep a copy of the whole word */
};
typedef struct ValidWordAbbrNode ValidWordAbbr;


/** Initialize your data structure here. */
static ValidWordAbbr*
trieCreate(void) {
    ValidWordAbbr* node;

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    memset(node->chars, 0, LEN * sizeof(struct ValidWordAbbr*));
    node->word = NULL;

    return node;
}

void
insert(ValidWordAbbr* root, const char * abbr, const char* word) {
    size_t i, abbr_len;
    ValidWordAbbr* tNode;
    ValidWordAbbr** tNodeNext;
    bool newWord;

    newWord = false;
    abbr_len = strlen(abbr);
    tNode = root;

    /* insert new node */
    for (i = 0 ; i < abbr_len ; i++) {
        tNodeNext = &tNode->chars[ASCII_SHIFT(abbr[i])];
        if (*tNodeNext == NULL) {
            *tNodeNext = trieCreate();
            newWord = true;
        }
        tNode = *tNodeNext;
    }

    if (newWord) {
        tNode->word = strdup(word);
        return;
    }

    if (tNode->word != NULL && strcmp(tNode->word, word)) {
        free(tNode->word);
        tNode->word = NULL;
    }
}


bool
search(const ValidWordAbbr* root, const char* abbr, const char* word) {
    size_t i, abbr_len;
    const ValidWordAbbr* tNode = root;

    abbr_len = strlen(abbr);
    if (!abbr_len) {
        return true;
    }
    for (i = 0 ; i < abbr_len && tNode->chars[ASCII_SHIFT(abbr[i])] != NULL ; i++) {
        tNode = tNode->chars[ASCII_SHIFT(abbr[i])];
    }
    if (i != abbr_len) {
        return true;
    }

    /* at end of word in tree */
    if (tNode->word == NULL) {
        /* Several word for this abbreviation, not unique */
        return false;
    }
    if (!strcmp(tNode->word, word)) {
        /* Same word for this abbreviation, unique */
        return true;
    }
    /* One word inserted for this abbreviation
     * but another word with same abbr searched,
     * not unique
     */
    return false;
}

/** Deallocates memory previously allocated for the ValidWordAbbr. */
void
trieFree(ValidWordAbbr* root) {
    size_t i;
    if (root->word != NULL) {
        free(root->word);
    }
    for (i = 0 ; i < LEN ; i++) {
        if (root->chars[i] == NULL) {
            continue;
        }
        trieFree(root->chars[i]);
    }
    free(root);
}


static char *
createAbbr(const char * word) {
    size_t word_len = strlen(word);
    char * abbr;

    abbr = malloc(5);
    if (abbr == NULL) {
        return NULL;
    }
    //sprintf(abbr, "%c%lu%c\0", word[0], word_len - 2, word[word_len - 1]);
    sprintf(abbr, "%c%lu%c", word[0], word_len - 2, word[word_len - 1]);

    return abbr;
}

static void
destroyAbbr(char * word) {
    free(word);
}


ValidWordAbbr*
validWordAbbrCreate(char** dictionary, int dictionarySize) {
    ValidWordAbbr* node;
    char * abbr;
    int i;

    node = trieCreate();
    for (i = 0 ; i < dictionarySize ; i++) {
        abbr = createAbbr(dictionary[i]);
        insert(node, abbr, dictionary[i]);
        destroyAbbr(abbr);
    }

    return node;
}

bool
validWordAbbrIsUnique(ValidWordAbbr* vwa, char* word) {
    char * abbr;
    bool ret;

    abbr = createAbbr(word);
    ret = search(vwa, abbr, word);
    destroyAbbr(abbr);
    if (!ret) {
        return false;
    }
    return true;
}


/** Deallocates memory previously allocated for the data structure. */
void
validWordAbbrFree(ValidWordAbbr* vwa) {
    trieFree(vwa);
}

int
main(int argc, char ** argv)
{
    (void)argc; (void)argv;
    //char * words[] = {"dig", "dog", "hello", "hello"};
    char * words[] = {"deer", "door", "cake", "card"};
    //char * words[] = {"cake"};
    //size_t wordsSize = 3;
    size_t wordsSize = 4;
    char * word;
    int i;
    ValidWordAbbr* vwa;

    printf("insert :\n");
    for (i = 0 ; i < wordsSize; i++) {
        printf("%s\n", words[i]);
    }

    vwa = validWordAbbrCreate(words, wordsSize);

    /*
    word = "hello"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    word = "dog"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    word = "doge"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    */
    word = "dear"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    word = "cart"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    word = "cane"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    word = "make"; printf("isUnique(%s)=%d\n", word, validWordAbbrIsUnique(vwa, word));
    validWordAbbrFree(vwa);

    return 0;
}
