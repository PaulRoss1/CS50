#include <string.h>
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // get text from user
    string s = get_string("Text: ");
    int n = strlen(s);

    // count number of letters in text
    float letter = 0;
    for (int i = 0; i < n; i++)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {
            letter++;
        }
    }

    // count number of words in text
    float worda = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == ' ')
        {
            worda++;
        }
    }
    float word = worda + 1;

    // count number of sentences in text
    float sentence = 0;
    for (int i = 0; i < n; i++)
    {
        if ((s[i] == '.' || s[i] == '?' || s[i] == '!'))
        {
            sentence++;
        }
    }

    // calculate index
    float index = 0.0588 * (letter / (word) * 100) - 0.296 * (sentence / (word) * 100) - 15.8;
    int grade =  roundf(index);

    // print grade
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }



}























