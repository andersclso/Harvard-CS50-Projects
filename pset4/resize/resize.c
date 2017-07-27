/**
 * Copies a BMP piece by piece, just because.
 **/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember resize multiple & filenames
    int multiple = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // ensures multiple is within range
    if (multiple < 1 || multiple > 100)
    {
        fprintf(stderr, "Must enter a positive multiple between 1-100");
    }
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // remember Infile's dimensions and padding
    int InfilebiWidth = bi.biWidth;
    int InfilebiHeight = bi.biHeight;
    int InfilePadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // resize dimensions and padding for Outfile image
    bi.biWidth *= multiple;
    bi.biHeight *= multiple;
    int OutfilePadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // update Outfile image size and total file size
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + OutfilePadding) * abs(bi.biHeight); 
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // need to create a long variable to hold negative value so fseek can move cursor backwards
    long offset = (InfilebiWidth * sizeof(RGBTRIPLE));

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    for (int row = 0; row < abs(InfilebiHeight); row++)
    {   
        // copies row "multiple" amount of times
        for (int k = 0; k < multiple; k++)
        {
            for (int column = 0; column < InfilebiWidth; column++)
            {
                // creates temp storage for RGBTRIPLE
                RGBTRIPLE triple;
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // writes each pixel "multiple" times into Outfile
                for (int i = 0; i < multiple; i++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // write padding to Outfile if any is required
            for (int i = 0; i < OutfilePadding; i++)
            {
                fputc(0x00, outptr);
            }
            
            if (k < multiple-1)
            {
                // moves cursor back to beginning of current row
                fseek(inptr, -offset, SEEK_CUR);
            }
        }
        // skips over Infile's padding to move onto it's next row
        fseek(inptr, InfilePadding, SEEK_CUR);
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}