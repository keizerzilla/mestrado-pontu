#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main()
{
	struct cloud *cloud = cloud_load_xyz("../samples/bunny_trans.xyz");
	struct vector3 *center = vector3_zero();
	real rad = 0.0f;
	
	cloud_ritter(cloud, center, &rad);
	
	printf("%.4f, ", rad);
	vector3_debug(center, stdout);
	
	cloud_free(cloud);
	
	return 0;
}

