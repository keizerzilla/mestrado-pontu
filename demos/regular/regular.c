#define GNU_SOURCE_

#include "../../src/include/hu.h"
#include "../../src/include/legendre.h"
#include "../../src/include/spheric.h"

int main(int argc, char** argv)
{
	struct cloud* raw = cloud_load_xyz("../../dump/bunny_raw.xyz");
	struct cloud* trans = cloud_load_xyz("../../dump/bunny_trans.xyz");
	struct cloud* rotate = cloud_load_xyz("../../dump/bunny_rotate.xyz");
	struct cloud* scale = cloud_load_xyz("../../dump/bunny_scale.xyz");
	
	struct matrix* raw_mmt;
	struct matrix* trans_mmt;
	struct matrix* rotate_mmt;
	struct matrix* scale_mmt;
	
	FILE* dump = fopen("dump.txt", "w");
	
	printf("RAW:\n");
	raw_mmt = spheric_cloud_moments(raw);
	matrix_debug(raw_mmt, dump);
	matrix_debug(raw_mmt, stdout);
	
	printf("TRANS:\n");
	trans_mmt = spheric_cloud_moments(trans);
	matrix_debug(trans_mmt, dump);
	matrix_debug(trans_mmt, stdout);
	
	printf("ROTATE:\n");
	rotate_mmt = spheric_cloud_moments(rotate);
	matrix_debug(rotate_mmt, dump);
	matrix_debug(rotate_mmt, stdout);
	
	printf("SCALE:\n");
	scale_mmt = spheric_cloud_moments(scale);
	matrix_debug(scale_mmt, dump);
	matrix_debug(scale_mmt, stdout);
	
	printf("\n\n");
	
	fclose(dump);
	cloud_free(scale);
	cloud_free(rotate);
	cloud_free(raw);
	
	return 0;
}

