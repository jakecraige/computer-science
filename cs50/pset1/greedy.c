/**
 * greedy.c
 *
 * Jake Craige
 * james.craige+c@gmail.com
 *
 * Output how many coins it would take to give change
 * for a specific number.
 */

#include <stdio.h>
#include <math.h>
#include <cs50.h>

int GetUserInput(void);

int main(void)
{
    printf("O hai! ");
    int cents = GetUserInput();

    int quarters = cents / 25;
    cents -= quarters * 25;
    int dimes = cents / 10;
    cents -= dimes * 10;
    int nickels = cents / 5;
    cents -= nickels * 5;
    int pennies = cents;

    printf("%i\n", quarters + dimes + nickels + pennies);
}

int GetUserInput(void)
{
    printf("How much change is owed?\n");
    float f = GetFloat();
    while (f <= 0 || f > 23)
    {
        printf("Retry: \n");
        f = GetFloat();
    }
    return round(f * 100.0);
}
