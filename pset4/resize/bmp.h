/**
 * BMP-related data types based on Microsoft's own.
 */

#include <stdint.h>

/**
 * Common Data Types 
 *
 * The data types in this section are essentially aliases for C/C++ 
 * primitive data types.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx.
 */
typedef struct 
{ 
    WORD bfType; //2 Bytes: Letters 'BM' to ensure it is a BMP file
    DWORD bfSize; //4 Bytes: The size of the BMP file in Bytes
    WORD bfReserved1; //2 Bytes: Reserved
    WORD bfReserved2; //2 Bytes: Reserved
    DWORD bfOffBits; //4 Bytes: The offset (starting address) of the Byte where the BMP image data can be found
} __attribute__((__packed__)) 
BITMAPFILEHEADER; //TOTAL: 14 Bytes

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the 
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx.
 */
typedef struct
{
    DWORD biSize; //4 Bytes: Size of this Header (40 Bytes)
    LONG biWidth; //4 Bytes: BMP Width in Pixels (Signed Int)
    LONG biHeight; //4 Bytes: BMP Height in Pixels (Signed Int)
    WORD biPlanes; //2 Bytes: # of Color planes (must be 1)
    WORD biBitCount; //2 Bytes: # of Bits per Pixel
    DWORD biCompression;
    DWORD biSizeImage; //4 Bytes: The image size
    LONG biXPelsPerMeter; 
    LONG biYPelsPerMeter; 
    DWORD biClrUsed; 
    DWORD biClrImportant; 
} __attribute__((__packed__))
BITMAPINFOHEADER; //TOTAL: 40 Bytes

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx.
 */
typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
