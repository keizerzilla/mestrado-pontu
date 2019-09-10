/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief 3D space partitioning using the kdtree data structure.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "vector3.h"

/**
 * \brief Struct to store a kdtree node
 */
struct kdtree {
	struct vector3 **points;
	uint numpts;
	struct vector3 *median;
	struct kdtree *left;
	struct kdtree *right;
};

/**
 * \brief Initializes a kdtree
 * \param points Vector of points
 * \param numpts Size of the points vector
 * \return NULL if it fails, or the pointer to the kdtree if it doesn't
 */
struct kdtree *kdtree_new(struct vector3 *points, uint numpts);

/**
 * \brief Frees a kdtree
 * \param kdt Kdtree to be freed
 */
void kdtree_free(struct kdtree *kdt);

/**
 * \brief Partitionates the points of a cloud recursively
 * \param kdt Target kdtree
 * \param axis The first axis to partitionate
 * \param depth The depth to particionate (how many times to partitionate)
 */
void kdtree_partitionate(struct kdtree *kdt, int axis, int depth);

/**
 * \brief Finds the point in the kdtree nearest to a target point
 * \param kdt The target kdtree
 * \param p The target point
 * \return Address of the closest point to p in kdt
 */
struct vector3 *kdtree_nearest_point(struct kdtree *kdt, struct vector3* p);

/**
 * \brief Debugs a kdtree (number of points in a leaf)
 * \param kdt Target kdtree
 * \param output File to output the debug in
 */
void kdtree_debug(struct kdtree *kdt, FILE *output);

#endif // KDTREE_H

