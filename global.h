#ifndef GLOBAL_H
#define GLOBAL_H
#include "ppmIO.h"


//global variables dimension of image 
int rows;
int cols;
int colors;

/*functions used for brightness*/
unsigned char clamp(double input);//clamp chars to [0, 255]

/*function used for contrast*/
double clamp2(double input);//clamp doubles to [-127, 127]
int convert(unsigned char value);//convert value to [-127, 127]
double restore(double value);//restore value back to [0, 255]

/*functions used for blur*/
void matrix(double array[], int dim, double sigma);//create a matrix of weights according to Gaussian distribution
void Blur_Pixel(Image* image, double array[], int dim, int cur_r, int cur_c);//blur the pixel at given position, cur_r*cols+cur_c 

/*functions used for sharpen*/
void Sharpen_Pixel(Image *im, Image* bl, double amt, int i);//sharpens the pixel at given position index, i.

/*functions used for edge detection*/
double intensity(Pixel* pix, int r, int c);//calculates intensity at given position, r*cols+c
unsigned char set_tone(double intensity, double threshold);//set pixel to black or white depending on threshold

#endif
