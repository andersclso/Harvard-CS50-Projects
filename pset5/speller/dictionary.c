/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH+1];
    struct node *next;
}node;

// this hash function is used to calculate a word's hash value, which is used to associate it with a hashtable bucket
int hash_it(char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(needs_hashing); i < n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHSIZE;
}
// ***credit to reddit user Delipity who posted this @ https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/


bool loaded = false; //preset loaded to false so we know to return size once every word is loaded from dictionary
int wordcount; //variable to keep track of how many words in dictionary
char word[LENGTH]; //char array with 45 elements (max word length)
node *Hashtable[HASHSIZE]; //creates a hashtable with x buckets to store nodes

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{   
    int wordsize = strlen(word); //+1 more element b/c we need space to add NULL terminator at end of each word
    char copyword[wordsize+1]; //make a copy of word b/c you cannot make changes to a 'const' char variable
    
    for (int i = 0; i < wordsize; i++) //copies each word in text file letter by letter
    {
        if (isalpha(word[i]) && isupper(word[i])) //changes any uppercase letters to lowercase
        {
            copyword[i] = tolower(word[i]);
        }
        else //if not uppercase then just make a copy of the letter
        {
            copyword[i] = word[i];
        }
    }
    
    copyword[wordsize] = '\0'; //make sure to use '' instead of "" for single characters
        
    int hashed = hash_it(copyword); //calls hash function to hash the current word in the text file
    
    node* cursor = Hashtable[hashed]; //sets the cursor to point to the correct hashtable node
    
    while(cursor != NULL) //loops as long as cursor is not pointed to the last word of the linked-list
    {
        if (strcmp (cursor -> word, copyword) == 0) //if there are no differences between the two words, it means it's a word in the dictionary
        {
            return true;
        }
        else
        {
            cursor = cursor -> next; //if there are differences between the words, go to the next word in the linked-list
        }
    }
    
    return false; //if the word cannot be found in the linked-list, return false
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); //open dictionary file for reading
    if (file == NULL)
    {
        printf ("cannot open file/invalid file\n");
        return false;
    }
    
    for (int i = 0; i < HASHSIZE; i++) //sets all Hashtable nodes to NULL
    {
        Hashtable[i] = NULL;
    }
    
    while(fscanf(file, "%s", word) != EOF) //scans the dictionary file for the current word, stores it in the char array called word until it reachs EOF
    {
        node *newd = malloc(sizeof(node)); //for each word, allocate enough memory for a new node called "newd"
        if (newd == NULL) //unless there is no more dynamic memory
        {
            unload();
            return false;
        }
        
        strcpy(newd -> word, word); //store the current scanned word into "newd"
        
        int hashed = hash_it(newd -> word); //hash the current word
        
        if (Hashtable[hashed] == NULL) //if bucket is NULL, fill it with the new node and assign it's pointer to NULL to represent end of the linked-list
        {
            Hashtable[hashed] = newd; //why Hashtable[hashed] -> next = newd; give me a seg fault? Allocating 8 Bytes that I do not have?
            newd -> next = NULL;
        }
        else //if there is already a node in the bucket, we add new node to the beginning of the linked-list and set it as the head of the bucket.
        {   
            newd -> next = Hashtable[hashed];
            Hashtable[hashed] = newd;
        }
        
        wordcount++; //add one to wordcount after each iteration of the loop
    }
    
    fclose (file); //close dictionary file when finished
    loaded = true; //change loaded to true
    return true; //success
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loaded)
    {
        return wordcount; //return the word count if all loads well
    }
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i <= HASHSIZE; i++) //go through each bucket
    {
        node *cursor = Hashtable[i]; //starting from bucket[0]
        if (Hashtable[i] != NULL) //if there is a node in the bucket...
        {
            while (cursor != NULL) //and cursor is not pointing to the end of the last node...
            {
                node *temp = cursor; //set temp pointer to point towards the node the cursor is pointing to...
                cursor = cursor -> next; //set cursor to point to the next node first...
                free(temp); //then free up the previous bucket
            }
            Hashtable[i] = NULL; //set current bucket to null so we can exit out of the while loop and move onto the next bucket
        }
    }
    
    return true; //success
}
