#ifndef CBMP_CBMP_H
#define CBMP_CBMP_H

#define BMP_WIDTH 950
#define BMP_HEIGHT 950
#define BMP_CHANNELS 3

// Public function declarations
void read_bitmap(char *input_file_path, unsigned char output_image_array[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS]);
void write_bitmap(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS], char *output_file_path);
void write_bitmap2(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGHT], char *output_file_path);
#endif // CBMP_CBMP_H
