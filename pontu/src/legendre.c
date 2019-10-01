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
	struct vector3 *centroid = cloud_get_centroid(cloud);
	
	real moment = 0.0f;
	real centroid_x = 0.0f;
	real centroid_y = 0.0f;
	real centroid_z = 0.0f;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		struct vector3 *point = vector3_from_vector(set->point);
		
		centroid_x = point->x - centroid->x;
		centroid_y = point->y - centroid->y;
		centroid_z = point->z - centroid->z;

		moment += legendre_poly(p, centroid_x) *
		          legendre_poly(q, centroid_y) *
		          legendre_poly(r, centroid_z) *
		          vector3_distance(point, centroid);

		vector3_free(&point);
	}

	vector3_free(&centroid);

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

