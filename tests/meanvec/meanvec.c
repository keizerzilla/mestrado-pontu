#include "../../src/include/cloud.h"

int main(int argc, char** argv)
{
	// checa linha de comando
	if (argc != 2) {
		printf("faltando argumento arquivo nuvem!");
		exit(1);
	}
	
	// carrega nuvem
	printf("carregando [%s]\n", argv[1]);
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	if (cloud == NULL) {
		printf("abort\n");
		exit(1);
	}
	
	// calcula vetores direcao e os salva em subnuvem
	struct vector3* centroid = cloud_get_center(cloud);
	struct cloud* sub = cloud_new(0);
	for (uint i = 0; i < cloud->num_pts; i++)
		cloud_add_point_cpy(sub, vector3_sub(&cloud->points[i], centroid));
	
	vector3_debug(centroid, "centroid nuvem original", stdout);
	vector3_debug(cloud_get_center(sub), "centroid subnuvem", stdout);
	
	// calcula centroid da subnuvem = vetor direcao medio
	struct vector3* mvec = cloud_get_center(sub);
	vector3_debug(mvec, "direcao media", stdout);
	
	// libera memoria
	vector3_free(mvec);
	cloud_free(sub);
	vector3_free(centroid);
	cloud_free(cloud);
	
	// fin
	return 0;
}

