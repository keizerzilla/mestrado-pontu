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
	if (argc != 3) {
		printf("! paramentros insuficientes !\n");
		return 1;
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct vector3* p = cloud_point_faraway_bestfit(cloud);
	struct cloud* nose = cloud_empty();
	
	cloud_add_point_vector(nose, p);
	cloud_save_xyz(nose, argv[2]);
	
	cloud_free(nose);
	vector3_free(p);
	cloud_free(cloud);
	
	return 0;
}

