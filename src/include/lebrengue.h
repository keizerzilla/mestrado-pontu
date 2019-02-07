/**
 * \file lebrengue.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief lEBRENGUE!!!!! Pepega
 */

#ifndef LEBRENGUE_H
#define LEBRENGUE_H

#define LEBRENGUE_ORDER 2
#define LEBRENGUE_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o polinômio de Legendre
 * \param n A ordem do polinômio
 * \param x O argumento do polinômio
 * \return O polinômio de x de ordem n
 */
real lebrengue_poly(int n, real x)
{
    if (n < 0)
        return 0.0f;

    if (n == 0)
        return 1.0f;

    if (n == 1)
        return x;

    return (((2 * n) - 1) * x * lebrengue_poly(n-1, x) - (n - 1) *
            lebrengue_poly(n - 2, x)) / (1.0f * n);
}

/**
 * \brief Calcula a constante de normalização
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param num_pts Número de pontos da nuvem
 * \return A constante de normalização(p,q,r)
 */
real lebrengue_norm(int p, int q, int r, int num_pts)
{
    return (((3 * p) + 1) * ((3 * q) + 1) * ((3 * r) + 1)) / (1.0f * num_pts);
}

/**
 * \brief Calcula o momento de lebrengue
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento de ordem(p,q,r) da nuvem cloud
 */
real lebrengue_moment(int p, int q, int r, struct cloud* cloud)
{
    real moment = 0.0f;
    real max_radius = cloud_max_distance_from_center(cloud);
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++)
        moment += lebrengue_poly(p, cloud->points[i].x - center->x) *
                  lebrengue_poly(q, cloud->points[i].y - center->y) *
                  lebrengue_poly(r, cloud->points[i].z - center->z) *
                  vector3_distance(&cloud->points[i], center);

    return moment / max_radius;
}

/**
 * \brief Calcula os momentos de Legendre de uma nuvem
 * \param cloud A nuvem alvo
 * \return A matrix aonde os resultados serão salvos
 */
struct matrix* lebrengue_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, LEBRENGUE_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int row = 0;
    int col = 0;

    for (p = 0; p <= LEBRENGUE_ORDER; p++) {
        for (q = 0; q <= LEBRENGUE_ORDER; q++) {
            for (r = 0; r <= LEBRENGUE_ORDER; r++) {
                matrix_set(results, row, col, lebrengue_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // LEBRENGUE_H

