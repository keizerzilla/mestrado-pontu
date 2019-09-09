/**
 * \file matrix.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Simple matrix implementation for fast dataframe handling.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/**
 * \brief Struct to store 2D arrays
 */
struct matrix {
	uint rows;
	uint cols;
	real *data;
};

/**
 * \brief Initializes a new matrix
 * \param rows Number of rows
 * \param cols Number of columns
 * \return Pointer to the new matrix
 */
struct matrix *matrix_new(uint rows, uint cols);

/**
 * \brief Frees a matrix
 * \param mat Matrix to be freed
 */
void matrix_free(struct matrix *mat);

/**
 * \brief Adds a new row into a matrix
 * \param mat Target matrix
 * \return New quantity of rows of the matrix
 */
int matrix_add_row(struct matrix *mat);

/**
 * \brief Adds a new column into a matrix
 * \param mat Target matrix
 * \return New quantity of columns of the matrix
 */
int matrix_add_col(struct matrix *mat);

/**
 * \brief Removes a row from a matrix
 * \param mat Target matrix
 * \return New matrix with the row removed
 */
struct matrix *matrix_remove_row(struct matrix *mat, uint row);

/**
 * \brief Removes a column from a matrix
 * \param mat Target matrix
 * \return New matrix with the column removed
 */
struct matrix *matrix_remove_col(struct matrix *mat, uint col);

/**
 * \brief Set a value on the matrix
 * \param mat Target matrix
 * \param i Row index
 * \param j Column index
 * \param value The value for the element
 * \return Pointer to the element (i,j) of the target matrix
 */
real *matrix_set(struct matrix *mat, uint i, uint j, real value);

/**
 * \brief Gets an element of a matrix
 * \param mat Target matrix
 * \param i Row index
 * \param j Column index
 * \return Value of the element (i, j) of the target matrix
 */
real matrix_get(struct matrix *mat, uint i, uint j);

/**
 * \brief Concatenates horizontally two matrices
 * \param m1 First matrix
 * \param m2 Second matrix
 * \return Matrix of m1 and m2 concatenated
 */
struct matrix *matrix_concat_hor(struct matrix *m1, struct matrix *m2);

/**
 * \brief Concatenates vertically two matrices
 * \param m1 First matrix
 * \param m2 Second matrix
 * \return Matrix of m1 and m2 concatenated
 */
struct matrix *matrix_concat_ver(struct matrix *m1, struct matrix *m2);

/**
 * \brief Saves a matrix in a fileSalva uma matriz em arquivo
 * \param mat Matrix to be saved
 * \param filename File name
 * \param m Write mode on the file
 * \return 1 if the matrix were saved on the file, or 0 if it fails
 */
int matrix_save_to_file(struct matrix *mat,
                        const char *filename,
			            const char *m);

/**
 * \brief Debugs a matrix printing all its values
 * \param mat Target matrix
 * \param output File to save the debug
 */
void matrix_debug(struct matrix *mat, FILE * output);

#endif // MATRIX_H

