/**
 * caesar.c
 *
 * Jake Craige
 * james.craige+c@gmail.com
 *
 * Implementation of caesar encryption algorithm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

string EncryptString(string input, int key);
char EncryptChar(char c, int key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Error: Number argument required.\n");
        return 1;
    }

    int key = atoi(argv[1]);
    string input = GetString();

    string encrypted = EncryptString(input, key);
    printf("%s\n", encrypted);

    return 0;
}

/*
 * Takes a string to encrypt and the key
 * and runs the encryption on it
 */
string EncryptString(string input, int key)
{
    char *output = malloc(strlen(input) + 1);

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        char c = input[i];
        output[i] = isalpha(c) ? EncryptChar(input[i], key) : c;
    }

    return output;
}

/*
 * Encrypts a single char given a key
 */
char EncryptChar(char c, int key)
{
    int startLetter = isupper(c) ? 'A' : 'a';
    int encrypedCharIndex = ((c - startLetter) + key) % 26;

    return startLetter + encrypedCharIndex;
}
