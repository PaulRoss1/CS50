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
        printf("Usage: ./substitution key\n");
        return 1;
    }

// check that all characters are letters
    int n = strlen(argv[1]);
    for (int c = 0; c < n; c++)
    {
        if (!isalpha(argv[1][c]))
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

// make sure key is 26 characters long
    if (n != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

// check for duplicates
    for (int c = 0; c < n; c++)
    {
        for (int d = 1; d <= n; d++)
        {
            while (c + d < n)
            {
                if (argv[1][c] == argv[1][c + d])
                {
                    printf("Key mustn't contain dublicates\n");
                    return 1;
                }

                d++;
            }
        }
    }

// make argv uppercase
    for (int c = 0; c < n; c++)
    {
        argv[1][c] = toupper(argv[1][c]);
    }

// get plain text from user
    string p = get_string("plaintext: ");

// get string length of plaintext
    int no = strlen(p);

    printf("ciphertext: ");

// if uppercase print uppercase + key
// if lowercase print lowercase + key
// otherwise print character as is

    for (int c = 0; c < no; c++)
    {
        if (islower(p[c]) && isalpha(p[c]))
        {
            int i = p[c] - 97;
            if (argv[1][c] > 96)
            {
                printf("%c", (argv[1][i]));
            }
            else
            {
                printf("%c", (argv[1][i] + 32));
            }
        }
        else if (isupper(p[c])  && isalpha(p[c]))
        {
            int i = p[c] - 65;
            if (argv[1][c] > 96)
            {
                printf("%c", (argv[1][i] - 32));
            }
            else
            {
                printf("%c", (argv[1][i]));
            }
        }
        else
        {
            printf("%c", p[c]);
        }
    }
    printf("\n");
    return 0;
}