#include "../include/spharmonics.h"

int spharmonics_conditions(int n, int m, int l)
{
	return zernike_conditions(n, m) && l >= -m && l <= m;
}

int spharmonics_nummoments(int ord, int rep, int spin)
{
	int count = 0;

	for (int n = 0; n <= ord; n++)
		for (int m = 0; m <= rep; m++)
			for (int l = 0; l <= spin; l++)
				if (spharmonics_conditions(n, m, l))
					count++;

	return count;
}

real spharmonics_norm(int m, int l)
{
	real num = ((2.0f * m) + 1.0f) * calc_factorial(m - l);
	real den = (4.0f * CALC_PI * calc_factorial(m + l));
	real norm = sqrt(num / den);
	
	return norm;
}

real spharmonics_legendrepoly(int m, int l, real x)
{
	real p1 = pow(-1.0f, l) * pow(2.0f, m) * pow(1.0f - x*x, l/2.0f);
	real p2 = 0.0f;
	
	real i1 = 0.0f;
	real i2 = 0.0f;
	real i3 = 0.0f;
	for (int k = l; k <= m; k++) {
		i1 = (calc_factorial(k) / calc_factorial(k - l)) * pow(x, k-l);
		i2 = calc_binom_coeff(m, k);
		i3 = calc_binom_coeff(ceil((m + k - 1.0f)/2.0f), m);
		
		p2 += i1 * i2 * i3;
	}
	
	return p1 * p2;
}

real spharmonics_harmonic_odd(int m, int l, real theta, real phi)
{
	real sh = spharmonics_norm(m, l) *
	          spharmonics_legendrepoly(m, l, cos(theta)) *
	          sin(l * phi);
	
	return sh;
}

real spharmonics_harmonic_even(int m, int l, real theta, real phi)
{
	real sh = spharmonics_norm(m, l) *
	          spharmonics_legendrepoly(m, l, cos(theta)) *
	          cos(l * phi);
	
	return sh;
}

real spharmonics_harmonic_mag(int m, int l, real theta)
{
	real sh = spharmonics_norm(m, l) *
	          spharmonics_legendrepoly(m, l, cos(theta));
	
	return sh;
}

real spharmonics_harmonic_full(int m, int l, real theta, real phi)
{
	real sh = spharmonics_norm(m, l) *
	          spharmonics_legendrepoly(m, l, cos(theta)) *
	          (cos(l * phi) + sin(l * phi));
	
	return sh;
}

real spharmonics_moment_odd(int n, int m, int l, real r, struct cloud *cloud)
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
		harmonic = spharmonics_harmonic_odd(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

real spharmonics_moment_even(int n, int m, int l, real r, struct cloud *cloud)
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
		harmonic = spharmonics_harmonic_even(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

real spharmonics_moment_mag(int n, int m, int l, real r, struct cloud *cloud)
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
		harmonic = spharmonics_harmonic_mag(m, l, theta);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

real spharmonics_moment_full(int n, int m, int l, real r, struct cloud *cloud)
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
		harmonic = spharmonics_harmonic_full(m, l, theta, phi);
		
		moment += radpoly * harmonic;
	}
	
	vector3_free(center);
	
	return (3.0f * moment) / (4.0f * CALC_PI);
}

struct matrix *spharmonics_cloud_moments_odd(struct cloud *cloud)
{
	int s = spharmonics_nummoments(SPHARM_ORD, SPHARM_REP, SPHARM_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	real moment = 0.0f;
	
	int col = 0;
	for (int n = 0; n <= SPHARM_ORD; n++) {
		for (int m = 0; m <= SPHARM_REP; m++) {
			for (int l = 0; l <= SPHARM_SPIN; l++) {
				if (spharmonics_conditions(n, m, l)) {
					moment = spharmonics_moment_odd(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct matrix *spharmonics_cloud_moments_even(struct cloud *cloud)
{
	int s = spharmonics_nummoments(SPHARM_ORD, SPHARM_REP, SPHARM_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	real moment = 0.0f;
	
	int col = 0;
	for (int n = 0; n <= SPHARM_ORD; n++) {
		for (int m = 0; m <= SPHARM_REP; m++) {
			for (int l = 0; l <= SPHARM_SPIN; l++) {
				if (spharmonics_conditions(n, m, l)) {
					moment = spharmonics_moment_even(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct matrix *spharmonics_cloud_moments_mag(struct cloud *cloud)
{
	int s = spharmonics_nummoments(SPHARM_ORD, SPHARM_REP, SPHARM_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	real moment = 0.0f;
	
	int col = 0;
	for (int n = 0; n <= SPHARM_ORD; n++) {
		for (int m = 0; m <= SPHARM_REP; m++) {
			for (int l = 0; l <= SPHARM_SPIN; l++) {
				if (spharmonics_conditions(n, m, l)) {
					moment = spharmonics_moment_mag(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

struct matrix *spharmonics_cloud_moments_full(struct cloud *cloud)
{
	int s = spharmonics_nummoments(SPHARM_ORD, SPHARM_REP, SPHARM_SPIN);
	struct matrix *results = matrix_new(1, s);
	real r = cloud_max_distance_from_center(cloud);
	real moment = 0.0f;
	
	int col = 0;
	for (int n = 0; n <= SPHARM_ORD; n++) {
		for (int m = 0; m <= SPHARM_REP; m++) {
			for (int l = 0; l <= SPHARM_SPIN; l++) {
				if (spharmonics_conditions(n, m, l)) {
					moment = spharmonics_moment_full(n, m, l, r, cloud);
					matrix_set(results, 0, col, moment);
					col++;
				}
			}
		}
	}

	return results;
}

