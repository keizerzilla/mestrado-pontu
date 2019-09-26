#include "../include/kdtree.h"

struct kdtree *kdtree_new(struct vector3 *points, uint numpts, int axis)
{
	struct kdtree *kdt = malloc(sizeof(struct kdtree));
	if (kdt == NULL)
		return NULL;
	
	kdt->points = malloc(numpts * sizeof(struct vector3 *));
	if (kdt->points == NULL)
		return NULL;
	
	kdt->midpnt = vector3_zero();
	if (kdt->midpnt == NULL)
		return NULL;
	
	for (uint i = 0; i < numpts; i++) {
		kdt->points[i] = &points[i];
		
		kdt->midpnt->x += points[i].x;
		kdt->midpnt->y += points[i].y;
		kdt->midpnt->z += points[i].z;
	}
	
	kdt->midpnt->x /= numpts;
	kdt->midpnt->y /= numpts;
	kdt->midpnt->z /= numpts;
	
	kdt->numpts = numpts;
	kdt->axis = axis;
	kdt->left = NULL;
	kdt->right = NULL;
	
	return kdt;
}

void kdtree_free(struct kdtree **kdt)
{
	if (*kdt == NULL)
		return;
	
	vector3_free(&(*kdt)->midpnt);
	
	if ((*kdt)->points != NULL) {
		free((*kdt)->points);
		(*kdt)->points = NULL;
	}
	
	kdtree_free(&(*kdt)->left);
	kdtree_free(&(*kdt)->right);
	
	free(*kdt);
	*kdt = NULL;
}

void kdtree_partitionate(struct kdtree *kdt, int axis)
{
	kdt->axis = axis % 3;
	
	size_t size_node = kdt->numpts * sizeof(struct vector3 *);
	struct vector3 **left_points = malloc(size_node);
	struct vector3 **right_points = malloc(size_node);
	uint num_left = 0;
	uint num_right = 0;
	
	for (uint i = 0; i < kdt->numpts; i++) {
		if (kdt->points[i]->coord[kdt->axis] < kdt->midpnt->coord[kdt->axis]) {
			left_points[num_left] = kdt->points[i];
			num_left++;
		} else {
			right_points[num_right] = kdt->points[i];
			num_right++;
		}
	}
	
	kdt->left = kdtree_new(*left_points, num_left, axis + 1);
	free(left_points);
	kdt->right = kdtree_new(*right_points, num_right, axis + 1);
	free(right_points);
	
	if (num_left > 1)
		kdtree_partitionate(kdt->left, axis + 1);
	
	if (num_right > 1)
		kdtree_partitionate(kdt->right, axis + 1);
}

struct vector3 *kdtree_nearest_neighbor(struct kdtree *kdt, struct vector3 *p)
{
	if (kdt->left == NULL && kdt->right == NULL)
		return kdt->midpnt;
	
	real left_d = vector3_squared_distance(kdt->left->midpnt, p);
	real right_d = vector3_squared_distance(kdt->right->midpnt, p);
	
	if (left_d < right_d)
		return kdtree_nearest_neighbor(kdt->left, p);
	else
		return kdtree_nearest_neighbor(kdt->right, p);
}

void kdtree_debug(struct kdtree *kdt, FILE *output)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL) {
		fprintf(output, "node (%.4f, %.4f, %.4f), size: %d\n", kdt->midpnt->x,
		                                                       kdt->midpnt->y,
		                                                       kdt->midpnt->z,
		                                                       kdt->numpts);
	}
	
	kdtree_debug(kdt->left, output);
	kdtree_debug(kdt->right, output);
}

