#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

struct cloud *cloud_gaussian_filter(struct cloud *cloud, real sig)
{
	struct cloud *blured = cloud_copy(cloud);
	if (blured == NULL)
		return NULL;
	
	struct vector3 *center = cloud_get_center(blured);
	
	for (uint i = 0; i < blured->numpts; i++) {
		real d = vector3_distance(center, &blured->points[i]);
		real bd = calc_gaussian(d, sig);
		vector3_scale(&blured->points[i], bd);
		
		//blured->points[i].x *= calc_gaussian(blured->points[i].x, sig);
		//blured->points[i].y *= calc_gaussian(blured->points[i].y, sig);
		//blured->points[i].z *= calc_gaussian(blured->points[i].z, sig);
	}
	
	vector3_free(center);
	
	return blured;
}

real cloud_nmbe(struct cloud *cloud, real sig, struct vector3 *center, real rad)
{
	real nmbe = 0.0f;
	struct cloud *sub = cubic_voxel_grid(cloud, sig);
	struct vector3 *p = NULL;
	
	for (uint i = 0; i < sub->numpts; i++) {
		p = &sub->points[i];
		
		struct vector3 *dir = vector3_sub(p, center);
		real len = vector3_length(dir);
		
		nmbe += pow(rad - len, 2);
		
		vector3_free(dir);
	}
	
	cloud_free(sub);
	
	return nmbe / (1.0f * sub->numpts * rad);
}

int main()
{
	struct cloud *bunny = cloud_load_xyz("../samples/bunny.xyz");
	struct cloud *blured = cloud_gaussian_filter(bunny, 0.2f);
	cloud_save_pcd(blured, "blured_bunny.pcd");
	cloud_free(blured);
	cloud_free(bunny);
	
	/**
	char *clouds[4] = {
		"../samples/bunny.xyz",
		"../samples/bunny_trans.xyz",
		"../samples/bunny_scale.xyz",
		"../samples/bunny_rotate.xyz"
	};
	
	for (int i = 0; i < 4; i++) {
		printf("> %s\n", clouds[i]);
		
		struct cloud *cloud = cloud_load_xyz(clouds[i]);
		struct vector3 *center = vector3_zero();
		real rad = 0.0f;
		
		cloud_ritter(cloud, &center, &rad);
		
		for (real sig = 0.02f; sig <= 0.2f; sig += 0.02f) {
			real nmbe = calc_logt(cloud_nmbe(cloud, sig, center, rad));
			
			printf("sig = %.4f | nmbe = %.4f\n", sig, nmbe);
		}
		
		cloud_free(cloud);
		vector3_free(center);
	}
	*/
	
	return 0;
}

