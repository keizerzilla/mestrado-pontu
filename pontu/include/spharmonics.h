/**
 * \file spharmonics.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief The Spherical Harmonics descriptors.
 */

#ifndef SPHARMONICS_H
#define SPHARMONICS_H

#ifndef SPHARM_ORD
#define SPHARM_ORD 7
#endif

#ifndef SPHARM_REP
#define SPHARM_REP 7
#endif

#ifndef SPHARM_SPIN
#define SPHARM_SPIN 7
#endif

#include "cloud.h"
#include "matrix.h"
#include "zernike.h"

/**
 * \brief
 * \param
 * \return
 */
int spharmonics_conditions(int n, int m, int l);

/**
 * \brief
 * \param
 * \return
 */
int spharmonics_nummoments(int ord, int rep, int spin);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_norm(int m, int l);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_legendrepoly(int m, int l, real x);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_harmonic_odd(int m, int l, real theta, real phi);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_harmonic_even(int m, int l, real theta, real phi);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_harmonic_mag(int m, int l, real theta);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_harmonic_complete(int m, int l, real theta, real phi);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_moment_odd(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_moment_even(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_moment_mag(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
real spharmonics_moment_full(int n, int m, int l, real r, struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct matrix *spharmonics_cloud_moments_odd(struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct matrix *spharmonics_cloud_moments_even(struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct matrix *spharmonics_cloud_moments_mag(struct cloud *cloud);

/**
 * \brief
 * \param
 * \return
 */
struct matrix *spharmonics_cloud_moments_full(struct cloud *cloud);

#endif // SPHARMONICS_H

