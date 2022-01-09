#include <stdio.h>
#include "cs1010.h"
#include <stdbool.h>

typedef struct {
  int red;
  int green;
  int blue;
  int x;
  int y;
}
Pixel;

void fill_pixels(long q, Pixel **pixels, size_t nrows, size_t ncols);
bool is_same_fill(Pixel **pixels, Pixel fill_pixel);
void update_color(Pixel **pixels, Pixel fill_pixel, int x, int y);
bool is_same_color(Pixel first, Pixel second);
void fill(Pixel **pixels, Pixel fill_pixel, Pixel curr, size_t nrows, size_t ncols);
void free_pixels(Pixel **pixels, size_t nrows);
void print_pixels(Pixel **pixels, size_t nrows, size_t ncols);
Pixel **read_pixels(size_t nrows, size_t ncols);

int main(void) {
  size_t nrows, ncols;
  scanf("%zu %zu", &nrows, &ncols);
  Pixel **pixels = read_pixels(nrows, ncols);
  if (pixels == NULL) {
    return 1;
  }
  long q;
  scanf("%ld", &q);
  fill_pixels(q, pixels, nrows, ncols);
  print_pixels(pixels, nrows, ncols);
  free_pixels(pixels, nrows);
}

void fill(Pixel **pixels, Pixel fill_pixel, Pixel curr, size_t nrows, size_t ncols) {
  int x = fill_pixel.x;
  int y = fill_pixel.y;

  if (y != 0) {
    if (is_same_color(curr, pixels[y - 1][x])) {
      update_color(pixels, fill_pixel, x, y);
      fill_pixel.y--;
      fill(pixels, fill_pixel, curr, nrows, ncols);
    }
  }
  if (x != 0) {
    if (is_same_color(curr, pixels[y][x - 1])) {
      update_color(pixels, fill_pixel, x, y);
      fill_pixel.x--;
      fill(pixels, fill_pixel, curr, nrows, ncols);
    }
  }
  if (y != (int)nrows - 1) {
    if (is_same_color(curr, pixels[y + 1][x])) {
      update_color(pixels, fill_pixel, x, y);
      fill_pixel.y++;
      fill(pixels, fill_pixel, curr, nrows, ncols);
    }
  }
  if (x != (int)ncols - 1) {
    if (is_same_color(curr, pixels[y][x + 1])) {
      update_color(pixels, fill_pixel, x, y);
      fill_pixel.x++;
      fill(pixels, fill_pixel, curr, nrows, ncols);
    }
  }
  update_color(pixels, fill_pixel, x, y);
}

bool is_same_color(Pixel first, Pixel second) {
  return (first.red == second.red &&
      first.green == second.green &&
      first.blue == second.blue);
  return false;
}

void update_color(Pixel **pixels, Pixel fill_pixel, int x, int y) {
  pixels[y][x].red = fill_pixel.red; 
  pixels[y][x].green = fill_pixel.green;
  pixels[y][x].blue = fill_pixel.blue;
}

bool is_same_fill(Pixel **pixels, Pixel fill_pixel) {
  return (pixels[fill_pixel.y][fill_pixel.x].red == fill_pixel.red &&  
      pixels[fill_pixel.y][fill_pixel.x].green == fill_pixel.green &&
      pixels[fill_pixel.y][fill_pixel.x].blue == fill_pixel.blue);
  return false;
}

void fill_pixels(long q, Pixel **pixels, size_t nrows, size_t ncols) {
  for (long i = 0; i < q; i++) {
    Pixel fill_pixel;
    scanf("%d %d %d %d %d", &fill_pixel.y, &fill_pixel.x, &fill_pixel.red, &fill_pixel.green, &fill_pixel.blue);
    Pixel curr = {
      .red = pixels[fill_pixel.y][fill_pixel.x].red,
      .green = pixels[fill_pixel.y][fill_pixel.x].green,
      .blue = pixels[fill_pixel.y][fill_pixel.x].blue
    };
    if (!is_same_fill(pixels, fill_pixel)) {
      fill(pixels, fill_pixel, curr, nrows, ncols);
    }
  }
}

void free_pixels(Pixel **pixels, size_t nrows) {
  for (size_t i = 0; i < nrows; i++) {
    free(pixels[i]);
  }
  free(pixels);
}

void print_pixels(Pixel **pixels, size_t nrows, size_t ncols) {
  printf("P3 %zu %zu 255\n", ncols, nrows);
  for (size_t i = 0; i < nrows; i++) {
    for (size_t j = 0; j < ncols; j++) {
      printf("%i %i %i\n", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
  }
}

Pixel **read_pixels(size_t nrows, size_t ncols) {
  Pixel **pixels = malloc(sizeof(Pixel) * nrows);
  if (pixels == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < nrows; i++) {
    pixels[i] = malloc(sizeof(Pixel) * ncols);
    if (pixels[i] == NULL) {
      for (size_t j = 0; j < i; j++) {
        free(pixels[j]);
      }
      free(pixels);
      return NULL;
    }
  }
  for (size_t i = 0; i < nrows; i++) {
    for (size_t j = 0; j < ncols; j++) {
      scanf("%d %d %d", &pixels[i][j].red, &pixels[i][j].green, &pixels[i][j].blue);
    }
  }
  return pixels;
}
