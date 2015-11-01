/*
 * resize.c
 *
 * CS50
 * Problem Set 4
 * Jake Craige
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

BITMAPINFOHEADER calculateResizedInfoHeader(BITMAPINFOHEADER* bi, int resizeFactor);
int getPadding(BITMAPINFOHEADER* bi);
void readLine(FILE* infile, int biWidth, RGBTRIPLE* linePixels);
void writeLine(FILE* outfile, int biWidth, int resizeFactor, int outPadding, RGBTRIPLE* linePixels);

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printf("Error: You must provide the new size, input and output file\n");
        return 1;
    }

    int resizeFactor = atoi(argv[1]);
    if (resizeFactor < 1 || resizeFactor > 100)
    {
        printf("Error: Resize factor must be between 1 and 100 inclusive.\n");
        return 1;
    }

    FILE* infile = fopen(argv[2], "r");
    FILE* outfile = fopen(argv[3], "w");

    if (infile == NULL)
    {
        printf("Error: Could not read file at: %s", argv[2]);
        fclose(outfile);
        return 1;
    }

    if (outfile == NULL)
    {
        printf("Error: Could not write file at: %s", argv[3]);
        fclose(infile);
        return 1;
    }


    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, infile);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, infile);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outfile);
        fclose(infile);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    BITMAPFILEHEADER outbf = bf;
    BITMAPINFOHEADER outbi = calculateResizedInfoHeader(&bi, resizeFactor);

    int inPadding = getPadding(&bi);
    int outPadding = getPadding(&outbi);

    // (Byte size of width + the padding) * height
    outbi.biSizeImage = abs(outbi.biHeight) * ((outbi.biWidth * sizeof(RGBTRIPLE)) + outPadding);
    // Original bf size minus it's image + new biSizeImage
    outbf.bfSize = (bf.bfSize - bi.biSizeImage) + outbi.biSizeImage;

    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outfile);
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outfile);

    for(int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        RGBTRIPLE linePixels[bi.biWidth];
        readLine(infile, bi.biWidth, linePixels);

        fseek(infile, inPadding, SEEK_CUR);

        for (int j = 0; j < resizeFactor; j++)
        {
            writeLine(outfile, bi.biWidth, resizeFactor, outPadding, linePixels);
        }
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}

void writeLine(FILE* outfile, int biWidth, int resizeFactor, int outPadding, RGBTRIPLE* linePixels)
{
    for (int i = 0; i < biWidth; i++)
    {
        for (int j = 0; j < resizeFactor; j++)
        {
            fwrite(&linePixels[i], sizeof(RGBTRIPLE), 1, outfile);
        }
    }

    for (int i = 0; i < outPadding; i++)
    {
        fputc(0x00, outfile);
    }
}

void readLine(FILE* infile, int biWidth, RGBTRIPLE* linePixels)
{
    for (int j = 0; j < biWidth; j++)
    {
        RGBTRIPLE triple;

        fread(&triple, sizeof(RGBTRIPLE), 1, infile);

        linePixels[j] = triple;
    }
}

BITMAPINFOHEADER calculateResizedInfoHeader(BITMAPINFOHEADER* bi, int resizeFactor)
{
    BITMAPINFOHEADER out = *bi;

    out.biWidth = bi->biWidth * resizeFactor;
    out.biHeight = bi->biHeight * resizeFactor;

    return out;
}

int getPadding(BITMAPINFOHEADER* bi)
{
    return (4 - (bi->biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
}
