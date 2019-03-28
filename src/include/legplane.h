/**
 * \file legplane.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementações necessárias para o cálculo dos momentos de Legplane
 * para nuvens de pontos. O momentos de Legplane são derivados da relação
 * recursiva dos polinômios de Legplane.
 */

#ifndef LEGPLANE_H
#define LEGPLANE_H

#define LEGPLANE_ORDER 2
#define LEGPLANE_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o polinômio de Legplane
 * \param n A ordem do polinômio
 * \param x O argumento do polinômio
 * \return O polinômio de x de ordem n
 */
real legplane_poly(int n, real x)
{
    if (n < 0)
        return 0.0f;

    if (n == 0)
        return 1.0f;

    if (n == 1)
        return x;

    return (((2 * n) - 1) * x * legplane_poly(n-1, x) - (n - 1) *
            legplane_poly(n - 2, x)) / (1.0f * n);
}

/**
 * \brief Calcula a constante de normalização
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param num_pts Número de pontos da nuvem
 * \return A constante de normalização(p,q,r)
 */
real legplane_norm(int p, int q, int r, int num_pts)
{
    return (((2 * p) + 1) * ((2 * q) + 1) * ((2 * r) + 1)) / (1.0f * num_pts);
}

/**
 * \brief Calcula o momento de legplane
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento de ordem p+q+r da nuvem cloud
 */
real legplane_moment(int p, int q, int r, struct cloud* cloud)
{
    real moment = 0.0f;
    real norm = legplane_norm(p, q, r, cloud->num_pts);
    
    struct vector3* center = cloud_get_center(cloud);
    struct vector3* direction = cloud_average_direction(cloud);
	struct plane* plane = plane_new(direction, center);
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += legplane_poly(p, cloud->points[i].x - center->x) *
                  legplane_poly(q, cloud->points[i].y - center->y) *
                  legplane_poly(r, cloud->points[i].z - center->z) *
                  plane_distance2point(plane, &cloud->points[i]);
	}
	
	plane_free(plane);
	vector3_free(direction);
	vector3_free(center);
	
    return norm * moment;
}

/**
 * \brief Calcula o momento escalado (???) de Legplane
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento escalado (???) p+q+r da nuvem cloud
 */
real legplane_psi(int p, int q, int r, struct cloud* cloud)
{
	real xi = 0;
	real psi = legplane_moment(p, q, r, cloud);
	real zero = legplane_moment(0, 0, 0, cloud);
	
	real xord = legplane_moment(p + xi, 0, 0, cloud);
	real yord = legplane_moment(0, q + xi, 0, cloud);
	real zord = legplane_moment(0, 0, r + xi, cloud);
	
	return (psi * pow(zero, xi + 2)) / (xord * yord * zord);
}

/**
 * \brief Calcula os momentos de Legplane de uma nuvem
 * \param cloud A nuvem alvo
 * \return A matrix aonde os resultados serão salvos
 */
struct matrix* legplane_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, LEGPLANE_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= LEGPLANE_ORDER; p++) {
        for (q = 0; q <= LEGPLANE_ORDER; q++) {
            for (r = 0; r <= LEGPLANE_ORDER; r++) {
                matrix_set(results, 0, col, legplane_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // LEGPLANE_H

