#include "../include/hu.h"

real hu_regular_moment(int p, int q, int r, struct cloud *cloud)
{
	real moment = 0.0f;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		moment += pow(set->point->x, p) *
		          pow(set->point->y, q) *
		          pow(set->point->z, r);
	}
	
	return moment;
}

real hu_central_moment(int p, int q, int r, struct cloud *cloud)
{
	real moment = 0.0f;
	struct vector3 *centroid = cloud_get_centroid(cloud);

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		moment += pow(set->point->x - centroid->x, p) *
		          pow(set->point->y - centroid->y, q) *
		          pow(set->point->z - centroid->z, r) *
		          vector3_distance(set->point, centroid);
	}

	vector3_free(&centroid);

	return moment;
}

real hu_normalized_moment(int p, int q, int r, struct cloud *cloud)
{
	real central = hu_central_moment(p, q, r, cloud);
	real zero = hu_central_moment(0, 0, 0, cloud);
	
	return central / pow(zero, ((p + q + r) / 3.0f) + 1.0f);
}

real hu_refined_moment(int p, int q, int r, struct cloud *cloud)
{
	real central = hu_central_moment(p, q, r, cloud);
	real zero = hu_central_moment(0, 0, 0, cloud);

	return central / pow(zero, 3);
}

struct dataframe *hu_cloud_moments_hu1980(struct cloud *cloud)
{
	struct dataframe *results = dataframe_new(1, 3);

	real hu200 = hu_refined_moment(2, 0, 0, cloud);
	real hu020 = hu_refined_moment(0, 2, 0, cloud);
	real hu002 = hu_refined_moment(0, 0, 2, cloud);
	real hu110 = hu_refined_moment(1, 1, 0, cloud);
	real hu101 = hu_refined_moment(1, 0, 1, cloud);
	real hu011 = hu_refined_moment(0, 1, 1, cloud);

	real j1 = hu200 + hu020 + hu002;
	real j2 = (hu200 * hu020) + (hu200 * hu002) + (hu020 * hu002) -
	          (hu110 * hu110) - (hu101 * hu101) - (hu011 * hu011);
	real j3 = (hu200 * hu020 * hu002) + (2 * hu110 * hu101 * hu011) -
	          (hu002 * hu110 * hu110) - (hu020 * hu101 * hu101) -
	          (hu200 * hu011 * hu011);

	dataframe_set(results, 0, 0, j1);
	dataframe_set(results, 0, 1, j2);
	dataframe_set(results, 0, 2, j3);

	return results;
}

struct dataframe *hu_cloud_raw_moments(struct cloud *cloud, int p, int q, int r)
{
	struct dataframe *ans = dataframe_new(1, (p + 1) * (q + 1) * (r + 1));
	
	int col = 0;
	for (int i = 0; i <= p; i++) {
		for (int j = 0; j <= q; j++) {
			for (int k = 0; k <= r; k++) {
				dataframe_set(ans,
				              0,
				              col,
				              hu_normalized_moment(i, j, k, cloud));
				col++;
			}
		}
	}
	
	return ans;
}

struct dataframe *hu_cloud_moments_hututu(struct cloud *cloud)
{
	struct dataframe *results = dataframe_new(1, HU_MOMENTS);
	real i1;
	real i2;
	real i3;
	real i4;
	real i5;
	real i6;
	real i7;
	real a;
	real b;
	real c;
	real d;
	real e;
	real f;
	real g;

	a = hu_normalized_moment(0, 2, 0, cloud);
	b = hu_normalized_moment(0, 3, 0, cloud);
	c = hu_normalized_moment(1, 1, 0, cloud);
	d = hu_normalized_moment(1, 2, 0, cloud);
	e = hu_normalized_moment(2, 0, 0, cloud);
	f = hu_normalized_moment(2, 1, 0, cloud);
	g = hu_normalized_moment(3, 0, 0, cloud);

	i1 = e + a;

	i2 = pow((e - a), 2) + 4 * pow(c, 2);

	i3 = pow((g - 3 * d), 2) + pow((3 * f - b), 2);

	i4 = pow((g + d), 2) + pow((f + b), 2);

	i5 = (g - 3 * d) * (g + d) * (pow((g + d), 2) - 3 * pow((f + b), 2)) +
	     (3 * f - b) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2));

	i6 = (e - a) * (pow((g + d), 2) - pow((f + b), 2)) +
	     4 * c * (g + d) * (f + b);

	i7 = (3 * f - b) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2)) -
	     (g - 3 * d) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2));

	dataframe_set(results, 0, 0, i1);
	dataframe_set(results, 0, 1, i2);
	dataframe_set(results, 0, 2, i3);
	dataframe_set(results, 0, 3, i4);
	dataframe_set(results, 0, 4, i5);
	dataframe_set(results, 0, 5, i6);
	dataframe_set(results, 0, 6, i7);

	a = hu_normalized_moment(0, 0, 2, cloud);
	b = hu_normalized_moment(0, 0, 3, cloud);
	c = hu_normalized_moment(1, 0, 1, cloud);
	d = hu_normalized_moment(1, 0, 2, cloud);
	e = hu_normalized_moment(2, 0, 0, cloud);
	f = hu_normalized_moment(2, 0, 1, cloud);
	g = hu_normalized_moment(3, 0, 0, cloud);

	i1 = e + a;

	i2 = pow((e - a), 2) + 4 * pow(c, 2);

	i3 = pow((g - 3 * d), 2) + pow((3 * f - b), 2);

	i4 = pow((g + d), 2) + pow((f + b), 2);

	i5 = (g - 3 * d) * (g + d) * (pow((g + d), 2) - 3 * pow((f + b), 2)) +
	     (3 * f - b) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2));

	i6 = (e - a) * (pow((g + d), 2) - pow((f + b), 2)) +
	     4 * c * (g + d) * (f + b);

	i7 = (3 * f - b) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2)) -
	     (g - 3 * d) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2));

	dataframe_set(results, 0, 7, i1);
	dataframe_set(results, 0, 8, i2);
	dataframe_set(results, 0, 9, i3);
	dataframe_set(results, 0, 10, i4);
	dataframe_set(results, 0, 11, i5);
	dataframe_set(results, 0, 12, i6);
	dataframe_set(results, 0, 13, i7);

	a = hu_normalized_moment(0, 0, 2, cloud);
	b = hu_normalized_moment(0, 0, 3, cloud);
	c = hu_normalized_moment(0, 1, 1, cloud);
	d = hu_normalized_moment(0, 1, 2, cloud);
	e = hu_normalized_moment(0, 2, 0, cloud);
	f = hu_normalized_moment(0, 2, 1, cloud);
	g = hu_normalized_moment(0, 3, 0, cloud);

	i1 = e + a;

	i2 = pow((e - a), 2) + 4 * pow(c, 2);

	i3 = pow((g - 3 * d), 2) + pow((3 * f - b), 2);

	i4 = pow((g + d), 2) + pow((f + b), 2);

	i5 = (g - 3 * d) * (g + d) * (pow((g + d), 2) - 3 * pow((f + b), 2)) +
	     (3 * f - b) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2));

	i6 = (e - a) * (pow((g + d), 2) - pow((f + b), 2)) +
	     4 * c * (g + d) * (f + b);

	i7 = (3 * f - b) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2)) -
	     (g - 3 * d) * (f + b) * (3 * pow((g + d), 2) - pow((f + b), 2));

	dataframe_set(results, 0, 14, i1);
	dataframe_set(results, 0, 15, i2);
	dataframe_set(results, 0, 16, i3);
	dataframe_set(results, 0, 17, i4);
	dataframe_set(results, 0, 18, i5);
	dataframe_set(results, 0, 19, i6);
	dataframe_set(results, 0, 20, i7);

	return results;
}

