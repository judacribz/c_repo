/*============================================================================*/
/*| Lab 3 Sudoku Project for SOFE 3950U / CSCI 3020U: Operating Systems      |*/
/*|                                                                          |*/
/*| sudoku_check.h                                                           |*/
/*|                                                                          |*/
/*| Copyright (C) 2015, Jude Sheron Balasingam (100504990)                   |*/
/*| All rights reserved.                                                     |*/
/*============================================================================*/
#ifndef SUDOKU_CHECK_H_
#define SUDOKU_CHECK_H_


#include <stdbool.h>



// Structure to pass in to sudoku_check function for the starting point of
// check.
// ----------------------------------------------------------------------------
typedef struct
{
    int row;
    int column;
} parameters;


extern bool has_empty_spots(void);


extern void *check_row(void *arg);


extern void *check_column(void *arg);


extern void *check_grid(void *arg);


extern void check_puzzle(void);

#endif /* SUDOKU_CHECK_H_ */
