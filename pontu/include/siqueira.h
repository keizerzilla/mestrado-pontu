/**
 * \file siqueira.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief A complete set of functions implementing the work of Robson Siqueira.
 */

#ifndef SIQUEIRA_H
#define SIQUEIRA_H

#define CUT_SIZE 20

#include "matrix.h"
#include "cloud.h"

/**
 * \brief Calculates central Hu moment in the XY plane
 * \param p Order of the x coordinate
 * \param q Order of the y coordinate
 * \param cloud The target cloud
 * \param ref The nosetip (or other reference point)
 * \return Central moment of order p+q of cloud referent to the XY plane
 */
real siqueira_moment_xy(int p, int q, struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Calculates central Hu moment in the XZ plane
 * \param p Order of the x coordinate
 * \param q Order of the z coordinate
 * \param cloud The target cloud
 * \param ref The nosetip (or other reference point)
 * \return Central moment of order p+q of cloud referent to the XZ plane
 */
real siqueira_moment_xz(int p, int q, struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Calculates central Hu moment in the YZ plane
 * \param p Order of the y coordinate
 * \param q Order of the z coordinate
 * \param cloud The target cloud
 * \param ref The nosetip (or other reference point)
 * \return Central moment of order p+q of cloud referent to the YZ plane
 */
real siqueira_moment_yz(int p, int q, struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Calculate the Hu moment invariants according to (SIQUEIRA; 2018)
 * \param cloud The target cloud
 * \return A matrix containing the calculated moments
 */
struct matrix* siqueira_cloud_moments(struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Returns the sign of a real number
 * \param n The real number
 * \return 1.0f if n is positive, -1.0f otherwise
 */
real siqueira_sign(real n);

/**
 * \brief Tranversal segmentation
 * \param cloud The target cloud
 * \return ref The nosetip (or other reference)
 * \return The extracted moments
 */
struct matrix* siqueira_transversal(struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Saggital segmentation
 * \param cloud The target cloud
 * \return ref The nosetip (or other reference)
 * \return The extracted moments
 */
struct matrix* siqueira_sagittal(struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Frontal segmentation
 * \param cloud The target cloud
 * \return ref The nosetip (or other reference)
 * \return The extracted moments
 */
struct matrix* siqueira_frontal(struct cloud *cloud, struct vector3 *ref);

#endif // SIQUEIRA_H

