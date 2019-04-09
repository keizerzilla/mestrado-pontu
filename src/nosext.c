/**
 * \file nosext.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Extrator de ponta do nariz.
 */

#include "../src/include/cloud.h"
#include "../src/include/plane.h"
#include "../src/include/vector3.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("! paramentros insuficientes !\n");
		return 1;
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct plane* plane = cloud_plane_fitting(cloud);
	struct vector3* p = cloud_max_distance_from_plane(cloud, plane);
	
	vector3_debug(plane->normal, stdout);
	vector3_debug(p, stdout);
	
	vector3_free(p);
	plane_free(plane);
	cloud_free(cloud);
	
	return 0;
}

