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
void pointset_free(struct pointset **set);

/**
 * @TODO
 */
struct vector3 *pointset_insert(struct pointset **set, real x, real y, real z);

/**
 * @TODO
 */
struct pointset *pointset_copy(struct pointset *set);

/**
 * @TODO
 */
struct pointset *pointset_tail(struct pointset *set);

/**
 * @TODO
 */
struct pointset *pointset_segment(struct pointset *begin,
                                  struct pointset *end,
                                  uint *size);

/**
 * @TODO
 */
struct pointset *pointset_segment_reverse(struct pointset *begin,
                                          struct pointset *end,
                                          uint *size);

/**
 * @TODO
 */
uint pointset_size(struct pointset *set);

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
struct pointset *pointset_median(struct pointset *set, int axis, uint size);

/**
 * @TODO
 */
void pointset_debug(struct pointset *set, FILE *output);

/**
 * @TODO
 */
void pointset_debug_reverse(struct pointset *set, FILE *output);

#endif // POINTSET_H

