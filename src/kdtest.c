#include "include/kdtree.h"

int main(int argc, char** argv)
{
	printf("\n");
	printf("===============================\n");
	printf(" KDTREE POINT CLOUD DATA BEGIN \n");
	printf("===============================\n");
	
	if (argc != 4) {
		util_error("numero insuficiente de parametros");
		exit(1);
	}
	
	struct kdtree* kdt = NULL;
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct cloud* kdcloud = cloud_new(0);
	struct cloud* nose = cloud_new(0);
	
	kdt = kdtree_cloud_partition(cloud, 10);
	kdtree_kdtree2cloud(kdt, kdcloud);
	
	cloud_save_pcd(kdcloud, argv[2]);
	cloud_save_xyz(nose, argv[3]);
	
	vector3_debug(cloud_min_z(kdcloud), "kdt min z", stdout);
	vector3_debug(cloud_min_z(cloud), "cloud min z", stdout);
	
	cloud_free(nose);
	cloud_free(kdcloud);
	cloud_free(cloud);
	kdtree_free(kdt);
	
	printf("\n");
	
	return 0;
}

