#ifndef PLANE_H
#define PLANE_H

#include <stdlib.h>

#include "vector3.h"

/**
 * \brief Struct to store a plane with 3 dimensions
 */
struct plane {
	struct vector3* normal;
	struct vector3* point;
	real d;
};

/**
 * \brief Creates a new plane
 * \param normal Normal vector of the plane
 * \param point Reference point inside the plane
 * \return Pointer to a plane
 */
struct plane* plane_new (struct vector3* normal, struct vector3* point);

/**
 * \brief Frees a plane
 * \param plane Plane to be freed
 */
void plane_free (struct plane* plane);

/**
 * \brief Calculates the absolute distance between a plane and a point
 * \param plane The plane
 * \param point The point
 * \return The distance between the plane and the point
 */
real plane_distance2point (struct plane* plane, struct vector3* point);

/**
 * \brief Indicates if a point is in the positive region of the plane
 * \param plane Target plane
 * \param point Point to be verified
 * \return 1 if the point is inside the positive region, or
 * 0 if it isn't
 */
uint plane_on_direction (struct plane* plane, struct vector3* point);

/**
 * \brief Displays a plane (debug plane)
 * \param plane Plane to be displayed
 * \param out Output file
 */
void plane_debug (struct plane* plane, FILE* out);

#endif // PLANE_H
