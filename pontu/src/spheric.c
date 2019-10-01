#include "../include/spheric.h"

real spheric_quad(real x, real y, real z, int p, int q, int r)
{
	return sqrt(pow(x, 2 * p) + pow(y, 2 * q) + pow(z, 2 * r));
}

real spheric_moment(int p, int q, int r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	real moment = 0.0;
	real centroid_x = 0.0;
	real centroid_y = 0.0;
	real centroid_z = 0.0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		centroid_x = set->point->x - centroid->x;
		centroid_y = set->point->y - centroid->y;
		centroid_z = set->point->z - centroid->z;

		moment += pow(centroid_x, p) *
		          pow(centroid_y, q) *
		          pow(centroid_z, r) *
		          spheric_quad(centroid_x, centroid_y, centroid_z, p, q, r);
	}

	vector3_free(&centroid);

	return moment / cloud_boundingbox_volume(cloud);
}

real spheric_normalized_moment(int p, int q, int r, struct cloud *cloud)
{
	real central = spheric_moment(p, q, r, cloud);
	real zero = spheric_moment(0, 0, 0, cloud);
	
	return central / pow(zero, ((p + q + r) / 3.0) + 1.0);
}

struct dataframe *spheric_cloud_moments(struct cloud *cloud)
{
	int m = (SPHERIC_ORDER_X + 1) *
	        (SPHERIC_ORDER_Y + 1) *
	        (SPHERIC_ORDER_Z + 1);
	struct dataframe *results = dataframe_new(1, m);

	int p = 0;
	int q = 0;
	int r = 0;
	int col = 0;

	for (p = 0; p <= SPHERIC_ORDER_X; p++) {
		for (q = 0; q <= SPHERIC_ORDER_Y; q++) {
			for (r = 0; r <= SPHERIC_ORDER_Z; r++) {
				dataframe_set(results, 0, col, spheric_moment(p, q, r, cloud));
				col++;
			}
		}
	}

	return results;
}

