/**
 * \file zernike.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief The Zernike moment invariants.
 */

#ifndef ZERNIKE_H
#define ZERNIKE_H

#ifndef ZERNIKE_ORD
#define ZERNIKE_ORD 15
#endif

#ifndef ZERNIKE_REP
#define ZERNIKE_REP 13
#endif

#include "./cloud.h"
#include "./dataframe.h"

/**
 * \brief Zernike polynomial boundary conditions
 * \param n Polynomial order
 * \param m Repetitions
 * \return 1 if the conditions were satisfied, or else 0
 */
int zernike_conditions(int n, int m);

/**
 * \brief Auxilliary function for valid moments count
 * \param ord Order
 * \param rep Repetitions
 * \return Quantity of valid moments
 */
int zernike_nummoments(int ord, int rep);

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
real zernike_moment_odd(int n, int m, real r, struct cloud *cloud);

/**
 * \brief Calculates Zernike even moments
 * \param n Polynomial order
 * \param m Repetitions
 * \param r Radius
 * \param cloud Target cloud
 * \return Zernike moment of order(n) and repetition(m)
 */
real zernike_moment_even(int n, int m, real r, struct cloud *cloud);

/**
 * \brief Calculates the magnitude of Zernike moments
 * \param n Polynomial order
 * \param m Repetitions
 * \param r Radius
 * \param cloud Target cloud
 * \return Zernike moment of order(n) and repetition(m)
 */
real zernike_moment_mag(int n, int m, real r, struct cloud *cloud);

/**
 * \brief Calculates Zernike moments in a full form (both phases)
 * \param n Polynomial order
 * \param m Repetitions
 * \param r Radius
 * \param cloud Target cloud
 * \return Zernike moment of order(n) and repetition(m)
 */
real zernike_moment_full(int n, int m, real r, struct cloud *cloud);

/**
 * \brief Calculates Zernike odd moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct dataframe *zernike_cloud_moments_odd(struct cloud *cloud);

/**
 * \brief Calculates Zernike even moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct dataframe *zernike_cloud_moments_even(struct cloud *cloud);

/**
 * \brief Calculates the magnitude of Zernike moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct dataframe *zernike_cloud_moments_mag(struct cloud *cloud);

/**
 * \brief Calculates full form Zernike moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct dataframe *zernike_cloud_moments_full(struct cloud *cloud);

#endif // ZERNIKE_H

