#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{


// make sure user inputs 1 argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

// check that all characters are digits
    int n = strlen(argv[1]);
    for (int c = 0; c < n; c++)
    {
        if (!isdigit(argv[1][c]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

// save key as integer
    int k = atoi(argv[1]);

// get plain text from user
    string p = get_string("plaintext: ");

// get string length of plaintext
    int no = strlen(p);

    printf("ciphertext: ");

// if uppercase print uppercase + key
// if lowercase print lowercase + key
// otherwise print character as is

    for (int co = 0; co < no; co++)
    {
        if (isupper(p[co]))
        {
            printf("%c", ((p[co] - 65 + k) % 26 + 65));
        }
        else if (islower(p[co]))
        {
            printf("%c", ((p[co] - 97 + k) % 26 + 97));
        }
        else
        {
            printf("%c", p[co]);
        }
    }

    printf("\n");
    return 0;
}