/**
 * \file cmatrix.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Complex matrix struct and functions.
 */

#ifndef CMATRIX_H
#define CMATRIX_H

#include <complex.h>

#include "algebra.h"
#include "calc.h"
#include "matrix.h"
#include "vector3.h"

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
 * \brief Sum two complex matrices
 * \param a First complex matrix
 * \param b Second complex matrix
 * \return Complex matrix sum of a and b, or NULL if it fails
 */
struct cmatrix *cmatrix_sum(struct cmatrix *a, struct cmatrix *b);

/**
 * \brief Subtraction between two complex matrices
 * \param a First complex matrix
 * \param b Second complex matrix
 * \return Copmlex matrix result of subtraction of b in a, or NULL if it fails
 */
struct cmatrix *cmatrix_sub(struct cmatrix *a, struct cmatrix *b);

/**
 * \brief Calculates the product between two complex matrices
 * \param a First complex matrix
 * \param b Second complex matrix
 * \return Complex matrix product between a and b, or NULL if it fails
 */
struct cmatrix *cmatrix_prod(struct cmatrix *a, struct cmatrix *b);

/**
 * \brief Multiplies a complex matrix by a scalar
 * \param mat The complex matrix
 * \param scalar The scalar
 */
struct cmatrix *cmatrix_vs_scalar(struct cmatrix *mat, cnum scalar);

/**
 * \brief Transposes a complex matrix
 * \param mat The complex matrix
 * \return Complex matrix mat transposed, or NULL if it fails
 */
struct cmatrix *cmatrix_transpose(struct cmatrix *mat);

/**
 * \brief Calculates the determinat of a complex square matrix
 * \param mat The complex matrix
 * \return The determinat of the complex matrix, or 0 if it fails
 */
 cnum cmatrix_det(struct cmatrix *mat);


struct cmatrix *cmatrix_eigenvalues(struct cmatrix *mat);

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
