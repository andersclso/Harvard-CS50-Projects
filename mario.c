/*This program will create a Mario-style half pyrimid based on how
high you want Mario to jump*/

#include <stdio.h>
#include <cs50.h>

int main(void)

{
int height, rows, spaces, hashes;

    do
    {
        printf ("ITSAMEEE MAAARIO! How high do you wantamee to jump?: ");
        height = get_int();  
    }
    while (height < 0 || height > 23);
                                                                            //do-while loop is used to prompt the user again if the input is not a # between 0-23. "||" means 'or'.
    {
        for (rows = 0; rows < height; rows++)
        {
            for (spaces = 0; spaces < (height - rows - 1); spaces++)
            {                                                               //proper curly-braces placement is very important.
                printf (" ");                                               //needed a lot of help figuring out the equations to place into the for loop.
            }                                                               //took a while to figure out and make clear on how for loops work and how to lay in multiple for loops.
                for (hashes = 0; hashes < (rows + 2); hashes++)
                {
                    printf ("#");
                }
                        printf ("\n");                                      //this proceeds to the next row and then the for loop repeats.
        }
    }
}