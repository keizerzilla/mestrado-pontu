// hmmmmmm
// conclusoes:
// 1- todos, com excecao do zernike, sao invariantes apenas a translacao
// 2- zernike eh invariante apenas a escala!

#include "../../src/include/tchebychef.h"
#include "../../src/include/chebyshev.h"
#include "../../src/include/legendre.h"
#include "../../src/include/zernike.h"
#include "../../src/include/hu.h"

int main(int argc, char** argv)
{
	
	const char* clouds[3];
	clouds[0] = "../../dump/bunny.xyz";
	clouds[1] = "../../dump/bunny_trans.xyz";
	clouds[2] = "../../dump/bunny_trans_scale.xyz";
	
	FILE* dump = fopen("dump.txt", "w");
	
	for (int i = 0; i < 3; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		struct matrix* moments = tchebychef_cloud_moments(cloud);
		
		matrix_debug(moments, dump);
		
		matrix_free(moments);
		cloud_free(cloud);
	}
	
	fclose(dump);
	
	return 0;
}

