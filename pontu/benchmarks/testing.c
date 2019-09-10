#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main()
{
	struct cloud *cloud = cloud_load_xyz("../samples/bunny.xyz");
	struct matrix *ans = spheric_cloud_moments(cloud);
	
	matrix_debug(ans, stdout);
	
	matrix_free(ans);
	cloud_free(cloud);
	
	return 0;
}

