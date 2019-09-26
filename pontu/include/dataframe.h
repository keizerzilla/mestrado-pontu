/**
 * \file dataframe.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Simple implementation for fast (?) dataframe handling.
 */

#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <stdlib.h>
#include <stdio.h>

#include "./calc.h"

/**
 * \brief Struct to store 2D arrays
 */
struct dataframe {
	uint rows;
	uint cols;
	real *data;
};

/**
 * \brief Initializes a new dataframe
 * \param rows Number of rows
 * \param cols Number of columns
 * \return Pointer to the new dataframe
 */
struct dataframe *dataframe_new(uint rows, uint cols);

/**
 * \brief Frees a dataframe
 * \param mat Dataframe to be freed
 */
void dataframe_free(struct dataframe **mat);

/**
 * \brief Adds a new row into a dataframe
 * \param mat Target dataframe
 * \return New quantity of rows of the dataframe
 */
int dataframe_add_row(struct dataframe *mat);

/**
 * \brief Adds a new column into a dataframe
 * \param mat Target dataframe
 * \return New quantity of columns of the dataframe
 */
int dataframe_add_col(struct dataframe *mat);

/**
 * \brief Removes a row from a dataframe
 * \param mat Target dataframe
 * \return New dataframe with the row removed
 */
struct dataframe *dataframe_remove_row(struct dataframe *mat, uint row);

/**
 * \brief Removes a column from a dataframe
 * \param mat Target dataframe
 * \return New dataframe with the column removed
 */
struct dataframe *dataframe_remove_col(struct dataframe *mat, uint col);

/**
 * \brief Set a value on the dataframe
 * \param mat Target dataframe
 * \param i Row index
 * \param j Column index
 * \param value The value for the element
 * \return Pointer to the element (i,j) of the target dataframe
 */
real *dataframe_set(struct dataframe *mat, uint i, uint j, real value);

/**
 * \brief Gets an element of a dataframe
 * \param mat Target dataframe
 * \param i Row index
 * \param j Column index
 * \return Value of the element (i, j) of the target dataframe
 */
real dataframe_get(struct dataframe *mat, uint i, uint j);

/**
 * \brief Concatenates horizontally two matrices
 * \param m1 First dataframe
 * \param m2 Second dataframe
 * \return Dataframe of m1 and m2 concatenated
 */
struct dataframe *dataframe_concat_hor(struct dataframe *m1,
                                       struct dataframe *m2);

/**
 * \brief Concatenates vertically two matrices
 * \param m1 First dataframe
 * \param m2 Second dataframe
 * \return Dataframe of m1 and m2 concatenated
 */
struct dataframe *dataframe_concat_ver(struct dataframe *m1,
                                       struct dataframe *m2);

/**
 * \brief Saves a dataframe in a file
 * \param mat Dataframe to be saved
 * \param filename File name
 * \param m Write mode on the file
 * \return 1 if the dataframe were saved on the file, or 0 if it fails
 */
int dataframe_save_to_file(struct dataframe *mat,
                           const char *filename,
			               const char *m);

/**
 * \brief Debugs a dataframe printing all its values
 * \param mat Target dataframe
 * \param output File to save the debug
 */
void dataframe_debug(struct dataframe *mat, FILE * output);

#endif // DATAFRAME_H

