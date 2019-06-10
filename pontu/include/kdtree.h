/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief 3D space partitioning using the kdtree data structure.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "vector3.h"
#include "cloud.h"

#define KDTREE_AXIS_X 0
#define KDTREE_AXIS_Y 1
#define KDTREE_AXIS_Z 2
#define KDTREE_MAXBUFFER 512

/**
 * \brief Struct to store a kdtree
 */
struct kdtree {
	real median;
	uint numpts;
	struct vector3 **points;
	struct kdtree *left;
	struct kdtree *right;
};

/**
 * \brief Initializes a kdtree
 * \return Pointer to the initialized kdtree
 */
struct kdtree *kdtree_new();

/**
 * \brief Frees a kdtree
 * \param kdt Kdtree to be freed
 */
void kdtree_free(struct kdtree *kdt);

/**
 * \brief Adds a point to the list of points in a node
 * \param kdt Target kdtree
 * \param p New point
 * \return NULL if it fails, or the new target node pointer
 */
struct kdtree *kdtree_add_point(struct kdtree *kdt, struct vector3 *p);

/**
 * \brief Partitionates recursively points of a cloud
 * \param kdt Target kdtree
 * \param axis The first axis to partitionate
 * \param depth The depth to particionate (how many times to partitionate)
 */
void kdtree_partitionate(struct kdtree *kdt, int axis, int depth);

/**
 * \brief Initializes a kdtree from a point cloud
 * \param kdt Target kdtree
 * \param cloud Base cloud
 * \return NULL if it fails, or the pointer to the kdtree if it doesn't
 */
struct kdtree *kdtree_init(struct kdtree *kdt, struct cloud *cloud);

/**
 * \brief Debugs a kdtree (number of points in a leaf)
 * \param kdt Target kdtree
 */
void kdtree_debug(struct kdtree *kdt);

/**
 * \brief Converts points of a node into a cloud
 * \param kdt Target kdtree
 * \return Cloud with the kdtree points
 */
struct cloud *kdtree_tocloud(struct kdtree *kdt);

/**
 * \brief Saves points of a leaf in a pcd file
 * \param kdt Target kdtree
 * \param path Path to the directory where the cloud will be saved
 */
void kdtree_tofile(struct kdtree *kdt, const char *path);

#endif // KDTREE_H

