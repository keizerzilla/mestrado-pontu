/**
 * \file normx.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Cálcula plano de melhor ajuste a uma nuvem e retorna normal
 */

#include "../src/include/cloud.h"
#include "../src/include/plane.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("normx: paramentros insuficientes!\n");
		return 1;
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct plane* bestfit = cloud_plane_fitting(cloud);
	
	vector3_debug(bestfit->normal, stdout);
	
	plane_free(bestfit);
	cloud_free(cloud);
	
	return 0;
}

