#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])    //activates command-line arguments
{
    if (argc != 3)      //ensures proper usage
    {
        fprintf (stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    
    char *infile = argv[1];     //easier to remember in and out file names
    char *outfile = argv[2];
    
    FILE *inptr = fopen (infile, "r");       //opens the infile for reading
    if (inptr == NULL)
    {
        fprintf (stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    FILE *outptr = fopen (outfile, "w");     //opens the outfile for writing
    if (outptr == NULL)
    {
        fprintf (stderr, "Could not open %s.\n", outfile);
        return 3;
    }
    
    BITMAPFILEHEADER bf;        //reads infile's BITMAPFILEHEADER
    fread (&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    BITMAPINFOHEADER bi;        //reads infile's BITMAPINFOHEADER
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||     //ensures infile is a 24-bit uncompressed BMP 4.0
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose (outptr);
        fclose (inptr);
        fprintf (stderr, "Unsupported file format.\n");
        return 4;
    }
    
    fwrite (&bf, sizeof(BITMAPFILEHEADER), 1, outptr);      //writes outfiles's BITMAPFILEHEADER
    fwrite (&bi, sizeof(BITMAPINFOHEADER), 1, outptr);      //writes outfiles's BITMAPINFOHEADER
    
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;      //determines if scanlines needs padding
    
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)        //interates over scanlines
    {
        for (int j = 0; j < bi.biWidth; j++)        //iterates over the pixels of each scanline
        {
            RGBTRIPLE triple;       //naming struct "RGBTRIPLE" as triple
            
    //pointer to struct containing data, size of element to read, # of elements to read, FILE * to read from
            fread (&triple, sizeof(RGBTRIPLE), 1, inptr);       //read RGBTRIPLE of infile
            
            if (triple.rgbtRed == 0xff && triple.rgbtBlue == 0xff && triple.rgbtGreen == 0xff)
            {
                triple.rgbtBlue = 0xff;
                triple.rgbtGreen = 0x00;
                triple.rgbtRed = 0xff;
            }
            
            fwrite (&triple, sizeof(RGBTRIPLE), 1, outptr);     //writes RGBTRIPLE to outfile
        }
        
    //file * to seek over, # of bytes to move cursor, from what position
        fseek (inptr, padding, SEEK_CUR);       //this skips over padding
  
        for (int k = 0; k < padding; k++)       //writes padding to outfile
        {
            //char to write, FILE * to write to
            fputc(0x00, outptr);
        }
    }
    
    fclose (inptr);     //close infile
    fclose (outptr);    //close outfile
    
    return 0;
}