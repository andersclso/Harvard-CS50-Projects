/*This program will encypt a message for you using Vigenere's cipher*/

#include <stdio.h>
#include <cs50.h>
#include <string.h> //header file for strlen function
#include <ctype.h>  //header file for isalpha, isupper, islower function

int main(int argc, string argv[])

{
    if (argc != 2)      //print error and exit if command-line does not contain exactly two strings
    {
        printf ("error\n");
        return 1;
    }
    
int u, l, keyl = strlen(argv[1]);       //variable declarations
string word, key = argv[1];

    for (int a = 0, j = 0; a < keyl; a++)       //for-loop is used to go through every letter of the key
    {
        if (!isalpha(argv[1][a]))       //checks if there are non-alphabetic chars, if so, error and exit
        {
            printf ("error\n");
            return 1;
        }
        else if (isupper(argv[1][a]))       //converts any uppercase letters to its respective alpha-index value (0-25)
        {
            key[j] = (argv[1][a]) - 'A';    //'A' = 97 in ascii index
            j++;                            //stores value in 'j' and proceeds to next char
        }
        else if (islower(argv[1][a]))       //converts any lowercase letters to its respective alpha-index value (0-25)
        {
            key[j] = (argv[1][a]) - 'a';    //'a' = 65 in ascii index
            j++;                            //stores value in 'j' and proceeds to next char
        }
    }
    
    printf ("plaintext: ");     //prompts user for message
    word = get_string();
    
    printf ("ciphertext: ");
    for (int i = 0, j = 0, length = strlen(word); i < length; i++)      //for-loop is used to go through every letter of the message
    {
        if (isalpha(word[i]) && isupper(word[i]))
        {
            u = (((word[i]-65)+key[j%keyl])%26);        //j%keyl: 'j' is the stored value calculated earlier, %keyl will Mod the length of the key and this will allow
            printf ("%c", u+65);                        //the program to keep looping over the key's values to cipher the message
            j++;
        }
        else if (isalpha(word[i]) && islower(word[i]))
        {
            l = (((word[i]-97)+key[j%keyl])%26);
            printf ("%c", l+97);
            j++;
        }
        else
        {
            printf ("%c", word[i]);     //if char is not a alphabetical letter, just print it
        }
    }
    printf ("\n");      //new line
    return 0;
}