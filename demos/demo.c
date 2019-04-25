#include "../src/include/hu.h"
#include "../src/include/matrix.h"
#include "../src/include/spheric.h"
#include "../src/include/legendre.h"
#include "../src/include/chebyshev.h"
#include "../src/include/tutu.h"
#include "../src/include/extraction.h"
#include "../src/include/kdtree.h"
#include "../src/include/zernike.h"

int main(int argc, char** argv)
{
	const char* clouds[4] = {"../dump/bunny_raw.xyz",
	                         "../dump/bunny_trans.xyz",
	                         "../dump/bunny_scale.xyz",
	                         "../dump/bunny_rotate.xyz"};
	
	for (int i = 0; i < 4; i++) {
		printf("%s\n", clouds[i]);
		
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		struct matrix* ans = zernike_cloud_moments(cloud);
		
		matrix_debug(ans, stdout);
		printf("\n");
	}
	
	return 0;
}

