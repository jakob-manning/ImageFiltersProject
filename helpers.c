#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    //for each row for (i)
    for (int i = 0 ; i < height; i++){
        //Each column (j)
        for (int j = 0; j < width; j++){
            //take pixel average
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;
            int average = llround((red + blue + green)/3);
            //set rgb values  to average (creating gray)
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //for each row for (i)
    for (int i = 0 ; i < height; i++){
        //Each column (j)
        for (int j = 0; j < width; j++){
            //take pixel average
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;
            int newRed = llround(
                    .393 * red
                +   .769 * green
                +   .189 * blue
                );
            int newGreen = llround(
                    .349 * red
                +   .686 * green
                +   .168 * blue
                );
            int newBlue = llround(
                    .272 * red
                +   .534 * green
                +   .131 * blue
                );
            //set 255 as the maximum value
            if (newRed > 255){
                newRed = 255;
            }
            if (newGreen > 255){
                newGreen = 255;
            }
            if (newBlue > 255){
                newBlue = 255;
            }
            //set rgb values  to serpia values
            image[i][j].rgbtBlue = newBlue;
            image[i][j].rgbtGreen = newGreen;
            image[i][j].rgbtRed = newRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //create a new array
    RGBTRIPLE(*reflect)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (reflect == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }

    //create a reflected image
    for (int i = 0 ; i < height; i++){
        for (int j = 0; j < width; j++){
            reflect[i][width - j - 1].rgbtRed = image[i][j].rgbtRed;
            reflect[i][width - j - 1].rgbtBlue = image[i][j].rgbtBlue;
            reflect[i][width - j - 1].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    //copy the reflected image back to the original source
    for (int i = 0 ; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j].rgbtRed = reflect[i][j].rgbtRed;
            image[i][j].rgbtBlue = reflect[i][j].rgbtBlue;
            image[i][j].rgbtGreen = reflect[i][j].rgbtGreen;
        }
    }
    free(reflect);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a new array
    RGBTRIPLE(*blur)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (blur == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }

    //create a reflected image
    int newRed;
    int newGreen;
    int newBlue;
    for (int i = 0 ; i < height - 1; i++){
        for (int j = 0; j < width - 1; j++){

            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;

            //Blur Red
            if (i == 0){
                if (j == 0){
                    newRed = llround(
                        (image[i][j].rgbtRed
                    +   image[i + 1][j + 1].rgbtRed
                    +   image[i + 1][j    ].rgbtRed
                    +   image[i    ][j + 1].rgbtRed
                        )/ 4
                    );
                }
                else if (j == width - 1){
                    newRed = llround(
                        (image[i][j].rgbtRed
                    +   image[i + 1][j    ].rgbtRed
                    +   image[i + 1][j - 1].rgbtRed
                    +   image[i    ][j - 1].rgbtRed
                        )/ 4
                    );
                }
                else {
                    newRed = llround(
                        (image[i][j].rgbtRed
                    +   image[i + 1][j + 1].rgbtRed
                    +   image[i + 1][j    ].rgbtRed
                    +   image[i + 1][j - 1].rgbtRed
                    +   image[i    ][j - 1].rgbtRed
                    +   image[i    ][j + 1].rgbtRed
                        )/ 6
                    );
                }
            }
            else if (i == height - 1)
            {
                if (j == 0){
                    newRed = llround(
                        (image[i][j].rgbtRed
                    +   image[i - 1][j    ].rgbtRed
                    +   image[i - 1][j + 1].rgbtRed
                    +   image[i    ][j + 1].rgbtRed
                        )/ 4
                    );
                }
                else if (j == width - 1)
                {
                    newRed = llround(
                        (image[i][j].rgbtRed
                    +   image[i    ][j - 1].rgbtRed
                    +   image[i - 1][j - 1].rgbtRed
                    +   image[i - 1][j    ].rgbtRed
                        )/ 4
                    );
                }
                else {
                    newRed = llround(
                        (image[i][j].rgbtRed
                    +   image[i    ][j - 1].rgbtRed
                    +   image[i - 1][j - 1].rgbtRed
                    +   image[i - 1][j    ].rgbtRed
                    +   image[i - 1][j + 1].rgbtRed
                    +   image[i    ][j + 1].rgbtRed
                        )/ 6
                    );
                }

            }
            else if (j == 0)
            {
                newRed = llround(
                    (image[i][j].rgbtRed
                +   image[i + 1][j + 1].rgbtRed
                +   image[i + 1][j    ].rgbtRed
                +   image[i - 1][j    ].rgbtRed
                +   image[i - 1][j + 1].rgbtRed
                +   image[i    ][j + 1].rgbtRed
                    )/ 6
                );
            }
            else if (j == width - 1)
            {
                newRed = llround(
                    (image[i][j].rgbtRed
                +   image[i + 1][j    ].rgbtRed
                +   image[i + 1][j - 1].rgbtRed
                +   image[i    ][j - 1].rgbtRed
                +   image[i - 1][j - 1].rgbtRed
                +   image[i - 1][j    ].rgbtRed
                    )/ 6
                );
            }
            else
            {
                newRed = llround(
                    (image[i][j].rgbtRed
                +   image[i + 1][j + 1].rgbtRed
                +   image[i + 1][j    ].rgbtRed
                +   image[i + 1][j - 1].rgbtRed
                +   image[i    ][j - 1].rgbtRed
                +   image[i - 1][j - 1].rgbtRed
                +   image[i - 1][j    ].rgbtRed
                +   image[i - 1][j + 1].rgbtRed
                +   image[i    ][j + 1].rgbtRed
                    )/ 9
                );
            }

            //blur Blue
            if (i == 0){
                if (j == 0){
                    newBlue = llround(
                        (image[i][j].rgbtBlue
                    +   image[i + 1][j + 1].rgbtBlue
                    +   image[i + 1][j    ].rgbtBlue
                    +   image[i    ][j + 1].rgbtBlue
                        )/ 4
                    );
                }
                else if (j == width - 1){
                    newBlue = llround(
                        (image[i][j].rgbtBlue
                    +   image[i + 1][j    ].rgbtBlue
                    +   image[i + 1][j - 1].rgbtBlue
                    +   image[i    ][j - 1].rgbtBlue
                        )/ 4
                    );
                }
                else {
                    newBlue = llround(
                        (image[i][j].rgbtBlue
                    +   image[i + 1][j + 1].rgbtBlue
                    +   image[i + 1][j    ].rgbtBlue
                    +   image[i + 1][j - 1].rgbtBlue
                    +   image[i    ][j - 1].rgbtBlue
                    +   image[i    ][j + 1].rgbtBlue
                        )/ 6
                    );
                }
            }
            else if (i == height - 1)
            {
                if (j == 0){
                    newBlue = llround(
                        (image[i][j].rgbtBlue
                    +   image[i - 1][j    ].rgbtBlue
                    +   image[i - 1][j + 1].rgbtBlue
                    +   image[i    ][j + 1].rgbtBlue
                        )/ 4
                    );
                }
                else if (j == width - 1)
                {
                    newBlue = llround(
                        (image[i][j].rgbtBlue
                    +   image[i    ][j - 1].rgbtBlue
                    +   image[i - 1][j - 1].rgbtBlue
                    +   image[i - 1][j    ].rgbtBlue
                        )/ 4
                    );
                }
                else {
                    newBlue = llround(
                        (image[i][j].rgbtBlue
                    +   image[i    ][j - 1].rgbtBlue
                    +   image[i - 1][j - 1].rgbtBlue
                    +   image[i - 1][j    ].rgbtBlue
                    +   image[i - 1][j + 1].rgbtBlue
                    +   image[i    ][j + 1].rgbtBlue
                        )/ 6
                    );
                }

            }
            else if (j == 0)
            {
                newBlue = llround(
                    (image[i][j].rgbtBlue
                +   image[i + 1][j + 1].rgbtBlue
                +   image[i + 1][j    ].rgbtBlue
                +   image[i - 1][j    ].rgbtBlue
                +   image[i - 1][j + 1].rgbtBlue
                +   image[i    ][j + 1].rgbtBlue
                    )/ 6
                );
            }
            else if (j == width - 1)
            {
                newBlue = llround(
                    (image[i][j].rgbtBlue
                +   image[i + 1][j    ].rgbtBlue
                +   image[i + 1][j - 1].rgbtBlue
                +   image[i    ][j - 1].rgbtBlue
                +   image[i - 1][j - 1].rgbtBlue
                +   image[i - 1][j    ].rgbtBlue
                    )/ 6
                );
            }
            else
            {
                newBlue = llround(
                    (image[i][j].rgbtBlue
                +   image[i + 1][j + 1].rgbtBlue
                +   image[i + 1][j    ].rgbtBlue
                +   image[i + 1][j - 1].rgbtBlue
                +   image[i    ][j - 1].rgbtBlue
                +   image[i - 1][j - 1].rgbtBlue
                +   image[i - 1][j    ].rgbtBlue
                +   image[i - 1][j + 1].rgbtBlue
                +   image[i    ][j + 1].rgbtBlue
                    )/ 9
                );
            }

            //blur Green
            if (i == 0){
                if (j == 0){
                    newGreen = llround(
                        (image[i][j].rgbtGreen
                    +   image[i + 1][j + 1].rgbtGreen
                    +   image[i + 1][j    ].rgbtGreen
                    +   image[i    ][j + 1].rgbtGreen
                        )/ 4
                    );
                }
                else if (j == width - 1){
                    newGreen = llround(
                        (image[i][j].rgbtGreen
                    +   image[i + 1][j    ].rgbtGreen
                    +   image[i + 1][j - 1].rgbtGreen
                    +   image[i    ][j - 1].rgbtGreen
                        )/ 4
                    );
                }
                else {
                    newGreen = llround(
                        (image[i][j].rgbtGreen
                    +   image[i + 1][j + 1].rgbtGreen
                    +   image[i + 1][j    ].rgbtGreen
                    +   image[i + 1][j - 1].rgbtGreen
                    +   image[i    ][j - 1].rgbtGreen
                    +   image[i    ][j + 1].rgbtGreen
                        )/ 6
                    );
                }
            }
            else if (i == height - 1)
            {
                if (j == 0){
                    newGreen = llround(
                        (image[i][j].rgbtGreen
                    +   image[i - 1][j    ].rgbtGreen
                    +   image[i - 1][j + 1].rgbtGreen
                    +   image[i    ][j + 1].rgbtGreen
                        )/ 4
                    );
                }
                else if (j == width - 1)
                {
                    newGreen = llround(
                        (image[i][j].rgbtGreen
                    +   image[i    ][j - 1].rgbtGreen
                    +   image[i - 1][j - 1].rgbtGreen
                    +   image[i - 1][j    ].rgbtGreen
                        )/ 4
                    );
                }
                else {
                    newGreen = llround(
                        (image[i][j].rgbtGreen
                    +   image[i    ][j - 1].rgbtGreen
                    +   image[i - 1][j - 1].rgbtGreen
                    +   image[i - 1][j    ].rgbtGreen
                    +   image[i - 1][j + 1].rgbtGreen
                    +   image[i    ][j + 1].rgbtGreen
                        )/ 6
                    );
                }

            }
            else if (j == 0)
            {
                newGreen = llround(
                    (image[i][j].rgbtGreen
                +   image[i + 1][j + 1].rgbtGreen
                +   image[i + 1][j    ].rgbtGreen
                +   image[i - 1][j    ].rgbtGreen
                +   image[i - 1][j + 1].rgbtGreen
                +   image[i    ][j + 1].rgbtGreen
                    )/ 6
                );
            }
            else if (j == width - 1)
            {
                newGreen = llround(
                    (image[i][j].rgbtGreen
                +   image[i + 1][j    ].rgbtGreen
                +   image[i + 1][j - 1].rgbtGreen
                +   image[i    ][j - 1].rgbtGreen
                +   image[i - 1][j - 1].rgbtGreen
                +   image[i - 1][j    ].rgbtGreen
                    )/ 6
                );
            }
            else
            {
                newGreen = llround(
                    (image[i][j].rgbtGreen
                +   image[i + 1][j + 1].rgbtGreen
                +   image[i + 1][j    ].rgbtGreen
                +   image[i + 1][j - 1].rgbtGreen
                +   image[i    ][j - 1].rgbtGreen
                +   image[i - 1][j - 1].rgbtGreen
                +   image[i - 1][j    ].rgbtGreen
                +   image[i - 1][j + 1].rgbtGreen
                +   image[i    ][j + 1].rgbtGreen
                    )/ 9
                );
            }



            //set 255 as the maximum value
            if (newRed > 255){
                newRed = 255;
            }
            if (newGreen > 255){
                newGreen = 255;
            }
            if (newBlue > 255){
                newBlue = 255;
            }


            blur[i][j].rgbtRed = newRed;
            blur[i][j].rgbtBlue = newBlue;
            blur[i][j].rgbtGreen = newGreen;

        }
    }

    //copy the blurred image back to the original source
    for (int i = 0 ; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
        }
    }
    free(blur);
    return;
}
