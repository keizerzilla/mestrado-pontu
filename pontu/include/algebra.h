/**
 * \file algebra.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Algebric functions.
 */

#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "./matrix.h"

#define ALGEBRA_NUM_ERR 0.0000001

/**
 * \brief Finds the root of a affine equation
 * \param a Coefficient a
 * \param b Coefficient b
 * \return A complex matrix 1x1 with the root of the equation
 */
struct matrix *algebra_affine_root(real a, real b);

/**
 * \brief Finds the roots of a quadratic function
 * \param a Coefficient a
 * \param b Coefficient b
 * \param c Coefficient c
 * \return A complex matrix 2x1 with the roots of the equation
 */
struct matrix *algebra_quadratic_roots(real a, real b, real c);

/**
 * \brief Finds the roots of a cubic function
 * \param a Coefficient a
 * \param b Coefficient b
 * \param c Coefficient c
 * \param d Coefficient d
 * \return A complex matrix 3x1 with the roots of the equation
 */
struct matrix *algebra_cubic_roots(real a, real b, real c, real d);

/**
 * \brief Finds the roots of a quartic function
 * \param a Coefficient a
 * \param b Coefficient b
 * \param c Coefficient c
 * \param d Coefficient d
 * \param e Coefficient e
 * \return A complex matrix 4x1 with the roots of the equation
 */
struct matrix *algebra_quartic_roots(real a, real b, real c, real d, real e);

/**
 * \brief Applies the Gauss elimination method to solve a system from a matrix 
 * Nx(N+1)
 * \param mat Target matrix
 * \return A vector Nx1 with the solutions of the system
 */
struct matrix *algebra_gauss_elim(struct matrix *mat);

/**
 * \brief Applies the Gauss elimination method to reduce a square matrix
 * \param mat Target matrix
 * \return The matrix reduced
 */
struct matrix *algebra_gauss_elim_pivot(struct matrix *mat);

/**
 * \brief Sum two complex matrices
 * \param a First complex matrix
 * \param b Second complex matrix
 * \return Complex matrix sum of a and b, or NULL if it fails
 */
struct matrix *algebra_mat_sum(struct matrix *a, struct matrix *b);

/**
 * \brief Subtraction between two complex matrices
 * \param a First complex matrix
 * \param b Second complex matrix
 * \return Copmlex matrix result of subtraction of b in a, or NULL if it fails
 */
struct matrix *algebra_mat_sub(struct matrix *a, struct matrix *b);

/**
 * \brief Calculates the product between two complex matrices
 * \param a First complex matrix
 * \param b Second complex matrix
 * \return Complex matrix product between a and b, or NULL if it fails
 */
struct matrix *algebra_mat_prod(struct matrix *a, struct matrix *b);

/**
 * \brief Multiplies a complex matrix by a scalar
 * \param mat The complex matrix
 * \param scalar The scalar
 */
struct matrix *algebra_mat_vs_scalar(struct matrix *mat, cnum scalar);

/**
 * \brief Transposes a complex matrix
 * \param mat The complex matrix
 * \return Complex matrix mat transposed, or NULL if it fails
 */
struct matrix *algebra_mat_transpose(struct matrix *mat);

/**
 * \brief Sums elements of the main diagonal of a matrix
 * \param mat The complex matrix
 * \return The trace of a matrix
 */
cnum algebra_mat_trace(struct matrix *mat);

/**
 * \brief Calculates the determinat of a complex square matrix
 * \param mat The complex matrix
 * \return The determinat of the complex matrix, or 0 if it fails
 */
cnum algebra_mat_det(struct matrix *mat);

/**
 * \brief Finds the eigen values of a square matrix NxN
 * \param mat Target matrix
 * \return A vector Nx1 with the eigen values of mat
 */
struct matrix *algebra_mat_eigen_val(struct matrix *mat);

/**
 * \brief Finds the eigen vector of a square matrix NxN
 * \param mat Target matrix
 * \param eig_val Eigen values of mat
 * \return A matrix NxN, where each column is a eigen vector
 */
struct matrix *algebra_mat_eigen_vec(struct matrix *mat,
                                      struct matrix *eig_val);

#endif // ALGEBRA_H

