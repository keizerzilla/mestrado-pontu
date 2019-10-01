#include "../include/zernike.h"

int zernike_conditions(int n, int m)
{
	return !(n < m || (n - m) % 2);
}

int zernike_nummoments(int ord, int rep)
{
	int count = 0;

	for (int n = 0; n <= ord; n++)
		for (int m = 0; m <= rep; m++)
			if (zernike_conditions(n, m))
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

real zernike_moment_odd(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);

	real cx = 0.0f;
	real cy = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real moment = 0.0f;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(cy, cx);

		moment += poly * sin(m * azimuth);
	}

	vector3_free(&centroid);

	return ((n + 1.0f) / CALC_PI) * moment;
}

real zernike_moment_even(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);

	real cx = 0.0f;
	real cy = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real moment = 0.0f;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(cy, cx);
		moment += poly * cos(m * azimuth);
	}

	vector3_free(&centroid);

	return ((n + 1.0f) / CALC_PI) * moment;
}

real zernike_moment_mag(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);

	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real moment = 0.0f;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		
		moment += poly;
	}

	vector3_free(&centroid);

	return ((n + 1.0f) / CALC_PI) * moment;
}

real zernike_moment_full(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);

	real cx = 0.0f;
	real cy = 0.0f;
	real cz = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real zenith = 0.0;
	real moment = 0.0f;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cx = set->point->x - centroid->x;
		cy = set->point->y - centroid->y;
		cz = set->point->z - centroid->z;
		d = vector3_distance(centroid, set->point);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(cy, cx);
		zenith = zernike_zenith(cz, d);

		moment += poly * (cos(m * azimuth) + sin(m * zenith));
	}

	vector3_free(&centroid);

	return ((n + 1.0f) / CALC_PI) * moment;
}

struct dataframe *zernike_cloud_moments_odd(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_odd(n, m, r, cloud);
				dataframe_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct dataframe *zernike_cloud_moments_even(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_even(n, m, r, cloud);
				dataframe_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct dataframe *zernike_cloud_moments_mag(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_mag(n, m, r, cloud);
				dataframe_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct dataframe *zernike_cloud_moments_full(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct dataframe *results = dataframe_new(1, s);
	real r = cloud_max_distance_from_centroid(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_full(n, m, r, cloud);
				dataframe_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

