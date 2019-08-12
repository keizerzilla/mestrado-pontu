/**
 * \file hu.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Hu moment invariants and other literature variants.
 */

#ifndef HU_H
#define HU_H

#define HU_SUPERSET_ORDER 1
#define HU_SUPERSET_MOMENTS 8
#define HU_MOMENTS 21

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calculates regular Hu 3D moment
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Regular moment p+q+r of the cloud
 */
real hu_regular_moment(int p, int q, int r, struct cloud *cloud);

/**
 * \brief Calculates central moment of Hu 3D
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Central moment p+q+r of the cloud
 */
real hu_central_moment(int p, int q, int r, struct cloud *cloud);

/**
 * \brief Calculates normalized moment of Hu 3D
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Normalized moment p+q+r of the cloud
 */
real hu_normalized_moment(int p, int q, int r, struct cloud *cloud);

/**
 * \brief Calculates the refined normalized moments
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Normalized moments p+q+r by Guo of the cloud
 */
real hu_refined_moment(int p, int q, int r, struct cloud *cloud);

/**
 * \brief Calculates invariants moments of Hu (by SADJADI and HALL - 1980)
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct matrix *hu_cloud_moments_hu1980(struct cloud *cloud);

/**
 * \brief Calculates a set of central raw moment of Hu 3D
 * \param p Maximum order of dimension x
 * \param q Maximum order of dimension y
 * \param r Maximum order of dimension z
 * \param cloud Target cloud
 * \return Orders up to p, q, r of cloud
 */
struct matrix *hu_cloud_raw_moments(struct cloud *cloud, int p, int q, int r);

/**
 * \brief Calculates invariants moments of Hu (Rocha Neto - 2017)
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct matrix *hu_cloud_moments_hututu(struct cloud *cloud);

#endif // HU_H

