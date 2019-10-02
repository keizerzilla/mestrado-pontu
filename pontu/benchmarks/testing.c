#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"
#include "../pontu_registration.h"

void pointset_test()
{
	struct pointset *set = pointset_new();
	int n = 10;
	
	for (int i = 0; i < n; i++)
		pointset_insert(&set, i, i, i);
	
	pointset_debug(set, stdout);
	
	pointset_free(&set);
}

void kdtree_to_cloud(struct kdtree *kdt, struct cloud *cloud)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL)
		cloud_insert_vector3(cloud, kdt->median->point);
	
	kdtree_to_cloud(kdt->left, cloud);
	kdtree_to_cloud(kdt->right, cloud);
}

void registration_test()
{
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *aligned = NULL;
	struct matrix *rt = registration_icp(source,
	                                     target,
	                                     &aligned,
	                                     0.00001,
	                                     30,
	                                     &registration_closest_points_tree);
	
	matrix_debug_real(rt, stdout);
	
	cloud_save_xyz(aligned, "../samples/icp_test.xyz");
	
	matrix_free(&rt);
	cloud_free(&aligned);
	cloud_free(&target);
	cloud_free(&source);
}

void kdtree_test()
{
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *cloud = cloud_new();
	struct kdtree *kdt = kdtree_new(target->points, target->numpts, 0);
	
	kdtree_partitionate(kdt);
	kdtree_to_cloud(kdt, cloud);
	cloud_save_xyz(cloud, "../samples/KDTREE_TEST.xyz");
	
	printf("target: %u\ncloud: %u\n", target->numpts, cloud->numpts);
	
	kdtree_free(&kdt);
	cloud_free(&cloud);
	cloud_free(&target);
}

void octree_test()
{
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	struct cloud *closest_points = cloud_new();
	struct octree *oct = octree_new(target->points, target->numpts, 4);
	
	octree_partitionate(oct);
	
	struct vector3 *closest = NULL;
	for (struct pointset *set = source->points; set != NULL; set = set->next) {
		closest = octree_nearest_neighbor(oct, set->point);
		
		if (closest == NULL)
			break;
		
		cloud_insert_vector3(closest_points, closest);
	}
	
	cloud_save_xyz(closest_points, "../samples/OCTREE_TEST.xyz");
	octree_free(&oct);
	cloud_free(&closest_points);
	cloud_free(&source);
	cloud_free(&target);
}

int main()
{
	//octree_test();
	//kdtree_test();
	registration_test();
	pointset_test();
	
	return 0;
}

