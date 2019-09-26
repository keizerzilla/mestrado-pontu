/**
 * \file harmonics.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief The Spherical Harmonics descriptors.
 */

#ifndef HARMONICS_H
#define HARMONICS_H

#ifndef HARMON_ORD
#define HARMON_ORD 7
#endif

#ifndef HARMON_REP
#define HARMON_REP 7
#endif

#ifndef HARMON_SPIN
#define HARMON_SPIN 7
#endif

#include "./cloud.h"
#include "./dataframe.h"
#include "./zernike.h"

/**
 * \brief
 * \param
 * \return
 */
int harmonics_conditions(int n, int m, int l);

/**
 * \brief
 * \param
 * \return
 */
int harmonics_nummoments(int ord, int rep, int spin);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_norm(int m, int l);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_legendrepoly(int m, int l, real x);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_harmonic_odd(int m, int l, real theta, real phi);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_harmonic_even(int m, int l, real theta, real phi);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_harmonic_mag(int m, int l, real theta);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_harmonic_complete(int m, int l, real theta, real phi);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_moment_odd(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_moment_even(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_moment_mag(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
real harmonics_moment_full(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct dataframe *harmonics_cloud_moments_odd(struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct dataframe *harmonics_cloud_moments_even(struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct dataframe *harmonics_cloud_moments_mag(struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct dataframe *harmonics_cloud_moments_full(struct cloud *cloud);

#endif // HARMONICS_H

