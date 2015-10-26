/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n <= 0)
    {
        return false;
    }

    for (int i = 0; i < n; i++) {
        if (values[i] == value)
        {
            return true;
        }
    }

    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int smallestIdx;
    for (int i = 0; i < n; i++)
    {
        smallestIdx = i;
        for (int j = i; j < n; j++)
        {
            if (values[j] < values[smallestIdx])
            {
                smallestIdx = j;
            }
        }
        int temp = values[smallestIdx];
        values[smallestIdx] = values[i];
        values[i] = temp;
    }
}
