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

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printf("Error: You must provide the new size, input and output file");
        return 1;
    }

    // ./resize 4 small.bmp resized-large.bmp

    int resizeFactor = atoi(argv[1]);
    printf("%i", resizeFactor);
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
    BITMAPINFOHEADER outbi = bi;

    outbi.biWidth = bi.biWidth * resizeFactor;
    outbi.biHeight = bi.biHeight * resizeFactor;
    outbi.biSizeImage = bi.biSizeImage * resizeFactor;
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER);

    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outfile);
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outfile);

    int inPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outPadding =  (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    for(int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        RGBTRIPLE line[bi.biWidth];

        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE triple;

            fread(&triple, sizeof(RGBTRIPLE), 1, infile);

            line[j] = triple;
        }

        fseek(infile, inPadding, SEEK_CUR);

        for (int r = 0; r < resizeFactor; r++)
        {
            for (int j = 0; j < bi.biWidth; j++)
            {
                for (int k = 0; k < resizeFactor; k++)
                {
                    fwrite(&line[j], sizeof(RGBTRIPLE), 1, outfile);
                }
            }

            for (int k = 0; k < outPadding; k++)
            {
                fputc(0x00, outfile);
            }
        }
    }

    fclose(infile);
    fclose(outfile);
    // TODO: return 0, this is useful to see output from Dispatch
    return 1;
}
