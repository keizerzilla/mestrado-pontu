/**
 * \file zernike.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief The Zernike moment invariants.
 */

#ifndef ZERNIKE_H
#define ZERNIKE_H

#ifndef ZERNIKE_ORDER
#define ZERNIKE_ORDER 12
#endif

#ifndef ZERNIKE_REPETITION
#define ZERNIKE_REPETITION 8
#endif

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Zernike polynomial boundary conditions
 * \param n Polynomial order
 * \param m Repetitions
 * \return 1 if the conditions were satisfied, or else 0
 */
int zernike_conditions(int n, int m);

/**
 * \brief Zernike polynomial odd boundary conditions
 * \param n Polynomial order
 * \param m Repetitions
 * \return 1 if the conditions were satisfied, or else 0
 */
int zernike_conditions_odd(int n, int m);

/**
 * \brief Auxilliary function for valid moments count
 * \param ord Order
 * \param rep Repetitions
 * \return Quantity of valid moments
 */
int zernike_num_moments(int ord, int rep);

/**
 * \brief Auxilliary function for valid odd moments count
 * \param ord Order
 * \param rep Repetitions
 * \return Quantity of valid moments
 */
int zernike_num_moments_odd(int ord, int rep);

/**
 * \brief Calculates the radial Zernike polynomial
 * \param n Polynomial order
 * \param m Repetitions
 * \param distance Radial distance
 * \return Radial Zernike polynomial of order (n) and repetitions (m)
 */
real zernike_radpoly(int n, int m, real distance);

/**
 * \brief Calculates the azimutal angle
 * \param y Coordinate y
 * \param x Coordinate x
 * \return Angle of the vector
 */
real zernike_azimuth(real y, real x);

/**
 * \brief Calculates the zenite angle
 * \param z Coordinate z
 * \param r Zernike radius
 * \return Zenite
 */
real zernike_zenith(real z, real r);

/**
 * \brief Calculates Zernike odd moments
 * \param n Polynomial order
 * \param m Repetitions
 * \param r Radius
 * \param cloud Target cloud
 * \return Zernike moment of order(n) and repetition(m)
 */
real zernike_odd_moment(int n, int m, real r, struct cloud *cloud);

/**
 * \brief Calculates Zernike even moments
 * \param n Polynomial order
 * \param m Repetitions
 * \param r Radius
 * \param cloud Target cloud
 * \return Zernike moment of order(n) and repetition(m)
 */
real zernike_even_moment(int n, int m, real r, struct cloud *cloud);

/**
 * \brief Calculates Zernike moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct matrix *zernike_cloud_moments(struct cloud *cloud);

#endif // ZERNIKE_H

