#include "../include/golden.h"

real golden_phi(real a)
{
	return a*a - a - 1.0f;
}

real golden_moment(int p, int q, int r, struct cloud *cloud)
{
	real moment = 0.0f;
	struct vector3 *center = cloud_get_center(cloud);

	for (uint i = 0; i < cloud->numpts; i++) {
		moment += pow(golden_phi(cloud->points[i].x - center->x), p) *
		          pow(golden_phi(cloud->points[i].y - center->y), q) *
		          pow(golden_phi(cloud->points[i].z - center->z), r) *
		          vector3_distance(&cloud->points[i], center);
	}

	vector3_free(center);

	return moment / CALC_PHI;
}

struct matrix *golden_cloud_moments(struct cloud *cloud, int p, int q, int r)
{
	struct matrix *ans = matrix_new(1, (p + 1) * (q + 1) * (r + 1));
	
	int col = 0;
	for (int i = 0; i <= p; i++) {
		for (int j = 0; j <= q; j++) {
			for (int k = 0; k <= r; k++) {
				matrix_set(ans, 0, col, golden_moment(i, j, k, cloud));
				col++;
			}
		}
	}
	
	return ans;
}

