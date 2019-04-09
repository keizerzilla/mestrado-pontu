/**
 * \file tutu.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementação dos "Momentos Tutu" (autoria minha??? SIM!!!)
 */

#ifndef TUTU_H
#define TUTU_H

#define TUTU_ORDER 2
#define TUTU_MOMENTS 27

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
real tutu_quad(real x, real y, real z, int p, int q, int r)
{
	return sqrt(pow(x, 2*p) + pow(y, 2*q) + pow(z, 2*q));
}

/**
 * \brief Cálculo dos momentos esféricos normalizados
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return O momento esférico normalizado de ordem p+q+r de cloud
 */
real tutu_norm(int p, int q, int r, struct cloud* cloud)
{
	//real radius = cloud_max_distance_from_center(cloud);
	//real v = (4.0f * CALC_PI * pow(radius, 3)) / (3.0f * cloud_size(cloud));
	//return v;
	
	real V = cloud_boundingbox_volume(cloud);
	real A = cloud_boundingbox_area(cloud);
	return (pow(CALC_PI, 1.0f/3.0f) * pow(6.0f * V, 2.0f/3.0f)) / A;
	
	//return cloud_size(cloud);
}

/**
 * \brief Cálculo dos momentos esféricos
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return O momento esférico de ordem p+q+r de cloud
 */
real tutu_moment(int p, int q, int r, struct cloud* cloud)
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
                * tutu_quad(center_x, center_y, center_z, p, q, r);
	}
	
	vector3_free(center);
	
	return moment * tutu_norm(p, q, r, cloud);
}

/**
 * \brief Cálculo completo dos momentos esféricos de uma nuvem de pontos
 * \param cloud A nuvem alvo
 * \return Matriz contendo os momentos calculados
 */
struct matrix* tutu_cloud_moments(struct cloud* cloud)
{
	/**
    struct matrix* results = matrix_new(1, TUTU_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= TUTU_ORDER; p++) {
        for (q = 0; q <= TUTU_ORDER; q++) {
            for (r = 0; r <= TUTU_ORDER; r++) {
                matrix_set(results, 0, col, tutu_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
    */
    
    struct matrix* results = matrix_new(1, 1);
    matrix_set(results, 0, 0, tutu_moment(2, 2, 2, cloud));
    return results;
}

#endif // TUTU_H

