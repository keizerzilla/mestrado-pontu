/**
 * \file nosex.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Extrator de ponta do nariz
 */

#include "../src/include/cloud.h"
#include "../src/include/plane.h"
#include "../src/include/vector3.h"

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

