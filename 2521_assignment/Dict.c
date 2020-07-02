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
// Defining Custom Helper Functions.
Link createNewNode(char *w);
Link insertRecursivelyHelper(Link currNode, char *w, WFreq **insertedWord); 
void recursiveHelperTopN(Link currNode, WFreq * results, int *size, int max);
void insertInOrder(WFreq * results, WFreq *nodeToInsert, int *size, int nWordSize);
int calculateHeight(Link currNode);
int max(int a, int b);
// Defining balancer helper functions makes code cleaner and easier to read.
Link balancer_RightRight(Link currNode);
Link balancer_RightLeft(Link currNode);
Link balancer_LeftRight(Link currNode);
Link balancer_RightLeft(Link currNode);
int test(Link node);
int lol = 0;

int height(Link n);
// create new empty Dictionary
Dict newDict()
{
   Dict newTree = malloc(sizeof(struct _DictRep));
   // Check whether memory was allocated or not. 
   if (newTree == NULL) {
      fprintf(stderr, "Couldn't allocate memory to create new tree");
      exit(EXIT_FAILURE);
   }
   newTree->tree = NULL;
   return newTree;
}

int calculateHeight(Link currNode) {
   if (currNode == NULL) {
      return -1;
   } else {
      return currNode->height;
   }
}

int max(int a, int b) {
   return a > b ? a : b;
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
   newNode->data.word = strdup(w); // copy the string over.
   newNode->data.freq = 1;
   return newNode;
}
Link rotateLeft(Link n) {
    // TODO: Add your code here and change
    //       the return statement if needed
    if (n== NULL || n->right == NULL) return n;
    Link newRoot = n->right;
    if (newRoot != NULL) {
        n->right = newRoot->left;
        newRoot->left = n;
        if (n->right != NULL || n->left != NULL) {
            n->height = 1 + max(height(n->right), height(n->left));
        } else {
            n->height = 0;
        }
        newRoot->height = 1+max(height(newRoot->right), height(newRoot->left));
    }
    return newRoot;
}
int height(Link n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}
Link rotateRight(Link n) {
    // TODO: Add your code here and change
    //       the return statement if needed
    // Added null case.
    if (n== NULL || n->left == NULL) return n;
    Link newRoot = n->left;
    if (newRoot != NULL) {
        n->left = newRoot->right;
        newRoot->right = n;
        if (n->right != NULL || n->left != NULL) {
            n->height = 1 + max(height(n->right), height(n->left));
        } else {
            n->height = 0;
        }
        newRoot->height = 1+max(height(newRoot->right), height(newRoot->left));
    }
    return newRoot;
}

Link insertRecursivelyHelper(Link currNode, char *w, WFreq **insertedWord) {
   if (currNode == NULL) {
      // Insert the node. 
      lol+=1;
      Link n = createNewNode(w);
      *insertedWord = &(n->data);
      return n;
   } 
   int cmpFactor = strcmp(currNode->data.word, w);

   if (cmpFactor > 0) {
      // To the right.
      currNode->left = insertRecursivelyHelper(currNode->left, w, insertedWord);
   } else if (cmpFactor < 0) {
      // To the left. 
      currNode->right = insertRecursivelyHelper(currNode->right, w, insertedWord);
   } else {
      // Already exists in the tree.
      currNode->data.freq+=1; 
      return currNode;
   }

   // Balance the tree
   currNode->height = 1 + max(calculateHeight(currNode->left), calculateHeight(currNode->right));

   int leftHeight = calculateHeight(currNode->left);
   int rightHeight = calculateHeight(currNode->right);
   if (rightHeight - leftHeight > 1) {
        if (currNode->right != NULL) {
            currNode->right = rotateRight(currNode->right);
        }
        currNode = rotateLeft(currNode);
    }else if (leftHeight - rightHeight > 1) {
        if (currNode->left != NULL) {
            currNode->left = rotateLeft(currNode->left);
        }
        currNode = rotateRight(currNode);
    }
   return currNode; 
}
// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
   // TODO
   assert(d != NULL); // Can't be inserting into a non-existent tree. 
   WFreq *insertedWord = NULL;
   d->tree = insertRecursivelyHelper(d->tree, w, &insertedWord);
   // Found a place to insert.
   return insertedWord;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   // TODO
   Link curr = d->tree; 
   while(curr != NULL) {
      int cmpFactor = strcmp(curr->data.word, w);
      if (cmpFactor > 0) {
         // Left
         curr=curr->left;
      } else if (cmpFactor < 0) {
         // right
         curr = curr->right;
      } else {
         return &curr->data;

      }
   }
   return NULL;
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array

// Descending order
int binarySearch(WFreq *results, WFreq *nodeToInsert, int low, int high) {
   if (high <= low) {
      return (nodeToInsert->freq > results[low].freq) ? low : low+1;
   }
   int mid = (low + high)/2;
   //if (nodeToInsert->freq == results[mid].freq) return mid+1;
   if (nodeToInsert->freq <= results[mid].freq) {
      return binarySearch(results, nodeToInsert, mid + 1, high);
   }
   return binarySearch(results, nodeToInsert, low, mid -1);
}

void printer(WFreq * results, int k) {
   for (int i = 0; i < k; i++) {
      printf("%d %s\n", results[i].freq, results[i].word);
   }
}

void insertInOrder(WFreq *nodeToInsert, WFreq *results, int *size, int nWordSize) {
   if (*size == 0) {
      results[0] = *nodeToInsert; 
      *size+=1;
      return;
   }
   if (*size < nWordSize) {
      // Insert in order, can extend arary to 0..(*size -1)
      // If smallest than the end, and not max size then insrt at end.
      if (nodeToInsert->freq < results[*size].freq) {
         results[*size] = *nodeToInsert;
      } else if ((*nodeToInsert).freq > results[0].freq) {
         for (int i = *size; i >0; i--) {
            results[i] = results[i-1];
         }
         results[0] = *nodeToInsert;
         *size+=1;
      } else {
         int loc = binarySearch(results, nodeToInsert, 0, *size - 1);
         for (int i = *size; i > loc; i--) {
            results[i] = results[i-1];
         }
         results[loc] = *nodeToInsert;
         *size+=1;
      }
   } else {
      // Must insert in order
      int loc = binarySearch(results, nodeToInsert, 0, nWordSize-1);
      for (int i = nWordSize - 1; i > loc; i--) {
         results[i] = results[i-1];
      }
      results[loc] = *nodeToInsert;
   }
}
// In-order traversal. Insertion will always be lexiographic order. No need for strcmp()
void recursiveHelperTopN(Link currNode, WFreq * results, int *size, int max) {
   if (currNode == NULL) return;
   recursiveHelperTopN(currNode->left, results, size, max);
   insertInOrder(&(currNode->data), results, size, max);
   recursiveHelperTopN(currNode->right, results, size, max);
}

int test(Link node) {
   if (node == NULL) return 0; 
   int kek = max(test(node->right), test(node->left));
   return max(kek, node->data.freq);
}

int findTopN(Dict d, WFreq *wfs, int n)
{
   // TODO
   int startingSize = 0; 
   recursiveHelperTopN(d->tree, wfs, &startingSize, n);
   //printf("Largest is %d", test(d->tree));
   //printf("Top one is %s and %d \n", d->tree->data.word, d->tree->data.freq);
   //printf("Num unique words: %d\n", lol); 
   return startingSize;
}

void testFunc(Dict d, char * word) {
   WFreq * t = DictFind(d, word);
   if (t != NULL) {
      printf("Word: %s, freq: %d \n", t->word, t->freq);
   } else {
      printf("Not found!");
   }
}

// print a dictionary
void showDict(Dict d)
{
   // TODO if you need to display Dict's for debugging
   return;
}

void testDictImplementation(Dict d) {
   printf("TESTING ==== \n");
   testFunc(d, "time");
   printf("PASSED ALL TESTS!!\n");
}
