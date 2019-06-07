#include <time.h>

#include "../pontu/pontu_core.h"
#include "../pontu/pontu_features.h"
#include "../pontu/pontu_sampling.h"

int main(int argc, char** argv)
{
	// INITIATING CHRONOMETER
	clock_t t;
	t = clock();
	
	// LOADING TEST
	printf("loading cloud... ");
	struct cloud *cloud = cloud_load_xyz("../dump/bunny.xyz");
	if (cloud) {
		printf("cloud loaded! %d points!\n", cloud_size(cloud));
	} else {
		printf("loading cloud error! file not found!\n");
		return 1;
	}
	
	// MOMENT EXTRACTION TEST
	printf("extracting legendre moments... ");
	struct matrix *legendre = legendre_cloud_moments(cloud);
	matrix_free(legendre);
	printf("legendre extracted!\n");
	printf("extracting hututu moments... ");
	struct matrix *hututu = hu_cloud_moments_hututu(cloud);
	matrix_free(hututu);
	printf("hututu extracted!\n");
	
	// KDTREE TEST
	printf("creating 256 kdtree partitions... ");
	struct kdtree *kdt = kdtree_new();
	kdtree_init(kdt, cloud);
	kdtree_partitionate(kdt, KDTREE_AXIS_X, 8);
	kdtree_free(kdt);
	printf("kdtree ok!\n");
	
	// SUBSAMPLING TEST
	printf("subsampling test (leafsize=2)... ");
	struct cloud *sampled = cubic_voxel_grid(cloud, 2.0f);
	cloud_free(sampled);
	printf("cubic voxelgrid ok!\n");
	
	t = clock() - t;
	float sec = ((float)t) / CLOCKS_PER_SEC;
	
	// FIN
	printf("\nBENCHMARK DONE: %ld CPU cycles (%f seconds)\n\n", t, sec);
	cloud_free(cloud);
	
	return 0;
}

