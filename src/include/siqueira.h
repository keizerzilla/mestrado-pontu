/**
 * \file siqueira.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Coleção completa de extração de momentos proposta por Robson Siqueira.
 */

#ifndef SIQUEIRA_H
#define SIQUEIRA_H

#define CUT_SIZE 20

#include "matrix.h"
#include "cloud.h"

/**
 * \brief Calcula o momento central de Hu 2D no plano XY
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param cloud A nuvem alvo
 * \param nose A ponta do nariz
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
 * \param nose A ponta do nariz
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
 * \param nose A ponta do nariz
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
    
    u02 = siqueira_moment_xy(0, 2, cloud, nose);
    u03 = siqueira_moment_xy(0, 3, cloud, nose);
    u11 = siqueira_moment_xy(1, 1, cloud, nose);
    u12 = siqueira_moment_xy(1, 2, cloud, nose);
    u20 = siqueira_moment_xy(2, 0, cloud, nose);
    u21 = siqueira_moment_xy(2, 1, cloud, nose);
    u30 = siqueira_moment_xy(3, 0, cloud, nose);
    
    i1 = u20 + u02;
    i2 = pow((u20 - u02), 2.0f) + 4.0f*pow(u11, 2.0f);
    i3 = pow((u30 - 3.0f*u12), 2.0f) + pow((3.0f*u21 - u03), 2.0f);
    i4 = pow((u30 + u12), 2.0f) + pow((u21 + u03), 2.0f);
    
    u02 = siqueira_moment_xz(0, 2, cloud, nose);
    u03 = siqueira_moment_xz(0, 3, cloud, nose);
    u11 = siqueira_moment_xz(1, 1, cloud, nose);
    u12 = siqueira_moment_xz(1, 2, cloud, nose);
    u20 = siqueira_moment_xz(2, 0, cloud, nose);
    u21 = siqueira_moment_xz(2, 1, cloud, nose);
    u30 = siqueira_moment_xz(3, 0, cloud, nose);
    
    i5 = u20 + u02;
    i6 = pow((u20 - u02), 2.0f) + 4.0f*pow(u11, 2.0f);
    i7 = pow((u30 - 3.0f*u12), 2.0f) + pow((3.0f*u21 - u03), 2.0f);
    i8 = pow((u30 + u12), 2.0f) + pow((u21 + u03), 2.0f);
    
    u02 = siqueira_moment_yz(0, 2, cloud, nose);
    u03 = siqueira_moment_yz(0, 3, cloud, nose);
    u11 = siqueira_moment_yz(1, 1, cloud, nose);
    u12 = siqueira_moment_yz(1, 2, cloud, nose);
    u20 = siqueira_moment_yz(2, 0, cloud, nose);
    u21 = siqueira_moment_yz(2, 1, cloud, nose);
    u30 = siqueira_moment_yz(3, 0, cloud, nose);
    
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

/**
 * \brief Retorna sinal de uma valor real
 * \param n O valor que se quer descobrir o sinal
 * \return 1.0f se n for positivo, 1.0f caso-contrário
 */
real siqueira_sign(real n)
{
	if (n >= 0.0f)
		return 1.0f;
	else
		return -1.0f;
}

/**
 * \brief Corte transversal
 * \param cloud A nuvem alvo
 * \return nose A ponta do nariz
 * \return Momentos extraídos do corte transversal
 */
struct matrix* siqueira_transversal(struct cloud* cloud, struct vector3* nose)
{
	struct matrix* ans = NULL;
	
	struct cloud* sub0 = cloud_empty();
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	struct cloud* sub3 = cloud_empty();
	struct cloud* sub4 = cloud_empty();
	struct cloud* sub5 = cloud_empty();
	struct cloud* sub6 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real segment = floor(fabs(nose->y - cloud->points[i].y) / CUT_SIZE);
		real sign = siqueira_sign(nose->y - cloud->points[i].y);
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
	
	struct matrix* moments0 = siqueira_cloud_moments(sub0, nose);
	struct matrix* moments1 = siqueira_cloud_moments(sub1, nose);
	struct matrix* moments2 = siqueira_cloud_moments(sub2, nose);
	struct matrix* moments3 = siqueira_cloud_moments(sub3, nose);
	struct matrix* moments4 = siqueira_cloud_moments(sub4, nose);
	struct matrix* moments5 = siqueira_cloud_moments(sub5, nose);
	struct matrix* moments6 = siqueira_cloud_moments(sub6, nose);
	struct matrix* concat0 = matrix_concat_hor(moments0, moments1);
	struct matrix* concat1 = matrix_concat_hor(concat0, moments2);
	struct matrix* concat2 = matrix_concat_hor(concat1, moments3);
	struct matrix* concat3 = matrix_concat_hor(concat2, moments4);
	struct matrix* concat4 = matrix_concat_hor(concat3, moments5);
	
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

/**
 * \brief Corte sagital
 * \param cloud A nuvem alvo
 * \return nose A ponta do nariz
 * \return Momentos extraídos do corte sagital
 */
struct matrix* siqueira_sagittal(struct cloud* cloud, struct vector3* nose)
{
	struct matrix* ans = NULL;
	
	struct cloud* sub0 = cloud_empty();
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	struct cloud* sub3 = cloud_empty();
	struct cloud* sub4 = cloud_empty();
	struct cloud* sub5 = cloud_empty();
	struct cloud* sub6 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real segment = floor(fabs(nose->x - cloud->points[i].x) / CUT_SIZE);
		real sign = siqueira_sign(nose->x - cloud->points[i].x);
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
	
	struct matrix* moments0 = siqueira_cloud_moments(sub0, nose);
	struct matrix* moments1 = siqueira_cloud_moments(sub1, nose);
	struct matrix* moments2 = siqueira_cloud_moments(sub2, nose);
	struct matrix* moments3 = siqueira_cloud_moments(sub3, nose);
	struct matrix* moments4 = siqueira_cloud_moments(sub4, nose);
	struct matrix* moments5 = siqueira_cloud_moments(sub5, nose);
	struct matrix* moments6 = siqueira_cloud_moments(sub6, nose);
	struct matrix* concat0 = matrix_concat_hor(moments0, moments1);
	struct matrix* concat1 = matrix_concat_hor(concat0, moments2);
	struct matrix* concat2 = matrix_concat_hor(concat1, moments3);
	struct matrix* concat3 = matrix_concat_hor(concat2, moments4);
	struct matrix* concat4 = matrix_concat_hor(concat3, moments5);
	
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

/**
 * \brief Corte frontal
 * \param cloud A nuvem alvo
 * \return nose A ponta do nariz
 * \return Momentos extraídos do corte frontal
 */
struct matrix* siqueira_frontal(struct cloud* cloud, struct vector3* nose)
{
	struct matrix* ans = NULL;
	struct cloud* sub0 = cloud_empty();
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		real segment = floor(fabs(nose->z - cloud->points[i].z) / CUT_SIZE);
		
		if (segment == 0.0f)
			cloud_add_point_vector(sub0, &cloud->points[i]);
		else if (segment == 1.0f)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (segment == 2.0f)
			cloud_add_point_vector(sub2, &cloud->points[i]);
	}
	
	struct matrix* moments0 = siqueira_cloud_moments(sub0, nose);
	struct matrix* moments1 = siqueira_cloud_moments(sub1, nose);
	struct matrix* moments2 = siqueira_cloud_moments(sub2, nose);
	struct matrix* concat0 = matrix_concat_hor(moments0, moments1);
	
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

#endif // SIQUEIRA_H

