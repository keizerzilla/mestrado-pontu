/**
 * \file pointset.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief The fundamental data structure that holds a set of 3D Points (DLL)
 */

#ifndef POINTSET_H
#define POINTSET_H

#include "./vector3.h"

/**
 * @TODO
 */
struct pointset {
	struct vector3 *point;
	struct pointset *next;
	struct pointset *prev;
};

/**
 * @TODO
 */
struct pointset *pointset_new();

/**
 * @TODO
 */
void pointset_insert_real(struct pointset **set, real x, real y, real z);

/**
 * @TODO
 */
void pointset_insert_vector3(struct pointset **set, struct vector3 *v);

/**
 * @TODO
 */
struct vector3 *pointset_next(struct pointset *set);

/**
 * @TODO
 */
struct pointset *pointset_tail(struct pointset *set);

/**
 * @TODO
 */
void pointset_swap(struct vector3 **a, struct vector3 **b);

/**
 * @TODO
 */
struct pointset *pointset_partition(struct pointset *head,
                                    struct pointset *end,
                                    int axis);

/**
 * @TODO
 */
void pointset_recursive_sort(struct pointset *head,
                             struct pointset *end,
                             int axis);

/**
 * @TODO
 */
void pointset_sort(struct pointset *set, int axis);

/**
 * @TODO
 */
void pointset_debug(struct pointset *set, FILE *out);

/**
 * @TODO
 */
void pointset_debug_reverse(struct pointset *set, FILE *out);

/**
 * @TODO
 */
void pointset_free(struct pointset **set);

#endif // POINTSET_H

