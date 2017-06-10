/*This program will tell you how many quarters, dimes, nickles, 
and pennies are needed to give you your change back using
the least amount of coins possible*/

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)

{
float dollar;                   //created a 'float' variable due to input being in decimals.
int q, d, n, p, change;         //declared variables for quarters, dimes, nickles, pennies.

    do
    {
        printf ("Hello! How much change do you need back?: ");
        dollar = get_float();
    }
    while (dollar < 0);         //prompt user for input again if a negative value is entered.
    
    change = (int)round(dollar*100);      //this is to convert user input (dollars) to cents (multiply by 100 cents).
                                          //due to imprecision, we need to round 
                                          
    q = (change/25);            //in order to figure out least amount of coins used, we start have to start with the quarter. This line is used to calculate and store # of quarters used.
    change %= 25;               //used Modulus operator (gives the remainder value in division calculation) to update the amount left over after quarters have been deducted from original value.

    d = (change/10);
    change %= 10;               //change gets smaller and smaller with each step using the Modulo.

    n = (change/5);
    change %= 5;

    p = (change/1);
    change %= 1;                //this will ultimately reduce the change variable to 0.
                
                        printf ("Quarters Used: %d\n", q);
                        printf ("Dimes Used: %d\n", d);
                        printf ("Nickles Used: %d\n", n);
                        printf ("Pennies Used: %d\n", p);
                        printf ("TOTAL COINS USED: %d\n", q+d+n+p);
}

//LESSONS LEARNED: My first attempt at this problem was to use multiple while-loops but the program was not behaving as intended. 
//For example, if I enter 0.55 as the change I needed back, the program will display that I need to grab 2 Quarters and 1 nickle as
//it should, but for Dimes and Pennies, which is suppose to give me a "0" back, it is giving me 1494214400 for Dimes and 4283568 for Pennies.
//The reason behind this is due to uninitialized variables since the body of the dimes and pennies did not run, 'd' and 'p' did not get
//assigned a value, causing them to keep their original random values. To solve this problem, I needed to assign each variable as '0' in the
//beginning. However, I did not need to use so many while-loops, even none at all was fine.
//I learned to think step-by-step like a computer does and not to over-engineer a problem.