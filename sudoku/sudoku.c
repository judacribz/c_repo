/*============================================================================*/
/*| Lab 3 Sudoku Project for SOFE 3950U / CSCI 3020U: Operating Systems      |*/
/*|                                                                          |*/
/*| sudoku.c                                                                 |*/
/*|                                                                          |*/
/*| Copyright (C) 2015, Jude Sheron Balasingam (100504990)                   |*/
/*| All rights reserved.                                                     |*/
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "sudoku_check.h"
#include "sudoku.h"


// Fills in the empty spot with the value provided. If no value is provided,
// then the next spot to be filled is found and filled.
// ----------------------------------------------------------------------------
void fill_next_spot(int value, parameters *data)
{
    int sum = 0;

    if (value == 0)
    {
        for (int i = 0; i < PUZZ_DIM; i++)
        {
            for (int j = 0; j < PUZZ_DIM; j++)
            {
                sum = sudoku[i][j].pos_val[0];

                if (sum / 10 == 0 && sudoku[i][j].value == 0)
                {
                    if (sum == sudoku[i][j].pos_val[sum])
                    {
                        sudoku[i][j].value = sum;
                    }
                }
            }
        }    
    }
    else
    {
        sudoku[data->row][data->column].value = value;
    }
}


// Updates the possible values of an empty spot.
// ----------------------------------------------------------------------------
void update_poss_val(int value, parameters *data)
{
    int  row = data->row;
    int  col = data->column;
    int *val = &sudoku[row][col].pos_val[value];
    int *sum = &sudoku[row][col].pos_val[0];

    // Subtracts number in row from possible values and total. Makes sure the
    // the same value was not subtracted already before.
    if (*val != 0)
    {
        *val  = 0;
        *sum -= value;
    }

    // If only one possble number remains, that number becomes the answer for
    // the current position.
    if (*sum / 10 == 0)
    {
        if (   *sum == sudoku[row][col].pos_val[*sum]
            &&  sudoku[row][col].value == 0)
        {
            fill_next_spot(*sum, data);
        }
    }
}


// Sets up the position being checked to all possible values based on other
// filled in values in the same row.
// ----------------------------------------------------------------------------
void *row_compare(void *args)
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
                data = args;

    int row = data->row;
    int col = data->column;

    for (int j = 0; j < PUZZ_DIM; j++)
    {
        // If a non zero-number exists in the same row, it needs to be removed
        // from the possible values of the blank field we are checking.
        if (sudoku[row][j].value != 0 && col != j)
        {
            update_poss_val(sudoku[row][j].value, data);
        }
    }            

    return NULL;
}


// Sets up the position being checked to all possible values based on other
// filled in values in the same column.
// ----------------------------------------------------------------------------
void *column_compare(void *args)
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
                data = args;

    int row = data->row;
    int col = data->column;

    for (int i = 0; i < PUZZ_DIM; i++)
    {
        // If a non zero-number exists in the same column, it needs to be
        // removed from the possible values of the blank field we are checking.
        if (sudoku[i][col].value != 0 && row != i)
        {
            update_poss_val(sudoku[i][col].value, data);
        }
    }

    return NULL;
}


// Sets up the position being checked to all possible values based on other
// filled in values in the same sub-grid.
// ----------------------------------------------------------------------------
void *grid_compare(void *args)
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
                data = args;

    int row      = data->row;
    int col      = data->column;
    int row_mult = row / GRID_DIM; // Assists in finding row location
    int col_mult = col / GRID_DIM; // Assists in finding column location

    for (int i = GRID_DIM * row_mult; i < GRID_DIM * (row_mult + 1); i++)
    {
        for (int j = GRID_DIM * col_mult; j < GRID_DIM * (col_mult + 1); j++)
        {
            // If a non zero-number exists in the same sub-grid, it needs to be
            // removed from the possible values of the blank field we are
            // checking.
            if (sudoku[i][j].value != 0 && (row != i || col != j))
            {
                update_poss_val(sudoku[i][j].value, data);
            }
        }
    }

    return NULL;
}


// Runs 3 threads for each empty spot on in the puzzle. 3 threads are used for
// each empty spot because we need to determine possible values for the spot
// based on filled in spots in the same row, same column, and same sub-grid.
// ----------------------------------------------------------------------------
void fill_puzzle(void)
{
    parameters *data;
    pthread_t pth[PUZZ_DIM*3];
    int check[PUZZ_DIM];       // Used as a flag for threads created

    for (int i = 0; i < PUZZ_DIM; i++)
    {
        // Maximum of 27 threads would be running at the same time.
        for (int j = 0; j < PUZZ_DIM; j++)
        {
            // Threads are only created if the current spot is empty.
            if (sudoku[i][j].value == 0)
            {
                check[j]     = 1;
                data         = malloc(sizeof(parameters));
                data->row    = i;
                data->column = j;

                pthread_create(&pth[j], 0, row_compare, (void *) data);
                pthread_create(&pth[PUZZ_DIM+j], 0, column_compare,
                               (void *) data);
                pthread_create(&pth[2*PUZZ_DIM+j], 0, grid_compare,
                               (void *) data); 
            }
            else
            {
                check[j] = 0;
            }
        }

        // Waits for the threads that were created to finish.
        for (int k = 0; k < PUZZ_DIM; k++)
        {
            if (check[k])
            {
                pthread_join(pth[k], 0);
                pthread_join(pth[PUZZ_DIM+k], 0);
                pthread_join(pth[2*PUZZ_DIM+k], 0);
            }
        }
    }
}


// Loads puzzle into 2-d array from text file. Sets the possible values for the
// empty spots as {45, 1, 2, 3, 4, 5, 6, 7, 8, 9}. The first position, [0],
// represents the total of the possible values. 
// ----------------------------------------------------------------------------
void load_puzzle(char *file)
{
    FILE *fp;
    // Possible values for empty spots at initialization. [0] position is the
    // total of all possible values for the empty spot.
    int POSS_VAL[PUZZ_DIM+1] = {45, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open %s.\n", file);
        exit(EXIT_FAILURE);
    }
    
    while (!feof(fp))
    {
        for (int i = 0; i < PUZZ_DIM; i++)
        {
            for (int j = 0; j < PUZZ_DIM; j++)
            {
                fscanf(fp, "%i", &sudoku[i][j].value);

                // Initializes the possible values for each empty spot.
                if (sudoku[i][j].value == 0)
                {
                    memcpy(sudoku[i][j].pos_val, POSS_VAL,
                           sizeof(sudoku[i][j].pos_val));
                }
            }
        }
    }

    fclose(fp);
}


// Function that prints out the current values of the puzzle.
// ----------------------------------------------------------------------------
void fprint_puzzle(char *file)
{
    FILE *fp;
    fp = fopen(file, "w");

    for (int i = 0; i < PUZZ_DIM; i++)
    {
        for (int j = 0; j < PUZZ_DIM; j++)
        {
            fprintf(fp, "%i", sudoku[i][j].value);

            if ((j + 1) % 9 == 0)
            {
                fprintf(fp, "\n");
            }
            // Adds space between values.
            else
            {
                fprintf(fp, " ");
            }

        }
    }
}


// Function that prints out the current values of the puzzle.
// ----------------------------------------------------------------------------
void print_puzzle(void)
{
    for (int i = 0; i < PUZZ_DIM; i++)
    {
        for (int j = 0; j < PUZZ_DIM; j++)
        {
            // Adds divider between grids by row.
            if (j == 0 && i % 3 == 0 && i != 0)
            {
                printf("------|-------|------\n");
            }

            printf("%i", sudoku[i][j].value);

            if ((j + 1) % 9 == 0)
            {
                printf("\n");
            }
            // Adds divider between grids by column.
            else if ((j + 1) % 3 == 0)
            {
                printf(" | ");
            }
            // Adds space between values.
            else
            {
                printf(" ");
            }

        }
    }

    printf("\n");
}


// First completes a sudoku puzzle which was provided in a text file. Next the
// puzzle is checked to see all the values are correctly placed.
// ----------------------------------------------------------------------------
int main()
{
    printf("Sudoku puzzle to solve:\n");
    load_puzzle(FILENAME);
    print_puzzle();

    while (has_empty_spots())
    {
        fill_puzzle();
        fill_next_spot(0, NULL);
    }

    printf("Solution to above puzzle:\n");
    print_puzzle();
    fprint_puzzle(OUTFILE);

    check_puzzle();
    
    return 0;
}
