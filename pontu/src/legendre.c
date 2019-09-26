#include "../include/legendre.h"

real legendre_poly(int n, real x)
{
	if (n < 0)
		return 0.0f;

	if (n == 0)
		return 1.0f;

	if (n == 1)
		return x;

	return (((2 * n) - 1) * x * legendre_poly(n - 1, x) -
	       (n - 1) * legendre_poly(n - 2, x)) / (1.0f * n);
}

real legendre_norm(int p, int q, int r, struct cloud *cloud)
{
	real num = ((2.0f * p) + 1) * ((2.0f * q) + 1) * ((2.0f * r) + 1);
	real den = 1.0f * cloud_size(cloud);

	return num / den;
}

real legendre_moment(int p, int q, int r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	
	real moment = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		struct vector3 *point = vector3_from_vector(&cloud->points[i]);
		
		center_x = point->x - center->x;
		center_y = point->y - center->y;
		center_z = point->z - center->z;

		moment += legendre_poly(p, center_x) *
		          legendre_poly(q, center_y) *
		          legendre_poly(r, center_z) *
		          vector3_distance(point, center);

		vector3_free(&point);
	}

	vector3_free(&center);

	return legendre_norm(p, q, r, cloud) * moment;
}

struct dataframe *legendre_cloud_moments(struct cloud *cloud)
{
	struct dataframe *results = dataframe_new(1, LEGENDRE_MOMENTS);

	int p = 0;
	int q = 0;
	int r = 0;
	int col = 0;

	for (p = 0; p <= LEGENDRE_ORDER; p++) {
		for (q = 0; q <= LEGENDRE_ORDER; q++) {
			for (r = 0; r <= LEGENDRE_ORDER; r++) {
				dataframe_set(results, 0, col, legendre_moment(p, q, r, cloud));
				col++;
			}
		}
	}

	return results;
}

