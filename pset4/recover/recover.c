/**
 * This program recovers deleted JPEG files from a memory card
 * */
 
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) //to accept command-line arguments
{
    if (argc != 2) //ensure proper usage
    {
        fprintf (stderr, "error: check correct usage");
        return 1;
    }
    
    FILE *infile = fopen(argv[1], "r"); //'infile' variable will open memory card file for reading
    if (argv[1] == NULL)
    {
        fprintf (stderr, "error: cannot open file");
        return 2;
    }
    
    unsigned char buffer[512]; //creating an array of 512 "unsigned" chars
    char filename[8]; //creating a char array of 8 bytes to hold filename
    int filecounter = 0; //to keep track of # of jpegs
    FILE *outfile = NULL; //create a address labeled 'outfile'
    
    while (fread(buffer, 512, 1, infile) == 1) //checks if all 512 bytes are read to ensure we are still reading a jpeg
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) //detects beginning of a new jpeg file
        {
            if (outfile != NULL) //if there is currently a jpeg file opened, close it
            {
                fclose (outfile);
            }
            
            sprintf (filename, "%03i.jpg", filecounter); //create new jpeg file
            outfile = fopen(filename, "a"); //open the file for appending
            filecounter++; //add one to # of jpegs currently recovered
        }
        
        if (outfile != NULL) //if current block is from a jpeg and it is not the beginning of a new jpeg and there is currently a jpeg file opened, continue writing in it
        {
            fwrite(buffer, 512, 1, outfile);
        }
    }
    
    fclose(infile); //close infile
    fclose(outfile); //close outfile
    return 0; //success!
}