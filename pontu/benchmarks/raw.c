#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

#define LIM 7

int main()
{
	struct cloud *cloud = cloud_load_off("../samples/ant.off");
	
	cloud_save_xyz(cloud, "../samples/ant.xyz");
	printf("pronto!\n");
	
	cloud_free(cloud);
	
	return 0;
}

