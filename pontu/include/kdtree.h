/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief 3D space partitioning using the kdtree data structure.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "./vector3.h"
#include "./pointset.h"

/**
 * @TODO
 */
struct kdtree {
	struct pointset *points;
	struct pointset *median;
	int axis;
	uint numpts;
	struct kdtree *left;
	struct kdtree *right;
};

/**
 * @TODO
 */
struct kdtree *kdtree_new(struct pointset *points, uint numpts, int axis);

/**
 * @TODO
 */
void kdtree_free(struct kdtree **kdt);

/**
 * @TODO
 */
void kdtree_partitionate(struct kdtree *kdt);

/**
 * @TODO
 */
real kdtree_dist_hyperplane(struct kdtree *k1, struct kdtree *k2);

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
 * @TODO
 */
void kdtree_debug(struct kdtree *kdt, FILE *output);

#endif // KDTREE_H

