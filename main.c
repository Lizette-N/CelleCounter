// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <unistd.h>

// Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}
/*void printarray(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS])
{
  for (int x = 0; x < 10; x++)
  {
    for (int y = 0; y < 10; y++)
    {
      printf("R: %i ", input_image[x][y][0]);
      printf("G: %i ", input_image[x][y][1]);
      printf("B: %i :", input_image[x][y][2]);
    }
  }
}
*/
unsigned char arrayA[BMP_WIDTH][BMP_HEIGHT];
unsigned char arrayB[BMP_WIDTH][BMP_HEIGHT];
unsigned char greyImage2D[BMP_WIDTH][BMP_HEIGHT];
unsigned char greyImage3D[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
unsigned char array2D[BMP_WIDTH][BMP_HEIGHT];
unsigned char array3D[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
int wasEroded=0;
int *ptr=&wasEroded;
void ConvertToGrey(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      greyImage2D[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
    }
  }
}
void upscale2DTo3D(unsigned char array2D[BMP_WIDTH][BMP_HEIGHT], unsigned char array3D[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        array3D[x][y][c] = array2D[x][y];
      }
    }
  }
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];

void binaryThreshold(unsigned char greyImage2D[BMP_WIDTH][BMP_HEIGHT])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      if (greyImage2D[x][y] <= 90)
      {
        greyImage2D[x][y] = 0;
      }
      else
      {
        greyImage2D[x][y] = 254;
      }
    }
  }
}

void erode(unsigned char arrayA[BMP_WIDTH][BMP_HEIGHT], unsigned char arrayB[BMP_WIDTH][BMP_HEIGHT]){
  //*ptr=0;
  for (int x = 1; x< BMP_WIDTH-1; x++){
      for (int y = 1; y< BMP_HEIGHT-1; y++){
          if (arrayA[x-1][y]== 0||arrayA[x][y-1]==0||arrayA[x][y+1]==0||arrayA[x+1][y]==0){
            arrayB[x][y]=0;
            // was eroded kommer her ind
            //*ptr=1;
          }
          else{
            arrayB[x][y]=255;
          }
      }
    
    }
    for (int x = 0; x < BMP_WIDTH; x++){
      arrayB[x][0]=0;
      arrayB[x][BMP_WIDTH-1]=0;
      arrayB[0][x]=0;
      arrayB[BMP_HEIGHT-1][x]=0;
  }
}

// Main function
int main(int argc, char **argv)
{
  // argc counts how may arguments are passed
  // argv[0] is a string with the name of the program
  // argv[1] is the first command line argument (input image)
  // argv[2] is the second command line argument (output image)

  // Checking that 2 arguments are passed
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run inversion
  invert(input_image, output_image);
  ConvertToGrey(input_image);
  binaryThreshold(greyImage2D);
  
  erode(greyImage2D,arrayB);
  erode(arrayB,arrayA);
  upscale2DTo3D(arrayA, greyImage3D);
  write_bitmap(greyImage3D, argv[2]);

for (int i = 0; i<10; i++){
  if (i%2==0){
    erode(arrayB,arrayA);
    upscale2DTo3D(arrayA, greyImage3D);
  }
 else{
  erode(arrayA,arrayB);
  upscale2DTo3D(arrayB, greyImage3D);
 }
  write_bitmap(greyImage3D, argv[2]);
  sleep(1);
}
//write_bitmap(greyImage3D, argv[2]);

  
//upscale2DTo3D(arrayB, greyImage3D);

  // printarray(input_image);

  // Save image to file
  //write_bitmap(output_image, argv[2]);
  //write_bitmap(greyImage3D, argv[2]);
  printf("Done!\n");
  return 0;
}
