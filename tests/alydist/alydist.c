#include "../../src/include/cloud.h"

int main(int argc, char** argv)
{
	if (argc != 4) {
		printf("faltando argumentos\n");
		exit(1);
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	cloud_sort(cloud);
	
	struct vector3* start = vector3_from_vector(&cloud->points[0]);
	struct vector3* end = vector3_from_vector(&cloud->points[9]);
	
	struct cloud* endpoints = cloud_empty();
	cloud_add_point(endpoints, start);
	cloud_add_point(endpoints, end);
	cloud_save_xyz(endpoints, argv[3]);
	
	struct cloud* slice = cloud_empty();
	cloud_riemann_segment(cloud, start, end, slice);
	cloud_save_xyz(slice, argv[2]);
	
	vector3_debug(start, stdout);
	vector3_debug(end, stdout);
	
	real ed = vector3_distance(start, end);
	printf("EUCLIDEAN DISTANCE:\t%.4f\n", ed);
	real rd = cloud_riemann_distance(slice, start, end);
	printf("RIEMANN DISTANCE:\t%.4f\n\n", rd);
	
	cloud_free(slice);
	cloud_free(endpoints);
	vector3_free(end);
	vector3_free(start);
	cloud_free(cloud);
	
	return 0;
}

