#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

string EncryptString(string input, string key);
char EncryptChar(char c, char key);
bool AllAlpha(string s);

int main(int argc, string argv[])
{
    if (argc != 2 || !AllAlpha(argv[1])) {
        printf("Error: Argument must be all alpha characters.\n");
        return 1;
    }

    string key = argv[1];
    string input = GetString();

    string encrypted = EncryptString(input, key);
    printf("%s\n", encrypted);

    return 0;
}

string EncryptString(string input, string key)
{
    int inputLength = strlen(input);
    char *output = malloc(inputLength + 1);
    int keyLength = strlen(key);

    for (int i = 0, k = 0; i < inputLength; i++)
    {
        char c = input[i];
        if (isalpha(c))
        {
            output[i] = EncryptChar(c, key[k]);
            k++;
            if (k > (keyLength - 1)) {
                k = 0;
            }
        }
        else
        {
            output[i] = c;
        }
    }

    return output;
}

char EncryptChar(char c, char key)
{
    int charStartLetter = isupper(c) ? 'A' : 'a';
    int keyStartLetter = isupper(key) ? 'A' : 'a';
    int encrypedCharIndex = ((c - charStartLetter) + (key - keyStartLetter)) % 26;

    return charStartLetter + encrypedCharIndex;
}

bool AllAlpha(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++) {
        if (!isalpha(s[i])) {
            return false;
        }
    }

    return true;
}
