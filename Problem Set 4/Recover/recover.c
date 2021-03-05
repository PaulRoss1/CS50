#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 512


int input_error()
{
    printf("Usage: ./recover image\n");
    return 1;
}

int file_error(char filename[])
{
    printf("Unable to open file: %s\n", filename);
    return 1;
}

bool isJpgHeader(uint8_t buff[])
{
        return
        buff[0] == 0xff
        && buff[1] == 0xd8
        && buff[2] == 0xff
        && (buff[3] & 0xf0) == 0xe0;
}

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        return input_error();
    }

    char* inputFile = argv[1];
    if(inputFile==NULL)
    {
        return input_error();
    }

    FILE* inputPtr = fopen(inputFile, "r");
    if(inputPtr == NULL)
    {
        return file_error(inputFile);
    }

    char filename[8];
    FILE* outputPtr = NULL;
    uint8_t buff[BLOCK_SIZE];
    int jpgCounter = 0;

    while(fread(buff, sizeof(uint8_t), BLOCK_SIZE, inputPtr) || feof(inputPtr)==0)
    {
        if(isJpgHeader(buff))
        {
            if(outputPtr!=NULL)
            {
                fclose(outputPtr);
            }
            sprintf(filename, "%03i.jpg", jpgCounter);
            outputPtr = fopen(filename, "w");
            jpgCounter++;
        }
        if(outputPtr!=NULL)
        {
            fwrite(buff, sizeof(buff), 1, outputPtr);
        }
    }

    if(inputPtr==NULL)
    {
        fclose(inputPtr);
    }
    if(outputPtr==NULL)
    {
        fclose(outputPtr);
    }
    return 0;
}
