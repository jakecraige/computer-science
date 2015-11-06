/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

node* words[MAX_HASH_VALUES];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int key = hashWord(word);
    node* element = words[key];

    while (element != NULL)
    {
        if (strcasecmp(element->value, word) == 0)
        {
            return true;
        }

        element = element->next;
    }

    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    FILE* file = fopen(dictionary, "r");

    if (file == NULL) { return false; }

    // \n and \0
    int extraChars = 2;
    char wordBuffer[LENGTH + extraChars];
    while (fgets(wordBuffer, LENGTH + extraChars, file) != NULL)
    {
        // Set end of string 1 earlier, to get rid of newline fgets includes
        wordBuffer[strlen(wordBuffer) - 1] = '\0';

        char* word = malloc(sizeof(char) * (strlen(wordBuffer) + 1));
        strcpy(word, wordBuffer);

        int key = hashWord(word);
        node* element = words[key];
        node* newElement = malloc(sizeof(node));
        newElement->value = word;
        newElement->next = NULL;

        // Node for this hash exists
        if (element != NULL)
        {
            // Add new element to head of list
            newElement->next = element;
        }
        words[key] = newElement;
    }

    fclose(file);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    int total = 0;

    for (int i = 0; i < MAX_HASH_VALUES; i++)
    {
        node* element = words[i];
        while (element != NULL)
        {
            total++;
            element = element->next;
        }
    }

    return total;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < MAX_HASH_VALUES; i++)
    {
        node* element = words[i];

        while (element != NULL)
        {
            node* next = element->next;
            free(element->value);
            free(element);
            element = next;
        }
    }

    return true;
}

int hashWord(const char* word)
{
    int value = 0;
    for (int i = 0, length = strlen(word); i < HASH_MULTIPLIER && i < length; i++)
    {
        int key = tolower(word[i]) - 'a';
        if (key < 0) { key = 0; }
        value += key;
    }
    return value;
}
