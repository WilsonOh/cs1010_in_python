/**
 * CS1010 Semester 1 AY21/22
 * Assignment 8: Maze
 *
 * @file: maze.c
 * @author: Wilson Oh (Group C04)
 */

#include "cs1010.h"
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define EMPTY '.'
#define WALL  '#'
#define USER  '@'

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

bool goto_empty(char **maze, char **temp, long prev_row, long prev_col, long curr_row, long curr_col, long nrows, long ncols, long *counter);
void move(char **maze, char **temp, long row0, long col0, long row1, long col1, long *counter, long nrows);
void find_scully(char **maze, long nrows, long ncols, long *scully_row, long *scully_col);
void solve_maze(char **maze, char **temp, long nrows, long ncols, long *counter);
char **read_maze(long nrows);
char **create_temp_maze(char **maze, long nrows, long ncols);
void print_maze(char **maze, long nrows, long steps);
void print_maze_row_with_color(char *maze_row);

int main() {
  long nrows = cs1010_read_long();
  long ncols = cs1010_read_long();
  char **maze = read_maze(nrows);
  char **temp = create_temp_maze(maze, nrows, ncols);
  if (maze == NULL || temp == NULL){
    return 1;
  }
  long counter = 0;
  print_maze(maze, nrows, counter);
  solve_maze(maze, temp, nrows, ncols, &counter); //pass the counter by reference
  for (long i = 0; i < nrows; i += 1){
    free(maze[i]);
    free(temp[i]);
  }
  free(maze);
  free(temp);
}

/**
 * The main function which calls the recursive function goto_empty so as to get scully out of the maze
 *
 * @param[in, out] maze The 2D array representing the maze
 * @param[in, out] temp The 2D array storing the path that scully has already taken
 * @param[in] nrows The number of rows in the 2D array
 * @param[in] ncols The number of columns in the 2D array
 * @param[out] counter The counter for the number of times scully moved
 *
 */
void solve_maze(char **maze, char **temp, long nrows, long ncols, long *counter){
  long scully_row;
  long scully_col;
  find_scully(maze, nrows, ncols, &scully_row, &scully_col); //pass scully row and col by reference
  goto_empty(maze, temp, -1, -1, scully_row, scully_col, nrows, ncols, counter);
}

/**
 * Scans the maze array to find scully
 *
 * @param[in] maze The 2D array representing the maze
 * @param[in] nrows The number of rows in the 2D array
 * @param[in] ncols The number of columns in the 2D array
 * @param[out] scully_row The row number containing scully
 * @param[out] scully_col The column number containing scully
 *
 */
void find_scully(char **maze, long nrows, long ncols, long *scully_row, long *scully_col){
  for (long i = 0; i < nrows; i += 1){
    for (long j = 0; j < ncols; j += 1){
      if (maze[i][j] == USER){
        *scully_row = i;
        *scully_col = j;
        return; //stop searching as soon as scully is found
      }
    }
  }
}

/**
 * Moves scully from one cell to another in the maze array
 *
 * @param[out] maze The 2D array representing the maze
 * @param[in, out] temp The 2D array storing the path that scully has already taken
 * @param[in] row0 The current row scully is in
 * @param[in] col0 The current column scully is in
 * @param[in] row1 The row number of the cell scully will be moving to
 * @param[in] col1 The column number of the cell scully will be moving to
 * @param[in, out] counter The counter for the number of moves scully has made
 * @param[in] nrows The number of rows of the maze array
 *
 */
void move(char **maze, char **temp, long row0, long col0, long row1, long col1, long *counter, long nrows){
  *counter += 1;
  maze[row0][col0] = EMPTY;
  maze[row1][col1] = USER;
  temp[row1][col1] = WALL; //places a wall in the path taken in the temp array so that scully won't take the same path twice
  print_maze(maze, nrows, *counter);
}

/**
 * 
 * @param[out] maze The 2D array representing the maze
 * @param[in, out] temp The 2D array storing the path that scully has already taken
 * @param[in] prev_row The row number scully was previously at
 * @param[in] prev_col The column number scully was previously at
 * @param[in] curr_row The row number scully is currently at
 * @param[in] curr_col The column number scully is currently at
 * @param[in] nrows The number of rows in the 2D array
 * @param[in] ncols The number of columns in the 2D array
 * @param[in, out] counter The counter for the number of moves scully has made
 *
 * @return Returns true if scully reaches a border cell, and false if she cannot find her way out of the maze
 *
 */
bool goto_empty(char **maze, char **temp, long prev_row, long prev_col, long curr_row, long curr_col, long nrows, long ncols, long *counter){
  if (curr_row == 0 || curr_row == nrows - 1 || curr_col == 0 || curr_col == ncols - 1){
    return true; //if scully reaches a border cell
  }
  //check if the adjacent cells are empty AND if scully has already visited that cell before by checking if that cell is filled in the temp array
  if ((maze[curr_row - 1][curr_col] == EMPTY) && (temp[curr_row - 1][curr_col] == EMPTY)){ //check top adjacent cell
    move(maze, temp, curr_row, curr_col, curr_row - 1, curr_col, counter, nrows);
    if (goto_empty(maze, temp, curr_row, curr_col, curr_row - 1, curr_col, nrows, ncols, counter)){
      return true;
    }
  }
  if ((maze[curr_row][curr_col + 1] == EMPTY) && (temp[curr_row][curr_col + 1] == EMPTY)){ //check right adjacent cell
    move(maze, temp, curr_row, curr_col, curr_row, curr_col + 1, counter, nrows);
    if (goto_empty(maze, temp, curr_row, curr_col, curr_row, curr_col + 1, nrows, ncols, counter)){
      return true;
    }
  }
  if ((maze[curr_row + 1][curr_col] == EMPTY) && (temp[curr_row + 1][curr_col] == EMPTY)){ //check bottom adjacent cell
    move(maze, temp, curr_row, curr_col, curr_row + 1, curr_col, counter, nrows);
    if (goto_empty(maze, temp, curr_row, curr_col, curr_row + 1, curr_col, nrows, ncols, counter)){
      return true;
    }
  }
  if ((maze[curr_row][curr_col - 1] == EMPTY) && (temp[curr_row][curr_col - 1] == EMPTY)){ //check left adjacent cell
    move(maze, temp, curr_row, curr_col, curr_row, curr_col - 1, counter, nrows);
    if (goto_empty(maze, temp, curr_row, curr_col, curr_row, curr_col - 1, nrows, ncols, counter)){
      return true;
    }
  }
  //if no empty cells, backtrack
  if (prev_col != -1){ //don't backtrack if the current stackframe is the inital stackframe
    move(maze, temp, curr_row, curr_col, prev_row, prev_col, counter, nrows);
  }
  return false;
}

/**
 * Creates a 2D array the same size as the maze and copies the maze over
 *
 * @param[in] maze The 2D array presenting the maze
 * @param[in] nrows The number of rows in the 2D array
 * @param[in] ncols The number of columns in the 2D array
 *
 * @return Returns the pointer to the temp array created if successful, and NULL otherwise
 *
 */
char **create_temp_maze(char **maze, long nrows, long ncols){
  char **temp = malloc(sizeof(char *) * (size_t)nrows);
  if (temp == NULL){
    return NULL;
  }
  for (long i = 0; i < nrows; i += 1){
    temp[i] = malloc(sizeof(char) * (size_t)ncols + 1);
    if (temp[i] == NULL){
      for (long j = 0; j < i; j += 1){
        free(temp[j]);
      }
      return NULL;
    }
  }
  for (long i = 0; i < nrows; i += 1){
    for (long j = 0; j < ncols + 1; j += 1){
      temp[i][j] = maze[i][j];
    }
  }
  return temp;
}

/**
 * Allocates the memory for the maze and reads in the characters forming the maze
 *
 * @param[in] nrows The number of rows of the maze
 *
 * @return Returns the pointer to the maze array if successful, and NULL otherwise
 *
 */
char **read_maze(long nrows){
  char **maze = malloc(sizeof(char *) * (size_t)nrows);
  if (maze == NULL){
    return NULL;
  }
  for (long i = 0; i < nrows; i += 1){
    maze[i] = cs1010_read_word();
    if (maze[i] == NULL){
      for (long j = 0; j < i; j += 1){
        free(maze[j]);
      }
      return NULL;
    }
  }
  return maze;
}


/**
 * Print one row of the maze to the screen (with colors)
 * 
 * @param[in] maze_row The 1D array representing a row of maze.
 */
void print_maze_row_with_color(char *maze_row) {
  long l = (long)strlen(maze_row); 
  for (long j = 0; j < l; j++) {
    if (maze_row[j] == EMPTY) {
      cs1010_print_string(BLUE);
    } else if (maze_row[j] == USER) {
      cs1010_print_string(RED);
    } else {
      cs1010_print_string(YELLOW);
    }
    putchar(maze_row[j]);
  }
  cs1010_println_string("");
  cs1010_print_string(RESET);
}

/**
 * Print the maze to the screen.
 * 
 * @param[in] maze The 2D array representing the maze.
 * @param[in] nrows The number of rows
 * @param[in] steps The number of steps taken so far.
 */
void print_maze(char **maze, long nrows, long steps) {
  if (isatty(fileno(stdout))) {
    cs1010_clear_screen();
  }
  for (long i = 0; i < nrows; i += 1) {
    if (!isatty(fileno(stdout))) {
      cs1010_println_string(maze[i]);
    } else {
      print_maze_row_with_color(maze[i]);
    }
  }
  cs1010_println_long(steps);
  if (isatty(fileno(stdout))) {
    usleep(100*1000);
  }
}
