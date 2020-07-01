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
int calculateHeight(Link currNode);
int max(int a, int b);
// Defining balancer helper functions makes code cleaner and easier to read.
Link balancer_RightRight(Link currNode);
Link balancer_RightLeft(Link currNode);
Link balancer_LeftRight(Link currNode);
Link balancer_RightLeft(Link currNode);


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


   if (height(currNode->right) - height(currNode->left) > 1) {
        if (currNode->right != NULL) {
            currNode->right = rotateRight(currNode->right);
        }
        currNode = rotateLeft(currNode);
    }else if (height(currNode->left) - height(currNode->right) > 1) {
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
int findTopN(Dict d, WFreq *wfs, int n)
{
   // TODO
   return 0;
}

// print a dictionary
void showDict(Dict d)
{
   // TODO if you need to display Dict's for debugging
   return;
}

void testDictImplementation() {
   Dict test = newDict();
   assert(test->tree == NULL);

   printf("PASSED ALL TESTS!!");
}
