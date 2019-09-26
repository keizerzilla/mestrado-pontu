#include "../include/chebyshev.h"

real chebyshev_poly(int p, uint n, real x)
{
	if (p <= 0)
		return 1.0f;

	if (p == 1)
		return x;

	real num1 = ((2 * p) - 1) * x * chebyshev_poly(p - 1, n, x);
	real num2 = (p - 1) *
	            (1 - (((p - 1) * (p - 1)) / (n * n))) *
	            chebyshev_poly(p - 2, n, x);

	return (num1 - num2) / p;
}

real chebyshev_moment(int p, int q, int r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	uint n = cloud->numpts;
	real moment = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		moment += chebyshev_poly(p, n, cloud->points[i].x - center->x) *
		          chebyshev_poly(q, n, cloud->points[i].y - center->y) *
		          chebyshev_poly(r, n, cloud->points[i].z - center->z) *
		          vector3_distance(&cloud->points[i], center);
	}

	vector3_free(&center);

	return moment;
}

struct dataframe *chebyshev_cloud_moments(struct cloud *cloud)
{
	int m = (CHEBYSHEV_ORDER_X + 1) *
	        (CHEBYSHEV_ORDER_Y + 1) *
	        (CHEBYSHEV_ORDER_Z + 1);
	struct dataframe *results = dataframe_new(1, m);

	int p = 0;
	int q = 0;
	int r = 0;
	int col = 0;

	for (p = 0; p <= CHEBYSHEV_ORDER_X; p++) {
		for (q = 0; q <= CHEBYSHEV_ORDER_Y; q++) {
			for (r = 0; r <= CHEBYSHEV_ORDER_Z; r++) {
				dataframe_set(results,
				              0,
				              col,
				              chebyshev_moment(p, q, r, cloud));
				col++;
			}
		}
	}

	return results;
}

