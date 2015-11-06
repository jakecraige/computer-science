/****************************************************************************
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// How many hash values our hash function generates
// # of letters in alphabet
#define HASH_MULTIPLIER 20
#define MAX_HASH_VALUES ('z' - 'a') * HASH_MULTIPLIER

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Returns the index of a word in the hash table
 */
int hashWord(const char* word);

typedef struct node
{
  char* value;
  struct node* next;
} node;

#endif // DICTIONARY_H
