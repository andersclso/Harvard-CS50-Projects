/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)     //function declaration: type-name-variables
{
    int start = 0, end = (n-1);     //declaring variables: start reps left end of array, end reps right end of array
    
    while (start <= end)    //(n>0) is not right b/c loop will run forever as n will always be greater than 0, even if all values has been checked.
    {
        if (values[(start+end)/2] == value)     //searches the value located in the middle of the array
        {
            return true;
        }
        else if (values[(start+end)/2] > value)     //if value in middle of array is greater than target...
        {
            end = ((start+end)/2)-1;    //since we know the target is on the left side of the array, we can make the end point one element less than the middle.
        }
        else if (values[(start+end)/2] < value)     //if value in the mdidle of array is less than the target...
        {
            start = ((start+end)/2)+1;      //since we know the target is on the right side of the array, we can make the start point one element more than the middle.
        }
    }
    return false;       //if value is not found in the array after start > end, we know that it is not one of the numbers in the list and we can return false.
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)      //function declaration: type-name-variables used
{
int swaps = -1, temp;       //variable declaration; swaps is set to -1 or else do-while loop will not execute

    do      //do-while loop is used b/c we need a loop where it checks condition AFTER all numbers have been checked
    {
        swaps = 0;      //sets swap-counter to 0 after each cycle
        for (int i = 0; i < (n-1); i++)     //n-1 to avoid segmentation fault due to 'i+1' down below
        {
            if (values[i] > values[i+1])    //if left value is greater than right value...
            {
                temp = values[i];   //store left value in temp
                values[i] = values[i+1];    //left value now equals right value
                values[i+1] = temp;     //right value now equals to the value stored in temp
                swaps++;    //swap is made, so add 1 to the counter... repeat loop until swap = 0
            }
        }
    }
    while (swaps != 0);
    return;     //terminate function and return control to the calling function
}