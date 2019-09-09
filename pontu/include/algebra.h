/**
 * \file algebra.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Algebric functions.
 */

#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "calc.h"
#include "matrix.h"
#include "vector3.h"

/**
 * \brief Calculates the product between two matrices
 * \param a First matrix
 * \param b Second matrix
 * \return Matrix product between a and b, or NULL if it fails
 */
struct matrix *algebra_mat_prod(struct matrix *a, struct matrix *b);


/**
 * \brief Sum two matrices
 * \param a First matrix
 * \param b Second matrix
 * \return Matrix sum of a and b, or NULL if it fails
 */
struct matrix *algebra_mat_sum(struct matrix *a, struct matrix *b);

/**
 * \brief Subtraction between two matrices
 * \param a First matrix
 * \param b Second matrix
 * \return Matrix result of subtraction of b in a, or NULL if it fails
 */
struct matrix *algebra_mat_sub(struct matrix *a, struct matrix *b);

/**
 * \brief Multiplies a matrix by a scalar
 * \param mat The matrix
 * \param scalar The scalar
 */
struct matrix *algebra_mat_vs_scalar(struct matrix *mat, real scalar);

/**
 * \brief Transposes a matrix
 * \param mat The matrix
 * \return Matrix mat transposed, or NULL if it fails
 */
struct matrix *algebra_trans_mat(struct matrix *mat);

/**
 * \brief Calculates the determinat of a square matrix
 * \param mat The matrix
 * \return The determinat of the matrix, or 0 if it fails
 */
real algebra_det(struct matrix *mat);

/**
 *
 *
 */
struct matrix *algebra_bhaskara(real a, real b, real c);

/**
 * \brief Calculates eigen values of a matrix
 * \param mat The matrix
 * \return A matrix Nx1 with the N eigen values calculated, or NULL if it fails
 */
struct matrix *algebra_eigen_val(struct matrix *mat);

#endif // ALGEBRA_H
