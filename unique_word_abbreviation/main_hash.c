/* https://leetcode.com/problems/unique-word-abbreviation
 *
 * O(n) time
 * O(n) space
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/queue.h"


struct helt {
    LIST_ENTRY(helt) next;

    char * abbr; /* key: abbreviation*/
    char * word; /* value: entire word */
};
#define helt_key(e) ((e)->abbr)
#define helt_value(e) ((e)->word)

#define HEADS_MAX (1 << 12)
#define ELT_MAX (1 << 14)

#define HEADS_MASK (HEADS_MAX - 1)

struct htable {
    struct helt pool[ELT_MAX];
    size_t pool_sz;

    /* Linked list heads */
    LIST_HEAD(, helt) heads[HEADS_MAX];
};
typedef struct htable ValidWordAbbr;

/* Jenkins one at a time hash algorithm */
static inline size_t
ht_hash(const char * abbr) {
    size_t hash;
    size_t i;
    size_t len;

    len = strlen(abbr);
    hash = 0;
    for (i = 0; i < len ; i++)
    {
        hash += abbr[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}


static inline void
htable_init(struct htable * ht) {
    int i;

    ht->pool_sz = 0;

    for (i = 0; i < HEADS_MAX ; i++) {
        LIST_INIT(&ht->heads[i]);
    }
}

static inline void htable_clean(struct htable * ht) {
    size_t i;
    struct helt * elm;

    for ( i = 0 ; i < HEADS_MAX ; i++ ) {
        LIST_FOREACH(elm, &ht->heads[i & HEADS_MASK], next) {
            if (helt_key(elm) != NULL) { free(helt_key(elm)); }
            if (helt_value(elm) != NULL) { free(helt_value(elm)); }
        }
    }
}

static inline struct helt *
htable_get(struct htable * ht, const char * key) {
    struct helt * elm;
    size_t hash;

    hash = ht_hash(key);
    LIST_FOREACH(elm, &ht->heads[hash & HEADS_MASK], next) {
        if (!strcmp(key, helt_key(elm))) {
            return elm;
        }
    }
    return NULL;
}

static inline int
htable_set(struct htable * ht, const char * key, const char * value) {
    struct helt * elm;
    size_t hash;
    hash = ht_hash(key);

    LIST_FOREACH(elm, &ht->heads[hash & HEADS_MASK], next) {
        if (!strcmp(key, helt_key(elm))) {
            /* abbreviation already inserted */
            if (helt_value(elm) != NULL && strcmp(helt_value(elm), value)) {
                /* but different word */
                free(helt_value(elm));
                helt_value(elm) = NULL;
            }
            return 1;
        }
    }

    /* Insert element if pool not empty */
    if (ht->pool_sz == ELT_MAX) {
        return -1;
    }
    helt_key(&ht->pool[ht->pool_sz]) = strdup(key);
    helt_value(&ht->pool[ht->pool_sz]) = strdup(value);
    LIST_INSERT_HEAD(&ht->heads[hash & HEADS_MASK], &ht->pool[ht->pool_sz], next);
    ht->pool_sz++;

    return 0;
}

static char *
createAbbr(const char * word) {
    size_t word_len;
    char * abbr;
    word_len = strlen(word);

    /* 5 characters : first and last char, up to three digits number,
     *        * and nul terminator */
    abbr = malloc(5);
    if (abbr == NULL) {
        return NULL;
    }
    sprintf(abbr, "%c%lu%c", word[0], word_len - 2, word[word_len - 1]);

    return abbr;
}

static void
destroyAbbr(char * word) {
    free(word);
}


ValidWordAbbr*
validWordAbbrCreate(char** dictionary, int dictionarySize) {
    ValidWordAbbr* ht;
    char * abbr;
    int i;

    ht = malloc(sizeof(*ht));
    if (ht == NULL) {
        return NULL;
    }
    htable_init(ht);
    for (i = 0 ; i < dictionarySize ; i++) {
        abbr = createAbbr(dictionary[i]);
        /* XXX free and return null if fail */
        htable_set(ht, abbr, dictionary[i]);

        destroyAbbr(abbr);
    }

    return ht;
}

bool
validWordAbbrIsUnique(ValidWordAbbr* vwa, char* word) {
    char * abbr;
    struct helt * e;

    abbr = createAbbr(word);
    e = htable_get(vwa, abbr);
    destroyAbbr(abbr);

    /* if word not null (i.e. only one word for the associated abbreviation),
     *        * and if it's the same word */
    if (e == NULL) {
        return true;
    }
    if (helt_value(e) != NULL && !strcmp(helt_value(e), word)) {
        return true;
    }
    return false;
}
/** Deallocates memory previously allocated for the data structure. */
void
validWordAbbrFree(ValidWordAbbr* vwa) {
    htable_clean(vwa);
    free(vwa);
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
