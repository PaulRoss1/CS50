#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int x;
    do
    {
        x = get_int("Enter height of pyramid (1 to 8)\n");
    }
    while (x > 8 || x < 1);



    for (int i = 0; i < x; i++)
    {
        for (int j = x; j > i + 1; j--)
        {

            printf(" ");
        }
        for (int k = 0; k <= i; k++)
        {

            printf("#");
        }

        printf("\n");
    }






}