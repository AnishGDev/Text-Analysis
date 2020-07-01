// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#include <assert.h>

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')
char * tokenise(char word[]);

int main( int argc, char *argv[])
{
   // TODO ... add any variables you need
   testDictImplementation();
   FILE  *in;         // currently open file
   Dict   stopwords;  // dictionary of stopwords
   Dict   wfreqs;     // dictionary of words from book
  // WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)

   char *fileName;    // name of file containing book text
   int   nWords;      // number of top frequency words to show

   char   line[MAXLINE];  // current input line
   char   word[MAXWORD];  // current word

   char *delimiters = " `~!@#$%^&*()_+-={[]}|\\;:\",<>./?\n\r";
   // process command-line args

   switch (argc) {
   case 2:
      nWords = 10;
      fileName = argv[1];
      break;
   case 3:
      nWords = atoi(argv[1]);
      if (nWords < 10) nWords = 10;
      fileName = argv[2];
      break;
   default:
      fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   in = fopen(fileName, "r");
   // Handle file not found/unreadable case. 
   
   if (in == NULL) {
      fprintf(stderr, "Can't open %s\n", fileName);
      exit(EXIT_FAILURE);
   }

   // build stopword dictionary
   stopwords = newDict();
   in = fopen(STOPWORDS, "r");
   // fgets(word, MAXWORD, in) != NULL 
   //printf("Building stopword dictionary....\n");
   while(fscanf(in, "%s", word) != EOF) {
      DictInsert(stopwords, word);
   }
   fclose(in);
   //printf("Stop dictionary has been built!!!!....\n");
   in = fopen(fileName, "r");
   // scan File, up to start of text
   while(fgets(line,MAXLINE,in) != NULL && strncmp(line, "***", 3)) {
      if (!strncmp(line, "***", 3)) break;
   }
   // TODO
   
   // scan File reading words and accumualting counts
   // According to Project Gutenberg, 70 characters per line
   // Token could be NULL. Check that.
   wfreqs = newDict();
   while(fgets(line,MAXLINE,in) != NULL && strncmp(line, "***", 3)) {
      //printf("The string is : %s\n", line);
      char *token = strtok(line, delimiters);
      while(token != NULL) {
         if (DictFind(stopwords, token) == NULL) {
            int end = stem(token , 0, strlen(token) - 1);
            token[end+1] = '\0';
            DictInsert(wfreqs,token);
         }
         // Proceed to next token
         token = strtok(NULL, delimiters);
      }
      //printf("\n");
      //printf("%s",line);
      //char * test = strtok(line, delimiters);
      //printf("%s",test);
   }

   // TODO

   // compute and display the top N words

   // TODO

   // done
   fclose(in);
   return EXIT_SUCCESS;
}
/*
void tokenise(char word[]) {
   int end = stem(word, 0, strlen(end)-1);
   word[end+1] = '\0';
   return word;
}
*/