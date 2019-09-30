/**
 * \file pointset.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief The fundamental data structure that holds a set of 3D Points.
 */

#ifndef POINTSET_H
#define POINTSET_H

#include "./vector3.h"

struct pointset {
	struct vector3 *point;
	struct pointset *next;
	struct pointset *prev;
};

struct pointset *pointset_new();

void pointset_add(struct pointset **set, real x, real y, real z);

struct pointset *pointset_tail(struct pointset *set);

struct pointset *pointset_partition(struct pointset *head,
                                    struct pointset *end,
                                    struct pointset **newhead,
                                    struct pointset **newend,
                                    int axis);

struct pointset *pointset_recursive_sort(struct pointset *head,
                                         struct pointset *end,
                                         int axis);

void pointset_sort(struct pointset **set, int axis);

void pointset_debug(struct pointset *set, FILE *out);

void pointset_debug_reverse(struct pointset *set, FILE *out);

void pointset_free(struct pointset **set);

#endif // POINTSET_H

