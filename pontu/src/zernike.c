#include "../include/zernike.h"

int zernike_conditions_even(int n, int m)
{
	return !(n < m || (n - m) % 2);
}

int zernike_conditions_odd(int n, int m)
{
	return (!((n - m) % 2) && (m <= n) && (m != 0));
}

int zernike_num_moments_even(int ord, int rep)
{
	int count = 0;

	for (int n = 0; n <= ord; n++)
		for (int m = 0; m <= rep; m++)
			if (zernike_conditions_even(n, m))
				count++;

	return count;
}

int zernike_num_moments_odd(int ord, int rep)
{
	int count = 0;

	for (int n = 0; n <= ord; n++)
		for (int m = 0; m <= rep; m++)
			if (zernike_conditions_odd(n, m))
				count++;

	return count;
}

real zernike_radpoly(int n, int m, real distance)
{
	real radpoly = 0.0f;

	for (int s = 0; s <= (n - m) / 2; s++) {
		real num = pow(-1, s) * (calc_factorial(n - s));

		real den = calc_factorial(s) *
		           calc_factorial(((n + m) / 2) - s) *
		           calc_factorial(((n - m) / 2) - s);

		radpoly += (num * pow(distance, n - (2 * s))) / den;
	}

	return radpoly;
}

real zernike_azimuth(real y, real x)
{
	return atan2(y, x);
}

real zernike_zenith(real z, real r)
{
	return acos(z / r);
}

real zernike_moment_even(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);

	real center_y = 0.0f;
	real center_z = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real moment = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;

		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(center_y, center_z);

		moment += poly * cos(m * azimuth) / CALC_PI;
	}

	vector3_free(center);

	return ((n + 1.0f) / CALC_PI) * moment;
}

real zernike_moment_odd(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);

	real center_y = 0.0f;
	real center_z = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real moment = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;

		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(center_y, center_z);

		moment += poly * sin(m * azimuth) / CALC_PI;
	}

	vector3_free(center);

	return ((n + 1.0f) / CALC_PI) * moment;
}

real zernike_moment_mag(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);

	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real moment = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		
		moment += poly / CALC_PI;
	}

	vector3_free(center);

	return ((n + 1.0f) / CALC_PI) * moment;
}

struct matrix *zernike_cloud_moments_even(struct cloud *cloud)
{
	int n_moments = zernike_num_moments_even(ZERNIKE_ORDER, ZERNIKE_REPETITION);
	struct matrix *results = matrix_new(1, n_moments);
	real r = cloud_max_distance_from_center(cloud);

	int n = 0;
	int m = 0;
	int col = 0;

	for (n = 0; n <= ZERNIKE_ORDER; n++) {
		for (m = 0; m <= ZERNIKE_REPETITION; m++) {
			if (zernike_conditions_even(n, m)) {
				real moment = zernike_moment_even(n, m, r, cloud);
				matrix_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct matrix *zernike_cloud_moments_odd(struct cloud *cloud)
{
	int n_moments = zernike_num_moments_odd(ZERNIKE_ORDER, ZERNIKE_REPETITION);
	struct matrix *results = matrix_new(1, n_moments);
	real r = cloud_max_distance_from_center(cloud);

	int n = 0;
	int m = 0;
	int col = 0;

	for (n = 0; n <= ZERNIKE_ORDER; n++) {
		for (m = 0; m <= ZERNIKE_REPETITION; m++) {
			if (zernike_conditions_odd(n, m)) {
				real moment = zernike_moment_odd(n, m, r, cloud);
				matrix_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct matrix *zernike_cloud_moments_mag(struct cloud *cloud)
{
	int n_moments = zernike_num_moments_even(ZERNIKE_ORDER, ZERNIKE_REPETITION);
	struct matrix *results = matrix_new(1, n_moments);
	real r = cloud_max_distance_from_center(cloud);

	int n = 0;
	int m = 0;
	int col = 0;

	for (n = 0; n <= ZERNIKE_ORDER; n++) {
		for (m = 0; m <= ZERNIKE_REPETITION; m++) {
			if (zernike_conditions_even(n, m)) {
				real moment = zernike_moment_mag(n, m, r, cloud);
				matrix_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

