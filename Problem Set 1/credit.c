#include <stdio.h>
#include <cs50.h>
#include <math.h>


int main(void)
{
// keep prompting user for a credit card number until greater than 0
    long num;
    do
    {
        num = get_long("Number: \n");
    }
    while (num < 0);

// find out number of digits in credit card number
    long i = 0;
    long numx = num;
    do
    {
        numx = numx / 10;
        i++;
    }
    while (numx > 0);



//  Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.

    long p;
    long f = 0;
    for (int c = 1; c < (i / 2) + 1; c++)
    {
        p = pow(100, c);
        f =  f + (((num % p - num % (p / 10)) / (p / 10) * 2) % 10) + (((num % p - num % (p / 10)) / (p / 10) * 2) / 10);
    }


//   Find out sum of the digits that weren’t multiplied by 2

    long f2 = 0;
    for (int c = 1; c < (i) + 1; c = c + 2)
    {
        p = pow(10, c);
        f2 = f2 + ((num % p - num % (p / 10)) / (p / 10) % 10) + ((num % p - num % (p / 10)) / (p / 10) / 10);


    }



// int y is the first two digits of credit card number
// int z is the first digit of credit card number

    int y = floor(num / pow(10, (i - 2)));
    int z = floor(num / pow(10, (i - 1)));




// American Express: 15-digit
// American Express: start with 34 or 37

    if ((f + f2) % 10 == 0 && i == 15 && (y == 34 || y == 37))
    {
        printf("AMEX\n");

    }
    // MasterCard: 16-digit
    // MasterCard: start with 51, 52, 53, 54, or 55

    else if ((f + f2) % 10 == 0 && i == 16 && (y == 51 || y == 52 || y == 53 || y == 54 || y == 55))
    {
        printf("MASTERCARD\n");

    }
    // Visa: 13-digit and 16-digit
    // Visa: start with 4.
    else if ((f + f2) % 10 == 0 && (i == 13 || i == 16) && (z == 4))
    {
        printf("VISA\n");

    }
    else
    {
        printf("INVALID\n");
    }





}