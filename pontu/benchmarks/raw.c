#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main()
{
	/** usar isso deposi de debuggar
	struct cloud *bunny = cloud_load_xyz("../samples/bunny_scale.xyz");
	struct cloud *bunny_rotate = cloud_load_xyz("../samples/bunny_rotate.xyz");
	struct matrix *ans = zernike_cloud_moments_mag(bunny);
	struct matrix *ans_rotate = zernike_spharmon_cloud_moments_mag(bunny_rotate);
	
	
	matrix_debug(ans, stdout);
	matrix_debug(ans_rotate, stdout);
	
	matrix_free(ans_rotate);
	matrix_free(ans);
	cloud_free(bunny_rotate);
	cloud_free(bunny);
	
	return 0;
	*/
	
	for (int n = 0; n <= 4; n++)
		for (int m = 0; m <= 4; m++)
			if (zernike_conditions(n, m))
				printf("%d,%d: ok\n", n, m);
			else
				printf("%d,%d: notok\n", n, m);
	
	struct cloud *bunny = cloud_load_xyz("../samples/bunny_scale.xyz");
	real r = cloud_max_distance_from_center(bunny);
	real m = zernike_spharmon_moment_mag(2, 2, 0, r, bunny);
	
	printf("tutu: %.4f\n", m);
	
	return 0;
}

