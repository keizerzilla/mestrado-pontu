/**
 * \file registration.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Functions for clouds registrations.
 */

#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "./cloud.h"

/**
 * \brief Finds a equivalence cloud with closest points of source in target
 * \param source The source cloud
 * \param target The target cloud
 * \return The equivalence cloud
 */
struct cloud *registration_closest_points_bf(struct cloud *source,
                                             struct cloud *target);

/**
 * \brief Finds a equivalence cloud with closest points of source in target
 * \param source The source cloud
 * \param target The target cloud
 * \return The equivalence cloud
 */
struct cloud *registration_closest_points_tree(struct cloud *source,
                                               struct cloud *target);

/**
 * \brief Finds the transformation matrix 4x4 (rotation + translation)
 * \param source The source cloud
 * \param target The target cloud
 * \return The transformation matrix 4x4
 */
struct matrix *registration_align(struct cloud *source, struct cloud *target);

/**
 * \brief Applies the ICP algorithm to register source with target
 * \param source The source cloud
 * \param target The target cloud
 * \param aligned Pointer to the output cloud registered
 * \param t Stop criteria
 * \param k Maximun number of iterations
 * \return The transformation matrix 4x4
 */
struct matrix *registration_icp(struct cloud *source,
                                 struct cloud *target,
                                 struct cloud **aligned,
                                 real t,
                                 uint k);

#endif // REGISTRATION_H

