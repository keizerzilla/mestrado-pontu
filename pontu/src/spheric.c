#include "../include/spheric.h"

real spheric_quad(real x, real y, real z, int p, int q, int r)
{
	return sqrt(pow(x, 2 * p) + pow(y, 2 * q) + pow(z, 2 * r));
}

real spheric_moment(int p, int q, int r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	real moment = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;

		moment += pow(center_x, p) *
		          pow(center_y, q) *
		          pow(center_z, r) *
		          spheric_quad(center_x, center_y, center_z, p, q, r);
	}

	vector3_free(center);

	return moment / cloud_boundingbox_volume(cloud);
}

struct matrix *spheric_cloud_moments(struct cloud *cloud)
{
	int m = (SPHERIC_ORDER_X + 1) *
	        (SPHERIC_ORDER_Y + 1) *
	        (SPHERIC_ORDER_Z + 1);
	struct matrix *results = matrix_new(1, m);

	int p = 0;
	int q = 0;
	int r = 0;
	int col = 0;

	for (p = 0; p <= SPHERIC_ORDER_X; p++) {
		for (q = 0; q <= SPHERIC_ORDER_Y; q++) {
			for (r = 0; r <= SPHERIC_ORDER_Z; r++) {
				matrix_set(results, 0, col, spheric_moment(p, q, r, cloud));
				col++;
			}
		}
	}

	return results;
}

