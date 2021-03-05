#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // 1. Open memory card
    // 2. Look for beginning of a JPEG
    // 3. Open a new JPEG file
    // 4. Write 512 bytes until a new JPEG is found


    // 1. Open memory card
    // FILE *f = fopen(filename, "r");
    // first three bytes = 0xff 0xd8 0xff
    // last byte = 0xe0, 0xe1, 0xe2, ..., 0xef
    // block is 512
    
    // 2. Look for beginning of a JPEG
    
    // fread(data, size, number, inptr)
    // data: pointer to where to store data you are reading - some buffer that might be an array
    // size: size of each element to read - number of bytes of each element
    // number: number of elements to read
    // inptr: FILE * to read from
    
    // fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    
    // check if it is jpeg
    // buffer[0] == 0xff
    // buffer[1] == 0xd8
    // buffer[2] == 0xff
    // (buffer[3] & 0xf0) == 0xe0
    
    //FILE *file = fopen("card.raw", "r");
    FILE *file = fopen(argv[1], "r"); // asi stejny
    
    // char buffer[1512];
    // fread(buffer, 1, 1512, file);
    
    // printf("%s\n", buffer);
    // printf("%c\n", buffer[0]);
    // printf("%d\n", buffer[0]);
    
    
    if (!file)
    {
        return 1;
    }

    // Read first three bytes
    unsigned char bytes[1048576 ];
    fread(bytes, 1, 1048576 , file);
    
        for(int i = 0; i < 1048576 ; i++) 
        {
            printf("%d ", bytes[i]);

        }

    // Check first three bytes
    if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff)
    {
        printf("Maybe\n");
    }
    else
    {
        printf("No\n");
    }

    // Close file
   // fclose(file);

    // Working condition
    if(argv[1])
    {
       exit(0);
    } 
    else 
    {
       exit(1);
    }
}
