#include <time.h>
#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

void closest_testing(struct cloud *src,
                     struct cloud *tgt,
                     const char *func_name,
                     real (*func)(struct cloud*,
                                  struct cloud *,
                                  struct vector3 **,
                                  struct vector3 **))
{
	clock_t start_t;
	clock_t end_t;
	double total;
	struct vector3 *src_pt = NULL;
	struct vector3 *tgt_pt = NULL;
	
	printf("%s testing...\n", func_name);
	start_t = clock();
	
	real dist = (*func)(src, tgt, &src_pt, &tgt_pt);
	
	end_t = clock();
	total = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	
	vector3_debug(src_pt, stdout);
	vector3_debug(tgt_pt, stdout);
	
	printf("%s closest distance:\t%lf\n", func_name, dist);
	printf("%s CPU time:\t\t%lf\n", func_name, total);
}

void testing_know_point()
{
	clock_t start_t;
	clock_t end_t;
	
	struct cloud *src = cloud_load_xyz("../samples/bunny.xyz");
	struct vector3 *p = vector3_new(5.9600, 5.9386, 5.9841);
	struct kdtree *kdt = kdtree_new(src->points, src->numpts, 0);
	kdtree_partitionate(kdt, 0);
	
	start_t = clock();
	struct vector3 *closest_bruteforce = cloud_closest_point(src, p);
	end_t = clock();
	printf("bruteforce:\t%lf\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
	
	start_t = clock();
	struct vector3 *closest_kdtree = kdtree_nearest_neighbor(kdt, p);
	end_t = clock();
	printf("kdtree:\t\t%lf\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
	
	vector3_debug(closest_kdtree, stdout);
	vector3_debug(closest_bruteforce, stdout);
	
	kdtree_free(kdt);
	vector3_free(p);
	cloud_free(src);
}

int main()
{
	struct cloud *src = cloud_load_xyz("../samples/bunny.xyz");
	struct cloud *tgt = cloud_load_xyz("../samples/bunny_trans.xyz");
	
	printf("||src|| = %u\n||tgt|| = %u\n", src->numpts, tgt->numpts);
	
	struct octree *oct = octree_new(src->points, src->numpts, 5);
	octree_partitionate(oct, 5);
	
	struct kdtree *kdt = kdtree_new(src->points, src->numpts, 0);
	kdtree_partitionate(kdt, 0);
	
	struct vector3 *p = vector3_new(7.0f, 6.0f, -50.0f);
	clock_t bf_t, ot_t, kd_t;
	
	// bruteforce
	bf_t = clock();
	struct vector3 *bf = cloud_closest_point(src, p);
	bf_t = clock() - bf_t;
	
	// octree
	ot_t = clock();
	struct vector3 *ot = octree_nearest_neighbor(oct, p);
	ot_t = clock() - ot_t;
	
	// kdtree
	kd_t = clock();
	struct vector3 *kd = kdtree_nearest_neighbor(kdt, p);
	kd_t = clock() - kd_t;
	
	printf("bf_d = %f | bf_ticks = %ld | ", vector3_distance(p, bf), bf_t);
	vector3_debug(bf, stdout);
	
	printf("ot_d = %f | ot_ticks = %ld | ", vector3_distance(p, ot), ot_t);
	vector3_debug(ot, stdout);
	
	printf("kd_d = %f | kd_ticks = %ld | ", vector3_distance(p, kd), kd_t);
	vector3_debug(kd, stdout);
	
	vector3_free(p);
	kdtree_free(kdt);
	octree_free(oct);
	cloud_free(tgt);
	cloud_free(src);
	
	return 0;
}

