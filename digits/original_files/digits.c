/**
 * CS1010 Semester 1 AY21/22
 * Assignment 9: Digits
 *
 * @file: digits.c
 * @author: Wilson Oh (Group C04)
 */

#include "cs1010.h"
#include <stdbool.h>
#include <limits.h>

#define NCOLS 28
#define NROWS 28
#define WHITE '.'
#define BLACK '#'
#define K 5

typedef struct {
  char **ar;
  int label;
} digit_t;

typedef struct {
  digit_t digit;
  long dist;
} neighbor_t;

typedef struct {
  int count;
  long dist;
} num_index_t;

int find_majority(neighbor_t *k_neighbors);
int find_k_smallest(neighbor_t *neighbors, size_t ntrain);
void free_digits(digit_t digit);
long read_digits(digit_t *digit);
digit_t *initialise_digits(size_t n);
long digits_diff(digit_t *train_digit, digit_t *test_digit);
void k_nearest(neighbor_t *neighbors, digit_t *testing, size_t ntrain, size_t ntest);
neighbor_t *initialise_neighbors(size_t ntrain);
void free_neighbor_ar(neighbor_t *neighbors, size_t ntrain);
void free_digit_ar(digit_t *digits, size_t num);

int main() {
  size_t ntrain = cs1010_read_size_t();
  neighbor_t *neighbors = initialise_neighbors(ntrain);
  size_t ntest = cs1010_read_size_t();
  digit_t *testing = initialise_digits(ntest);
  k_nearest(neighbors, testing, ntrain, ntest);
  free_neighbor_ar(neighbors, ntrain);
  free_digit_ar(testing, ntest);
}

/**
 * Main function which tests each test sample against all the training samples
 *
 * @param[in] neighbors The neighbor_t array containing data of the training samples
 * @param[in] testing The digit_t array containing data of the test samples
 * @param[in] ntrain The number of training samples
 * @param[in] ntest The number of test samples
 */
void k_nearest(neighbor_t *neighbors, digit_t *testing, size_t ntrain, size_t ntest) {
  int match;
  int count = 0;
  for (size_t i = 0; i < ntest; i += 1) {
    for (size_t j = 0; j < ntrain; j += 1) {
      neighbors[j].dist = digits_diff(&neighbors[j].digit, &testing[i]); //stores the distance of each neighbor from the test sample 
    }                                                                      //into the dist member                                                     
    match = find_k_smallest(neighbors, ntrain);
    if (match == -1) {
      return;
    }
    cs1010_print_long(testing[i].label);
    putchar(' ');
    cs1010_println_long(match);
    if (match == testing[i].label) {
      count += 1; //increment counter if the nearest digit detected matches the test label
    }
  }
  cs1010_println_double(((float)count/ntest) * 100); 
}

/**
 * Given an array of neighbors containing the distance data, the function will find the majority neighbor with the smallest distance
 *
 * @return Returns the label of the majority neighbor with the smallest distance
 */
int find_k_smallest(neighbor_t *neighbors, size_t ntrain) {
  neighbor_t k_neighbors[K];
  bool *deselect_neighbors = calloc(ntrain, sizeof(bool)); 
  if (deselect_neighbors == NULL) {
    return -1;
  }
  long smallest = LONG_MAX;
  size_t x = 0;
  for (size_t i = 0; i < K; i += 1) { //kinda janky way to find K smallest elements from an array
    for (size_t j = 0; j < ntrain; j += 1) {
      if (smallest > neighbors[j].dist && !deselect_neighbors[j]) { //only reassign smallest when the index on the bool array is false
        smallest = neighbors[j].dist;
        x = j;
      }
      if ((smallest == neighbors[j].dist && !deselect_neighbors[j]) && (neighbors[j].digit.label < neighbors[x].digit.label)) { 
        x = j; //tiebreaker for if 2 nearest neighbors have the same distance. Break tie by taking the neighbor with a smaller label value    
      }
    }
    k_neighbors[i] = neighbors[x]; //store the neighbor with the shortest distance into an array after each loop
    deselect_neighbors[x] = true; //switch the flag on the bool array so that the same neighbor won't be stored more than once
    smallest = LONG_MAX; //reset value of smallest
  }
  int majority = find_majority(k_neighbors);
  free(deselect_neighbors);
  return majority;
}

/**
 * Finds the majority label in an array of K neighbors
 *
 * @return Returns the label that is in the majority
 */
int find_majority(neighbor_t *k_neighbors) {
  num_index_t num_index[10]; //index to store the count of each label plus it's associated distance
  for (int i = 0; i < 10; i += 1) {
    num_index[i].count = 0;
    num_index[i].dist = LONG_MAX; //initialise all dist values so that there won't be any
  }                               //memory corruption going on in the if-block below    
  for (int i = 0; i < K; i += 1) {
    num_index[k_neighbors[i].digit.label].count += 1;
    if (k_neighbors[i].dist < num_index[k_neighbors[i].digit.label].dist) { //only store the distance if it is a smaller value than the previously stored neighbor
      num_index[k_neighbors[i].digit.label].dist = k_neighbors[i].dist;     
    }
  }
  int largest_count = INT_MIN; //stores the largest_count number of occurrences of any label in K nearest neighbors
  long smallest_dist = 0;
  int maj_num = 0;
  for (int i = 0; i < 10; i += 1) { //another scuffed way to find the majority label from and array of K neighbors
    if (num_index[i].count != 0) {
      if (largest_count < num_index[i].count) {
        largest_count = num_index[i].count;
        smallest_dist = num_index[i].dist; //store the distance in the index so that we can decide who to choose in a tiebreaker
        maj_num = i; //store current index number as the majority label
      }
      else if (largest_count == num_index[i].count) { //first tiebreaker condition: 2 labels have the same count
        if (smallest_dist > num_index[i].dist) { //only update largest_count if the associated distance of the label is smaller than largest_count_dist
          largest_count = num_index[i].count;
          smallest_dist = num_index[i].dist;
          maj_num = i;
        }
        else if (smallest_dist == num_index[i].dist && maj_num > i) { //second tiebreaker: 2 labels have same count and same distance
          //break tie by taking the smaller label
          maj_num = i;
          largest_count = num_index[i].count;
          smallest_dist = num_index[i].dist;
        }
      }
    }
  }
  return maj_num;
}

/**
 * Scans through the digit array to count the number of pixels that are different between
 * the training sample and the test sample
 *
 * @return Returns the distance between the training sample digit and the test digit
 */
long digits_diff(digit_t *train_digit, digit_t *test_digit) { //pass the digits in by reference
  long diff_count = 0;
  for (long i = 0; i < NROWS; i += 1) {
    for (long j = 0; j < NCOLS; j += 1) {
      if (train_digit -> ar[i][j] != test_digit -> ar[i][j]) {
        diff_count += 1;
      }
    }
  }
  return diff_count;
}

/*
 * @brief Frees an array of digits
 */
void free_digit_ar(digit_t *digits, size_t num) {
  for (size_t i = 0; i < num; i += 1) {
    free_digits(digits[i]);
  }
  free(digits);
}

/**
 * Initializes ntrain number of neighbors and initializes it's digit to that of the training samples
 *
 * @param[in] ntrain the number of training samples
 *
 * @return Returns a pointer to the neighbor_t array initialized, and NULL if there is failure to allocate memory
 */
neighbor_t *initialise_neighbors(size_t ntrain) {
  neighbor_t *neighbors = malloc(sizeof(neighbor_t) * ntrain);
  if (neighbors == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < ntrain; i += 1) {
    long err = read_digits(&neighbors[i].digit);
    if (err == 1) {
      for (size_t j = 0; j < i; j += 1) {
        free_digits(neighbors[j].digit);
      }
      free(neighbors);
      return NULL;
    }
  }
  return neighbors;
}

/**
 * @brief Initialises an array of digits and the label and 2D char array associated with it
 */
digit_t *initialise_digits(size_t n) {
  digit_t *digits = malloc(sizeof(digit_t) * n);
  if (digits == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < n; i += 1) {
    long err = read_digits(&digits[i]); //have to pass in each digit by reference if not only a copy of the digit
    if (err == 1) {                     //will be passed into the function and the actual digit won't be updated
      for (size_t j = 0; j < i; j += 1) {
        free_digits(digits[j]);
      }
      free(digits);
      return NULL;
    }
  }
  return digits;
}

/**
 * @brief Frees a single digit and the 2D array associated with it
 */
void free_digits(digit_t digit) {
  for (long i = 0; i < NROWS; i += 1) {
    free(digit.ar[i]);
  }
  free(digit.ar);
}
/**
 * @brief Same thing as the free_digits function but customized for neighbor_t arrays
 */
void free_neighbor_ar(neighbor_t *neighbors, size_t ntrain) {
  for (size_t i = 0; i < ntrain; i += 1) {
    free_digits(neighbors[i].digit);
  }
  free(neighbors);
}

/**
 * @brief Initialises and reads in values for the label member and 2D char array member of a digit_t element
 *
 * @return Returns 1 if there is failure to allocate memory, and 0 if no errors occur
 */
long read_digits(digit_t *digit) {
  digit -> label = (int)cs1010_read_long();  
  digit -> ar = malloc(sizeof(char *) * NROWS);
  if (digit -> ar == NULL) {
    return 1;
  }
  for (size_t j = 0; j < NROWS; j += 1) {
    digit -> ar[j] = cs1010_read_word();
    if (digit -> ar[j] == NULL) {
      for (size_t k = 0; k < j; k += 1) {
        free(digit -> ar[k]);
      }
      free(digit -> ar);
      return 1;
    }
  }
  return 0;
}
