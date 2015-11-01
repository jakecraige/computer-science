/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define CHUNK_SIZE 512

int startOfJpg(BYTE* chunk);
char* getFilename(int fileNumber);

int main(int argc, char* argv[])
{
    FILE* input = fopen("card.raw", "r");

    BYTE* chunk = malloc(sizeof(BYTE) * CHUNK_SIZE);
    if (chunk == NULL)
    {
        return 1;
    }

    int filesCreated = 0;
    FILE* outfile = NULL;

    while (fread(chunk, CHUNK_SIZE, 1, input) != 0)
    {
        if (startOfJpg(chunk))
        {
            if (outfile != NULL)
            {
                fclose(outfile);
            }

            char* filename = getFilename(filesCreated);
            if (filename == NULL)
            {
                return 1;
            }

            outfile = fopen(filename, "w");
            if (outfile == NULL)
            {
                return 1;
            }

            free(filename);
            filesCreated++;
        }

        if (outfile != NULL)
        {
            fwrite(chunk, CHUNK_SIZE, 1, outfile);
        }
    }

    if (outfile != NULL)
    {
        fclose(outfile);
    }
    fclose(input);
    free(chunk);

    return 0;
}

int startOfJpg(BYTE* chunk)
{
    return (
            chunk[0] == 0xff &&
            chunk[1] == 0xd8 &&
            chunk[2] == 0xff &&
            (chunk[3] == 0xe0 || chunk[3] == 0xe1)
           );
}

char* getFilename(int fileNumber)
{
    char* filename = malloc(sizeof(char) * 8);

    if (filename != NULL)
    {
        sprintf(filename, "%03d.jpg", fileNumber);
    }

    return filename;
}
