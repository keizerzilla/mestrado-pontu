#define _GNU_SOURCE

#include "../src/include/hu.h"
#include "../src/include/golden.h"
#include "../src/include/matrix.h"
#include "../src/include/spheric.h"
#include "../src/include/legendre.h"
#include "../src/include/chebyshev.h"

int main()
{
	char* clouds[4] = {"../dump/bunny_raw.xyz",
	                   "../dump/bunny_trans.xyz",
	                   "../dump/bunny_scale.xyz",
	                   "../dump/bunny_rotate.xyz",};
	
	for (int i = 0; i < 4; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		struct matrix* ans = hu_cloud_moments_hu1980(cloud);
		//struct matrix* ans = chebyshev_invariant_moments(cloud);
		//struct matrix* ans = legendre_invariant_moments(cloud);
		
		printf("%s\n", clouds[i]);
		matrix_debug(ans, stdout);
		
		matrix_free(ans);
		cloud_free(cloud);
	}
	
	return 0;
}

