// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <unistd.h>

int cellLocations[BMP_HEIGHT][2];
unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
unsigned char arrayA[BMP_WIDTH][BMP_HEIGHT];
unsigned char arrayB[BMP_WIDTH][BMP_HEIGHT];
int wasEroded = 0;
int *ptr = &wasEroded;
int cells = 0;

void ConvertToGrey(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      arrayA[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
    }
  }
}

void binaryThreshold(unsigned char arrayA[BMP_WIDTH][BMP_HEIGHT])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      if (arrayA[x][y] <= 90)
      {
        arrayA[x][y] = 0;
      }
      else
      {
        arrayA[x][y] = 255;
      }
    }
  }
}

void erode(unsigned char arrayA[BMP_WIDTH][BMP_HEIGHT], unsigned char arrayB[BMP_WIDTH][BMP_HEIGHT])
{
  *ptr = 0;
  for (int x = 1; x < BMP_WIDTH - 1; x++)
  {
    for (int y = 1; y < BMP_HEIGHT - 1; y++)
    {
      if (arrayA[x - 1][y] == 0 || arrayA[x + 1][y] == 0 || arrayA[x][y - 1] == 0 || arrayA[x][y + 1] == 0 || arrayA[x + 1][y + 1] == 0 || arrayA[x + 1][y - 1] == 0 || arrayA[x - 1][y + 1] == 0 || arrayA[x - 1][y - 1] == 0)
      {
        arrayB[x][y] = 0;
      }
      else
      {
        arrayB[x][y] = 255;
        *ptr = 1;
      }
    }
  }
}
void detect(unsigned char arrayA[BMP_WIDTH][BMP_HEIGHT])
{
  for (int x = 0; x < BMP_WIDTH - 10; x++) // every celle run through
  {
    for (int y = 0; y < BMP_HEIGHT - 10; y++)
    {
      int blackBorder = 1;
      int containsWhite = 0;

      for (int i = x; i < x + 9; i++) // border check
      {
        if (arrayA[i][y] == 255 || arrayA[i][y + 8] == 255)
        {
          blackBorder = 0;
          break;
        }
      }
      for (int i = y + 1; i < y + 9; i++)
      {
        if (arrayA[x][i] == 255 || arrayA[x + 8][i] == 255)
        {
          blackBorder = 0;
          break;
        }
      }
      if (blackBorder == 1) // celle detection
      {
        for (int i = x + 1; i < x + 8; i++)
        {
          for (int j = y + 1; j < y + 8; j++)
          {
            if (arrayA[i][j] == 255)
            {
              containsWhite = 1;
              cellLocations[cells][0] = x + 6;
              cellLocations[cells][1] = y + 6;
              cells++;
              break;
            }
          }
          if (containsWhite)
          {
            break;
          }
        }
      }
      if (containsWhite == 1)
      {
        for (int i = x + 1; i < x + 8; i++)
        {
          for (int j = y + 1; j < y + 8; j++)
          {
            arrayA[i][j] = 0;
          }
        }
      }
    }
  }
}

void markCells(int cellLocations[BMP_WIDTH][2], int cells)
{
  printf("Cell Coordinates:\n");
  for (int c = 0; c < cells; c++)
  {

    int a = cellLocations[c][0];
    int b = cellLocations[c][1];
    printf("Cell %i, at (%i, %i)\n", c + 1, a, b);
    for (int j = a - 7; j < a + 7; j++)
    {
      for (int k = b - 7; k < b + 7; k++)
      {
        input_image[j][k][0] = 255;
        input_image[j][k][1] = 0;
        input_image[j][k][2] = 0;
      }
    }
  }
}
void deleteBoarder(unsigned char arrayB[BMP_WIDTH][BMP_HEIGHT])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    arrayB[x][0] = 0;
    arrayB[x][BMP_WIDTH - 1] = 0;
    arrayB[0][x] = 0;
    arrayB[BMP_HEIGHT - 1][x] = 0;
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
  ConvertToGrey(input_image);
  binaryThreshold(arrayA);
  deleteBoarder(arrayA);
  erode(arrayA, arrayB);
  int i = 0;

  while (*ptr != 0){
    if (i % 2 == 0)
    {
      erode(arrayB, arrayA);
      detect(arrayA);
    }
    else
    {
      erode(arrayA, arrayB);
      detect(arrayB);
    }
  i++;
  }
  markCells(cellLocations,cells);
  write_bitmap(input_image, argv[2]);
  printf("Celler fundet: %i \n", cells);
  printf("Done!\n");
  return 0;
}
