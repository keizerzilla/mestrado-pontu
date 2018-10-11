/**
 * \file chebyshev.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de chebyshev. Os momentos de chebyshev são calculados com base no
 * próprio espaço dos pontos da nuvem. Ou seja, diferente de outros momentos,
 * não há necessidade de efetuar uma transformação das coordenadas dos pontos de
 * um domínio para outro.
 */

#ifndef CHEBYSHEV_H
#define CHEBYSHEV_H

#define CHEBYSHEV_ORDER 2
#define CHEBYSHEV_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o polinômio de chebyshev
 * \param p A ordem do polinômio
 * \param n O número de pontos da nuvem
 * \param x O argumento do polinômio
 * \return O polinômio de chebyshev de ordem(p)
 */
real chebyshev_poly(int p, int n, real x)
{
    if (p <= 0)
        return 1.0f;

    if (p == 1)
        return x;

    real num1 = ((2 * p) - 1) * x * chebyshev_poly(p - 1, n, x);
    real num2 = (p - 1) * (1 - (((p - 1) * (p - 1)) / (n * n))) *
                chebyshev_poly(p - 2, n, x);

    return (num1 - num2) / p;
}

/**
 * \brief A norma dos polinômios de chebyshev
 * \param p A ordem do polinômio
 * \param n O número de pontos da nuvem
 * \return A norma do polinômio de ordem(p)
 */
real chebyshev_norm(int p, int n)
{
    real norm = 1.0f * n;
    real num_pts = 1.0f * n;

    int i;
    for (i = 1; i <= p; i++)
        norm *= (1.0f - ((i * i) / num_pts));

    return norm / (p + 1.0f);
}

/**
 * \brief Calcula um momento de chebyshev
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param n O número de pontos da nuvem
 * \param cloud A nuvem alvo
 * \return O momento de chebyshev de ordem(p,q,r)
 */
real chebyshev_moment(int p, int q, int r, int n, struct cloud* cloud)
{
    real moment = 0.0f;
    real norm = chebyshev_norm(p, n) *
                chebyshev_norm(q, n) *
                chebyshev_norm(r, n);
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++)
        moment += chebyshev_poly(p, n, cloud->points[i].x - center->x) *
                  chebyshev_poly(q, n, cloud->points[i].y - center->y) *
                  chebyshev_poly(r, n, cloud->points[i].z - center->z) *
                  vector3_distance(&cloud->points[i], center);

    return moment / norm;
}

/**
 * \brief Calcula todos os momentos de chebyshev
 * \param cloud A nuvem alvo
 * \param cut O corte da nuvem
 * \return A matriz aonde os resultados serão salvos
 */
struct matrix* chebyshev_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, CHEBYSHEV_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int row = 0;
    int col = 0;
    int n = cloud_size(cloud);

    for (p = 0; p <= CHEBYSHEV_ORDER; p++) {
        for (q = 0; q <= CHEBYSHEV_ORDER; q++) {
            for (r = 0; r <= CHEBYSHEV_ORDER; r++) {
                matrix_set(results, row, col, chebyshev_moment(p, q, r,
                                                               n, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // CHEBYSHEV_H

