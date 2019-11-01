#ifndef PPMIO_H
#define PPMIO_H
#include <stdio.h>
#include <stdlib.h>


/* A struct to store an RGB pixel, one byte per channel. */
typedef struct _pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

/* A struct to bundle together a pixel array. */
typedef struct _image {
  Pixel *data;
  int rows;
  int cols;
  int colors;
} Image;

/*function to read a PPM image and store in in an Image struct, return a pointer to the struct*/
Image* readPPM(char *filename);

/*function to write a PPM formatted image file to disk*/
int writePPMtoDisk(Pixel *image, int rows, int cols, int colors, FILE *fp);

/*function to write an Image struct to a PPM file*/
int writePPM(Image *im, char *filename);

#endif

