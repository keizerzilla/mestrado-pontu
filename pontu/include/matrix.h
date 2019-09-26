/**
 * \file matrix.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Complex matrix struct and functions.
 */

#ifndef CMATRIX_H
#define CMATRIX_H

#include <complex.h>

#include "./vector3.h"

/**
 * \brief Struct to store complex matrices
 */
struct matrix {
    uint rows;
    uint cols;
    cnum *data;
};

/**
 * \brief Initializes a new complex matrix
 * \param rows Number of rows
 * \param cols Number of columns
 * \return Pointer to the new matrix
 */
struct matrix *matrix_new(uint rows, uint cols);

/**
 * \brief Frees a complex matrix
 * \param mat Complex Matrix to be freed
 */
void matrix_free(struct matrix **mat);

/**
 * \brief Copies a matrix
 * \param mat Target matrix
 * \return Pointer to the new copy of mat
 */
struct matrix *matrix_copy(struct matrix *mat);

/**
 * \brief Adds a new row into a complex matrix
 * \param mat Target complex matrix
 * \return New quantity of rows of the complex matrix
 */
int matrix_add_row(struct matrix *mat);

/**
 * \brief Adds a new column into a complex matrix
 * \param mat Target complex matrix
 * \return New quantity of columns of the  complex matrix
 */
int matrix_add_col(struct matrix *mat);

/**
 * \brief Removes a row from a complex matrix
 * \param mat Target complex matrix
 * \return New complex matrix with the row complex removed
 */
struct matrix *matrix_remove_row(struct matrix *mat, uint row);

/**
 * \brief Removes a column from a complex matrix
 * \param mat Target complex matrix
 * \return New complex matrix with the column removed
 */
struct matrix *matrix_remove_col(struct matrix *mat, uint col);

/**
 * \brief Set a value on the complex matrix
 * \param mat Target complex matrix
 * \param i Row index
 * \param j Column index
 * \param value The value for the element
 * \return Pointer to the element (i,j) of the target complex matrix
 */
cnum *matrix_set(struct matrix *mat, uint i, uint j, cnum value);

/**
 * \brief Gets an element of a complex matrix
 * \param mat Target matrix
 * \param i Row index
 * \param j Column index
 * \return Value of the element (i, j) of the target complex matrix
 */
cnum matrix_get(struct matrix *mat, uint i, uint j);

/**
 * \brief Concatenates horizontally two complex matrices
 * \param m1 First complex matrix
 * \param m2 Second complex matrix
 * \return Complex matrix of m1 and m2 concatenated
 */
struct matrix *matrix_concat_hor(struct matrix *m1, struct matrix *m2);

/**
 * \brief Concatenates vertically two complex matrices
 * \param m1 First complex matrix
 * \param m2 Second complex matrix
 * \return Complex matrix of m1 and m2 concatenated
 */
struct matrix *matrix_concat_ver(struct matrix *m1, struct matrix *m2);

/**
 * \brief Saves a complex matrix in a file
 * \param mat Copmlex matrix to be saved
 * \param filename File name
 * \param m Write mode on the file
 * \return 1 if the complex matrix were saved on the file, or 0 if it fails
 */
int matrix_save_to_file(struct matrix *mat,
                         const char *filename,
			             const char *m);

/**
 * \brief Debugs a complex matrix printing all its values
 * \param mat Target complex matrix
 * \param output File to save the debug
 */
void matrix_debug(struct matrix *mat, FILE * output);

#endif // MATRIX_H

