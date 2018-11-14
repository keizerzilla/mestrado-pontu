#include "../../src/include/cloud.h"

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("faltando argumentos\n");
		exit(1);
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct vector3* start = cloud_min_z(cloud);
	struct vector3* end = NULL;
	real rd = 0.0f;
	FILE* dump = fopen(argv[2], "w");
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		end = &cloud->points[i];
		rd = cloud_riemann_distance(cloud, start, end);
		fprintf(dump, "%f\n", rd);
	}
	
	fclose(dump);
	vector3_free(start);
	cloud_free(cloud);
	
	return 0;
}

