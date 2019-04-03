/**
 * \file golden.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementação de momentos baseado na relação algébrica da razão áurea.
 */

#ifndef GOLDEN_H
#define GOLDEN_H

#define GOLDEN_ORDER 2
#define GOLDEN_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Função imagem baseada na relação algébrica da razão áurea
 * \param x A coordenada x
 * \param y A coordenada y
 * \param z A coordenada z
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \return Real representativo da relação áurea
 */
real golden_ratio(real x, real y, real z, int p, int q, int r)
{
	real xord = x*x - x - 1;
	real yord = y*y - y - 1;
	real zord = z*z - z - 1;
	
	return (p+1)*xord + (q+1)*yord + (r+1)*zord;
}

/**
 * \brief Cálculo dos momentos golden
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return O momento esférico de ordem p+q+r de cloud
 */
real golden_moment(int p, int q, int r, struct cloud* cloud)
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
                * golden_ratio(center_x, center_y, center_z, p, q, r);
	}
	
	vector3_free(center);
	
	return moment / (1.0f * cloud_size(cloud));
}

/**
 * \brief Cálculo completo dos momentos golden de uma nuvem de pontos
 * \param cloud A nuvem alvo
 * \return Matriz contendo os momentos calculados
 */
struct matrix* golden_invariant_moments(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, 3);
	
    real g200 = golden_moment(2, 0, 0, cloud);
    real g020 = golden_moment(0, 2, 0, cloud);
    real g002 = golden_moment(0, 0, 2, cloud);
    real g110 = golden_moment(1, 1, 0, cloud);
    real g101 = golden_moment(1, 0, 1, cloud);
    real g011 = golden_moment(0, 1, 1, cloud);

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

#endif // GOLDEN_H

