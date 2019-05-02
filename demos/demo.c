#include "../src/include/cloud.h"
#include "../src/include/plane.h"
#include "../src/include/vector3.h"
#include "../src/include/chebyshev.h"
#include "../src/include/zernike.h"
#include "../src/include/legendre.h"
#include "../src/include/spheric.h"
#include "../src/include/hu.h"

int main(int argc, char** argv)
{
	const char* clouds[5] = {"../dump/bunny_raw.xyz",
	                         "../dump/bunny_scale.xyz",
	                         "../dump/bunny_scale_2.xyz",
	                         "../dump/bunny_scale_3.xyz",
	                         "../dump/bunny_rotate.xyz"};
	
	for (int i = 0; i < 5; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		struct matrix* ans = zernike_cloud_moments(cloud);
		
		printf("%s\n", clouds[i]);
		matrix_debug(ans, stdout);
		
		matrix_free(ans);
		cloud_free(cloud);
	}
	
	return 0;
}

