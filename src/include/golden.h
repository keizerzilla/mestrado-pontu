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
	
	//return moment / (1.0f * cloud_size(cloud)); // original
	return moment;
}

/**
 * \brief Cálculo completo dos momentos esféricos de uma nuvem de pontos
 * \param cloud A nuvem alvo
 * \return Matriz contendo os momentos calculados
 */
struct matrix* golden_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, GOLDEN_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= GOLDEN_ORDER; p++) {
        for (q = 0; q <= GOLDEN_ORDER; q++) {
            for (r = 0; r <= GOLDEN_ORDER; r++) {
                matrix_set(results, 0, col, golden_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // GOLDEN_H

