#include "../include/siqueira.h"

real siqueira_moment_xy(int p, int q, struct cloud *cloud, struct vector3 *ref)
{
	real moment = 0.0f;
	struct vector3 *center = cloud_get_center(cloud);
	
    for (uint i = 0; i < cloud->numpts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].y - center->y, q)
                * vector3_distance(&cloud->points[i], ref);
	}
	
	vector3_free(center);
	
    return moment;
}

real siqueira_moment_xz(int p, int q, struct cloud *cloud, struct vector3 *ref)
{
	real moment = 0.0f;
	struct vector3 *center = cloud_get_center(cloud);
	
    for (uint i = 0; i < cloud->numpts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].z - center->z, q)
                * vector3_distance(&cloud->points[i], ref);
	}
	
	vector3_free(center);
	
    return moment;
}

real siqueira_moment_yz(int p, int q, struct cloud *cloud, struct vector3 *ref)
{
	real moment = 0.0f;
	struct vector3 *center = cloud_get_center(cloud);
	
    for (uint i = 0; i < cloud->numpts; i++) {
        moment += pow(cloud->points[i].y - center->y, p)
                * pow(cloud->points[i].z - center->z, q)
                * vector3_distance(&cloud->points[i], ref);
	}
	
	vector3_free(center);
	
    return moment;
}

struct matrix *siqueira_cloud_moments(struct cloud *cloud, struct vector3 *ref)
{
	struct matrix *results = matrix_new(1, 12);
	
	real u02 = 0.0f;
    real u03 = 0.0f;
    real u11 = 0.0f;
    real u12 = 0.0f;
    real u20 = 0.0f;
    real u21 = 0.0f;
    real u30 = 0.0f;
    real i1 = 0.0f;
    real i2 = 0.0f;
    real i3 = 0.0f;
    real i4 = 0.0f;
    real i5 = 0.0f;
    real i6 = 0.0f;
    real i7 = 0.0f;
    real i8 = 0.0f;
    real i9 = 0.0f;
    real i10 = 0.0f;
    real i11 = 0.0f;
    real i12 = 0.0f;
    
    u02 = siqueira_moment_xy(0, 2, cloud, ref);
    u03 = siqueira_moment_xy(0, 3, cloud, ref);
    u11 = siqueira_moment_xy(1, 1, cloud, ref);
    u12 = siqueira_moment_xy(1, 2, cloud, ref);
    u20 = siqueira_moment_xy(2, 0, cloud, ref);
    u21 = siqueira_moment_xy(2, 1, cloud, ref);
    u30 = siqueira_moment_xy(3, 0, cloud, ref);
    
    i1 = u20 + u02;
    i2 = pow((u20 - u02), 2.0f) + 4.0f*pow(u11, 2.0f);
    i3 = pow((u30 - 3.0f*u12), 2.0f) + pow((3.0f*u21 - u03), 2.0f);
    i4 = pow((u30 + u12), 2.0f) + pow((u21 + u03), 2.0f);
    
    u02 = siqueira_moment_xz(0, 2, cloud, ref);
    u03 = siqueira_moment_xz(0, 3, cloud, ref);
    u11 = siqueira_moment_xz(1, 1, cloud, ref);
    u12 = siqueira_moment_xz(1, 2, cloud, ref);
    u20 = siqueira_moment_xz(2, 0, cloud, ref);
    u21 = siqueira_moment_xz(2, 1, cloud, ref);
    u30 = siqueira_moment_xz(3, 0, cloud, ref);
    
    i5 = u20 + u02;
    i6 = pow((u20 - u02), 2.0f) + 4.0f*pow(u11, 2.0f);
    i7 = pow((u30 - 3.0f*u12), 2.0f) + pow((3.0f*u21 - u03), 2.0f);
    i8 = pow((u30 + u12), 2.0f) + pow((u21 + u03), 2.0f);
    
    u02 = siqueira_moment_yz(0, 2, cloud, ref);
    u03 = siqueira_moment_yz(0, 3, cloud, ref);
    u11 = siqueira_moment_yz(1, 1, cloud, ref);
    u12 = siqueira_moment_yz(1, 2, cloud, ref);
    u20 = siqueira_moment_yz(2, 0, cloud, ref);
    u21 = siqueira_moment_yz(2, 1, cloud, ref);
    u30 = siqueira_moment_yz(3, 0, cloud, ref);
    
    i9 = u20 + u02;
    i10 = pow((u20 - u02), 2.0f) + 4.0f*pow(u11, 2.0f);
    i11 = pow((u30 - 3.0f*u12), 2.0f) + pow((3.0f*u21 - u03), 2.0f);
    i12 = pow((u30 + u12), 2.0f) + pow((u21 + u03), 2.0f);
    
    matrix_set(results, 0, 0, i1);
    matrix_set(results, 0, 1, i2);
    matrix_set(results, 0, 2, i3);
    matrix_set(results, 0, 3, i4);
    matrix_set(results, 0, 4, i5);
    matrix_set(results, 0, 5, i6);
    matrix_set(results, 0, 6, i7);
    matrix_set(results, 0, 7, i8);
    matrix_set(results, 0, 8, i9);
    matrix_set(results, 0, 9, i10);
    matrix_set(results, 0, 10, i11);
    matrix_set(results, 0, 11, i12);
    
	return results;
}

real siqueira_sign(real n)
{
	if (n >= 0.0f)
		return 1.0f;
	else
		return -1.0f;
}

struct matrix *siqueira_transversal(struct cloud *cloud, struct vector3 *ref)
{
	struct matrix *ans = NULL;
	
	struct cloud *sub0 = cloud_empty();
	struct cloud *sub1 = cloud_empty();
	struct cloud *sub2 = cloud_empty();
	struct cloud *sub3 = cloud_empty();
	struct cloud *sub4 = cloud_empty();
	struct cloud *sub5 = cloud_empty();
	struct cloud *sub6 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real segment = floor(fabs(ref->y - cloud->points[i].y) / CUT_SIZE);
		real sign = siqueira_sign(ref->y - cloud->points[i].y);
		segment = segment * sign;
		
		if (segment == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (segment == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (segment == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
		else if (segment == 3.0f)
			cloud_add_point_vector(sub3, &cloud->points[i]);
		else if (segment == -1.0f)
			cloud_add_point_vector(sub4, &cloud->points[i]);
		else if (segment == -2.0f)
			cloud_add_point_vector(sub5, &cloud->points[i]);
		else if (segment == -3.0f)
			cloud_add_point_vector(sub6, &cloud->points[i]);
	}
	
	struct matrix *moments0 = siqueira_cloud_moments(sub0, ref);
	struct matrix *moments1 = siqueira_cloud_moments(sub1, ref);
	struct matrix *moments2 = siqueira_cloud_moments(sub2, ref);
	struct matrix *moments3 = siqueira_cloud_moments(sub3, ref);
	struct matrix *moments4 = siqueira_cloud_moments(sub4, ref);
	struct matrix *moments5 = siqueira_cloud_moments(sub5, ref);
	struct matrix *moments6 = siqueira_cloud_moments(sub6, ref);
	struct matrix *concat0 = matrix_concat_hor(moments0, moments1);
	struct matrix *concat1 = matrix_concat_hor(concat0, moments2);
	struct matrix *concat2 = matrix_concat_hor(concat1, moments3);
	struct matrix *concat3 = matrix_concat_hor(concat2, moments4);
	struct matrix *concat4 = matrix_concat_hor(concat3, moments5);
	
	ans = matrix_concat_hor(concat4, moments6);
	
	matrix_free(moments0);
	matrix_free(moments1);
	matrix_free(moments2);
	matrix_free(moments3);
	matrix_free(moments4);
	matrix_free(moments5);
	matrix_free(moments6);
	matrix_free(concat0);
	matrix_free(concat1);
	matrix_free(concat2);
	matrix_free(concat3);
	matrix_free(concat4);
	cloud_free(sub0);
	cloud_free(sub1);
	cloud_free(sub2);
	cloud_free(sub3);
	cloud_free(sub4);
	cloud_free(sub5);
	cloud_free(sub6);
	
	return ans;
}

struct matrix *siqueira_sagittal(struct cloud *cloud, struct vector3 *ref)
{
	struct matrix *ans = NULL;
	
	struct cloud *sub0 = cloud_empty();
	struct cloud *sub1 = cloud_empty();
	struct cloud *sub2 = cloud_empty();
	struct cloud *sub3 = cloud_empty();
	struct cloud *sub4 = cloud_empty();
	struct cloud *sub5 = cloud_empty();
	struct cloud *sub6 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real segment = floor(fabs(ref->x - cloud->points[i].x) / CUT_SIZE);
		real sign = siqueira_sign(ref->x - cloud->points[i].x);
		segment = segment * sign;
		
		if (segment == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (segment == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (segment == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
		else if (segment == 3.0f)
			cloud_add_point_vector(sub3, &cloud->points[i]);
		else if (segment == -1.0f)
			cloud_add_point_vector(sub4, &cloud->points[i]);
		else if (segment == -2.0f)
			cloud_add_point_vector(sub5, &cloud->points[i]);
		else if (segment == -3.0f)
			cloud_add_point_vector(sub6, &cloud->points[i]);
	}
	
	struct matrix *moments0 = siqueira_cloud_moments(sub0, ref);
	struct matrix *moments1 = siqueira_cloud_moments(sub1, ref);
	struct matrix *moments2 = siqueira_cloud_moments(sub2, ref);
	struct matrix *moments3 = siqueira_cloud_moments(sub3, ref);
	struct matrix *moments4 = siqueira_cloud_moments(sub4, ref);
	struct matrix *moments5 = siqueira_cloud_moments(sub5, ref);
	struct matrix *moments6 = siqueira_cloud_moments(sub6, ref);
	struct matrix *concat0 = matrix_concat_hor(moments0, moments1);
	struct matrix *concat1 = matrix_concat_hor(concat0, moments2);
	struct matrix *concat2 = matrix_concat_hor(concat1, moments3);
	struct matrix *concat3 = matrix_concat_hor(concat2, moments4);
	struct matrix *concat4 = matrix_concat_hor(concat3, moments5);
	
	ans = matrix_concat_hor(concat4, moments6);
	
	matrix_free(moments0);
	matrix_free(moments1);
	matrix_free(moments2);
	matrix_free(moments3);
	matrix_free(moments4);
	matrix_free(moments5);
	matrix_free(moments6);
	matrix_free(concat0);
	matrix_free(concat1);
	matrix_free(concat2);
	matrix_free(concat3);
	matrix_free(concat4);
	cloud_free(sub0);
	cloud_free(sub1);
	cloud_free(sub2);
	cloud_free(sub3);
	cloud_free(sub4);
	cloud_free(sub5);
	cloud_free(sub6);
	
	return ans;
}

struct matrix *siqueira_frontal(struct cloud *cloud, struct vector3 *ref)
{
	struct matrix *ans = NULL;
	struct cloud *sub0 = cloud_empty();
	struct cloud *sub1 = cloud_empty();
	struct cloud *sub2 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real segment = floor(fabs(ref->z - cloud->points[i].z) / CUT_SIZE);
		
		if (segment == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (segment == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (segment == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
	}
	
	struct matrix *moments0 = siqueira_cloud_moments(sub0, ref);
	struct matrix *moments1 = siqueira_cloud_moments(sub1, ref);
	struct matrix *moments2 = siqueira_cloud_moments(sub2, ref);
	struct matrix *concat0 = matrix_concat_hor(moments0, moments1);
	
	ans = matrix_concat_hor(concat0, moments2);
	
	matrix_free(moments0);
	matrix_free(moments1);
	matrix_free(moments2);
	matrix_free(concat0);
	cloud_free(sub0);
	cloud_free(sub1);
	cloud_free(sub2);
	
	return ans;
}

