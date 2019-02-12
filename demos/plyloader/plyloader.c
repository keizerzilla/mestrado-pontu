#include "../../src/include/cloud.h"

int main(int argc, char** argv)
{
	const char* filename = "../../datasets/pcl/example.pcd";
	struct cloud* cloud = cloud_load_pcd(filename);
	
	cloud_debug(cloud, stdout);
	printf("size = %d\n", cloud_size(cloud));
	cloud_save_xyz(cloud, "dump.xyz");
	cloud_save_ply(cloud, "dump.ply");
	cloud_save_pcd(cloud, "dump.pcd");
	cloud_free(cloud);
	
	return 0;
}

