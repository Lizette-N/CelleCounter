// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

// Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}
/*void printarray(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
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
unsigned char greyImage2D[BMP_WIDTH][BMP_HEIGTH];
unsigned char greyImage3D[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char array2D[BMP_WIDTH][BMP_HEIGTH];
unsigned char array3D[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
void ConvertToGrey(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      greyImage2D[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
    }
  }
}
void upscale2DTo3D(unsigned char array2D[BMP_WIDTH][BMP_HEIGTH], unsigned char array3D[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        array3D[x][y][c] = array2D[x][y];
      }
    }
  }
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void binaryThreshold(unsigned char greyImage2D[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
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
  upscale2DTo3D(greyImage2D, greyImage3D);

  // printarray(input_image);

  // Save image to file
  write_bitmap(output_image, argv[2]);
  write_bitmap(greyImage3D, argv[2]);
  printf("Done!\n");
  return 0;
}
