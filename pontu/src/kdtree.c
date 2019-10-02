#include "../include/kdtree.h"

struct kdtree *kdtree_new(struct pointset *points, uint numpts, int axis)
{
	if (numpts == 0)
		return NULL;
	
	struct kdtree *kdt = malloc(sizeof(struct kdtree));
	if (kdt == NULL)
		return NULL;
	
	kdt->points = pointset_copy(points);
	if (kdt->points == NULL) {
		free(kdt);
		return NULL;
	}
	
	kdt->median = pointset_median(kdt->points, axis % 3, numpts);
	
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
	
	pointset_free(&(*kdt)->points);
	
	kdtree_free(&(*kdt)->left);
	kdtree_free(&(*kdt)->right);
	
	free(*kdt);
	*kdt = NULL;
}

void kdtree_partitionate(struct kdtree *kdt)
{
	if (kdt == NULL)
		return;
	
	if (kdt->numpts <= 1)
		return;
	
	uint numpts_left = 0;
	uint numpts_right = 0;
	struct pointset *tail = pointset_tail(kdt->points);
	struct pointset *left_points = pointset_segment(kdt->points,
	                                                kdt->median->next,
	                                                &numpts_left);
	                                                
	struct pointset *right_points = pointset_segment_reverse(tail,
	                                                         kdt->median,
	                                                         &numpts_right);
	
	kdt->left = kdtree_new(left_points, numpts_left, kdt->axis + 1);
	pointset_free(&left_points);
	
	kdt->right = kdtree_new(right_points, numpts_right, kdt->axis + 1);	
	pointset_free(&right_points);
	
	kdtree_partitionate(kdt->left);
	kdtree_partitionate(kdt->right);
}

real kdtree_dist_hyperplane(struct kdtree *k1, struct kdtree *k2)
{
	real d = fabs(k1->median->point->coord[k1->axis] -
	              k2->median->point->coord[k1->axis]);
	
	return d;
}

struct kdtree *kdtree_closest_node(struct kdtree *kdt,
                                   struct vector3 *p,
                                   real *r)
{
	*r = vector3_squared_distance(p, kdt->median->point);
	
	if (kdt->left == NULL && kdt->right == NULL)
		return kdt;
	
	if (p->coord[kdt->axis] < kdt->median->point->coord[kdt->axis]) {
		if (kdt->left != NULL)
			return kdtree_closest_node(kdt->left, p, r);
		else
			return kdt;
	} else {
		if (kdt->right != NULL)
			return kdtree_closest_node(kdt->right, p, r);
		else
			return kdt;
	}
}

void kdtree_closest_point(struct kdtree *node,
                          struct kdtree* current,
                          struct vector3 *point,
                          struct vector3 **best,
                          real *radius,
                          real *dist)
{
	if (current == NULL)
		return;
	
	if (kdtree_dist_hyperplane(node, current) < *radius) {
		real d = vector3_squared_distance(point, current->median->point);
		
		if (d < *dist) {
			*best = current->median->point;
			*dist = d;
		}
	}
	
	kdtree_closest_point(node, current->left, point, best, radius, dist);
	kdtree_closest_point(node, current->right, point, best, radius, dist);
}
struct vector3 *kdtree_nearest_neighbor(struct kdtree *kdt,
                                        struct vector3 *point)
{
	real radius = 0.0f;
	struct kdtree *node = kdtree_closest_node(kdt, point, &radius);
	return node->median->point;
}

void kdtree_debug(struct kdtree *kdt, FILE *output)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL) {
		fprintf(output,
		        "node (%.4f, %.4f, %.4f), size: %d\n",
		        kdt->median->point->x,
		        kdt->median->point->y,
		        kdt->median->point->z,
		        kdt->numpts);
	}
	
	kdtree_debug(kdt->left, output);
	kdtree_debug(kdt->right, output);
}

