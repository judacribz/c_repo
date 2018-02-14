/*============================================================================*/
/*| Lab 3 Sudoku Project for SOFE 3950U / CSCI 3020U: Operating Systems      |*/
/*|                                                                          |*/
/*| sudoku.h                                                                 |*/
/*|                                                                          |*/
/*| Copyright (C) 2015, Jude Sheron Balasingam (100504990)                   |*/
/*| All rights reserved.                                                     |*/
/*============================================================================*/
#ifndef SUDOKU_H_
#define SUDOKU_H_

#define FILENAME  "puzzle.txt"      // Name of input text file
#define OUTFILE   "solution.txt"    // Name of file to print solution to
#define PUZZ_DIM  9                 // Puzzle dimension
#define GRID_DIM  3                 // Sub-grid dimension
#define GRID_AREA PUZZ_DIM*PUZZ_DIM // Area of whole puzzle



// Structure to store each value of sudoku grid.
// ----------------------------------------------------------------------------
typedef struct
{
    int value;
    int pos_val[PUZZ_DIM+1];
} sudoku_values;


// Initializing 9x9 sudoku grid to be used.
// ----------------------------------------------------------------------------
sudoku_values sudoku[PUZZ_DIM][PUZZ_DIM];

#endif /* SUDOKU_H_ */
