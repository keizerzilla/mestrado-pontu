/**
 * @file tchebychev.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de Tchebychev.
 *
 * Os momentos de Tchebychev são calculados com base no próprio espaço dos
 * pontos da nuvem. Ou seja, diferente de outros momentos, não há necessidade de
 * efetuar uma transformação das coordenadas dos pontos de um domínio para
 * outro.
 */

#ifndef TCHEBYCHEV_H
#define TCHEBYCHEV_H

#define TCHEBYCHEV_ORDER 2

#include "cloud.h"
#include "matrix.h"

/**
 * @brief tchebychev_poly Calcula o polinômio de Tchebychev
 * @param p A ordem do polinômio
 * @param n O número de pontos da nuvem
 * @param x O argumento do polinômio
 * @return O polinômio de Tchebychev de ordem(p)
 */
real tchebychev_poly(int p, int n, real x)
{
    if (p <= 0)
        return 1.0f;

    if (p == 1)
        return x;

    real num1 = ((2 * p) - 1) * x * tchebychev_poly(p - 1, n, x);
    real num2 = (p - 1) * (1 - (((p - 1) * (p - 1)) / (n * n))) *
                tchebychev_poly(p - 2, n, x);

    return (num1 - num2) / p;
}

/**
 * @brief tchebychev_norm A norma dos polinômios de Tchebychev
 * @param p A ordem do polinômio
 * @param n O número de pontos da nuvem
 * @return A norma do polinômio de ordem(p)
 */
real tchebychev_norm(int p, int n)
{
    real norm = 1.0f * n;
    real num_pts = 1.0f * n;

    int i;
    for (i = 1; i <= p; i++)
        norm *= (1.0f - ((i * i) / num_pts));

    return norm / (p + 1.0f);
}

/**
 * @brief tchebychev_moment Calcula um momento de Tchebychev
 * @param p A ordem da dimensão x
 * @param q A ordem da dimensão y
 * @param r A ordem da dimensão z
 * @param n O número de pontos da nuvem
 * @param cloud A nuvem alvo
 * @return O momento de Tchebychev de ordem(p,q,r)
 */
real tchebychev_moment(int p, int q, int r, int n, struct cloud* cloud)
{
    real moment = 0.0f;
    real norm = tchebychev_norm(p, n) *
                tchebychev_norm(q, n) *
                tchebychev_norm(r, n);
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++)
        moment += tchebychev_poly(p, n, cloud->points[i].x - center->x) *
                  tchebychev_poly(q, n, cloud->points[i].y - center->y) *
                  tchebychev_poly(r, n, cloud->points[i].z - center->z) *
                  vector3_distance(&cloud->points[i], center);

    return moment / norm;
}

/**
 * @brief tchebychev_cloud_moments Calcula todos os momentos de Tchebychev
 * @param cloud A nuvem alvo
 * @param cut O corte da nuvem
 * @param results A matriz aonde os resultados serão salvos
 */
void tchebychev_cloud_moments(struct cloud* cloud, struct matrix* results)
{
    int p = 0;
    int q = 0;
    int r = 0;
    int row = 0;
    int col = 0;
    int n = cloud_num_of_points(cloud);

    for (p = 0; p <= TCHEBYCHEV_ORDER; p++) {
        for (q = 0; q <= TCHEBYCHEV_ORDER; q++) {
            for (r = 0; r <= TCHEBYCHEV_ORDER+1; r++) {
                matrix_set(results, row, col, tchebychev_moment(p, q, r,
                                                                n, cloud));
                col++;
            }
        }
    }
}

#endif // TCHEBYCHEV_H
