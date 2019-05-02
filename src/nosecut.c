/**
 * \file nosecut.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Corte da região do nariz
 */

#include "../src/include/cloud.h"

int main(int argc, char** argv)
{
	if (argc != 4) {
		printf("nosecut: paramentros insuficientes!\n");
		return 1;
	}
	
	struct vector3* zero = vector3_new(0.0f, 0.0f, 0.0f);
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct cloud* nose = cloud_load_xyz(argv[2]);
	struct cloud* region = cloud_cut_radius(cloud, &nose->points[0], 35);
	
	cloud_translate_vector(region, zero);
	cloud_save_xyz(region, argv[3]);
	
	cloud_free(region);
	cloud_free(nose);
	cloud_free(cloud);
	vector3_free(zero);
	
	return 0;
}

