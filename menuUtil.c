/**
 * Implementation of user interface
 * Prompts user for input and directs
 * inputs to correct image manipulation functions
   */

#include <stdio.h>
#include <string.h>
#include "menuUtil.h"
#include "global.h"
#include "ppmIO.h"
#include "imageManip.h"


//uninitialized value
#define SENTINEL -999

//stores user input
char input[50];

//stores the filename with .ppm extention 
char filename[50];

//stores the value indicating the degree of the effect
float amt = SENTINEL ;

//stores the values of the corners
int x1 = SENTINEL;
int y1 = SENTINEL;
int x2 = SENTINEL;
int y2 = SENTINEL;

//stores the radius
float sigma = SENTINEL;

//stores the threshold 
float threshold = SENTINEL;

//stores the image struct
Image* image = NULL;

/**
 * Function to print all menu options to the user
 * Indicates the input format required
 */
void print_menu(){
  printf(
          "Main Menu:\n"
	"\tr <filename> - read image from <filename>\n"
        "\tw <filename> - write image to <filename>\n"
        "\ts - swap color channels\n"
        "\tbr <amt> - change brightness (up or down) by the given amount\n"
        "\tc <x1> <y1> <x2> <y2> - crop image to the box with the given corners\n"
        "\tg - convert to grayscale\n"
        "\tcn <amt> - change contrast (up or down) by the given amount\n"
        "\tbl <sigma> - Gaussian blur with the given radius (sigma)\n"
        "\tsh <sigma> <amt> - sharpen by given amount (intensity), with radius (sigma)\n"
        "\te <sigma> <threshold> - detect edges with intensity gradient above given threshold\n"
        "\tq - quit\n"
	 );
}

/**
 * Function to print out appropirate error message
 */
void error(int n){
  switch(n){

  case 1:
    printf("No image file detected. Please make sure you have read in a file.\n");
    break;

  case 2:
    printf("Your input is invalid. Please try again:\n");
    break;

  case 3:
    printf("Your parameter(s) is/are invalid. Please try again:\n");
    break;

  case 4:
    printf("System error: Memory allocation unsuccessful!\n");
    break;
  }
}

/**
 * Functions to break up the user input
 * and store them in the corresponding variables
 */
void input_r(){
  sscanf(input, "r %s", filename);
}

void input_w(){
  sscanf(input, "w %s", filename);
}

void input_br(){
  sscanf(input, "br %f", &amt);
}

void input_c(){
  sscanf(input, "c %d %d %d %d", &x1, &y1, &x2, &y2);
}

void input_cn(){
  sscanf(input, "cn %f", &amt);
}

void input_bl(){
  sscanf(input, "bl %f", &sigma);
}

void input_sh(){
  sscanf(input, "sh %f %f", &sigma, &amt);
}

void input_e(){
  sscanf(input, "e %f %f", &sigma, &threshold);
}

/**
 * Function to call the correct image manipulation
 * function based on user input. Prints out appropriate error message if input is invalid
 */
void choice(){
  //indicates whether or not user would like to make another manipulation
  int status = 1;
  
  do{
    //prints out menu
    print_menu();

    //obtains user input and stores in the variable input
    fgets(input, sizeof(input), stdin);

    if(!strncmp(input, "r ", 2)){
    //all functions called input breaks up user input and stores their values in the correct variable
    input_r();

    //check if filename is valid
    if(filename[0] != '\0'){
      //reads in file
      image = readPPM(filename);
      //checks if read is successful
      if(!image)
	printf("Failed to read %s\n", filename);
      else
	printf("Image read from %s\n", filename);
    }
    else{
      error(3);
    }
    filename[0] = '\0';//resets filename
  }
  else if(!strncmp(input, "w ", 2)){
    input_w();
    if(filename[0] != '\0'){
      if(!image){
	printf("Failed to write image.\n");
    }else{
	writePPM(image, filename);
      	printf("Image written to %s\n", filename);
      }
    }
    else{
      error(3);
    }
    filename[0] = '\0';
  }
  else if(!strncmp(input, "s", 1) && strncmp(input, "sh", 2)){
      swap(image);
    }
  else if(!strncmp(input, "br", 2)){
    input_br();
    //checks if the variables are uninitialized
    if(amt == SENTINEL){
      error(3);
    }
    else{
      bright(image, amt);
      //uninitializes value of variables
      amt = SENTINEL;
    }
  }
  else if(!strncmp(input, "c", 1) && strncmp(input, "cn", 2)){
    input_c();
    if(x1 == SENTINEL || y1 == SENTINEL || x2 == SENTINEL || y2 == SENTINEL){
      error(3);
    }
    else{
      crop(image, x1, y1, x2, y2);
      x1 = SENTINEL;
      y1 = SENTINEL;
      x2 = SENTINEL;
      y2 = SENTINEL;
    }
  }
  else if(!strncmp(input, "g", 1)){
      gray_scale(image);
  }
  else if(!strncmp(input, "cn", 2)){
    input_cn();
    if(amt == SENTINEL){
      error(3);
    }
    else{
      contrast(image, amt);
      amt = SENTINEL;
    }
  }
  else if(!strncmp(input, "bl", 2)){
    input_bl();
    if(image && sigma == SENTINEL){
      error(3);
    }
    else{
      printf("Applying blur filter...\n");
      blur(image, sigma);
      sigma = SENTINEL;
    }
  }
  else if(!strncmp(input, "sh", 2)){
    input_sh();
    if(image && (sigma == SENTINEL || amt == SENTINEL)){
      error(3);
    }
    else{
      printf("Sharpening image...\n");
      sharpen(image, sigma, amt);
      sigma = SENTINEL;
      amt = SENTINEL;
    }
  }
  else if(!strncmp(input, "e", 1)){
    input_e();
    if(image && (sigma == SENTINEL || threshold == SENTINEL)){
      error(3);
    }
    else{
      printf("Performing edge detection...\n");
      edge(image, sigma, threshold);
      sigma = SENTINEL;
      threshold = SENTINEL;
    }
  }
  else if(!strncmp(input, "q", 1)){
    //free the image if image is not NULL
    if(image){
      //free the data if data is not NULL
      if(image->data)
	free(image->data);
      free(image);
    }
    printf("Goodbye!\n");
    status = 0;
  }
  else
    //reprompts user for input
    error(2);

  //continuation status
  }while(status);
}

