/**
 * \file cmatrix.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Complex matrix struct and functions.
 */

#ifndef CMATRIX_H
#define CMATRIX_H

#include <complex.h>

#include "vector3.h"

/**
 * \brief Struct to store complex matrices
 */
struct cmatrix {
    uint rows;
    uint cols;
    cnum *data;
};

/**
 * \brief Initializes a new complex matrix
 * \param rows Number of rows
 * \param cols Number of columns
 * \return Pointer to the new cmatrix
 */
struct cmatrix *cmatrix_new(uint rows, uint cols);

/**
 * \brief Frees a complex matrix
 * \param mat Complex Matrix to be freed
 */
void cmatrix_free(struct cmatrix *mat);

/**
 * \brief Copies a matrix
 * \param mat Target matrix
 * \return Pointer to the new copy of mat
 */
struct cmatrix *cmatrix_copy(struct cmatrix *mat);

/**
 * \brief Adds a new row into a complex matrix
 * \param mat Target complex matrix
 * \return New quantity of rows of the complex matrix
 */
int cmatrix_add_row(struct cmatrix *mat);

/**
 * \brief Adds a new column into a complex matrix
 * \param mat Target complex matrix
 * \return New quantity of columns of the  complex matrix
 */
int cmatrix_add_col(struct cmatrix *mat);

/**
 * \brief Removes a row from a complex matrix
 * \param mat Target complex matrix
 * \return New complex matrix with the row complex removed
 */
struct cmatrix *cmatrix_remove_row(struct cmatrix *mat, uint row);

/**
 * \brief Removes a column from a complex matrix
 * \param mat Target complex matrix
 * \return New complex matrix with the column removed
 */
struct cmatrix *cmatrix_remove_col(struct cmatrix *mat, uint col);

/**
 * \brief Set a value on the complex matrix
 * \param mat Target complex matrix
 * \param i Row index
 * \param j Column index
 * \param value The value for the element
 * \return Pointer to the element (i,j) of the target complex matrix
 */
cnum *cmatrix_set(struct cmatrix *mat, uint i, uint j, cnum value);

/**
 * \brief Gets an element of a complex matrix
 * \param mat Target matrix
 * \param i Row index
 * \param j Column index
 * \return Value of the element (i, j) of the target complex matrix
 */
cnum cmatrix_get(struct cmatrix *mat, uint i, uint j);

/**
 * \brief Concatenates horizontally two complex matrices
 * \param m1 First complex matrix
 * \param m2 Second complex matrix
 * \return Complex matrix of m1 and m2 concatenated
 */
struct cmatrix *cmatrix_concat_hor(struct cmatrix *m1, struct cmatrix *m2);

/**
 * \brief Concatenates vertically two complex matrices
 * \param m1 First complex matrix
 * \param m2 Second complex matrix
 * \return Complex matrix of m1 and m2 concatenated
 */
struct cmatrix *cmatrix_concat_ver(struct cmatrix *m1, struct cmatrix *m2);

/**
 * \brief Saves a complex matrix in a file
 * \param mat Copmlex matrix to be saved
 * \param filename File name
 * \param m Write mode on the file
 * \return 1 if the complex matrix were saved on the file, or 0 if it fails
 */
int cmatrix_save_to_file(struct cmatrix *mat,
                         const char *filename,
			             const char *m);

/**
 * \brief Debugs a complex matrix printing all its values
 * \param mat Target complex matrix
 * \param output File to save the debug
 */
void cmatrix_debug(struct cmatrix *mat, FILE * output);

#endif // CMATRIX_H
