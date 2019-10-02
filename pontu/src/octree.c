#include "../include/octree.h"

struct octree *octree_new(struct pointset *points, uint numpts, int depth)
{
	if (numpts == 0)
		return NULL;
	
	struct octree *oct = malloc(sizeof(struct octree));
	if (oct == NULL)
		return NULL;
	
	oct->points = pointset_copy(points);
	if (oct->points == NULL) {
		free(oct);
		return NULL;
	}
	
	oct->midpnt = vector3_zero();
	if (oct->midpnt == NULL)
		return NULL;
	
	for (struct pointset *set = points; set != NULL; set = set->next) {
		oct->midpnt->x += set->point->x;
		oct->midpnt->y += set->point->y;
		oct->midpnt->z += set->point->z;
	}
	
	oct->midpnt->x /= numpts;
	oct->midpnt->y /= numpts;
	oct->midpnt->z /= numpts;
	
	oct->numpts = numpts;
	oct->depth = depth;
	
	for (uint i = 0; i < 8; i++)
		oct->child[i] = NULL;
	
	return oct;
}

void octree_free(struct octree **oct)
{
	if (*oct == NULL)
		return;
	
	pointset_free(&(*oct)->points);
	vector3_free(&(*oct)->midpnt);
	
	for (uint i = 0; i < 8; i++)
		octree_free(&(*oct)->child[i]);
	
	free(*oct);
	*oct = NULL;
}

int octree_quadrant(struct octree *oct, struct vector3 *p)
{
	int q = 0;
	
	q = ((p->x > oct->midpnt->x) ? (q | (1 << OCTREE_AXIS_X)) : (q)) |
	    ((p->y > oct->midpnt->y) ? (q | (1 << OCTREE_AXIS_Y)) : (q)) |
	    ((p->z > oct->midpnt->z) ? (q | (1 << OCTREE_AXIS_Z)) : (q));
	
	return q;
}

void octree_partitionate(struct octree *oct)
{
	if (oct == NULL || oct->depth <= 0 || oct->numpts <= 1)
		return;
	
	struct pointset *points[8];
	uint numpts[8];
	
	for (uint k = 0; k < 8; k++) {
		points[k] = pointset_new();
		numpts[k] = 0;
	}
	
	for (struct pointset *s = oct->points; s != NULL; s = s->next) {
		int q = octree_quadrant(oct, s->point);
		
		pointset_insert(&points[q], s->point->x, s->point->y, s->point->z);
		numpts[q]++;
	}
	
	for (uint j = 0; j < 8; j++) {
		if (numpts[j] > 0) {
			oct->child[j] = octree_new(points[j], numpts[j], oct->depth - 1);
			
			pointset_free(&points[j]);
			octree_partitionate(oct->child[j]);
		}
	}
}

struct vector3 *octree_closest(struct octree *oct, struct vector3 *p)
{
	struct vector3 *closest = NULL;
	real dist = INFINITY;
	real temp = 0.0;
	
	for (struct pointset *s = oct->points; s != NULL; s = s->next) {
		temp = vector3_squared_distance(p, s->point);
		
		if (temp < dist) {
			dist = temp;
			closest = s->point;
		}
	}
	
	return closest;
}

struct vector3 *octree_nearest_neighbor(struct octree *oct, struct vector3 *p)
{
	if (oct->depth <= 0) {
		return octree_closest(oct, p);
	} else {
		int q = octree_quadrant(oct, p);
		
		if (oct->child[q] != NULL)
			return octree_nearest_neighbor(oct->child[q], p);
		else
			return octree_closest(oct, p);
	}
}

void octree_debug(struct octree *oct, FILE *output)
{
	if (oct == NULL)
		return;
	
	fprintf(output, "depth: %d | numpts: %u\n", oct->depth, oct->numpts);
	
	for (uint i = 0; i < 8; i++)
		octree_debug(oct->child[i], output);
}

