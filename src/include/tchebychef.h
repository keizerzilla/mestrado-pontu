/**
 * \file tchebychef.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Novos momentos de Tchebychef??? Posso brincar a vontade!!!
 */

#ifndef TCHEBYCHEF_H
#define TCHEBYCHEF_H

#define TCHEBYCHEF_ORDER 2
#define TCHEBYCHEF_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o polinômio de tchebychef
 * \param p A ordem do polinômio
 * \param n O número de pontos da nuvem
 * \param x O argumento do polinômio
 * \return O polinômio de tchebychef de ordem(p)
 */
real tchebychef_poly(int p, int n, real x)
{
    if (p <= 0)
        return 1.0f;

    if (p == 1)
        return x;

    real num1 = ((2 * p) - 1) * x * tchebychef_poly(p - 1, n, x);
    real num2 = (p - 1) * (1 - (((p - 1) * (p - 1)) / (n * n))) *
                tchebychef_poly(p - 2, n, x);

    return (num1 - num2) / p;
}

/**
 * \brief A norma dos polinômios de tchebychef
 * \param p A ordem do polinômio
 * \param n O número de pontos da nuvem
 * \return A norma do polinômio de ordem(p)
 */
real tchebychef_norm(int p, int n)
{
    real norm = 1.0f * n;
    real num_pts = 1.0f * n;
    real num_quad = num_pts * num_pts;

    int i;
    for (i = 1; i <= p; i++)
        norm *= (1.0f - ((i * i) / num_quad));

    return (n * norm) / (p + 1.0f);
}

/**
 * \brief Calcula um momento de tchebychef
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param n O número de pontos da nuvem
 * \param cloud A nuvem alvo
 * \return O momento de tchebychef de ordem(p,q,r)
 */
real tchebychef_moment(int p, int q, int r, int n, struct cloud* cloud)
{
    real moment = 0.0f;
    real max_radius = cloud_max_distance_from_center(cloud);
    /**
    real norm = tchebychef_norm(p, n) *
                tchebychef_norm(q, n) *
                tchebychef_norm(r, n);
    */
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++)
        moment += tchebychef_poly(p, n, cloud->points[i].x - center->x) *
                  tchebychef_poly(q, n, cloud->points[i].y - center->y) *
                  tchebychef_poly(r, n, cloud->points[i].z - center->z) *
                  vector3_distance(&cloud->points[i], center);
	
    return moment / max_radius;
}

/**
 * \brief Calcula todos os momentos de tchebychef
 * \param cloud A nuvem alvo
 * \param cut O corte da nuvem
 * \return A matriz aonde os resultados serão salvos
 */
struct matrix* tchebychef_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, TCHEBYCHEF_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int row = 0;
    int col = 0;
    int n = cloud_size(cloud);

    for (p = 0; p <= TCHEBYCHEF_ORDER; p++) {
        for (q = 0; q <= TCHEBYCHEF_ORDER; q++) {
            for (r = 0; r <= TCHEBYCHEF_ORDER; r++) {
                matrix_set(results, row, col, tchebychef_moment(p, q, r,
                                                               n, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // TCHEBYCHEF_H

