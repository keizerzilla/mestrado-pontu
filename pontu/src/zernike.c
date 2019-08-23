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

real zernike_moment_even(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);

	real center_x = 0.0f;
	real center_y = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real moment = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;

		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(center_y, center_x);

		moment += poly * cos(m * azimuth);
	}

	vector3_free(center);

	return ((n + 1.0f) / CALC_PI) * moment;
}

real zernike_moment_odd(int n, int m, real r, struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);

	real center_x = 0.0f;
	real center_y = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real poly = 0.0f;
	real azimuth = 0.0f;
	real moment = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;

		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		poly = zernike_radpoly(n, m, dist);
		azimuth = zernike_azimuth(center_y, center_x);

		moment += poly * sin(m * azimuth);
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
		
		moment += poly;
	}

	vector3_free(center);

	return ((n + 1.0f) / CALC_PI) * moment;
}

struct matrix *zernike_cloud_moments_even(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_even(n, m, r, cloud);
				matrix_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct matrix *zernike_cloud_moments_odd(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_odd(n, m, r, cloud);
				matrix_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

struct matrix *zernike_cloud_moments_mag(struct cloud *cloud)
{
	int s = zernike_nummoments(ZERNIKE_ORD, ZERNIKE_REP);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			if (zernike_conditions(n, m)) {
				moment = zernike_moment_mag(n, m, r, cloud);
				matrix_set(results, 0, col, moment);
				col++;
			}
		}
	}

	return results;
}

// NEW -- NEW -- NEW -- NEW -- NEW -- NEW -- NEW -- NEW -- NEW -- NEW -- NEW --

int zernike_spharmon_conditions(int n, int m, int l)
{
	return zernike_conditions(n, m) && l >= 0;
}

int zernike_spharmon_nummoments(int ord, int rep, int spin)
{
	int count = 0;

	for (int n = 0; n <= ord; n++)
		for (int m = 0; m <= rep; m++)
			for (int l = 0; l <= spin; l++)
				if (zernike_spharmon_conditions(n, m, l))
					count++;

	return count;
}

real zernike_spharmon_norm(int m, int l)
{
	real num = ((2.0f * m) + 1.0f) * calc_factorial(m - l);
	real den = (4.0f * CALC_PI * calc_factorial(m + l));
	real norm = sqrt(num / den);
	
	return norm;
}

real zernike_spharmon_legendrepoly(int m, int l, real x)
{
	real p1 = pow(-1.0f, l) * pow(2.0f, m) * pow(1.0f - x*x, l/2.0f);
	real p2 = 0.0f;
	
	for (int k = l; k <= m; k++) {
		real i1 = (calc_factorial(k) / calc_factorial(k - l)) * pow(x, k-l);
		real i2 = calc_binom_coeff(m, k) * calc_binom_coeff((m+k-1)/2, m);
		p2 += i1 * i2;
	}
	
	return p1 * p2;
}

real zernike_spharmon_harmonic_odd(int m, int l, real theta, real phi)
{
	real sh = zernike_spharmon_norm(m, l) *
	          zernike_spharmon_legendrepoly(m, l, cos(theta)) *
	          sin(l * phi);
	
	return sh;
}

real zernike_spharmon_harmonic_even(int m, int l, real theta, real phi)
{
	real sh = zernike_spharmon_norm(m, l) *
	          zernike_spharmon_legendrepoly(m, l, cos(theta)) *
	          cos(l * phi);
	
	return sh;
}

real zernike_spharmon_harmonic_mag(int m, int l, real theta)
{
	real norm = zernike_spharmon_norm(m, l);
	real poly = zernike_spharmon_legendrepoly(m, l, cos(theta));
	real sh = norm * poly;
	
	return sh;
}

real zernike_spharmon_moment_odd(int n,
                                 int m,
                                 int l,
                                 real r,
                                 struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	
	real cx = 0.0f;
	real cy = 0.0f;
	real cz = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real theta = 0.0f;
	real phi = 0.0f;
	real radpoly = 0.0f;
	real harmonic = 0.0f;
	real moment = 0.0f;
	
	for (uint i = 0; i < cloud->numpts; i++) {
		cx = cloud->points[i].x - center->x;
		cy = cloud->points[i].y - center->y;
		cz = cloud->points[i].z - center->z;
		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		phi = zernike_zenith(cz, r);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = zernike_spharmon_harmonic_odd(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

real zernike_spharmon_moment_even(int n,
                                  int m,
                                  int l,
                                  real r,
                                  struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	
	real cx = 0.0f;
	real cy = 0.0f;
	real cz = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real theta = 0.0f;
	real phi = 0.0f;
	real radpoly = 0.0f;
	real harmonic = 0.0f;
	real moment = 0.0f;
	
	for (uint i = 0; i < cloud->numpts; i++) {
		cx = cloud->points[i].x - center->x;
		cy = cloud->points[i].y - center->y;
		cz = cloud->points[i].z - center->z;
		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		phi = zernike_zenith(cz, r);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = zernike_spharmon_harmonic_even(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

real zernike_spharmon_moment_mag(int n,
                                 int m,
                                 int l,
                                 real r,
                                 struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	
	real cx = 0.0f;
	real cy = 0.0f;
	real d = 0.0f;
	real dist = 0.0f;
	real theta = 0.0f;
	real radpoly = 0.0f;
	real harmonic = 0.0f;
	real moment = 0.0f;
	
	for (uint i = 0; i < cloud->numpts; i++) {
		cx = cloud->points[i].x - center->x;
		cy = cloud->points[i].y - center->y;
		d = vector3_distance(center, &cloud->points[i]);
		dist = d / r;
		theta = zernike_azimuth(cy, cx);
		radpoly = zernike_radpoly(n, m, dist);
		harmonic = zernike_spharmon_harmonic_mag(m, l, theta);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

struct matrix *zernike_spharmon_cloud_moments_odd(struct cloud *cloud)
{
	int s = zernike_spharmon_nummoments(ZERNIKE_ORD, ZERNIKE_REP, ZERNIKE_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			for (int l = 0; l <= ZERNIKE_SPIN; l++) {
				if (zernike_spharmon_conditions(n, m, l)) {
					moment = zernike_spharmon_moment_odd(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct matrix *zernike_spharmon_cloud_moments_even(struct cloud *cloud)
{
	int s = zernike_spharmon_nummoments(ZERNIKE_ORD, ZERNIKE_REP, ZERNIKE_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	
	int col = 0;
	real moment = 0.0f;

	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			for (int l = 0; l <= ZERNIKE_SPIN; l++) {
				if (zernike_spharmon_conditions(n, m, l)) {
					moment = zernike_spharmon_moment_even(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct matrix *zernike_spharmon_cloud_moments_mag(struct cloud *cloud)
{
	int s = zernike_spharmon_nummoments(ZERNIKE_ORD, ZERNIKE_REP, ZERNIKE_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	
	int col = 0;
	real moment = 0.0f;
	
	for (int n = 0; n <= ZERNIKE_ORD; n++) {
		for (int m = 0; m <= ZERNIKE_REP; m++) {
			for (int l = 0; l <= ZERNIKE_SPIN; l++) {
				if (zernike_spharmon_conditions(n, m, l)) {
					moment = zernike_spharmon_moment_mag(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

