#define _GNU_SOURCE

#include "../src/include/lechong.h"
#include "../src/include/legendre.h"
#include "../src/include/spheric.h"

int main(int argc, char** argv)
{
	char *clouds[] = {"../dump/bunny_raw.xyz", "../dump/bunny_trans.xyz", "../dump/bunny_scale.xyz", "../dump/bunny_rotate.xyz"};
	//char *clouds[] = {"../dump/bs000_raw.xyz", "../dump/bs000_trans.xyz", "../dump/bs000_scale.xyz", "../dump/bs000_rotate.xyz"};
	
	for (int i = 0; i < 4; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		
		struct matrix* ans = lechong_cloud_moments(cloud);
		//struct matrix* ans = legendre_cloud_moments(cloud);
		//struct matrix* ans = spheric_cloud_moments(cloud);
		
		printf("> %s\n", clouds[i]);
		matrix_debug(ans, stdout);
		printf("\n");
		
		matrix_free(ans);
		cloud_free(cloud);
	}
	
	return 0;
}

