#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>




// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i ++)
        {

// find average of red green and blue pixels - assign new value to red, green and blue pixels

            image[j][i].rgbtRed = round((round(image[j][i].rgbtRed) + round(image[j][i].rgbtGreen) +  round(image[j][i].rgbtBlue)) / 3);
            image[j][i].rgbtGreen = image[j][i].rgbtRed;
            image[j][i].rgbtBlue = image[j][i].rgbtRed;
        }
    }
    return;
}






// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

// create temporary variables red, green and blue with same values as rgbtREd, rgbtGreen and rgbtBlue

    BYTE red[height][width];
    BYTE green[height][width];
    BYTE blue[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red[i][j] = image[i][j].rgbtRed;
            green[i][j] = image[i][j].rgbtGreen;
            blue[i][j] = image[i][j].rgbtBlue;
        }
    }

// set rgbtRed, rgbtGreen nad rgbt Blue to 0 and keep adding one until values are either capped at 255
// or equal to rgbt*0.393 + rgbtGreen * 0.769 + rgbtBlue * 0.189 for rgbtRed and so on for the other two colors

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            image[i][j].rgbtRed = 0;
            image[i][j].rgbtGreen = 0;
            image[i][j].rgbtBlue = 0;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            while (image[i][j].rgbtRed < 255 && image[i][j].rgbtRed < round(red[i][j] * 0.393 + green[i][j] * 0.769 + blue[i][j] * 0.189))
            {
                image[i][j].rgbtRed++;
            }
            while (image[i][j].rgbtGreen < 255 && image[i][j].rgbtGreen < round(red[i][j] * 0.349 + green[i][j] * 0.686 + blue[i][j] * 0.168))
            {
                image[i][j].rgbtGreen++;
            }
            while (image[i][j].rgbtBlue < 255 && image[i][j].rgbtBlue < round(red[i][j] * 0.272 + green[i][j] * 0.534 + blue[i][j] * 0.131))
            {
                image[i][j].rgbtBlue++;
            }
        }
    }

    return;

}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

// create temporary variables red, green and blue with same values as rgbtREd, rgbtGreen and rgbtBlue

    BYTE red[height][width];
    BYTE green[height][width];
    BYTE blue[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red[i][j] = image[i][j].rgbtRed;
            green[i][j] = image[i][j].rgbtGreen;
            blue[i][j] = image[i][j].rgbtBlue;
        }
    }

// for images with a width of an odd number of pixels
// image[0][0] must equal image[0][width -1], image[0][1] must equal image[0][width - 2] and so on

    if (width % 2 == 1)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width / 2; j ++)
            {
                image[i][j].rgbtRed = red[i][width - 1 - j];
                image[i][j].rgbtGreen = green[i][width - 1 - j];
                image[i][j].rgbtBlue = blue[i][width - 1 - j];
            }
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width / 2; j ++)
            {
                image[i][1 + (width / 2) + j].rgbtRed = red[i][width / 2 - 1 - j];
                image[i][1 + (width / 2) + j].rgbtGreen = green[i][width / 2 - 1 - j];
                image[i][1 + (width / 2) + j].rgbtBlue = blue[i][width / 2 - 1 - j];
            }
        }
    }


// for images with a width of an even number of pixels

    else
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width / 2; j ++)
            {
                image[i][j].rgbtRed = red[i][width - 1 - j];
                image[i][j].rgbtGreen = green[i][width - 1 - j];
                image[i][j].rgbtBlue = blue[i][width - 1 - j];
            }
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width / 2; j ++)
            {
                image[i][width / 2 + j].rgbtRed = red[i][width / 2 - 1 - j];
                image[i][width / 2 + j].rgbtGreen = green[i][width / 2 - 1 - j];
                image[i][width / 2 + j].rgbtBlue = blue[i][width / 2 - 1 - j];
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE red[height][width];
    BYTE green[height][width];
    BYTE blue[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red[i][j] = image[i][j].rgbtRed;
            green[i][j] = image[i][j].rgbtGreen;
            blue[i][j] = image[i][j].rgbtBlue;
        }
    }


// all middle pixels

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            float r = (red[i - 1][j - 1]) + (red[i - 1][j]) + (red[i - 1][j + 1]) + (red[i][j - 1]) + (red[i][j]) + (red[i][j + 1]) +
                      (red[i + 1][j - 1]) + (red[i + 1][j]) + (red[i + 1][j + 1]);
            float g = (green[i - 1][j - 1]) + (green[i - 1][j]) + (green[i - 1][j + 1]) + (green[i][j - 1]) + (green[i][j]) +
                      (green[i][j + 1]) + (green[i + 1][j - 1]) + (green[i + 1][j]) + (green[i + 1][j + 1]);
            float b = (blue[i - 1][j - 1]) + (blue[i - 1][j]) + (blue[i - 1][j + 1]) + (blue[i][j - 1]) + (blue[i][j]) + (blue[i][j + 1]) +
                      (blue[i + 1][j - 1]) + (blue[i + 1][j]) + (blue[i + 1][j + 1]);

            image[i][j].rgbtRed =  round(r / 9);
            image[i][j].rgbtGreen = round(g / 9);
            image[i][j].rgbtBlue = round(b / 9);
        }
    }

// all side pixels

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            float r1 = (red[0][j - 1]) + (red[0][j]) + (red[0][j + 1]) + (red[1][j - 1]) + (red[1][j]) + (red[1][j + 1]);
            image[0][j].rgbtRed = round(r1 / 6);


            float r2 = (red[height - 1][j - 1]) + (red[height - 1][j]) + (red[height - 1][j + 1]) + (red[height - 2][j - 1]) +
                       (red[height - 2][j]) + (red[height - 2][j + 1]);
            image[height - 1][j].rgbtRed = round(r2 / 6);

            float r3 = (red[i - 1][0]) + (red[i][0]) + (red[i + 1][0]) + (red[i - 1][1]) + (red[i][1]) + (red[i + 1][1]);
            image[i][0].rgbtRed = round(r3 / 6);

            float r4 = (red[i - 1][width - 1]) + (red[i][width - 1]) + (red[i + 1][width - 1]) + (red[i - 1][width - 2]) +
                       (red[i][width - 2]) + (red[i + 1][width - 2]);
            image[i][width - 1].rgbtRed = round(r4 / 6);

            float g1 = (green[0][j - 1]) + (green[0][j]) + (green[0][j + 1]) + (green[1][j - 1]) + (green[1][j]) + (green[1][j + 1]);
            image[0][j].rgbtGreen = round(g1 / 6);

            float g2 = (green[height - 1][j - 1]) + (green[height - 1][j]) + (green[height - 1][j + 1]) + (green[height - 2][j - 1]) +
                       (green[height - 2][j]) + (green[height - 2][j + 1]);
            image[height - 1][j].rgbtGreen = round(g2 / 6);

            float g3 = (green[i - 1][0]) + (green[i][0]) + (green[i + 1][0]) + (green[i - 1][1]) + (green[i][1]) + (green[i + 1][1]);
            image[i][0].rgbtGreen = round(g3 / 6);

            float g4 = (green[i - 1][width - 1]) + (green[i][width - 1]) + (green[i + 1][width - 1]) + (green[i - 1][width - 2]) +
                       (green[i][width - 2]) + (green[i + 1][width - 2]);
            image[i][width - 1].rgbtGreen = round(g4 / 6);

            float b1 = (blue[0][j - 1]) + (blue[0][j]) + (blue[0][j + 1]) + (blue[1][j - 1]) + (blue[1][j]) + (blue[1][j + 1]);
            image[0][j].rgbtBlue = round(b1 / 6);

            float b2 = (blue[height - 1][j - 1]) + (blue[height - 1][j]) + (blue[height - 1][j + 1]) + (blue[height - 2][j - 1]) +
                       (blue[height - 2][j]) + (blue[height - 2][j + 1]);
            image[height - 1][j].rgbtBlue = round(b2 / 6);

            float b3 = (blue[i - 1][0]) + (blue[i][0]) + (blue[i + 1][0]) + (blue[i - 1][1]) + (blue[i][1]) + (blue[i + 1][1]);
            image[i][0].rgbtBlue = round(b3 / 6);

            float b4 = (blue[i - 1][width - 1]) + (blue[i][width - 1]) + (blue[i + 1][width - 1]) + (blue[i - 1][width - 2]) +
                       (blue[i][width - 2]) + (blue[i + 1][width - 2]);
            image[i][width - 1].rgbtBlue = round(b4 / 6);
        }
    }


// all corner pixels

    float r1 = (red[0][0] + red[1][0] + red[0][1] + red[1][1]);
    float g1 = (green[0][0] + green[1][0] + green[0][1] + green[1][1]);
    float b1 = (blue[0][0] + blue[1][0] + blue[0][1] + blue[1][1]);

    image[0][0].rgbtRed = round(r1 / 4);
    image[0][0].rgbtGreen = round(g1 / 4);
    image[0][0].rgbtBlue = round(b1 / 4);

    float r2 = (red[height - 1][0] + red[height - 2][0] + red[height - 2][1] + red[height - 1][1]);
    float g2 = (green[height - 1][0] + green[height - 2][0] + green[height - 2][1] + green[height - 1][1]);
    float b2 = (blue[height - 1][0] + blue[height - 2][0] + blue[height - 2][1] + blue[height - 1][1]);

    image[height - 1][0].rgbtRed = round(r2 / 4);
    image[height - 1][0].rgbtGreen = round(g2 / 4);
    image[height - 1][0].rgbtBlue = round(b2 / 4);

    float r3 = (red[0][width - 1] + red[0][width - 2] + red[1][width - 1] + red[1][width - 2]);
    float g3 = (green[0][width - 1] + green[0][width - 2] + green[1][width - 1] + green[1][width - 2]);
    float b3 = (blue[0][width - 1] + blue[0][width - 2] + blue[1][width - 1] + blue[1][width - 2]);

    image[0][width - 1].rgbtRed = round(r3 / 4);
    image[0][width - 1].rgbtGreen = round(g3 / 4);
    image[0][width - 1].rgbtBlue = round(b3 / 4);

    float r4 = (red[height - 1][width - 1] + red[height - 2][width - 1] + red[height - 2][width - 2] + red[height - 1][width - 2]);
    float g4 = (green[height - 1][width - 1] + green[height - 2][width - 1] + green[height - 2][width - 2] +
                green[height - 1][width - 2]);
    float b4 = (blue[height - 1][width - 1] + blue[height - 2][width - 1] + blue[height - 2][width - 2] + blue[height - 1][width - 2]);

    image[height - 1][width - 1].rgbtRed = round(r4 / 4);
    image[height - 1][width - 1].rgbtGreen = round(g4 / 4);
    image[height - 1][width - 1].rgbtBlue = round(b4 / 4);

    return;
}
