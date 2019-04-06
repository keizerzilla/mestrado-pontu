/**
 * \file golden.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementação de momentos baseados na razão áurea Phi.
 */

#ifndef GOLDEN_H
#define GOLDEN_H

#define GOLDEN_ORDER 2
#define GOLDEN_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Função imagem baseada na razão áurea
 * \param a O valor de coordenada
 * \param p Ordem associada à coordenada
 * \return Real representativo da relação áurea
 */
real golden_ratio(real a, int k)
{
	return pow(a*a - a - 1, k);
}

/**
 * \brief Norma dos momentos de ouro
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return A norma de ouro de ordem p+q+r de cloud
 */
real golden_norm(int p, int q, int r, struct cloud* cloud)
{
	return cloud_size(cloud);
}

/**
 * \brief Cálculo dos momentos de ouro
 * \param p A ordem da coordena x
 * \param q A ordem da coordena y
 * \param r A ordem da coordena z
 * \param cloud A nuvem alvo cujos momentos se quer calcular
 * \return O momento de ouro de ordem p+q+r de cloud
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
		
        moment += golden_ratio(center_x, p)
                * golden_ratio(center_y, q)
                * golden_ratio(center_z, r)
                * vector3_distance(center, &cloud->points[i]);
	}
	
	vector3_free(center);
	
	return moment / golden_norm(p, q, r, cloud);
}

/**
 * \brief Cálculo completo dos momentos de ouro de uma nuvem de pontos
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

