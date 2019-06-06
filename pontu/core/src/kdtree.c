#include "../kdtree.h"

struct kdtree* kdtree_new ()
{
	struct kdtree* kdt = malloc(sizeof(struct kdtree));
	if (!kdt)
		return NULL;
	
	kdt->median = 0.0f;
	kdt->numpts = 0;
	kdt->points = NULL;
	kdt->left = NULL;
	kdt->right = NULL;
	
	return kdt;
}

void kdtree_free (struct kdtree* kdt)
{
	if (!kdt)
		return;
	
	free(kdt->points);
	kdtree_free(kdt->left);
	kdtree_free(kdt->right);
	
	free(kdt);
}

struct kdtree* kdtree_add_point (struct kdtree* kdt, struct vector3* p)
{
	uint new_size = (kdt->numpts + 1) * sizeof(struct vector3*);

    struct vector3** new_points = realloc(kdt->points, new_size);
    if (!new_points )
        return NULL;
	
    kdt->points = new_points;
    kdt->numpts++;
    
    kdt->points[kdt->numpts - 1] = p;
    
    return kdt;
}

void kdtree_partitionate (struct kdtree* kdt, int axis, int depth)
{
	if (depth <= 0)
		return;
	
	int current_axis = axis % 3;
	real median = 0.0f;
	
	for (uint i = 0; i < kdt->numpts; i++)
		median += kdt->points[i]->coord[current_axis];
	
	median /= kdt->numpts;
	
	kdt->median = median;
	kdt->left = kdtree_new();
	kdt->right = kdtree_new();
	
	for (uint i = 0; i < kdt->numpts; i++) {
		if (kdt->points[i]->coord[axis] < median)
			kdtree_add_point(kdt->left, kdt->points[i]);
		else
			kdtree_add_point(kdt->right, kdt->points[i]);
	}
	
	kdtree_partitionate(kdt->left, axis + 1, depth - 1);
	kdtree_partitionate(kdt->right, axis + 1, depth - 1);
}

struct kdtree* kdtree_init (struct kdtree* kdt, struct cloud* cloud)
{
	if (!kdt)
		kdt = kdtree_new();
	
	int size = cloud_size(cloud);
	kdt->points = malloc(size * sizeof(struct vector3*));
	if (!kdt->points)
		return NULL;
	
	for (int i = 0; i < size; i++)
		kdt->points[i] = &cloud->points[i];
	
	kdt->numpts = size;		
	
	return kdt;
}

void kdtree_debug (struct kdtree* kdt)
{
	if (!kdt)
		return;
	
	if (!kdt->left && !kdt->right)
		printf("numpts = %d\n", kdt->numpts);
	
	kdtree_debug(kdt->left);
	kdtree_debug(kdt->right);
}

struct cloud* kdtree_tocloud (struct kdtree* kdt)
{
	struct cloud* cloud = cloud_empty();
		
	for (uint i = 0; i < kdt->numpts; i++)
		cloud_add_point_vector(cloud, kdt->points[i]);
	
	return cloud;
}

void kdtree_tofile (struct kdtree* kdt, const char* path)
{
	if (!kdt)
		return;
	
	if (!kdt->left && !kdt->right) {
		char buffer[80];
		sprintf(buffer, "%sleaf_%p.pcd", path, kdt);
		
		struct cloud* cloud = kdtree_tocloud(kdt);
		cloud_save_pcd(cloud, buffer);
		cloud_free(cloud);
		
		printf("%s ok!\n", buffer);
	}
	
	kdtree_tofile(kdt->left, path);
	kdtree_tofile(kdt->right, path);
}
