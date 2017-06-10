/*This program will calculate how many water bottles can be filled using the water from your shower*/

#include <stdio.h>
#include <cs50.h>

int main(void)

{
    printf ("What is your name?\n");    //asks user for their name and starts a new line.
    string n = get_string();    //declares string variable "n" and prompts user for input.
    printf ("Hello %s! How many minutes do you shower for?\n", n);  //%s is used as a placeholder the variable you want to display. "s" stands for string, you will use "d" for an integer.
    int m = get_int();  //prompts user for integer input.
    printf ("Shower Duration: %d minutes\n", m);    //displays the entered shower duration from the user using %d.
    printf ("You used the equivalent of %d bottles of water for your shower.\n", (192*m)/16);   //%d is used as a placeholder for the answer to the equation.
}
