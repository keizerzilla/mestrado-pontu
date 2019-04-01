#define _GNU_SOURCE

#include "../src/include/legendre.h"
#include "../src/include/spheric.h"
#include "../src/include/matrix.h"
#include "../src/include/plane.h"

struct cloud* tshape(struct cloud* cloud)
{
	struct cloud* t = cloud_empty();
	
	struct vector3* sagitvec = vector3_new(1, 0, 0);
	struct vector3* transvec = vector3_new(0, 1, 0);
	struct vector3* refpoint = cloud_get_center(cloud);
	
	vector3_free(refpoint);
	vector3_free(transvec);
	vector3_free(sagitvec);
	
	
	return t;
}

int main(int argc, char** argv)
{
	char *bunnies[] = {"../dump/bunny_raw.xyz",
	                   "../dump/bunny_trans.xyz",
	                   "../dump/bunny_scale.xyz",
	                   "../dump/bunny_rotate.xyz"};
	
	char *faces[] = {"../dump/bs000_N_N_0.xyz",
	                 "../dump/bs005_N_N_0.xyz",
	                 "../dump/bs010_N_N_0.xyz",
	                 "../dump/bs019_N_N_0.xyz"};
	
	for (int i = 0; i < 4; i++) {
		const char* cloud_file = bunnies[i];
		
		struct cloud* cloud = cloud_load_xyz(cloud_file);
		struct cloud* tshape = tshape(cloud);
		
		char buffer[80];
		sprintf(buffer, "mask_%d.pcd", i);
		
		cloud_save_pcd(tshape, tshape_file);
		
		cloud_free(tshape);
		cloud_free(cloud);
	}
	
	return 0;
}

