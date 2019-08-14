#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

// https://www.inf.pucrs.br/~manssour/OpenGL/Programando3D.html
// http://www.lighthouse3d.com/tutorials/glut-tutorial/
// SUPER UNDER CONSTRUCTION CAUTION WARNING KEEP OUT DANGER
// https://stackoverflow.com/questions/16619327/gaussian-blur-of-3d-data
struct cloud *cloud_gaussian_filter(struct cloud *cloud, real s)
{
	struct vector3 *center = cloud_get_center(cloud);
	struct cloud *blured = cloud_copy(cloud);
	if (blured == NULL)
		return NULL;
	
	for (uint i = 0; i < cloud->numpts; i++) {
		struct vector3 *p_cloud = &cloud->points[i];

		struct cloud *sub = cloud_cut_radius(cloud, p_cloud, s);
		
		real mean = 0.0f;
		real weight_sum = 0.0f;
		
		for (uint j = 0; j < sub->numpts; j++) {
			struct vector3 *p_sub = &sub->points[j];

			real dist = vector3_distance(p_sub, center);
			//real weight = calc_gaussian(dist, s);
			
			real weight = calc_gaussian3(p_sub->x, p_sub->y, p_sub->z, s);
			
			weight_sum += weight;
			mean += dist * weight;
			//mean += dist;
		}
		
		mean /= weight_sum;
		
		vector3_scale(&blured->points[i], mean);
		
		cloud_free(sub);
		
		printf("DEBUG: point #%u ok!\n", i);
	}
	
	vector3_free(center);
	
	return blured;
}

real cloud_nmbe(struct cloud *cloud, struct vector3 *center, real rad)
{
	real nmbe = 0.0f;
	
	struct vector3 *p = NULL;
	for (uint i = 0; i < cloud->numpts; i++) {
		p = &cloud->points[i];
		
		struct vector3 *dir = vector3_sub(p, center);
		real len = vector3_length(dir);
		
		nmbe += pow(rad - len, 2);
		
		vector3_free(dir);
	}
	
	return nmbe / (1.0f * cloud->numpts * rad);
}

struct cloud *cloud_gtutu(struct cloud *cloud, real s)
{
	struct vector3 *center = cloud_get_center(cloud);
	if (center == NULL)
		return NULL;
	
	struct cloud *blured = cloud_copy(cloud);
	if (blured == NULL)
		return NULL;
	
	struct vector3 *p = NULL;
	real d = 0.0f;
	real f = 0.0f;
	for (uint i = 0; i < blured->numpts; i++) {
		p = &blured->points[i];
		d = vector3_distance(p, center);
		f = fabs(calc_gaussian(d, s));
		
		vector3_scale(p, f);
	}
	
	return blured;
}

int main(int argc, char** argv)
{
	struct cloud *bunny = cloud_load_xyz("../samples/bunny.xyz");
	struct cloud *blured = cloud_gtutu(bunny, 0.05f);
	
	cloud_save_pcd(blured, "blured_bunny.pcd");
	
	cloud_free(blured);
	cloud_free(bunny);
	
	return 0;
}

