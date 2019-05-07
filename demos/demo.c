#include "../src/include/cloud.h"

int main(int argc, char** argv)
{
	struct cloud* cloud = cloud_load_obj("model.obj");
	
	cloud_save_xyz(cloud, "model.xyz");
	
	cloud_free(cloud);
	
	return 0;
}

