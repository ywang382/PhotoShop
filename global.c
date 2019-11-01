#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"
#include "ppmIO.h"
#define sq(x) ((x)*(x))

const float PI = 3.14159265;

/**
 * This file contains all the helper functions needed for the imageManip functions to work
 */

//clamp function that takes in a double and  returns a color channel value between [0, 255]
unsigned char clamp(double value){
    unsigned char high = colors;
    char low = 0;
    if(value < (double)low)
        return low;
    if(value > (double)high)
        return high;
    return (unsigned char) value;
}

//clamp function that shifts a color channel value to a double in the  range [-127, 127]
double clamp2(double value){
    int high = colors/2;
    int low = -colors/2;
    if(value < (double)low)
      return (double) low;
    if(value > (double)high)
      return (double)high;
    return (double) value;
}

//function returns a color channel value in range [-127, 127]
int convert(unsigned char value){
  return (int)value - colors/2;
}

//functions restores a color channel from a double value in range [-127, 127] to a double value in [0, 255]
double restore(double value){
  return value + (double)colors/2;
}

/**
 * Function to store the filter matrix in blurring in a 1D array of doubles
 * @param an array of doubles to store values of the filter matrix
 * @param the number of cols/rows of the matrix. This is sqaured matrix
 * @param the sigma value given by user input, indicates the Guassian radius for blurring
 */
void matrix(double array[], int dim, double sigma){
  int center = dim/2 + 1;
  int index = 0;
  //populate 1D array passed in, the matrix is actually stored in a 1D array for convenient usage
  for(int r = 0; r < dim; r++){
    for(int c = 0; c < dim; c++){
      int dx = fabs(c+1 - center);
      int dy = fabs(r+1 - center);
      array[index] = (1.0 / (2.0 * PI  * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
      index++;
    }
  }
}

/**
 * Function to perform blur on the pixel at a given position in the image
 * @param pointer to the image to be blurred
 * @param a double array containing weights from the filte matrix
 * @param length of the double array
 * @param current r (rows) of the pixel to be blurred
 * @param current c (cols) of the pixel to be blurred
 */
void Blur_Pixel(Image* image, double array[], int dim, int cur_r, int cur_c){
  int offset = dim/2;//used to indicate location where to start applying filter
  int index = 0;
  Pixel* pix = image->data;
  double sum = 0; //sum of the weights in the filter matrix
  //variables to store normal sum of different color channels
  double sum_r = 0;
  double sum_g = 0;
  double sum_b = 0;
  //loop through the filter to get normal sum for center pixel
  for(int r = cur_r-offset; r < cur_r-offset+dim; r++){
    for(int c = cur_c-offset; c < cur_c-offset+dim; c++){
      if(r >= 0 && r <= rows-1 && c >= 0 && c<=cols-1){
        sum_r += pix[r*cols+c].r*array[index];
        sum_g += pix[r*cols+c].g*array[index];
        sum_b += pix[r*cols+c].b*array[index];
        sum += array[index];
      }
      index++;
    }
  }
  //setting each color channel to correct value
  pix[cur_r*cols + cur_c].r = (unsigned char)(sum_r/sum);
  pix[cur_r*cols + cur_c].g = (unsigned char)(sum_g/sum);
  pix[cur_r*cols + cur_c].b = (unsigned char)(sum_b/sum);
}

/**
 * Function that performs sharpening on each pixel at the given position
 * @param pointer to the original image to be sharpened
 * @param pointer to the blurred image used for sharpening
 * @param amount by which the image is to be sharpened, provided by user
 * @param current position (r*cols+c) of the pixel to be sharpened
 */
void Sharpen_Pixel(Image* im, Image* bl, double amt, int i){
  int diff_r = im->data[i].r - bl->data[i].r;
  int diff_g = im->data[i].g - bl->data[i].g;
  int diff_b = im->data[i].b - bl->data[i].b;

  im->data[i].r = clamp(im->data[i].r + amt*diff_r);
  im->data[i].g = clamp(im->data[i].g + amt*diff_g);
  im->data[i].b = clamp(im->data[i].b + amt*diff_b);
}

/**
 * Function that calculates the intensity used for edge detection at a given position
 * @param pointer to pixel array 
 * @param current r (rows) of the pixel
 * @param current c (cols) of the pixel
 */
double intensity(Pixel* im, int r, int c){
  double g_x;//gradient x
  double g_y;//gradient y

  g_x = fabs(im[(r-1)*cols+c].g - im[(r+1)*cols+c].g)/2.0;//according to formula
  g_y = fabs(im[r*cols + c+1].g - im[r*cols + c-1].g)/2.0;

  return sqrt(sq(g_x) + sq(g_y));
}

/**
 * Function that sets the tone to either black or white for edge detection
 * @param the intensity, as calculated by the above function
 * @param the threshold value, given by user input
 */
unsigned char set_tone(double value, double threshold){
  unsigned char black = 0;
  unsigned char white = 255;
  if(value <= threshold)
      return white;
    else
      return black;
}





