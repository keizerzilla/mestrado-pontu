#ifndef LEGENDRE_H
#define LEGENDRE_H

#define LEGENDRE_ORDER 2
#define LEGENDRE_MOMENTS 27

#include "../core.h"
#include "matrix.h"

/**
 * \brief Calculates Legendre polynomial
 * \param n Polynomial order
 * \param x Polynomial argument
 * \return Polynomial of x of order n
 */
real legendre_poly (int n, real x);

/**
 * \brief Calculates normalized coordinate of Legendre
 * \param c Original coordinate value
 * \param numpts Number of points in the cloud
 * \return Normalized coordinate
 */
real legendre_normcoord (real c, int numpts);

/**
 * \brief Calculates a normalization constant
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Normalization(p,q,r) constant
 */
real legendre_norm (int p, int q, int r, struct cloud* cloud);

/**
 * \brief Calculates Legendre moment
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Moment of order p+q+r of the cloud
 */
real legendre_moment (int p, int q, int r, struct cloud* cloud);

/**
 * \brief Calculates Legendre moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct matrix* legendre_cloud_moments (struct cloud* cloud);

#endif // LEGENDRE_H

