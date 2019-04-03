/**
 * \file spheric.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementação dos "Momentos Esféricos" (autoria minha???)
 */

#ifndef SPHERIC_H
#define SPHERIC_H

#define SPHERIC_ORDER 2
#define SPHERIC_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Função imagem remanecente da equação da esfera
 * \param x A coordenada x
 * \param y A coordenada y
 * \param z A coordenada z
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \return Real representativo da relação esférica
 */
real spheric_quad(real x, real y, real z, int p, int q, int r)
{
	return sqrt(pow(x, 2*p) + pow(y, 2*q) + pow(z, 2*r));
}

/**
 * \brief Cálculo dos momentos esféricos
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return O momento esférico de ordem p+q+r de cloud
 */
real spheric_moment(int p, int q, int r, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	
	real moment = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;
		
        moment += pow(center_x, p)
                * pow(center_y, q)
                * pow(center_z, r)
                * spheric_quad(center_x, center_y, center_z, p, q, r);
	}
	
	vector3_free(center);
	
	return moment;
}

/**
 * \brief Cálculo dos momentos esféricos normalizados
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return O momento esférico normalizado de ordem p+q+r de cloud
 */
real spheric_norm(int p, int q, int r, struct cloud* cloud)
{
	real central = spheric_moment(p, q, r, cloud);
	real zero = spheric_moment(0, 0, 0, cloud);
	return central / (cloud_max_distance_from_center(cloud) * zero);
}

/**
 * \brief Cálculo completo dos momentos esféricos de uma nuvem de pontos
 * \param cloud A nuvem alvo
 * \return Matriz contendo os momentos calculados
 */
struct matrix* spheric_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, SPHERIC_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= SPHERIC_ORDER; p++) {
        for (q = 0; q <= SPHERIC_ORDER; q++) {
            for (r = 0; r <= SPHERIC_ORDER; r++) {
                matrix_set(results, 0, col, spheric_norm(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

/**
 * \brief Cálculo completo dos momentos esféricos invariantes de uma nuvem
 * \param cloud A nuvem alvo
 * \return Matriz contendo os momentos calculados
 */
struct matrix* spheric_invariant_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, 3);
	real size = cloud_size(cloud) * 1.0f;
	
    real g200 = spheric_moment(2, 0, 0, cloud) / size;
    real g020 = spheric_moment(0, 2, 0, cloud) / size;
    real g002 = spheric_moment(0, 0, 2, cloud) / size;
    real g110 = spheric_moment(1, 1, 0, cloud) / size;
    real g101 = spheric_moment(1, 0, 1, cloud) / size;
    real g011 = spheric_moment(0, 1, 1, cloud) / size;

    real j1 = g200 + g020 + g002;
    real j2 = g200*g020 + g200*g002 + g020*g002 -
              g110*g110 - g101*g101 - g011*g011;
    real j3 = g200*g020*g002 + 2*g110*g101*g011 -
              g002*g110*g110 - g020*g101*g101 - g200*g011*g011;
	
    matrix_set(results, 0, 0, j1);
    matrix_set(results, 0, 1, j2);
    matrix_set(results, 0, 2, j3);

    return results;
}

#endif // SPHERIC_H

