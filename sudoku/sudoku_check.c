/*============================================================================*/
/*| Lab 3 Sudoku Project for SOFE 3950U / CSCI 3020U: Operating Systems      |*/
/*|                                                                          |*/
/*| sudoku_check.c                                                           |*/
/*|                                                                          |*/
/*| Copyright (C) 2015, Jude Sheron Balasingam (100504990)                   |*/
/*| All rights reserved.                                                     |*/
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "sudoku.h"
#include "sudoku_check.h"


bool correct_sol = true; // Represnts if the solution is correct or not.


// Used to check if there are any empty spots in the puzzle.
// ----------------------------------------------------------------------------
bool has_empty_spots(void)
{
    bool has_empty = false;

    for (int i = 0; i < PUZZ_DIM; i++)
    {
        for (int j = 0; j < PUZZ_DIM; j++)
        {
            if (sudoku[i][j].value == 0)
            {
                has_empty = true;
                break;
            }
        }
    }
    
    return has_empty;    
}

// Used to check if a row is filled out completely and correctly.
// ----------------------------------------------------------------------------
void *check_row(void *args)
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
                data = args;

    int val              = 0;
    int row              = data->row;
    int col              = data->column;
    int values[PUZZ_DIM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (int j = col; j < PUZZ_DIM; j++)
    {
        val = sudoku[row][j].value;

        if (val == 0)
        {
            printf("Puzzle not filled in at row %i, col %i.\n", row, j);
            correct_sol = false;
        }
        else if (values[val-1] == -1)
        {
            values[val-1] = val;
        }
        else
        {
            printf("Number, %i, appears more than once in row %i.\n",
                   val, row);
            correct_sol = false; 
        }
    }

    return NULL;
}


// Used to check if a column is filled out completely and correctly.
// ----------------------------------------------------------------------------
void *check_column(void *args)
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
                data = args;

    int val              = 0;
    int row              = data->row;
    int col              = data->column;
    int values[PUZZ_DIM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (int i = row; i < PUZZ_DIM; i++)
    {
        val = sudoku[i][col].value;

        if (val == 0)
        {
            printf("Puzzle not filled in at row %i, col %i.\n", i, col);
            correct_sol = false;
        }
        else if (values[val-1] == -1)
        {
            values[val-1] = val;
        }
        else
        {
            printf("Number, %i, appears more than once in column %i.\n",
                   val, col);
            correct_sol = false;
        }
    }

    return NULL;
}


// Used to check if a sub-grid is filled out completely and correctly.
// ----------------------------------------------------------------------------
void *check_grid(void *args)
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
                data = args;

    int val              = 0;
    int row              = data->row;
    int col              = data->column;
    int values[PUZZ_DIM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (int i = row; i < row+3; i++)
    {
        for (int j = col; j < col+3; j++)
        {
            val = sudoku[i][j].value;

            if (val == 0)
            {
                printf("Puzzle not filled in at row %i, col %i.\n", i, j);
                correct_sol = false;
            }   
            else if (values[val-1] == -1)
            {
                values[val-1] = val;
            }
            else
            {   
                printf("Number, %i, appears more than once in sub-grid which "
                       "contains row %i and col %i.\n", val, row, col);
                correct_sol = false;
            }
        }
    }

    return NULL;
}


// Checks if the solution to the sudoku puzzle is correct.
// ---------------------------------------------------------------------------- 
void check_puzzle()
{
    parameters *row_data, *col_data, *grid_data;
    pthread_t   pth[PUZZ_DIM*3];

    for (int k = 0; k < PUZZ_DIM; k++)
    {
        row_data     = malloc(sizeof(parameters));
        row_data->row    = k;
        row_data->column = 0;

        col_data     = malloc(sizeof(parameters));
        col_data->row    = 0;
        col_data->column = k;

        grid_data     = malloc(sizeof(parameters));
        grid_data->row    = (k / 3) * 3;
        grid_data->column = (k % 3) * 3;
        
        pthread_create(&pth[k], 0, check_row, (void *) row_data);
        pthread_create(&pth[PUZZ_DIM+k], 0, check_column,
                       (void *) col_data);
        pthread_create(&pth[2*PUZZ_DIM+k], 0, check_grid,
                       (void *) grid_data);
    }
    
    // Waits for the threads that were created to finish.
    for (int k = 0; k < PUZZ_DIM*3; k++)
    {
        pthread_join(pth[k], 0);
    }

    if (correct_sol)
    {
        printf("Sudoku solution is correct!\n");
    }
}
