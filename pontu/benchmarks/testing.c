#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"
#include "../pontu_registration.h"

int main()
{
	/* Teste kd-tree */
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	
	struct cloud *closest_points = cloud_new(source->numpts);
	if (closest_points == NULL)
        return 1;
	
	cloud_partitionate(target);
	
	struct vector3 *closest = NULL;
	for (uint i = 0; i < source->numpts; i++) {
		closest = kdtree_nearest_neighbor(target->kdt, &source->points[i]);
		cloud_set_point_vector(closest_points, i, closest);
	}
	
	cloud_save_xyz(closest_points, "../samples/bun_closest_kdtutu.xyz");
	
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

