#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 26
#define ASCII_SHIFT(x) (x) - 'a'

struct TrieNode {
    struct TrieNode* chars[LEN];
    bool isWord;
};

/** Initialize your data structure here. */
struct TrieNode* trieCreate(void) {
    struct TrieNode* node;

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    memset(node->chars, 0, LEN * sizeof(struct TrieNode*));
    node->isWord = false;

    return node;
}

/** Inserts a word into the trie.
 * O(l*n) where l is the mean size of a word and n the count of words
 */
void insert(struct TrieNode* root, const char* word) {
    size_t i, word_len;
    struct TrieNode* tNode;
    struct TrieNode** tNodeNext;

    word_len = strlen(word);
    tNode = root;
    /* insert new node */
    for (i = 0 ; i < word_len ; i++) {
        tNodeNext = &tNode->chars[ASCII_SHIFT(word[i])];
        if (*tNodeNext == NULL) {
            *tNodeNext = trieCreate();
        }
        tNode = *tNodeNext;
    }
    tNode->isWord = true;
}

/** Returns if the word is in the trie.
 * O(l) where l is the mean size of a word
 */
bool search(const struct TrieNode* root, const char* word) {
    size_t i, word_len;
    const struct TrieNode* tNode = root;

    word_len = strlen(word);
    for (i = 0 ; i < word_len && tNode->chars[ASCII_SHIFT(word[i])] != NULL ; i++) {
        tNode = tNode->chars[ASCII_SHIFT(word[i])];
    }
    if (i == word_len) {
        return tNode->isWord;
    }
    return false;
}

/** Returns if there is any word in the trie
 *   that starts with the given prefix.
 * O(l) where l is the mean size of a word
 */
bool startsWith(const struct TrieNode* root, const char* prefix) {
    size_t i, prefix_len;
    const struct TrieNode* tNode;

    tNode = root;
    prefix_len = strlen(prefix);
    for (i = 0 ; i < prefix_len && tNode->chars[ASCII_SHIFT(prefix[i])] != NULL ; i++) {
        tNode = tNode->chars[ASCII_SHIFT(prefix[i])];
    }
    if (i == prefix_len) {
        return true;
    }
    return false;
}

/** Deallocates memory previously allocated for the TrieNode. */
void trieFree(struct TrieNode* root) {
    size_t i;
    for (i = 0 ; i < LEN ; i++) {
        if (root->chars[i] == NULL) {
            continue;
        }
        trieFree(root->chars[i]);
    }
    free(root);
}

    int
main(int argc, char ** argv)
{
    char ret;
    struct TrieNode* node = trieCreate();
    insert(node, "somestring");
    insert(node, "toto");
    insert(node, "tata");
    insert(node, "tutu");
    insert(node, "titi");
    insert(node, "tati");
    insert(node, "tatitu");
    ret = startsWith(node, "ta");
    printf("ret:%d\n", ret);
    ret = search(node, "titi");
    printf("ret:%d\n", ret);
    ret = search(node, "ta");
    printf("ret:%d\n", ret);
    ret = search(node, "tarte");
    printf("ret:%d\n", ret);
    trieFree(node);
    return 0;
}
