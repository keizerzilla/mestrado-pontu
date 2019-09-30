/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief 3D space partitioning using the kdtree data structure.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "./vector3.h"

/**
 * \brief Structure to store a kdtree node
 */
struct kdtree {
	struct vector3 **points;
	uint numpts;
	struct vector3 *midpnt;
	int axis;
	struct kdtree *parent;
	struct kdtree *left;
	struct kdtree *right;
};

/**
 * \brief Initializes a kdtree
 * \param points Vector of points
 * \param numpts Size of the points vector
 * \param axis The node's separation axis
 * \return NULL if it fails, or the pointer to the kdtree if it doesn't
 */
struct kdtree *kdtree_new(struct kdtree *parent,
                          struct vector3 *points,
                          uint numpts,
                          int axis);

/**
 * \brief Frees a kdtree
 * \param kdt Kdtree to be freed
 */
void kdtree_free(struct kdtree **kdt);

/**
 * \brief Partitionates the points of a cloud recursively
 * \param kdt Target kdtree
 */
void kdtree_partitionate(struct kdtree *kdt);

/**
 * @TODO
 */
real kdtree_dist_hyperplane(struct kdtree *k1, struct kdtree *k2);

/**
 * @TODO
 */
struct kdtree *kdtree_unwind(struct kdtree *node);

/**
 * @TODO
 */
struct kdtree *kdtree_closest_node(struct kdtree *kdt,
                                   struct vector3 *p,
                                   real *r);

/**
 * @TODO
 */
void kdtree_closest_point(struct kdtree *node,
                          struct kdtree* current,
                          struct vector3 *point,
                          struct vector3 **best,
                          real *radius,
                          real *dist);

/**
 * @TODO
 */
struct vector3 *kdtree_nearest_neighbor(struct kdtree *kdt,
                                        struct vector3 *point);

/**
 * \brief Debugs a kdtree (number of points in a leaf)
 * \param kdt Target kdtree
 * \param output File to output the debug in
 */
void kdtree_debug(struct kdtree *kdt, FILE *output);

#endif // KDTREE_H

