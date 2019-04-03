/**
 * \file hu.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Implementações necessárias para o cálculo dos momentos invariantes de
 * Hu para nuvens de pontos. Os momentos de Hu são 7 valores que descrevem uma
 * imagem com base em heurísticas invariantes à escala, à rotação e à
 * translação. Para nuvens de pontos, calculamos esses 7 momentos para cada uma
 * dos planos da nuvem (xy, xz e yz). No total, 21 momentos são extráidos de
 * cada nuvem.
 */

#ifndef HU_H
#define HU_H

#define HU_SUPERSET_ORDER 2
#define HU_SUPERSET_MOMENTS 27
#define HU_MOMENTS 21

#include "cloud.h"
#include "matrix.h"


/**
 * \brief Calcula o momento regular de Hu 3D
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento regular p+q+r da nuvem cloud
 */
real hu_regular_moment(int p, int q, int r, struct cloud* cloud)
{
    real moment = 0.0f;
    
    for (uint i = 0; i < cloud->num_pts; i++)
        moment += pow(cloud->points[i].x, p)
                * pow(cloud->points[i].y, q)
                * pow(cloud->points[i].z, r);

    return moment;
}

/**
 * \brief Calcula o momento central de Hu 3D
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento central p+q+r da nuvem cloud
 */
real hu_central_moment(int p, int q, int r, struct cloud* cloud)
{
    struct vector3* center = cloud_get_center(cloud);
	real moment = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].y - center->y, q)
                * pow(cloud->points[i].z - center->z, r)
                * vector3_distance(&cloud->points[i], center);
	}
	
	vector3_free(center);
	
    return moment;
}

/**
 * \brief Calcula o momento normalizado de Hu 3D
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento normalizado p+q+r da nuvem cloud
 */
real hu_normalized_moment(int p, int q, int r, struct cloud* cloud)
{
    real central = hu_central_moment(p, q, r, cloud);
    real zero = hu_central_moment(0, 0, 0, cloud);
    
    return central / (pow(zero, ((p + q + r)/3) + 1));
}

/**
 * \brief Calcula o momento normalizado refinado segundo (Guo, 1993) de Hu 3D
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento normalizado p+q+r por Guo da nuvem cloud
 */
real hu_refined_moment(int p, int q, int r, struct cloud* cloud)
{
	real central = hu_central_moment(p, q, r, cloud);
	real zero = hu_central_moment(0, 0, 0, cloud);
	real x = hu_central_moment(p, 0, 0, cloud);
	real y = hu_central_moment(0, q, 0, cloud);
	real z = hu_central_moment(0, 0, r, cloud);
	int size = cloud_size(cloud);
	
	return (central * pow(zero, 2)) / (x * y * z * size); // t+s
	
    //return hu_central_moment(p, q, r, cloud) / cloud_size(cloud); // t+r
}

/**
 * \brief Calcula os momentos invariantes de Hu segundo (SADJADI e HALL; 1980)
 * \param cloud A nuvem alvo
 * \return A matriz aonde os momentos serão salvos
 */
struct matrix* hu_cloud_moments_hu1980(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, 3);
	
    real hu200 = hu_refined_moment(2, 0, 0, cloud);
    real hu020 = hu_refined_moment(0, 2, 0, cloud);
    real hu002 = hu_refined_moment(0, 0, 2, cloud);
    real hu110 = hu_refined_moment(1, 1, 0, cloud);
    real hu101 = hu_refined_moment(1, 0, 1, cloud);
    real hu011 = hu_refined_moment(0, 1, 1, cloud);

    real j1 = hu200 + hu020 + hu002;
    real j2 = hu200*hu020 + hu200*hu002 + hu020*hu002 -
              hu110*hu110 - hu101*hu101 - hu011*hu011;
    real j3 = hu200*hu020*hu002 + 2*hu110*hu101*hu011 -
              hu002*hu110*hu110 - hu020*hu101*hu101 - hu200*hu011*hu011;
	
    matrix_set(results, 0, 0, j1);
    matrix_set(results, 0, 1, j2);
    matrix_set(results, 0, 2, j3);

    return results;
}

/**
 * \brief Calcula o momento central de Hu 2D no plano XY
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param cloud A nuvem alvo
 * \return O momento central(p,q) da nuvem cloud em relação ao plano XY
 */
real hu_central_moment2D_xy(int p, int q, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real moment = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].y - center->y, q)
                * vector3_distance(&cloud->points[i], center);
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
real hu_central_moment2D_xz(int p, int q, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real moment = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].x - center->x, p)
                * pow(cloud->points[i].z - center->z, q)
                * vector3_distance(&cloud->points[i], center);
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
real hu_central_moment2D_yz(int p, int q, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real moment = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].y - center->y, p)
                * pow(cloud->points[i].z - center->z, q)
                * vector3_distance(&cloud->points[i], center);
	}
	
	vector3_free(center);
	
    return moment;
}

/**
 * \brief Calcula os momentos invariantes de Hu segundo (SIQUEIRA; 2018)
 * \param cloud A nuvem alvo
 * \return A matriz aonde os momentos serão salvos
 */
struct matrix* hu_cloud_moments_husiq(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, 12);
	real a;
    real b;
    real c;
    real d;
    real e;
    real f;
    real g;
    real i1;
    real i2;
    real i3;
    real i4;
    real i5;
    real i6;
    real i7;
    real i8;
    real i9;
    real i10;
    real i11;
    real i12;
    
    a = hu_central_moment2D_xy(0, 2, cloud);
    b = hu_central_moment2D_xy(0, 3, cloud);
    c = hu_central_moment2D_xy(1, 1, cloud);
    d = hu_central_moment2D_xy(1, 2, cloud);
    e = hu_central_moment2D_xy(2, 0, cloud);
    f = hu_central_moment2D_xy(2, 1, cloud);
    g = hu_central_moment2D_xy(3, 0, cloud);
    
    i1 = e + a;
    i2 = pow((e - a), 2) + 4*pow(c, 2);
    i3 = pow((g - 3*d), 2) + pow((3*f - b), 2);
    i4 = pow((g + d), 2) + pow((f + b), 2);
    
    a = hu_central_moment2D_xz(0, 2, cloud);
    b = hu_central_moment2D_xz(0, 3, cloud);
    c = hu_central_moment2D_xz(1, 1, cloud);
    d = hu_central_moment2D_xz(1, 2, cloud);
    e = hu_central_moment2D_xz(2, 0, cloud);
    f = hu_central_moment2D_xz(2, 1, cloud);
    g = hu_central_moment2D_xz(3, 0, cloud);
    
    i5 = e + a;
    i6 = pow((e - a), 2) + 4*pow(c, 2);
    i7 = pow((g - 3*d), 2) + pow((3*f - b), 2);
    i8 = pow((g + d), 2) + pow((f + b), 2);
    
    a = hu_central_moment2D_yz(0, 2, cloud);
    b = hu_central_moment2D_yz(0, 3, cloud);
    c = hu_central_moment2D_yz(1, 1, cloud);
    d = hu_central_moment2D_yz(1, 2, cloud);
    e = hu_central_moment2D_yz(2, 0, cloud);
    f = hu_central_moment2D_yz(2, 1, cloud);
    g = hu_central_moment2D_yz(3, 0, cloud);
    
    i9 = e + a;
    i10 = pow((e - a), 2) + 4*pow(c, 2);
    i11 = pow((g - 3*d), 2) + pow((3*f - b), 2);
    i12 = pow((g + d), 2) + pow((f + b), 2);
    
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
 * \brief Calcula os momentos invariantes de Hu segundo (ROCHA NETO; 2017)
 * \param cloud A nuvem alvo
 * \return A matriz aonde os momentos serão salvos
 */
struct matrix* hu_cloud_moments_hututu(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, HU_MOMENTS);
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

    i2 = pow((e - a), 2) + 4*pow(c, 2);

    i3 = pow((g - 3*d), 2) + pow((3*f - b), 2);

    i4 = pow((g + d), 2) + pow((f + b), 2);

    i5 = (g - 3*d)*(g + d)*(pow((g + d), 2) - 3*pow((f + b), 2)) +
               (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    i6 = (e - a)*(pow((g + d), 2) - pow((f + b), 2)) +
               4*c*(g + d) * (f + b);

    i7 = (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2)) -
               (g - 3*d)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    matrix_set(results, 0, 0, i1);
    matrix_set(results, 0, 1, i2);
    matrix_set(results, 0, 2, i3);
    matrix_set(results, 0, 3, i4);
    matrix_set(results, 0, 4, i5);
    matrix_set(results, 0, 5, i6);
    matrix_set(results, 0, 6, i7);
	
    a = hu_normalized_moment(0, 0, 2, cloud);
    b = hu_normalized_moment(0, 0, 3, cloud);
    c = hu_normalized_moment(1, 0, 1, cloud);
    d = hu_normalized_moment(1, 0, 2, cloud);
    e = hu_normalized_moment(2, 0, 0, cloud);
    f = hu_normalized_moment(2, 0, 1, cloud);
    g = hu_normalized_moment(3, 0, 0, cloud);

    i1 = e + a;

    i2 = pow((e - a), 2) + 4*pow(c, 2);

    i3 = pow((g - 3*d), 2) + pow((3*f - b), 2);

    i4 = pow((g + d), 2) + pow((f + b), 2);

    i5 = (g - 3*d)*(g + d)*(pow((g + d), 2) - 3*pow((f + b), 2)) +
               (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    i6 = (e - a)*(pow((g + d), 2) - pow((f + b), 2)) +
               4*c*(g + d) * (f + b);

    i7 = (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2)) -
               (g - 3*d)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    matrix_set(results, 0, 7, i1);
    matrix_set(results, 0, 8, i2);
    matrix_set(results, 0, 9, i3);
    matrix_set(results, 0, 10, i4);
    matrix_set(results, 0, 11, i5);
    matrix_set(results, 0, 12, i6);
    matrix_set(results, 0, 13, i7);
	
    a = hu_normalized_moment(0, 0, 2, cloud);
    b = hu_normalized_moment(0, 0, 3, cloud);
    c = hu_normalized_moment(0, 1, 1, cloud);
    d = hu_normalized_moment(0, 1, 2, cloud);
    e = hu_normalized_moment(0, 2, 0, cloud);
    f = hu_normalized_moment(0, 2, 1, cloud);
    g = hu_normalized_moment(0, 3, 0, cloud);

    i1 = e + a;

    i2 = pow((e - a), 2) + 4*pow(c, 2);

    i3 = pow((g - 3*d), 2) + pow((3*f - b), 2);

    i4 = pow((g + d), 2) + pow((f + b), 2);

    i5 = (g - 3*d)*(g + d)*(pow((g + d), 2) - 3*pow((f + b), 2)) +
               (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    i6 = (e - a)*(pow((g + d), 2) - pow((f + b), 2)) +
               4*c*(g + d) * (f + b);

    i7 = (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2)) -
               (g - 3*d)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    matrix_set(results, 0, 14, i1);
    matrix_set(results, 0, 15, i2);
    matrix_set(results, 0, 16, i3);
    matrix_set(results, 0, 17, i4);
    matrix_set(results, 0, 18, i5);
    matrix_set(results, 0, 19, i6);
    matrix_set(results, 0, 20, i7);

    return results;
}

/**
 * \brief Calcula momentos normalizados de diferentes ordens
 * \param cloud A nuvem alvo
 * \return A matriz aonde os momentos serão salvos
 */
struct matrix* hu_superset(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, HU_SUPERSET_MOMENTS);
	
	int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= HU_SUPERSET_ORDER; p++) {
        for (q = 0; q <= HU_SUPERSET_ORDER; q++) {
            for (r = 0; r <= HU_SUPERSET_ORDER; r++) {
                matrix_set(results, 0, col, hu_normalized_moment(p,q,r,cloud));
                col++;
            }
        }
    }
	
	return results;
}

#endif // HU_H

