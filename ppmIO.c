#include "ppmIO.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function to read in PPM image and store it in an Image struct
Image* readPPM(char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    return NULL;
  }

  char buffer[1024];//for storing tags and comments

  fgets(buffer, 5, fp);//stores tag of PPM "P6"

  //check if PPM file is valid by checking the tag "P6"
  if(strncmp(buffer, "P6", 2)!= 0){
    printf("Error: invalid PPM file: \"%s\" !\n", filename);
    return NULL;
  }
    
  fgets(buffer, 1024, fp);//scans first line of comment
  
  while(buffer[0] == '#')//keeps scanning lines of comments
    fgets(buffer, 1024, fp);
  
  sscanf(buffer, "%d %d",&cols, &rows);//scans values for cols and rows (global variables)
  fscanf(fp,"%d\n", &colors);//scans values for colors (global variable)
  
  Pixel *pix = malloc(sizeof(Pixel)*rows*cols);//allocate memory for each pixel
  Image *im = malloc(sizeof(Pixel)*rows*cols + sizeof(int)*3);//allocate memory for entire image containing the pixels
  
  fread(pix, sizeof(Pixel), rows * cols, fp);//read pixel from binary
  fclose(fp);

  im->data = pix;
  im->rows = rows;
  im->cols = cols;
  im->colors = colors; 
  return im;  
}

//function to write a PPM formatted image file to disk
int writePPMtoDisk(Pixel *image, int rows, int cols, int colors, FILE *fp) {
  if (!fp) {
    fprintf(stderr, "Error:ppmIO - writePPM given a bad filehandle\n");
    return 0;
  }
  fprintf(fp, "P6\n");//write tag "P6"
  fprintf(fp, "%d %d\n%d\n", cols, rows, colors);//write dimensions
  int written = fwrite(image, sizeof(Pixel), rows * cols, fp);//write pixels
  if (written != (rows * cols)) {
    printf("Error: failed to write data to file!\n");
  }

  return written;
}

//function to write an Image struct to a PPM file, return number of pixels of image written
int writePPM(Image *im, char *filename) {

  FILE *fp = fopen(filename, "wb");

  if (!fp) {
    return 0;
  }

  int written = writePPMtoDisk(im->data, im->rows, im->cols, im->colors, fp);
  fclose(fp);
  return written;
}
