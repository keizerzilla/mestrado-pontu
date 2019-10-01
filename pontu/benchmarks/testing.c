#include <time.h>
#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"
#include "../pontu_registration.h"

void to_cloud(struct kdtree *kdt, struct cloud *cloud)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL) {
		cloud_insert_vector3(cloud, kdt->midpnt);
		return;
	}
	
	to_cloud(kdt->left, cloud);
	to_cloud(kdt->right, cloud);
}

int main()
{
	srand(time(NULL));
	
	struct pointset *set = pointset_new();
	uint n = 6;
	
	for (uint i = 0; i < n; i++)
		pointset_insert(&set, rand() % 10, rand() % 10, rand() % 10);
	
	
	vector3_debug(pointset_median(set, VECTOR3_AXIS_X, n)->point, stdout);
	printf("--------------------------\n");
	pointset_debug(set, stdout);
	
	pointset_free(&set);
	
	/**
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct kdtree *kdt = kdtree_new(NULL, target->points, target->numpts, 0);
	kdtree_partitionate(kdt);
	
	struct cloud *cloud = cloud_empty();
	
	to_cloud(kdt, cloud);
	printf("size target: %u\nsize cloud: %u\n", target->numpts, cloud->numpts);
	
	cloud_save_xyz(cloud, "../samples/to_cloud.xyz");
	
	cloud_free(&cloud);
	kdtree_free(&kdt);
	cloud_free(&target);
	*/
	
	/**
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	
	struct cloud *closest_points = cloud_new(source->numpts);
	if (closest_points == NULL)
        return 1;
	
	struct kdtree *kdt = kdtree_new(NULL, target->points, target->numpts, 0);
	kdtree_partitionate(kdt);
	
	struct vector3 *closest = NULL;
	for (uint i = 0; i < source->numpts; i++) {
		closest = kdtree_nearest_neighbor(kdt, &source->points[i]);
		
		if (closest == NULL)
			printf("CLOSEST NULL!!!\n");
		
		cloud_set_point_vector(closest_points, i, closest);
	}
	
	cloud_save_xyz(closest_points, "../samples/KDTUTU.xyz");
	
	kdtree_free(&kdt);
	cloud_free(&closest_points);
	cloud_free(&source);
	cloud_free(&target);
	*/
	
	return 0;
}

