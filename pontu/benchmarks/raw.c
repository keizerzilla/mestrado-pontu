#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main()
{
	struct cloud *bunny = cloud_load_xyz("../samples/bunny.xyz");
	struct cloud *bunny_rotate = cloud_load_xyz("../samples/bunny_rotate.xyz");
	struct matrix *ans = zernike_cloud_moments_mag(bunny);
	struct matrix *ans_rotate = zernike_cloud_moments_mag(bunny_rotate);
	
	
	matrix_debug(ans, stdout);
	matrix_debug(ans_rotate, stdout);
	
	matrix_free(ans_rotate);
	matrix_free(ans);
	cloud_free(bunny_rotate);
	cloud_free(bunny);
	
	return 0;
}

