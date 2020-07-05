// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File
// by Anish Ghai (z5218632)
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

#define MIN_NWORDS 10
#define MIN_WORD_LENGTH 1

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')
void customTokeniser(Dict stopwords, Dict wfreqs, char line[], char word_buffer[]);

int main( int argc, char *argv[]) {
    FILE  *in;          // currently open file
    Dict   stopwords;   // dictionary of stopwords
    Dict   wfreqs;      // dictionary of words from book
    WFreq *results;     // array of top N (word,freq) pairs
                        // (dynamically allocated)

    char *fileName;    // name of file containing book text
    int   nWords;      // number of top frequency words to show

    char   line[MAXLINE];  // current input line
    char   word[MAXWORD];  // current word
    
    // process command-line args
    switch (argc) {
    case 2:
        nWords = MIN_NWORDS;
        fileName = argv[1];
        break;
    case 3:
        nWords = atoi(argv[1]);
        if (nWords < MIN_NWORDS) nWords = MIN_NWORDS;
        fileName = argv[2];
        break;
    default:
        fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // build stopword dictionary
    in = fopen(STOPWORDS, "r");
    if (in == NULL) {
        fprintf(stderr, "Can't open stopwords\n");
        exit(EXIT_FAILURE);
    }
    stopwords = newDict();
    while(fgets(word, MAXWORD, in) != NULL) {
        word[strlen(word)-1] = '\0'; // Remove newline.
        DictInsert(stopwords, word);
    }
    fclose(in);

    //Read, tokenise and stem words. 
    in = fopen(fileName, "r");
    if (in == NULL) {
        fprintf(stderr, "Can't open %s\n",fileName);
        exit(EXIT_FAILURE);
    }
    // scan File, up to start of text
    fgets(line,MAXLINE,in);
    while(strstr(line, STARTING) == NULL) {
        if (feof(in)) {
            fprintf(stderr, "Not a Project Gutenberg book\n");
            exit(EXIT_FAILURE);
        }
        fgets(line,MAXLINE,in);
    }
    // scan File reading words and accumualting counts
    wfreqs = newDict();
    while(fgets(line,MAXLINE,in) != NULL && strncmp(line, ENDING, 3)) {
        customTokeniser(stopwords, wfreqs, line, word);
    }

    results = malloc((nWords+1) * sizeof(struct _WFreq));
    if (results == NULL) {
        fprintf(stderr, "Couldn't allocate memory to results table");
        exit(EXIT_FAILURE);
    }
    int numFreqs = findTopN(wfreqs, results, nWords);
    for (int i = 0; i < numFreqs; i++) {
        printf("%7d %s\n", results[i].freq, results[i].word);
    }

    // Free memory.
    fclose(in);
    free(wfreqs);
    free(stopwords);
    free(results);
    return EXIT_SUCCESS;
}


// Tokenises and stems in O(n) in one iteration which optimises and speeds up program.
void customTokeniser(Dict stopwords, Dict wfreqs, char * line, char word_buffer[]) {
    int length = 0;
    for (int i = 0; line[i] !='\0'; i++) {
        if (isWordChar(line[i])) {
            word_buffer[length] = tolower(line[i]);
            length++;
        } else {
            word_buffer[length] = '\0';       
            if (length > MIN_WORD_LENGTH && DictFind(stopwords, word_buffer) == NULL) {
                int end = stem(word_buffer , 0, length - 1);
                word_buffer[end+1] = '\0';
                DictInsert(wfreqs,word_buffer);
            }
            length=0;
        }
    }
}