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
Link insertRecursivelyHelper(Link currNode, char *w); 
int calculateHeight(Link currNode);
int max(int a, int b);
// Defining balancer helper functions makes code cleaner and easier to read.
Link balancer_RightRight(Link currNode);
Link balancer_RightLeft(Link currNode);
Link balancer_LeftRight(Link currNode);
Link balancer_RightLeft(Link currNode);

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

int max(a, b) {
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

Link insertRecursivelyHelper(Link currNode, char *w) {
   if (currNode == NULL) {
      // Insert the node. 
      return createNewNode(w);
   } 
   int cmpFactor = strcmp(currNode->data.word, w);

   if (cmpFactor > 0) {
      // To the right.
      currNode->right = insertRecursivelyHelper(currNode->right, w);
   } else if (cmpFactor < 0) {
      // To the left. 
      currNode->left = insertRecursivelyHelper(currNode->left, w);
   } else {
      // Already exists in the tree.
      return currNode;
   }

   // Balance the tree
   currNode->height = 1 + max(calculateHeight(currNode->left), calculateHeight(currNode->right));
   return currNode; 
}
// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
   // TODO
   assert(d != NULL); // Can't be inserting into a non-existent tree. 
   Link ret = insertRecursivelyHelper(d->tree, w);
   // Found a place to insert. 
   return &ret->data;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   // TODO
   Link curr = d->tree; 
   while(curr != NULL) {
      if (strcmp(curr->data.word, w) == 0) {
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
