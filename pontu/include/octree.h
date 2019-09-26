/**
 * \file octree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief 3D space partitioning using the octree data structure.
 */

#ifndef OCTREE_H
#define OCTREE_H

#define OCTREE_AXIS_X 2
#define OCTREE_AXIS_Y 1
#define OCTREE_AXIS_Z 0

#include "./vector3.h"

/**
 * \brief Structure to store a octree node
 */
struct octree {
	struct vector3 **points;
	uint numpts;
	struct vector3 *midpnt;
	int depth;
	struct octree *child[8];
};

/**
 * \brief Initializes a octree
 * \param points Vector of points
 * \param numpts Size of the points vector
 * \param depth Current depth of the octree node
 * \return NULL if it fails, or the pointer to the octree if it doesn't
 */
struct octree *octree_new(struct vector3 *points, uint numpts, int depth);

/**
 * \brief Frees a octree
 * \param oct octree to be freed
 */
void octree_free(struct octree **oct);

/**
 * \brief Calculates the quadrant of a point by the node's midpoint
 * \param oct The octree node
 * \param p The target point
 * \return Integer between 0 and 7
 */
int octree_quadrant(struct octree *oct, struct vector3 *p);

/**
 * \brief Partitionates the points of a cloud recursively
 * \param oct Target octree
 * \param depth The depth of the node
 */
void octree_partitionate(struct octree *oct, int depth);

/**
 * \brief Bruteforce search of the closest point in a quadrant
 * \param oct The octree node to search the points
 * \param p The point to look the closest
 * \return The address of the nearest point to p
 */
struct vector3 *octree_closest(struct octree *oct, struct vector3 *p);

/**
 * \brief Finds the point in the octree nearest to a target point
 * \param oct The target octree
 * \param p The target point
 * \return Address of the closest point to p in oct
 */
struct vector3 *octree_nearest_neighbor(struct octree *oct, struct vector3 *p);

/**
 * \brief Debugs a octree (number of points in a leaf)
 * \param oct Target octree
 * \param output File to output the debug in
 */
void octree_debug(struct octree *oct, FILE *output);

#endif // OCTREE_H

