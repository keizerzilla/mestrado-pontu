#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"
#include "../pontu_registration.h"

int main()
{
	/* Teste kdTree */
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
	
	
	/** Teste ICP
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	
	struct cloud *correspond = registration_closest_points_bf(source, target);
	cloud_save_ply(correspond, "../samples/bun_correspond_tutu.ply");
	
	cloud_free(&correspond);
	cloud_free(&source);
	cloud_free(&target);
	*/
	
	return 0;
}

