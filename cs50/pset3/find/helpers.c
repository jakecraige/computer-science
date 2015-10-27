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
 * Returns true if value is in array of between startIdx and endIdx, else false.
 */
bool binarysearch(int value, int values[], int startIdx, int endIdx)
{
    // We've looped back around and thus there's no match.
    if (startIdx > endIdx) {
        return false;
    }

    int middle = (startIdx + endIdx) / 2;
    if (values[middle] == value)
    {
        return true;
    }
    else if (value < values[middle])
    {
        return binarysearch(value, values, 0, middle - 1);
    }
    else if (value > values[middle])
    {
        return binarysearch(value, values, middle + 1, endIdx);
    }
    else {
        return false;
    }
}

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n <= 0)
    {
        return false;
    }

    return binarysearch(value, values, 0, n);
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
