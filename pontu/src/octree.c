#include "../include/octree.h"

struct octree *octree_new(struct vector3 *points, uint numpts, int depth)
{
	struct octree *oct = malloc(sizeof(struct octree));
	if (oct == NULL)
		return NULL;
	
	oct->points = malloc(numpts * sizeof(struct vector3 *));
	if (oct->points == NULL)
		return NULL;
	
	oct->midpnt = vector3_zero();
	if (oct->midpnt == NULL)
		return NULL;
	
	for (uint i = 0; i < numpts; i++) {
		oct->points[i] = &points[i];
		
		oct->midpnt->x += points[i].x;
		oct->midpnt->y += points[i].y;
		oct->midpnt->z += points[i].z;
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

void octree_free(struct octree *oct)
{
	if (oct == NULL)
		return;
	
	vector3_free(oct->midpnt);
	
	if (oct->points != NULL) {
		free(oct->points);
		oct->points = NULL;
	}
	
	for (uint i = 0; i < 8; i++)
		octree_free(oct->child[i]);
	
	free(oct);
	oct = NULL;
}

int octree_quadrant(struct octree *oct, struct vector3 *p)
{
	int q = 0;
	
	q = ((p->x > oct->midpnt->x) ? (q | (1 << OCTREE_AXIS_X)) : (q)) |
	    ((p->y > oct->midpnt->y) ? (q | (1 << OCTREE_AXIS_Y)) : (q)) |
	    ((p->z > oct->midpnt->z) ? (q | (1 << OCTREE_AXIS_Z)) : (q));
	
	return q;
}

void octree_partitionate(struct octree *oct, int depth)
{
	// @TODO
	
	if (depth <= 0 || oct->numpts <= 1)
		return;
	
	struct vector3 **points[8];
	uint numpts[8];
	size_t size_node = oct->numpts * sizeof(struct vector3 *);
	
	for (uint k = 0; k < 8; k++) {
		points[k] = malloc(size_node);
		numpts[k] = 0;
	}
	
	for (uint i = 0; i < oct->numpts; i++) {
		struct vector3 *p = oct->points[i];
		int q = octree_quadrant(oct, p);
		
		points[q][numpts[q]] = p;
		numpts[q]++;
	}
	
	for (uint j = 0; j < 8; j++) {
		if (numpts[j] > 0) {
			oct->child[j] = octree_new(*points[j], numpts[j], depth - 1);
			octree_partitionate(oct->child[j], depth - 1);
		}
		
		free(points[j]);
	}
}

struct vector3 *octree_closest(struct octree *oct, struct vector3 *p)
{
	// @TODO
	
	if (oct->numpts == 1) {
		printf("vruco!\n");
		return oct->points[0];
	}
	
	struct vector3 *closest = oct->points[0];
	real dist = vector3_squared_distance(p, closest);
	
	for (uint i = 1; i < oct->numpts; i++) {
		struct vector3 *temp_closest = oct->points[i];
		real temp_dist = vector3_squared_distance(p, temp_closest);
		
		if (temp_dist < dist) {
			dist = temp_dist;
			closest = temp_closest;
		}
	}
	
	return closest;
}

struct vector3 *octree_nearest_neighbor(struct octree *oct, struct vector3 *p)
{
	if (oct == NULL || oct->depth <= 0)
		return octree_closest(oct, p);
	else
		return octree_nearest_neighbor(oct->child[octree_quadrant(oct, p)], p);
}

void octree_debug(struct octree *oct, FILE *output)
{
	if (oct == NULL)
		return;
	
	fprintf(output, "depth: %d | numpts: %u\n", oct->depth, oct->numpts);
	
	for (uint i = 0; i < 8; i++)
		octree_debug(oct->child[i], output);
}

