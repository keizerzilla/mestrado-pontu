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
	struct vector3* dir = vector3_zero();
	for (uint i = 0; i < cloud->num_pts; i++) {
		//vector3_increase(dir, vector3_sub(&cloud->points[i], centroid));
		vector3_increase(dir, &cloud->points[i]);
		vector3_debug(dir, "temp dir", stdout);
	}
	
	vector3_normalize(centroid);
	vector3_normalize(dir);
	vector3_debug(centroid, "centroid nuvem original", stdout);
	vector3_debug(dir, "vetor direcao?", stdout);
	
	// libera memoria
	vector3_free(dir);
	vector3_free(centroid);
	cloud_free(cloud);
	
	// fin
	return 0;
}

