#include "../include/plane.h"

struct plane *plane_new(struct vector3 *normal, struct vector3 *point)
{
	struct plane *plane = malloc(sizeof(struct plane));
	if (plane == NULL)
		return NULL;

	plane->normal = vector3_from_vector(normal);
	plane->point = vector3_from_vector(point);
	plane->d = -1 * ((normal->x * point->x) +
			         (normal->y * point->y) +
			         (normal->z * point->z));

	return plane;
}

void plane_free(struct plane **plane)
{
	if (*plane == NULL)
		return;

	vector3_free(&(*plane)->normal);
	vector3_free(&(*plane)->point);
	free(*plane);
	*plane = NULL;
}

real plane_distance2point(struct plane *plane, struct vector3 *point)
{
	struct vector3 *proj = vector3_sub(point, plane->point);
	real d = vector3_dot(proj, plane->normal) / vector3_length(plane->normal);
	
	vector3_free(&proj);
	
	return fabs(d);
}

uint plane_on_direction(struct plane *plane, struct vector3 *point)
{
	struct vector3 *proj = vector3_sub(point, plane->point);
	real d = vector3_dot(proj, plane->normal);
	
	vector3_free(&proj);
	
	return (d >= 0.0f) ? 1 : 0;
}

void plane_debug(struct plane *plane, FILE *out)
{
	if (plane == NULL) {
		fprintf(out, "!!! plane empty !!!\n");
		return;
	}

	fprintf(out, "P(%.4f, %.4f, %.4f)|N(%.4f, %.4f, %.4f)", plane->point->x,
		                                                    plane->point->y,
		                                                    plane->point->z,
		                                                    plane->normal->x,
		                                                    plane->normal->y,
		                                                    plane->normal->z);
}

