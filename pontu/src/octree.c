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

void octree_partitionate(struct octree *oct, int depth)
{
	if (oct == NULL || depth <= 0)
		return;
	
	
	// @TODO
}

struct vector3 *octree_nearest_neighbor(struct octree *oct, struct vector3 *p)
{
	if (oct == NULL || p == NULL)
		return NULL;
	
	// @TODO
	
	return NULL;
}

void octree_debug(struct octree *oct, FILE *output)
{
	if (oct == NULL)
		return;
	
	fprintf(output, "depth: %04d | numpts: %u\n", oct->depth, oct->numpts);
	
	for (uint i = 0; i < 8; i++)
		octree_debug(oct->child[i], output);
}

