/**
 * \file siqueira.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Coleção completa de extração de momentos proposta por Robson Siqueira.
 */

#ifndef SIQUEIRA_H
#define SIQUEIRA_H

#include "matrix.h"
#include "cloud.h"
#include "util.h"

/**
 * \brief Calcula o momento central de Hu 2D no plano XY
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param cloud A nuvem alvo
 * \return O momento central(p,q) da nuvem cloud em relação ao plano XY
 */
real siqueira_moment_xy(int p, int q, struct cloud* cloud, struct vector3* nose)
{
	real moment = 0.0f;
	struct vector3* center = cloud_get_center(cloud);
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].y - center->y, q)
                * vector3_distance(&cloud->points[i], nose);
	}
	
	vector3_free(center);
	
    return moment;
}

/**
 * \brief Calcula o momento central de Hu 2D no plano XZ
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento central(p,q) da nuvem cloud em relação ao plano XZ
 */
real siqueira_moment_xz(int p, int q, struct cloud* cloud, struct vector3* nose)
{
	real moment = 0.0f;
	struct vector3* center = cloud_get_center(cloud);
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].z - center->z, q)
                * vector3_distance(&cloud->points[i], nose);
	}
	
	vector3_free(center);
	
    return moment;
}

/**
 * \brief Calcula o momento central de Hu 2D no plano YZ
 * \param p A ordem da dimensão y
 * \param q A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento central(p,q) da nuvem cloud em relação ao plano YZ
 */
real siqueira_moment_yz(int p, int q, struct cloud* cloud, struct vector3* nose)
{
	real moment = 0.0f;
	struct vector3* center = cloud_get_center(cloud);
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].y - center->y, p)
                * pow(cloud->points[i].z - center->z, q)
                * vector3_distance(&cloud->points[i], nose);
	}
	
	vector3_free(center);
	
    return moment;
}

/**
 * \brief Calcula os momentos invariantes de Hu segundo (SIQUEIRA; 2018)
 * \param cloud A nuvem alvo
 * \return A matriz aonde os momentos serão salvos
 */
struct matrix* siqueira_cloud_moments(struct cloud* cloud, struct vector3* nose)
{
	struct matrix* results = matrix_new(1, 12);
	real a = 0.0f;
    real b = 0.0f;
    real c = 0.0f;
    real d = 0.0f;
    real e = 0.0f;
    real f = 0.0f;
    real g = 0.0f;
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
    
    a = siqueira_moment_xy(0, 2, cloud, nose);
    b = siqueira_moment_xy(0, 3, cloud, nose);
    c = siqueira_moment_xy(1, 1, cloud, nose);
    d = siqueira_moment_xy(1, 2, cloud, nose);
    e = siqueira_moment_xy(2, 0, cloud, nose);
    f = siqueira_moment_xy(2, 1, cloud, nose);
    g = siqueira_moment_xy(3, 0, cloud, nose);
    
    i1 = e + a;
    i2 = pow((e - a), 2.0f) + 4.0f*pow(c, 2.0f);
    i3 = pow((g - 3.0f*d), 2.0f) + pow((3.0f*f - b), 2.0f);
    i4 = pow((g + d), 2.0f) + pow((f + b), 2.0f);
    
    a = siqueira_moment_xz(0, 2, cloud, nose);
    b = siqueira_moment_xz(0, 3, cloud, nose);
    c = siqueira_moment_xz(1, 1, cloud, nose);
    d = siqueira_moment_xz(1, 2, cloud, nose);
    e = siqueira_moment_xz(2, 0, cloud, nose);
    f = siqueira_moment_xz(2, 1, cloud, nose);
    g = siqueira_moment_xz(3, 0, cloud, nose);
    
    i5 = e + a;
    i6 = pow((e - a), 2.0f) + 4.0f*pow(c, 2.0f);
    i7 = pow((g - 3.0f*d), 2.0f) + pow((3.0f*f - b), 2.0f);
    i8 = pow((g + d), 2.0f) + pow((f + b), 2.0f);
    
    a = siqueira_moment_yz(0, 2, cloud, nose);
    b = siqueira_moment_yz(0, 3, cloud, nose);
    c = siqueira_moment_yz(1, 1, cloud, nose);
    d = siqueira_moment_yz(1, 2, cloud, nose);
    e = siqueira_moment_yz(2, 0, cloud, nose);
    f = siqueira_moment_yz(2, 1, cloud, nose);
    g = siqueira_moment_yz(3, 0, cloud, nose);
    
    i9 = e + a;
    i10 = pow((e - a), 2.0f) + 4.0f*pow(c, 2.0f);
    i11 = pow((g - 3.0f*d), 2.0f) + pow((3.0f*f - b), 2.0f);
    i12 = pow((g + d), 2.0f) + pow((f + b), 2.0f);
    
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

/**
 *
 */
int siqueira_sign(real a)
{
	return (a > 0.0f) - (a < 0.0f);
}

/**
 *
 */
struct matrix* siqueira_transversal(struct cloud* cloud, struct vector3* nose)
{
	struct cloud* sub0 = cloud_empty();
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	struct cloud* sub1minus = cloud_empty();
	struct cloud* sub2minus = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real n = floor(abs(nose->y - cloud->points[i].y) / 20.0f);
		n = n * siqueira_sign(nose->y - cloud->points[i].y);
		
		if (n == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (n == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (n == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
		else if (n == -1.0f)
			cloud_add_point_vector(sub1minus, &cloud->points[i]);
		else if (n == -2.0f)
			cloud_add_point_vector(sub2minus, &cloud->points[i]);
	}
	
	struct matrix* ans0 = siqueira_cloud_moments(sub0, nose);
	struct matrix* ans1 = siqueira_cloud_moments(sub1, nose);
	struct matrix* ans2 = siqueira_cloud_moments(sub2, nose);
	struct matrix* ans1minus = siqueira_cloud_moments(sub1minus, nose);
	struct matrix* ans2minus = siqueira_cloud_moments(sub2minus, nose);
	struct matrix* concat1 = matrix_concat_hor(ans0, ans1);
	struct matrix* concat2 = matrix_concat_hor(ans2, ans1minus);
	struct matrix* concat3 = matrix_concat_hor(ans2minus, concat1);
	
	struct matrix* concat4 = matrix_concat_hor(concat3, concat2);
	
	cloud_free(sub0);
	cloud_free(sub1);
	cloud_free(sub2);
	cloud_free(sub1minus);
	cloud_free(sub2minus);
	matrix_free(ans0);
	matrix_free(ans1);
	matrix_free(ans2);
	matrix_free(ans1minus);
	matrix_free(ans2minus);
	matrix_free(concat1);
	matrix_free(concat2);
	matrix_free(concat3);
	
	return concat4;
}

/**
 *
 */
struct matrix* siqueira_sagittal(struct cloud* cloud, struct vector3* nose)
{
	struct cloud* sub0 = cloud_empty();
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	struct cloud* sub1minus = cloud_empty();
	struct cloud* sub2minus = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real n = floor(abs(nose->x - cloud->points[i].x) / 20.0f);
		n = n * siqueira_sign(nose->x - cloud->points[i].x);
		
		if (n == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (n == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (n == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
		else if (n == -1.0f)
			cloud_add_point_vector(sub1minus, &cloud->points[i]);
		else if (n == -2.0f)
			cloud_add_point_vector(sub2minus, &cloud->points[i]);
	}
	
	struct matrix* ans0 = siqueira_cloud_moments(sub0, nose);
	struct matrix* ans1 = siqueira_cloud_moments(sub1, nose);
	struct matrix* ans2 = siqueira_cloud_moments(sub2, nose);
	struct matrix* ans1minus = siqueira_cloud_moments(sub1minus, nose);
	struct matrix* ans2minus = siqueira_cloud_moments(sub2minus, nose);
	struct matrix* concat1 = matrix_concat_hor(ans0, ans1);
	struct matrix* concat2 = matrix_concat_hor(ans2, ans1minus);
	struct matrix* concat3 = matrix_concat_hor(ans2minus, concat1);
	
	struct matrix* concat4 = matrix_concat_hor(concat3, concat2);
	
	cloud_free(sub0);
	cloud_free(sub1);
	cloud_free(sub2);
	cloud_free(sub1minus);
	cloud_free(sub2minus);
	matrix_free(ans0);
	matrix_free(ans1);
	matrix_free(ans2);
	matrix_free(ans1minus);
	matrix_free(ans2minus);
	matrix_free(concat1);
	matrix_free(concat2);
	matrix_free(concat3);
	
	return concat4;
}

/**
 *
 */
struct matrix* siqueira_frontal(struct cloud* cloud, struct vector3* nose)
{
	struct cloud* sub0 = cloud_empty();
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real n = floor(abs(nose->z - cloud->points[i].z) / 20.0f);
		
		if (n == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (n == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (n == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
	}
	
	struct matrix* ans0 = siqueira_cloud_moments(sub0, nose);
	struct matrix* ans1 = siqueira_cloud_moments(sub1, nose);
	struct matrix* ans2 = siqueira_cloud_moments(sub2, nose);
	struct matrix* concat1 = matrix_concat_hor(ans0, ans1);
	
	struct matrix* concat2 = matrix_concat_hor(concat1, ans2);
	
	cloud_free(sub0);
	cloud_free(sub1);
	cloud_free(sub2);
	matrix_free(ans0);
	matrix_free(ans1);
	matrix_free(ans2);
	matrix_free(concat1);
	
	return concat2;
}

#endif // SIQUEIRA_H

