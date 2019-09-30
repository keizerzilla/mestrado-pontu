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
	struct pointset *front;
	struct pointset *back;
};

struct pointset *pointset_new();

void pointset_add(struct pointset **set, real x, real y, real z);

void pointset_debug(struct pointset *set, FILE *out);

void pointset_free(struct pointset **set);

#endif // POINTSET_H

