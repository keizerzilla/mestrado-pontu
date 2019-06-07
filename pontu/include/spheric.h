/**
 * \file spheric.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief The spheric moment invariants by Artur Rodrigues Rocha Neto
 */

#ifndef SPHERIC_H
#define SPHERIC_H

#ifndef SPHERIC_ORDER_X
#define SPHERIC_ORDER_X 2
#endif

#ifndef SPHERIC_ORDER_Y
#define SPHERIC_ORDER_Y 2
#endif

#ifndef SPHERIC_ORDER_Z
#define SPHERIC_ORDER_Z 2
#endif

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Image function from the spheric equation
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 * \param p Order of coordinate x
 * \param q Order of coordinate y
 * \param r Order of coordinate z
 * \return Real number to represent the spheric relation
 */
real spheric_quad(real x, real y, real z, int p, int q, int r);

/**
 * \brief Calculates spheric moments of a cloud
 * \param p Order of coordinate x
 * \param q Order of coordinate y
 * \param r Order of coordinate z
 * \param cloud Target cloud
 * \return Spheric moment of order p+q+r
 */
real spheric_moment(int p, int q, int r, struct cloud *cloud);

/**
 * \brief Calculates spheric moments of a cloud
 * \param cloud Target cloud
 * \return Matrix with the moments
 */
struct matrix *spheric_cloud_moments(struct cloud *cloud);

#endif // SPHERIC_H

