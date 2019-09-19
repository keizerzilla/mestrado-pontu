#include <time.h>
#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main()
{
	struct cloud *src = cloud_load_xyz("../samples/bunny.xyz");
	struct cloud *tgt = cloud_load_xyz("../samples/bunny_trans.xyz");
	
	printf("||src|| = %u\n||tgt|| = %u\n", src->numpts, tgt->numpts);
	
	struct vector3 *src_bf;
	struct vector3 *tgt_bf;
	clock_t time_bf = clock();
	real d_bf = cloud_nearest_neighbors_bruteforce(src, tgt, &src_bf, &tgt_bf);
	time_bf = clock() - time_bf;
	
	printf("bruteforce:\n");
	vector3_debug(src_bf, stdout);
	vector3_debug(tgt_bf, stdout);
	printf("d_bf = %f | time_bf = %ld\n", d_bf, time_bf);
	
	struct vector3 *src_kd;
	struct vector3 *tgt_kd;
	clock_t time_kd = clock();
	real d_kd = cloud_nearest_neighbors_partition(src, tgt, &src_kd, &tgt_kd);
	time_kd = clock() - time_kd;
	
	printf("kdtree:\n");
	vector3_debug(src_kd, stdout);
	vector3_debug(tgt_kd, stdout);
	printf("d_kd = %f | time_kd = %ld\n", d_kd, time_kd);
	
	cloud_free(tgt);
	cloud_free(src);
	
	return 0;
}

