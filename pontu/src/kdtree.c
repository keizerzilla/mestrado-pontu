#include "../include/kdtree.h"

struct kdtree *kdtree_new(struct kdtree *parent,
                          struct vector3 *points,
                          uint numpts,
                          int axis)
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
	
	kdt->parent = parent;
	kdt->numpts = numpts;
	kdt->axis = axis % 3;
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

void kdtree_partitionate(struct kdtree *kdt)
{
	size_t size_node = kdt->numpts * sizeof(struct vector3 *);
	struct vector3 **left_points = malloc(size_node);
	struct vector3 **right_points = malloc(size_node);
	uint nleft = 0;
	uint nright = 0;
	
	for (uint i = 0; i < kdt->numpts; i++) {
		if (kdt->points[i]->coord[kdt->axis] < kdt->midpnt->coord[kdt->axis]) {
			left_points[nleft] = kdt->points[i];
			nleft++;
		} else {
			right_points[nright] = kdt->points[i];
			nright++;
		}
	}
	
	if (nleft != 0)
		kdt->left = kdtree_new(kdt, *left_points, nleft, kdt->axis + 1);
	
	free(left_points);
	
	if (nright != 0)
		kdt->right = kdtree_new(kdt, *right_points, nright, kdt->axis + 1);
	
	free(right_points);
	
	if (nleft > 1)
		kdtree_partitionate(kdt->left);
	
	if (nright > 1)
		kdtree_partitionate(kdt->right);
}

// @TODO
struct kdtree *kdtree_closest_node(struct kdtree *kdt,
                                   struct vector3 *p,
                                   real *r)
{
	if (kdt->left == NULL && kdt->right == NULL)
		return kdt;
	
	if (p->coord[kdt->axis] < kdt->midpnt->coord[kdt->axis]) {
		*r = vector3_squared_distance(p, kdt->left->midpnt);
		return kdtree_closest_node(kdt->left, p, r);
	} else {
		*r = vector3_squared_distance(p, kdt->right->midpnt);
		return kdtree_closest_node(kdt->right, p, r);
	}
}

// @TODO
real kdtree_dist_hyperplane(struct kdtree *k1, struct kdtree *k2)
{
	return fabs(k1->midpnt->coord[k1->axis] - k2->midpnt->coord[k1->axis]);
}

// @TODO
void kdtree_closest_point(struct kdtree *node,
                          struct kdtree* current,
                          struct vector3 *p,
                          struct vector3 *b,
                          real *r)
{
	printf(">>>DEBUG 00\n");
	if (current == NULL)
		return;
	
	printf(">>>DEBUG 01\n");
	if (kdtree_dist_hyperplane(node, current) <= *r) {
		printf(">>>DEBUG 02\n");
		real d = vector3_squared_distance(p, current->midpnt);
		
		printf(">>>DEBUG 03\n");
		if (d < *r) {
			b = current->midpnt;
			*r = d;
		}
		
		printf(">>>DEBUG 04\n");
		kdtree_closest_point(node, current->left, p, b, r);
		printf(">>>DEBUG 05\n");
		kdtree_closest_point(node, current->right, p, b, r);
		
		printf(">>>DEBUG 06\n");
		if (current->left == NULL && current->right == NULL) {
			b = current->midpnt;
			printf(">>>DEBUG 07\n");
			return;
		}
		
	} else {
		printf(">>>DEBUG 08\n");
		return;
	}
}

// @TODO
struct vector3 *kdtree_nearest_neighbor(struct kdtree *kdt, struct vector3 *p)
{
	real r = 0.0f;
	struct kdtree *node = kdtree_closest_node(kdt, p, &r);
	struct vector3 *b = node->midpnt;
	
	kdtree_closest_point(node, kdt, p, b, &r);
	
	return b;
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

