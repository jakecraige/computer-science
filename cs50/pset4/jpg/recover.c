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

int startOfJpg(BYTE chunk[]);

int main(int argc, char* argv[])
{
    FILE* input = fopen("card.raw", "r");

    BYTE chunk[CHUNK_SIZE];
    int filesCreated = 0;
    FILE* outfile = NULL;

    while (fread(&chunk, CHUNK_SIZE, 1, input) != 0)
    {
        if (startOfJpg(chunk))
        {
            if (outfile != NULL)
            {
                fclose(outfile);
            }

            char* filename = malloc(sizeof(char) * 8);
            if (filename == NULL)
            {
                printf("Cannot allocate memory for filename.\n");
                return 1;
            }
            sprintf(filename, "%03d.jpg", filesCreated);
            outfile = fopen(filename, "w");

            if (outfile == NULL)
            {
                printf("Cannot open file for writing.\n");
                return 1;
            }

            free(filename);
            filesCreated++;
        }

        if (outfile != NULL)
        {
            fwrite(&chunk, CHUNK_SIZE, 1, outfile);
        }
    }

    fclose(input);

    return 0;
}

int startOfJpg(BYTE chunk[])
{
    return (
            chunk[0] == 0xff &&
            chunk[1] == 0xd8 &&
            chunk[2] == 0xff &&
            (chunk[3] == 0xe0 || chunk[3] == 0xe1)
           );
}
