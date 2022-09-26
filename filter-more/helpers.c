#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        BYTE greyscale = 0x00;
        for (int j = 0; j < width; j++)
        {
            greyscale = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3;
            image[i][j].rgbtBlue = greyscale;
            image[i][j].rgbtGreen = greyscale;
            image[i][j].rgbtRed = greyscale;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            RGBTRIPLE t = image[i][j];
            image[i][j] = image[i][width - j];
            image[i][width - j] = t;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE Gx, Gy;
    //Sobel operator
    RGBTRIPLE (*bufspace)[width+2] = calloc(height+2, (width+2)*sizeof(RGBTRIPLE));
    for(int i=1; i < height+1; i++)
    {
        for(int j=1; j< width+1; j++)
        {
            bufspace[i][j] = image[i][j];
        }
    }
    for(int i=1; i < height+1; i++)
    {
        for(int j=1; j<width+1; j++)
        {
            Gx.rgbtBlue = bufspace[i-1][j-1].rgbtBlue*-1+bufspace[i][j-1].rgbtBlue*-2+bufspace[i+1][j-1].rgbtBlue*-1+bufspace[i-1][j+1].rgbtBlue+bufspace[i][j+1].rgbtBlue*2+bufspace[i+1][j+1].rgbtBlue;
            Gx.rgbtGreen= bufspace[i-1][j-1].rgbtGreen*-1+bufspace[i][j-1].rgbtGreen*-2+bufspace[i+1][j-1].rgbtGreen*-1+bufspace[i-1][j+1].rgbtGreen+bufspace[i][j+1].rgbtGreen*2+bufspace[i+1][j+1].rgbtGreen;
            Gx.rgbtRed= bufspace[i-1][j-1].rgbtRed*-1+bufspace[i][j-1].rgbtRed*-2+bufspace[i+1][j-1].rgbtRed*-1+bufspace[i-1][j+1].rgbtRed+bufspace[i][j+1].rgbtRed*2+bufspace[i+1][j+1].rgbtRed;
            Gy.rgbtBlue = bufspace[i-1][j-1].rgbtBlue*-1+bufspace[i-1][j].rgbtBlue*-2+bufspace[i-1][j+1].rgbtBlue*-1+bufspace[i+1][j-1].rgbtBlue+bufspace[i+1][j].rgbtBlue*2+bufspace[i+1][j+1].rgbtBlue;
            Gy.rgbtGreen= bufspace[i-1][j-1].rgbtGreen*-1+bufspace[i-1][j].rgbtGreen*-2+bufspace[i-1][j+1].rgbtGreen*-1+bufspace[i+1][j-1].rgbtGreen+bufspace[i+1][j].rgbtGreen*2+bufspace[i+1][j+1].rgbtGreen;
            Gy.rgbtRed= bufspace[i-1][j-1].rgbtRed*-1+bufspace[i-1][j].rgbtRed*-2+bufspace[i-1][j+1].rgbtRed*-1+bufspace[i+1][j-1].rgbtRed+bufspace[i+1][j].rgbtRed*2+bufspace[i+1][j+1].rgbtRed;
            image[i-1][j-1].rgbtBlue = round(sqrt(Gx.rgbtBlue * Gx.rgbtBlue + Gy.rgbtBlue * Gy.rgbtBlue)/4);
            image[i-1][j-1].rgbtGreen = round(sqrt(Gx.rgbtGreen * Gx.rgbtGreen + Gy.rgbtGreen * Gy.rgbtGreen)/4);
            image[i-1][j-1].rgbtRed = round(sqrt(Gx.rgbtRed * Gx.rgbtRed + Gy.rgbtRed * Gy.rgbtRed)/4);
        }
    }
    free(bufspace);
    return;
}
