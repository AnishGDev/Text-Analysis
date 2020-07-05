// COMP2521 20T2 Assignment 1
// Dict.c ... implementsation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Dict.h"
#include "WFreq.h"

typedef struct _DictNode *Link;

typedef struct  _DictNode {
   WFreq  data;
   int height; 
   Link   left;
   Link   right;
} DictNode;

struct _DictRep {
   Link tree;
};

// Helpful short inline functions. Safer and more readable than macros. 
static inline int calculateHeight(Link currNode);
static inline int max(int a, int b);

// Defining Custom Helper Functions.
Link createNewNode(char *w);

Link insertRecursivelyHelper(Link currNode, char *w, WFreq **insertedWord); 
Link rotateLeft(Link n);
Link rotateRight(Link n);

// Top N helper functions. 
void recursiveHelperTopN(WFreq * results, Link currNode, int *size, int max);
void insertInOrder( WFreq *results, WFreq *nodeToInsert, int *currSize, int nWordSize);
void shiftArrayAndInsert(WFreq * results, WFreq *nodeToInsert, int loc, int size);
int binarySearch(WFreq *results, WFreq *nodeToInsert, int low, int high);


static inline int calculateHeight(Link currNode) {
    return (currNode == NULL ? -1 : currNode->height); 
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

// create new empty Dictionary
Dict newDict() {
    Dict newTree = malloc(sizeof(struct _DictRep));
    // Check whether memory was allocated or not. 
    if (newTree == NULL) {
        fprintf(stderr, "Couldn't allocate memory to create new tree");
        exit(EXIT_FAILURE);
    }
    newTree->tree = NULL;
    return newTree;
}

Link createNewNode(char *w) {
    Link newNode = malloc(sizeof(struct _DictNode));
    if (newNode == NULL) {
        fprintf(stderr, "Couldn't allocate memory to create new node");
        exit(EXIT_FAILURE);
    }
    newNode->height = 0; 
    newNode->left = NULL;
    newNode->right = NULL;
    // copy the string over. strdup performs malloc checks automatically.
    newNode->data.word = strdup(w);
    newNode->data.freq = 1;
    return newNode;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w) {
    Link curr = d->tree; 
    while(curr != NULL) {
        int cmpFactor = strcmp(curr->data.word, w);
        if (cmpFactor > 0) {
            curr=curr->left;
        } else if (cmpFactor < 0) {
            curr = curr->right;
        } else {
            return &(curr->data);
        }
    }
    return NULL;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
    assert(d != NULL); // Can't be inserting into a non-existent tree. 
    WFreq *insertedWord = NULL;
    d->tree = insertRecursivelyHelper(d->tree, w, &insertedWord);
    return insertedWord;
}

// Adapted from lecture slides on AVL trees.
Link insertRecursivelyHelper(Link currNode, char *w, WFreq **insertedWord) {
    if (currNode == NULL) {
        // Insert the node. 
        Link n = createNewNode(w);
        *insertedWord = &(n->data);
        return n;
    }
    int cmpFactor = strcmp(w, currNode->data.word);

    if (cmpFactor < 0) {
        // To the right.
        currNode->left = insertRecursivelyHelper(currNode->left, w, insertedWord);
    } else if (cmpFactor > 0) {
        // To the left. 
        currNode->right = insertRecursivelyHelper(currNode->right, w, insertedWord);
    } else {
        // Already exists in the tree.
        currNode->data.freq+=1; 
        return currNode;
    }

    // Balance the tree
    int leftHeight = calculateHeight(currNode->left);
    int rightHeight = calculateHeight(currNode->right);
    currNode->height = 1 + max(leftHeight, rightHeight);
    int balance = rightHeight - leftHeight;

    if (balance > 1) {
        if (strcmp(w, currNode->right->data.word) < 0) {
            currNode->right = rotateRight(currNode->right);
        }
        currNode = rotateLeft(currNode);
    } else if (balance < -1) {
        if (strcmp(w, currNode->left->data.word) > 0) {
            currNode->left = rotateLeft(currNode->left);
        }
        currNode = rotateRight(currNode);
    }
    return currNode; 
}

Link rotateRight(Link n) {
    if (n== NULL || n->left == NULL) return n;
    Link newRoot = n->left;
    if (newRoot != NULL) {
        n->left = newRoot->right;
        newRoot->right = n;
        n->height = 1 + max(calculateHeight(n->right), calculateHeight(n->left));
        newRoot->height = 1+max(calculateHeight(newRoot->right), calculateHeight(newRoot->left));
    }
    return newRoot;
}

Link rotateLeft(Link n) {
    if (n== NULL || n->right == NULL) return n;
    Link newRoot = n->right;
    if (newRoot != NULL) {
        n->right = newRoot->left;
        newRoot->left = n;
        n->height = 1 + max(calculateHeight(n->right), calculateHeight(n->left));
        newRoot->height = 1+max(calculateHeight(newRoot->right), calculateHeight(newRoot->left));
    }
    return newRoot;
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array 
int findTopN(Dict d, WFreq *wfs, int n) {
    int startingSize = 0; 
    recursiveHelperTopN(wfs, d->tree, &startingSize, n);
    return startingSize;
}

/* Navigates tree with in-order traversal and calls the insertInOrder function.
Removes the need for strcmp() as the tree is sorted in lexicographic order, so any 
later insertions with the same frequency would be lexicographic as well. */
void recursiveHelperTopN(WFreq * results, Link currNode, int *currSize, int nWordSize) {
    if (currNode == NULL) return;
    recursiveHelperTopN(results, currNode->left, currSize, nWordSize);
    insertInOrder(results, &(currNode->data), currSize, nWordSize);
    recursiveHelperTopN(results, currNode->right, currSize, nWordSize);
}

/*Inserts in descending order. Checks start and end of array first to optimises it.
Also checks whether the size of the array is smaller than the dedicated nWords and 
adds to end instead of just shifting.Otherwise it performs a binary search to find insertion 
point and shifts accordingly.*/
void insertInOrder(WFreq *results, WFreq *nodeToInsert, int *currSize, int nWordSize) {
    if (*currSize == 0) {
        results[0] = *nodeToInsert; 
        *currSize+=1;
    } else if (*currSize < nWordSize) {
        if (nodeToInsert->freq < results[*currSize].freq) {
            results[*currSize] = *nodeToInsert;
            *currSize+=1;
        } else if ((*nodeToInsert).freq > results[0].freq) {
            shiftArrayAndInsert(results, nodeToInsert, 0, *currSize);
            *currSize+=1;
        } else {
            int loc = binarySearch(results, nodeToInsert, 0, *currSize - 1);
            shiftArrayAndInsert(results, nodeToInsert, loc, *currSize);
            *currSize+=1;
        }
    } else {
        // Must insert in order since we did in-order traversal. 
        int loc = binarySearch(results, nodeToInsert, 0, nWordSize-1);
        shiftArrayAndInsert(results, nodeToInsert, loc, nWordSize-1);
    }
}

// Shift array to right, and insert at that point.
void shiftArrayAndInsert(WFreq * results, WFreq *nodeToInsert, int loc, int size) {
    for (int i = size; i > loc; i--) {
        results[i] = results[i-1];
    }
    results[loc] = *nodeToInsert;
}

// Binary search is conducted for descending order array.
int binarySearch(WFreq *results, WFreq *nodeToInsert, int low, int high) {
    if (high <= low) {
        return (nodeToInsert->freq > results[low].freq) ? low : low+1;
    }
    int mid = (low + high)/2;
    if (nodeToInsert->freq <= results[mid].freq) {
        return binarySearch(results, nodeToInsert, mid + 1, high);
    }
    return binarySearch(results, nodeToInsert, low, mid -1);
}