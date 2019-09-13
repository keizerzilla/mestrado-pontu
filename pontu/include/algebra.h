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

#define ALGEBRA_SQRT_3 1.73205080756887729352744634150587237

struct cmatrix *algebra_affine_root(real a, real b);

/**
 *
 *
 */
struct cmatrix *algebra_quadratic_roots(real a, real b, real c);

struct cmatrix *algebra_cubic_roots(real a, real b, real c, real d);

struct cmatrix *algebra_quartic_roots(real a, real b, real c, real d, real e);

#endif // ALGEBRA_H
