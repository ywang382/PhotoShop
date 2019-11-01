#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H
#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

/*function to swap color channels of image*/
int swap(Image *image);

/*function to adjust image brightness*/
int bright(Image *image, double input);

/*function to crop image*/
int crop(Image *image, int x1, int y1, int x2, int y2);

/*function to convert image to grayscale*/
int gray_scale(Image *image);

/*function to adjust contrast of image*/
int contrast(Image *image, double amt);

/*function to blur image*/
int blur(Image* image, double sigma);

/*function to sharpen image*/
int sharpen(Image* image, double sigma, double amt);

/*function to perform edge detection on image*/
int edge(Image* image, double sigma, double threshold);

#endif
