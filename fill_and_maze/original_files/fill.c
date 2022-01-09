/**
 * CS1010 Semester 1 AY21/22
 * Assignment 8: Fill
 *
 * @file: fill.c
 * @author: Wilson Oh (Group C04)
 */

#include "cs1010.h"
#include <stdbool.h>

void fill(long **pixels, long **fill_vals, long q, long m, long n);
void fill_colour(long **pixels, long **fill_vals, long *curr_colour, long op_no, long y, long x, long m, long n);
void update_pixel_colour(long **pixels, long **fill_vals, long y, long x, long fill_no);
bool is_same_colour(long **pixels, long *curr_colour, long y, long x);
long **read_fill_vals(size_t q);
void print_pixels(long **pixels, size_t m, size_t n);
long **read_pixels(size_t m, size_t n);

int main() {
  size_t m = cs1010_read_size_t();
  size_t n = cs1010_read_size_t();
  long **pixels = read_pixels(m , n);
  size_t q = cs1010_read_size_t();
  long **fill_vals = read_fill_vals(q);
  if (pixels == NULL || fill_vals == NULL){
    return 1;
  }
  fill(pixels, fill_vals, (long)q, (long)m, (long)n);
  print_pixels(pixels, m, n);
  for (size_t i = 0; i < m; i += 1){
    free(pixels[i]);
    if (i < q){
      free(fill_vals[i]);
    }
  }
  free(fill_vals);
  free(pixels);
}

/**
 * The main function which iterates through the array of fill operations and calls the recursive fill_colour function to fill a pixel segment
 *
 * @param[in] pixels The pixel array to be filled
 * @param[in] fill_vals The array which stores the starting position of the fill operation and the colour to be filled in
 * @param[in] q The number of fill operations to be carried out
 * @param[in] m The number of rows in the pixel array
 * @param[in] n The number of columns in the pixel array
 *
 */ 
void fill(long **pixels, long **fill_vals, long q, long m, long n){
  long curr_colour[3];
  for (long i = 0; i < q; i += 1){
    for (long j = 0; j < 3; j += 1){
      curr_colour[j] = pixels[fill_vals[i][0]][(fill_vals[i][1] * 3) + j]; //stores the original RGB values of the pixel segment to be filled in
    }
    fill_colour(pixels, fill_vals, curr_colour, i, fill_vals[i][0], (fill_vals[i][1] * 3), m, n); //calls the function to fill in the pixel segment starting at the coordinate stored in fill_vals
  }
}

/**
 * The main recursive function which fills an entire pixel segment with a fill colour
 *
 * @param[in] pixels The pixel array to be filled
 * @param[in] fill_vals The array which stores the starting position of the fill operation and the colour to be filled in
 * @param[in] curr_colour The array holding the RGB values of the original pixel segment
 * @param[in] op_no The number of the current fill operation
 * @param[in] y The row number of the current pixel to be filled
 * @param[in] x The column number of the current pixel to be filled
 * @param[in] m The number of rows in the pixel array
 * @param[in] n The number of columns in the pixel array
 *
 */
void fill_colour(long **pixels, long **fill_vals, long *curr_colour, long op_no, long y, long x, long m, long n){
  if (y != 0){ //check top adjacent pixel
    if (is_same_colour(pixels, curr_colour, y - 1, x)){
      update_pixel_colour(pixels, fill_vals, y, x, op_no);
      fill_colour(pixels, fill_vals, curr_colour, op_no, y - 1, x, m, n);
    }
  }
  if (x != 0){ //check left adjacent pixel
    if (is_same_colour(pixels, curr_colour, y, x - 3)){
      update_pixel_colour(pixels, fill_vals, y, x, op_no);
      fill_colour(pixels, fill_vals, curr_colour, op_no, y, x - 3, m, n);
    }
  }
  if (y != m - 1){ //check bottom adjacent pixel
    if (is_same_colour(pixels, curr_colour, y + 1, x)){
      update_pixel_colour(pixels, fill_vals, y, x, op_no);
      fill_colour(pixels, fill_vals, curr_colour, op_no, y + 1, x, m, n);
    }
  }
  if (x != (n * 3) - 3){ //check right adjacent pixel
    if (is_same_colour(pixels, curr_colour, y, x + 3)){
      update_pixel_colour(pixels, fill_vals, y, x, op_no);
      fill_colour(pixels, fill_vals, curr_colour, op_no, y, x + 3, m, n);
    }
  }
  update_pixel_colour(pixels, fill_vals, y, x, op_no); //update the colour of the current pixel if no other pixels are found in the same pixel segment
}

/**
 * Checks if the input pixel is the same colour as the original pixel segment
 *
 * @param[in] pixels The input pixel array
 * @param[in] curr_colour The array holding the RGB values of the original pixel segment
 * @param[in] y The row number of the pixel
 * @param[in] x The column number of the pixel
 *
 * @return Returns true if the RGB values matches those of the original pixel segment and returns false otherwise
 *
 */
bool is_same_colour(long **pixels, long *curr_colour, long y, long x){
  for (long i = 0; i < 3; i += 1){
    if (pixels[y][x + i] != curr_colour[i]){
      return false;
    }
  }
  return true;
}

/*
 * Updates the input pixel colour to the fill colour
 *
 * @param[out] The pixel array to be modified 
 * @param[in] fill_vals The array holding the fill RGB values 
 * @param[in] y The row number of the pixel
 * @param[in] x The column number of the pixel
 * @param[in] fill_no The number of the current fill operation
 *
 */
void update_pixel_colour(long **pixels, long **fill_vals, long y, long x, long fill_no){
  for (long i = 0; i < 3; i += 1){
    pixels[y][x + i] = fill_vals[fill_no][2 + i];
  }
}

/**
 * Reads in the 3mn integer values which form an RGB pixel array
 *
 * @param[in] m The number of rows of the pixel array
 * @param[in] n The number of columns of the pixel array
 *
 * @return Returns the pointer to the pixel array if successful, and NULL otherwise
 *
 */
long **read_pixels(size_t m, size_t n){
  long **pixels = malloc(sizeof(long *) * m);
  if (pixels == NULL){
    return NULL;
  }
  for (size_t i = 0; i < m; i += 1){
    pixels[i] = malloc(sizeof(long) * 3 * n);
    if (pixels[i] == NULL){
      for (size_t j = 0; j < i; j += 1){
        free(pixels[j]);
      }
      return NULL;
    }
  }
  for (size_t i = 0; i < m; i += 1){
    for (size_t j = 0; j < n * 3; j += 1){
      pixels[i][j] = cs1010_read_long();
    }
  }
  return pixels;
}

/**
 * Reads in the coordinates and RGB values of the fill operation into a 2D array
 * Only has to take in one input for the number of rows as the number of inputs per row is always 5
 * 
 * @param[in] q The number of fill operations to be carried out
 *
 * @return Returns the pointer to the array if successful, and NULL otherwise
 *
 */
long **read_fill_vals(size_t q){
  long **fill_vals = malloc(sizeof(long *) * q);
  if (fill_vals == NULL){
    return NULL;
  }
  for (size_t i = 0; i < q; i += 1){
    fill_vals[i] = malloc(sizeof(long) * 5);
    if (fill_vals[i] == NULL){
      for (size_t j = 0; j < i; j += 1){
        free(fill_vals[j]);
      }
      return NULL;
    }
  }
  for (size_t i = 0; i < q; i += 1){
    for (size_t j = 0; j < 5; j += 1){
      fill_vals[i][j] = cs1010_read_long();
    }
  }
  return fill_vals;
}

/**
 * Prints the pixel array to stdout in the format supported by viu 
 *
 * @param[in] pixels The pixel array to be printed out
 * @param[in] m The number of rows in the pixel array
 * @param[in] n The number of columns in the pixel array
 *
 */
void print_pixels(long **pixels, size_t m, size_t n){
  cs1010_print_string("P3 ");
  cs1010_print_long((long)n);
  putchar(' ');
  cs1010_print_long((long)m);
  putchar(' ');
  cs1010_println_long(255);
  for (size_t i = 0; i < m; i += 1){
    for (size_t j = 0; j < n * 3; j += 1){
      cs1010_print_long(pixels[i][j]);
      cs1010_print_string(" ");
      if ((j + 1) % 3 == 0){
        putchar('\n');
      }
    }
  }
}

