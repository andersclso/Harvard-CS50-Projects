/*This program will prompt user for their full name and then proceed to
display only the initials of their full name in capital letters regardless
of whether or not they entered their full name with capitalized letters or
all lowercase letters.*/

#include <stdio.h>
#include <cs50.h>
#include <string.h> //this header file allows you to use 'strlen'
#include <ctype.h> //this header file allows you to use function 'toupper'

int main(void)

{
    printf ("Hello! What is your full name?: ");
    string name = get_string(); //prompts user for their full name.
    
    printf ("%c", toupper(name[0]));    //prints and capitalizes first initial.
    
    for (int i = 0, j = strlen(name); i < j; i++)   //loop will run 'string length minus one' times.
    {
        if (name[i] == ' ')     //if a space is detected...
        {
            printf ("%c", toupper (name[i+1]));     //then it will capitalize the following character.
        }
    }
    
    printf ("\n");      //prints a new line.
}