/*This program will encypt a message for you using Caesar's cipher*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>     //header file to use strlen function
#include <ctype.h>      //header file to use isalpha function
#include <stdlib.h>     //header file to use atoi function

int main(int argc, string argv[])   /*activates command-line arguments.
                                      'argv' = the array of strings 
                                      'argc' = total # of strings in argv */
{
int key, u, l;               //declaring variables for ascii to alpha index conversion formula
string word;            //word = message to be encrypt

    if (argc == 2)      //to ensure user only inputs a single numeric value
    {
        key = atoi(argv[1]);   //using atoi function to convert string to int
        printf ("plaintext: "); 
        word = get_string();
    }
    else              //if user enters nothing or enters more than two strings, will show error msg
    {
        printf ("Error\n");
        return 1;
    }
    
    printf ("ciphertext: ");
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        if (isalpha(word[i]) && isupper(word[i]))   //proceeds only if char is a letter in the alphabet and is uppercased
        {
            u = (((word[i]-65)+key)%26);   //converts uppercase ascii index (65-90) to alpha index (0-25) and ciphers char by 'k'
            printf ("%c", u+65);         //converts ciphered alpha index char back to its ascii equivalent
        }
        else if (isalpha(word[i]) && islower(word[i]))  ////proceeds only if char is a letter in the alphabet and is lowercased
        {
            l = (((word[i]-97)+key)%26);   //converts uppercase ascii index (97-122) to alpha index (0-25) and ciphers char by 'k'
            printf ("%c", l+97);         //converts ciphered alpha index char back to its ascii equivalent
        }
        else                        //if char is not a letter in the alphabet, then just print the character in the string
        {
            printf ("%c", word[i]);
        }
    }
    
    printf ("\n");      //new line
    return 0;           //return 0 if program runs perfectly
}
