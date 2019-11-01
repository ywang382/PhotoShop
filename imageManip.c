#include "imageManip.h"
#include "global.h"
#include "menuUtil.h"
#include "ppmIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//included for memcpy

/**
 * This file contains all the functions to carry out the various different image manipulations. 
 * Some functions make use of helper functions from the global.c file.
 */

//function to swap color channels of an image
int swap(Image* image){
  if(image == NULL){
    error(1);
    return 0;
  }
  Pixel *im = image->data;
  int count;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int i = r*cols+c;//index of pixel, used for convenience
      //swapping r and b channel
      unsigned char temp = im[i].r;
      im[i].r = im[i].g;
      im[i].g = im[i].b;
      im[i].b = temp;
      count++;
    }
  }
  printf("Swapped color channels.\n");
  return count;
}

//function to adjust image brightness
int bright(Image* image, double amt){
  if(image == NULL){
    error(1);
    return 0;
  }
  Pixel *im = image->data;
  int count;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int i = r*cols+c;//index of pixel
      //change each color channel by indicated amount
      im[i].r = clamp(im[i].r + amt);//clamping makes sure value doesn't go out of bound
      im[i].g = clamp(im[i].g + amt);
      im[i].b = clamp(im[i].b + amt);
      count++;
    }
  }
  printf("Adjusted brightness.\n");
  return count;
}

//function to crop image to specified size giving by four ints
int crop(Image* image, int x1, int y1, int x2, int y2){
  if(image == NULL){
    error(1);
    return 0;
  }

  //check for invalid crop dimension
  if(x1<0 || x2<=0 || y1<0 || y2<=0 || x1>=x2 || y1>=y2 || x1>=cols || x2>=cols || y1>=rows || y2>=rows){
    printf("Error:crop - invalid crop dimension!\n");
    return 0;
  }

  //set new dimension
  int newRows = y2 - y1 + 1;
  int newCols = x2 - x1 + 1;
  //allocate memory for new pixel array
  Pixel *pix = malloc(sizeof(Pixel)*newRows*newCols);
  //check if memory allocation is successful, otherwise prints out error
  if(pix == NULL){
    error(4);
    return 0;
  }
  int i = 0;
  //populate new pixel array
  for(int r = y1; r <= y2; r++){
    for(int c = x1; c <= x2; c++){
      pix[i] = image->data[r*cols+c];
      i++;
    }
  }
  
  //free original pixel array
  if(image->data)
    free(image->data);
  else{
    printf("Error:imageManip - cannot crop image!\n");
    return 0;
  }

  //set pointer to the new pixel array;
  image->data = pix;
  image->rows = newRows;
  image->cols = newCols;
  //change values of global variables
  rows = newRows;
  cols = newCols;

  printf("Image cropped.\n");
  return newRows*newCols;
}

//Function to transform a image to a shade of gray by NTSC standards
int gray_scale(Image* image){
  if(image == NULL){
    error(1);
    return 0;
  }
  
  Pixel *im = image->data;
  int count;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int i = r*cols+c;//index of pixel
      unsigned char intensity = 0.30*im[i].r + 0.59*im[i].g + 0.11*im[i].b; 
      //change each color channel to the appropriate intensity
      im[i].r = clamp(intensity);
      im[i].g = clamp(intensity);
      im[i].b = clamp(intensity);
      count++;
    }
  }
  printf("Converted to grayscale.\n");
  return count;
}

//Function to adjust contrast of image according to given amount
int contrast(Image* image, double amt){
  if(image == NULL){
    error(1);
    return 0;
  }
  
  Pixel *im = image->data;
  int count;

  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int i = r*cols + c;//index of pixel
      //assignes each color chanel new value
      im[i].r = clamp(restore((clamp2(amt*convert(im[i].r)))));
      im[i].g = clamp(restore((clamp2(amt*convert(im[i].g)))));
      im[i].b = clamp(restore((clamp2(amt*convert(im[i].b)))));
      count++;
    }
  }
  printf("Contrast adjusted.\n");
  return count;
}

//Function to blur image by a given sigma, Guassian radius
int blur(Image* image, double sigma){
  if(image == NULL){
    error(1);
    return 0;
  }

  int dim = (5*sigma)*2 + 1;//always gives odd dimension
  double weights[dim*dim];//create new double array to store matrix of weights
  
  //populate array of weights (filter matrix)
  matrix(weights, dim, sigma);
  
  //loop through entire array with filter
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      Blur_Pixel(image, weights, dim, r, c);//blur each individual pixel 
    }
  }
  printf("Image blurred.\n");
  return dim;
}

//Function to sharpen image given the sigma and a factor, amt
int sharpen(Image* image, double sigma, double amt){
  if(image == NULL){
    error(1);
    return 0;
  }

  //create temporary image struct to store blurred image
  Image* blurred = malloc(sizeof(Pixel)*rows*cols + 3*sizeof(int));//malloc for new blurred image
  Pixel* pix = malloc(sizeof(Pixel)*rows*cols);//malloc for new pixel array

  //check if allocation is successful
  if(pix && blurred){
    error(4);
    return 0;
  }
  
  memcpy(pix, image->data, sizeof(Pixel)*rows*cols);//copy pixel array over to new image
  //copy dimension over to new iamge
  blurred->data = pix;
  blurred->cols = image->cols;
  blurred->rows = image->rows;
  blurred->colors = image->colors;
  
  //blurring the temporary image struct
  blur(blurred, sigma);

  //superimposing the two images
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      Sharpen_Pixel(image, blurred, amt, r*cols+c);//sharpening each pixel
    }
  }

  //freeing temporary memory allocations
  if(blurred->data)
    free(blurred->data);
  if(blurred)
    free(blurred);

  printf("Image sharpened.\n");
  return 1;
}

//Function for edge detection with given sigma and threshold
int edge(Image* image, double sigma, double threshold){

  if(image == NULL){
    error(1);
    return 0;
  }

  //blur and grayscale image first
  blur(image, sigma);
  gray_scale(image);

  Pixel *pix = malloc(sizeof(Pixel)*rows*cols);//allocate memory to store new pixel arrasy
  //check for successful allocation
  if(pix == NULL){
    error(4);
    return 0;
  }

  memcpy(pix, image->data, sizeof(Pixel));//copy over pixel array to the new array

  //set color for each pixel to either black or white
  for(int r = 1; r < rows-1; r++){
    for(int c = 1; c < cols-1; c++){
      int i = r*cols+c;
      double inten = intensity(image->data, r, c);//finding out the intensity at given pixel
      unsigned char tone = set_tone(inten, threshold);//comparing intensity with threshold
      pix[i].r = tone;
      pix[i].g = tone;
      pix[i].b = tone;
    }
  }

  //freeing allocated memory
  if(image->data)
    free(image->data);
  image->data = pix;
  printf("Edge detection successful.\n");
  return 1;
}




