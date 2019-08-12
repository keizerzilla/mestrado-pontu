#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

void foo()
{
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			for (int k = 0; k <= 2; k++) {
				real w = ((i + j + k) / 3.0f) + 1.0f;
				real p = pow(127.0f, w);
				
				printf("(%d,%d,%d): %.4f - %.4f\n", i, j, k, w, p);
			}
		}
	}
}

int main()
{
	const char *clouds[4] = {
		"../samples/bunny.xyz",
		"../samples/bunny_trans.xyz",
		"../samples/bunny_scale.xyz",
		"../samples/bunny_rotate.xyz"
	};
	
	for (uint i = 0; i < 4; i++) {
		struct cloud *cloud = cloud_load_xyz(clouds[i]);
		struct matrix *ans = hu_cloud_raw_moments(cloud, 1, 1, 1);
		
		printf("%s: %u\n", clouds[i], cloud->numpts);
		matrix_debug(ans, stdout);
		printf("\n");
		
		matrix_free(ans);
		cloud_free(cloud);
	}
	
	return 0;
}

