#include <stdio.h>
#include <cs50.h>

int main(void)

{
    printf ("Minutes: "); int m = get_int();    //declares integer variable "m" and prompts user for input
    int b = (192*m)/16;     //declares integer variable "b" as the answer to the equation using variable "m"
    printf ("Bottles: %d\n", b);    //%d is used as a placeholder to display variable "b"
}