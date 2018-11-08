#include "../../src/include/cloud.h"
#include "../../src/include/hu.h"

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("faltando argumentos\n");
		exit(1);
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	//struct vector3* point = cloud_min_z(cloud);
	struct vector3* point = cloud_get_center(cloud);
	struct vector3* norm = vector3_new(0, 1, 0);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	
	cloud_save_xyz(sub, argv[2]);
	
	real d = hu_regular_moment(0, 0, 0, cloud);
	printf("%f\n%u\n", d, cloud_size(cloud));
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(norm);
	vector3_free(point);
	cloud_free(cloud);
	
	return 0;
}

