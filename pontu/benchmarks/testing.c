#include <time.h>
#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int has_point(struct vector3 **points, uint n, struct vector3 *p)
{
	if (p == NULL)
		return 0;
	
	for (uint i = 0; i < n; i++)
		if (p == points[i])
			return 1;
	
	return 0;
}

struct cloud *neighbors(struct cloud *source, struct cloud *target)
{
	struct cloud *ans = cloud_empty();
	struct vector3 *srcpt = NULL;
	struct vector3 *tgtpt = NULL;
	struct vector3 **points = malloc(target->numpts * sizeof(struct vector3 *));
	int n = 0;
	
	real d = 0.0f;
	for (uint i = 0; i < source->numpts; i++) {
		d = cloud_nearest_neighbors_bruteforce(source, target, &srcpt, &tgtpt);
		vector3_debug(srcpt, stdout);
		vector3_debug(tgtpt, stdout);
		printf("d = %f\n", d);		
	}
	
	free(points);
	
	return ans;
}

int main()
{
	struct cloud *source = cloud_load_ply("../samples/bun045.ply");
	struct cloud *target = cloud_load_ply("../samples/bun000.ply");
	
	struct cloud *ans = neighbors(source, target);
	cloud_free(ans);
	
	
	
	/**
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
	*/
	
	
	cloud_free(target);
	cloud_free(source);
	
	return 0;
}

