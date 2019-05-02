/**
 * \file nosex.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Extrator de ponta do nariz
 */

#include "../src/include/cloud.h"
#include "../src/include/plane.h"
#include "../src/include/vector3.h"

/**
#define NOSEX_CUT 10
#define NOSEX_CYL 30

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("demo: paramentros insuficientes!\n");
		return 1;
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct plane* bestfit = cloud_plane_fitting(cloud);
	struct vector3* nosetip = cloud_max_distance_from_plane(cloud, bestfit);
	struct cloud* cylinder = cloud_cut_cylinder(cloud,
	                                            nosetip,
	                                            bestfit->normal,
	                                            NOSEX_CYL);
	
	struct cloud* sub_nosetip = cloud_cut_radius(cloud, nosetip, NOSEX_CUT);
	struct vector3* pce_nosetip = cloud_normal(sub_nosetip, nosetip);
	real curv_nosetip = cloud_curvature(sub_nosetip);
	
	struct cloud* n_nosecloud = cloud_empty();
	real c = 0.0f;
	real dot = 0.0f;
	for (int i = 0; i < cloud_size(cylinder); i++) {
		struct vector3* p = &cylinder->points[i];
		struct cloud* sub = cloud_cut_radius(cloud, p, NOSEX_CUT);
		struct vector3* pce = cloud_normal(sub, p);
		
		dot = fabs(vector3_dot(pce, pce_nosetip));
		p->alpha = cloud_curvature(sub);
		
		if (dot >= 0.99 && p->alpha > curv_nosetip && p->alpha > c) {
			c = p->alpha;
			cloud_add_point_vector(n_nosecloud, p);
		}
		
		vector3_free(pce);
		cloud_free(sub);
	}
	
	struct vector3* gigatip = vector3_from_vector(cloud_max_alpha(n_nosecloud));
	//struct cloud* show = cloud_cut_radius(cloud, megatip, NOSEX_CUT);
	//struct vector3* gigatip = cloud_max_distance_from_plane(show, bestfit);
	
	if (argc == 3) {
		struct cloud* nose_cloud = cloud_empty();
		cloud_add_point_vector(nose_cloud, gigatip);
		cloud_save_xyz(nose_cloud, argv[2]);
		cloud_free(nose_cloud);
	} else {
		vector3_debug(gigatip, stdout);
	}
	
	vector3_free(gigatip);
	//cloud_free(show);
	vector3_free(megatip);
	plane_free(bestfit);
	cloud_free(sub_nosetip);
	vector3_free(pce_nosetip);
	cloud_free(n_nosecloud);
	cloud_free(cylinder);
	vector3_free(nosetip);
	cloud_free(cloud);
	
	return 0;
}
*/

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("nosex: paramentros insuficientes!\n");
		return 1;
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct vector3* nosetip = cloud_point_faraway_bestfit(cloud);
	
	if (argc == 3) {
		struct cloud* nose_cloud = cloud_empty();
		
		cloud_add_point_vector(nose_cloud, nosetip);
		cloud_save_xyz(nose_cloud, argv[2]);
		
		cloud_free(nose_cloud);
	} else {
		vector3_debug(nosetip, stdout);
	}
	
	vector3_free(nosetip);
	cloud_free(cloud);
	
	return 0;
}

