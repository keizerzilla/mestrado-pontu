/**
 * \file calc.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Math utility functions, constants and typedefs.
 */

#ifndef CALC_H
#define CALC_H

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <complex.h>

#define CALC_E			2.7182818284590452353602874713527
#define CALC_PHI		1.618033988749895
#define CALC_PI			3.14159265358979323846264338327950288
#define CALC_PI_2		1.57079632679489661923132169163975144
#define CALC_DEG2RAD	CALC_PI / 180.0
#define CALC_RAD2DEG	180.0 / CALC_PI

/**
 * \var real
 * \brief Mask for the float point number most accurate available on the system
 */
typedef double real;

/**
 * \var number
 * \brief Mask for the float point complex number most accurate available on the system
 */
typedef double complex cnum;

/**
 * \var uint
 * \brief Mask for the integer type w/o signal
 */
typedef unsigned int uint;

/**
 * \var int64
 * \brief Mask for the  64 bits integer type
 */
typedef long long int64;

/**
 * \brief Calculates the factorial of a number
 * \param n Number to calculate the factorial
 * \return n!
 */
int calc_factorial(int n);

/**
 * \brief Number of combinations N-K
 * \param n dimension 1
 * \param k dimension 2
 * \return N-K combinations
 */
int calc_binom_coeff(int n, int k);

/**
 * \brief Calculates the Kronecker Delta
 * \param i First sub-index
 * \param j Second sub-index
 * \return 1 if i==j and 0 if i!=j
 */
int calc_kronecker(int i, int j);

/**
 * \brief Calculates 3D square length
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 * \return The 3D square length of the point(x,y,z)
 */
real calc_squared_length3(real x, real y, real z);

/**
 * \brief Calculates 3D length
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 * \return The 3D length of the point(x,y,z)
 */
real calc_length3(real x, real y, real z);

/**
 * \brief Calculates 2D square length
 * \param x Coordinate x
 * \param y Coordinate y
 * \return The 2D squared length of the point(x,y)
 */
real calc_squared_length2(real x, real y);

/**
 * \brief Calculates 2D length
 * \param x Coordinate x
 * \param y Coordinate y
 * \return The 2D length of the point(x,y)
 */
real calc_length2(real x, real y);

/**
 * \brief Create random real numbers
 * \param max Maximun value possible
 * \return A random real number on the interval [0, max]
 */
real calc_randr(real max);

/**
 * \brief Gets the greater number out of 3 real numbers
 * \param a The first real
 * \param b The second real
 * \param c The third real
 * \return The greater value in (a, b, c)
 */
real calc_max3(real a, real b, real c);

/**
 * \brief Returns the signal of a real number
 * \param a The number
 * \return 1.0f if a >= 0.0f, or -1.0f otherwise
 */
real calc_sign(real a);

/**
 * \brief Performs a simple log transformation
 * \param a The number to be transformed
 * \return -1 * sign(a) * log(abs(a))
 */
real calc_logt(real a);

/**
 * \brief Calculates the 1D gaussian value
 * \param x The input number
 * \param s The standard deviation of the distribuition
 * \return The zero mean and s-deviation of the gaussian distribuition of x
 */
real calc_gaussian(real x, real s);

/**
 * \brief Calculates the gaussian value for a number
 * \param x The x value
 * \param y The y value
 * \param z The z value
 * \param sigma The standard deviation of the distribuition
 * \return zero-mean and s-deviation of the gaussian distribuition of (x,y,z)
 */
real calc_gaussian3(real x, real y, real z, real s);

#endif // CALC_H

