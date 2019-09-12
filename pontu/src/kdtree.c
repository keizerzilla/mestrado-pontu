#include "../include/kdtree.h"

struct kdtree *kdtree_new(struct vector3 *points, uint numpts)
{
	struct kdtree *kdt = malloc(sizeof(struct kdtree));
	if (kdt == NULL)
		return NULL;
	
	kdt->points = malloc(numpts * sizeof(struct vector3 *));
	if (kdt->points == NULL)
		return NULL;
	
	kdt->median = vector3_zero();
	if (kdt->median == NULL)
		return NULL;
	
	for (uint i = 0; i < numpts; i++) {
		kdt->points[i] = &points[i];
		
		kdt->median->x += points[i].x;
		kdt->median->y += points[i].y;
		kdt->median->z += points[i].z;
	}
	
	kdt->median->x /= numpts;
	kdt->median->y /= numpts;
	kdt->median->z /= numpts;
	kdt->numpts = numpts;
	kdt->axis = 0;
	kdt->left = NULL;
	kdt->right = NULL;
	
	return kdt;
}

void kdtree_free(struct kdtree *kdt)
{
	if (kdt == NULL)
		return;
	
	vector3_free(kdt->median);
	
	if (kdt->points != NULL)
		free(kdt->points);
	
	kdtree_free(kdt->left);
	kdtree_free(kdt->right);
	
	free(kdt);
	
	kdt = NULL;
}

void kdtree_partitionate(struct kdtree *kdt, int axis, int depth)
{
	if (depth <= 0)
		return;
	
	kdt->axis = axis % 3;
	
	size_t size_node = kdt->numpts * sizeof(struct vector3 *);
	struct vector3 **left_points = malloc(size_node);
	struct vector3 **right_points = malloc(size_node);
	uint num_left = 0;
	uint num_right = 0;
	
	for (uint i = 0; i < kdt->numpts; i++) {
		if (kdt->points[i]->coord[kdt->axis] < kdt->median->coord[kdt->axis]) {
			left_points[num_left] = kdt->points[i];
			num_left++;
		} else {
			right_points[num_right] = kdt->points[i];
			num_right++;
		}
	}
	
	kdt->left = kdtree_new(*left_points, num_left);
	kdt->right = kdtree_new(*right_points, num_right);
	
	free(left_points);
	free(right_points);
	
	kdtree_partitionate(kdt->left, axis + 1, depth - 1);
	kdtree_partitionate(kdt->right, axis + 1, depth - 1);
	
	// hmmm...
	free(kdt->points);
	kdt->points = NULL;
}

struct vector3 *kdtree_nearest_point(struct kdtree *kdt, struct vector3* p)
{
	if (kdt->numpts == 0)
		return kdt->median;
	
	if (p->coord[kdt->axis] < kdt->median->coord[kdt->axis]) {
		if (kdt->left != NULL) {
			return kdtree_nearest_point(kdt->left, p);
		} else if (kdt->right != NULL) {
			return kdtree_nearest_point(kdt->right, p);
		} else {
			return vector3_closest_to_list(kdt->points, kdt->numpts, p);
		}
	} else {
		if (kdt->right != NULL) {
			return kdtree_nearest_point(kdt->right, p);
		} else if (kdt->left != NULL) {
			return kdtree_nearest_point(kdt->left, p);
		} else {
			return vector3_closest_to_list(kdt->points, kdt->numpts, p);
		}
	}
}

void kdtree_debug(struct kdtree *kdt, FILE *output)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL)
		fprintf(output, "node (%.4f, %.4f, %.4f) | size: %d\n", kdt->median->x,
		                                                        kdt->median->y,
		                                                        kdt->median->z,
		                                                        kdt->numpts);

	kdtree_debug(kdt->left, output);
	kdtree_debug(kdt->right, output);
}

