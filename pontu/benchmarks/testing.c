#include <time.h>
#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"
#include "../pontu_registration.h"

void to_cloud(struct kdtree *kdt, struct cloud *cloud)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL)
		cloud_insert_vector3(cloud, kdt->median->point);
	
	to_cloud(kdt->left, cloud);
	to_cloud(kdt->right, cloud);
}

int main()
{
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	struct cloud *closest_points = cloud_new();
	struct kdtree *kdt = kdtree_new(target->points, target->numpts, 0);
	
	kdtree_partitionate(kdt);
	
	
	struct vector3 *closest = NULL;
	for (struct pointset *set = source->points; set != NULL; set = set->next) {
		closest = kdtree_nearest_neighbor(kdt, set->point);
		
		if (closest == NULL)
			break;
		
		cloud_insert_vector3(closest_points, closest);
	}
	
	cloud_save_xyz(closest_points, "../samples/KDTUTU.xyz");
	kdtree_free(&kdt);
	cloud_free(&closest_points);
	cloud_free(&source);
	cloud_free(&target);
	
	/**
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *cloud = cloud_new();
	struct kdtree *kdt = kdtree_new(target->points, target->numpts, 0);
	
	kdtree_partitionate(kdt);
	to_cloud(kdt, cloud);
	cloud_save_xyz(cloud, "../samples/to_cloud.xyz");
	
	printf("target: %u\ncloud: %u\n", target->numpts, cloud->numpts);
	
	kdtree_free(&kdt);
	cloud_free(&cloud);
	cloud_free(&target);
	*/
	
	return 0;
}

