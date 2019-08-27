#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

#define LIM 7

int main()
{
	struct cloud *bunny = cloud_load_xyz("../samples/bunny.xyz");
	struct matrix *ans1 = zernike_cloud_moments_even(bunny);
	struct matrix *ans2 = spharmonics_cloud_moments_even(bunny);
	
	matrix_debug(ans1, stdout);
	matrix_debug(ans2, stdout);
	
	printf("zernike:\t%d\n", ans1->cols);
	printf("spharmonics:\t%d\n", ans2->cols);
	
	matrix_free(ans2);
	matrix_free(ans1);
	cloud_free(bunny);
	
	return 0;
}

