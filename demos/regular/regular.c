#include "../../src/include/hu.h"
#include "../../src/include/legendre.h"

int main(int argc, char** argv)
{
	struct cloud* raw = cloud_load_ply("../../datasets/stanford/bunny.ply");
	struct cloud* trans = cloud_load_ply("../../datasets/stanford/bunny.ply");
	struct cloud* scale = cloud_load_ply("../../datasets/stanford/bunny.ply");
	
	cloud_translate_real(trans, 666, 666, 666);
	cloud_scale(scale, 7);
	
	struct matrix* raw_mmt;
	struct matrix* trans_mmt;
	struct matrix* scale_mmt;
				raw_mmt = legendre_cloud_moments(raw);
				trans_mmt = legendre_cloud_moments(trans);
				scale_mmt = legendre_cloud_moments(scale);
				
				printf("raw\n");
				matrix_debug(raw_mmt, stdout);
				
				printf("trans\n");
				matrix_debug(trans_mmt, stdout);
				
				printf("scale\n");
				matrix_debug(scale_mmt, stdout);
				
				printf("\n\n");
	
	cloud_free(scale);
	cloud_free(trans);
	cloud_free(raw);
	
	return 0;
}

