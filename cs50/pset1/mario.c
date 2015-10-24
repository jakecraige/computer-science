/**
 * mario.c
 *
 * Jake Craige
 * james.craige+c@gmail.com
 *
 * Output a mario like pyramid in the height of the
 * input you give.
 */

#include <stdio.h>
#include <cs50.h>

int GetUserInput(void);
void PrintLine(int spaces, int markers);

int main(void)
{
    int n = GetUserInput();
    for (int i = 1; i <= n; i++) 
    {
        PrintLine(n - i, i);
    }
}

void PrintLine(int spaces, int markers)
{
    int i;
    char c;
    for (i = 0; i < spaces + markers; i++)
    {
        if (i < spaces)
        {
            c = ' ';
        }
        else
        {
            c = '#';
        }
        printf("%c", c);
    }
    printf("#\n");
}

int GetUserInput(void)
{
    printf("Height: ");
    int n = GetInt();
    while (n < 0 || n > 23)
    {
        printf("Retry: ");
        n = GetInt();
    }
    return n;
}
