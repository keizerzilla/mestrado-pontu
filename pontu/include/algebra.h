/**
 * \file algebra.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Algebric functions.
 */

#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "cmatrix.h"
#include "calc.h"
#include "matrix.h"
#include "vector3.h"

/**
 * \brief Finds the root of a affine equation
 * \param a Coefficient a
 * \param b Coefficient b
 * \return A complex matrix 1x1 with the root of the equation
 */
struct cmatrix *algebra_affine_root(real a, real b);

/**
 * \brief Finds the roots of a quadratic function
 * \param a Coefficient a
 * \param b Coefficient b
 * \param c Coefficient c
 * \return A complex matrix 2x1 with the roots of the equation
 */
struct cmatrix *algebra_quadratic_roots(real a, real b, real c);

/**
 * \brief Finds the roots of a cubic function
 * \param a Coefficient a
 * \param b Coefficient b
 * \param c Coefficient c
 * \param d Coefficient d
 * \return A complex matrix 3x1 with the roots of the equation
 */
struct cmatrix *algebra_cubic_roots(real a, real b, real c, real d);

/**
 * \brief Finds the roots of a quartic function
 * \param a Coefficient a
 * \param b Coefficient b
 * \param c Coefficient c
 * \param d Coefficient d
 * \param e Coefficient e
 * \return A complex matrix 4x1 with the roots of the equation
 */
struct cmatrix *algebra_quartic_roots(real a, real b, real c, real d, real e);

#endif // ALGEBRA_H
