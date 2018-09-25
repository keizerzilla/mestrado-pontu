#include "include/kdtree.h"

int main(int argc, char** argv)
{
	printf("\n");
	printf("=========================\n");
	printf(" KDTREE POINT CLOUD DATA \n");
	printf("=========================\n");
	
	if (argc != 3) {
		util_error("numero insuficiente de parametros");
		exit(1);
	}
	
	struct kdtree* kdt = NULL;
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	struct cloud* kdcloud = cloud_new(0);
	
	kdt = kdtree_cloud_partition(cloud, 9);
	kdtree_kdtree2cloud(kdt, kdcloud);
	
	cloud_save_pcd(kdcloud, argv[2]);
	
	cloud_free(kdcloud);
	cloud_free(cloud);
	kdtree_free(kdt);
	
	printf("\n");
	
	return 0;
}

