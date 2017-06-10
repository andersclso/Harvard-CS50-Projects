/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // print error message if user does not specify # of random numbers wanted or # of random numbers wanted and a specific # to act as 'seed'
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // converts the string which represents "# of random numbers wanted" to an int using atoi function
    int n = atoi(argv[1]);

    // if input is three strings, that means user has chosen a 'seed' number for the drand48
    // else if the user does not provide a seed number for drand48, then just use current time as the seed
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // prints out the specified amount of random numbers
    //multiplies by the limit so the biggest number possible would be 1 * LIMIT
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
