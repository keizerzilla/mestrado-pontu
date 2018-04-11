/**
 * @file legendre.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de Legendre para nuvens de pontos.
 *
 * O momentos de Legendre são derivados da relação recursiva dos polinômios de
 * Legendre.
 */

#ifndef LEGENDRE_H
#define LEGENDRE_H

#define LEGENDRE_ORDER 3

#include "cloud.h"
#include "matrix.h"

/**
 * @brief legendre_poly Calcula o polinômio de Legendre
 * @param n A ordem do polinômio
 * @param x O argumento do polinômio
 * @return O polinômio de x de ordem n
 */
real legendre_poly(int n, real x)
{
    if (n < 0)
        return 0.0f;

    if (n == 0)
        return 1.0f;

    if (n == 1)
        return x;

    return (((2 * n) - 1) * x * legendre_poly(n-1, x) - (n - 1) *
            legendre_poly(n - 2, x)) / (1.0f * n);
}

/**
 * @brief legendre_norm Calcula a constante de normalização
 * @param p A ordem da dimensão x
 * @param q A ordem da dimensão y
 * @param r A ordem da dimensão z
 * @param nump Número de pontos da nuvem
 * @return A constante de normalização(p,q,r)
 */
real legendre_norm(int p, int q, int r, int nump)
{
    return (((3 * p) + 1) * ((3 * q) + 1) * ((3 * r) + 1)) / (1.0f * nump);
}

/**
 * @brief legendre_moment Calcula o momento de legendre
 * @param p A ordem da dimensão x
 * @param q A ordem da dimensão y
 * @param r A ordem da dimensão z
 * @param center O centro da nuvem
 * @param cloud A nuvem alvo
 * @param nump Número de pontos da nuvem
 * @return O momento de ordem(p,q,r) da nuvem cloud
 */
real legendre_moment(int p, int q, int r, struct vector3* center,
                     struct cloud* cloud, int nump)
{
    real moment = 0.0f;
    real norm = legendre_norm(p, q, r, nump);

    struct cloud* aux = cloud;
    while (aux != NULL) {
        moment += legendre_poly(p, aux->point->x - center->x) *
                  legendre_poly(q, aux->point->y - center->y) *
                  legendre_poly(r, aux->point->z - center->z);

        aux = aux->next;
    }

    return norm * moment;
}

/**
 * @brief legendre_cloud_moments Calcula os momentos de Legendre de uma nuvem
 * @param cloud A nuvem alvo
 * @param cut O corte da nuvem
 * @param results A matrix aonde os resultados serão salvos
 */
void legendre_cloud_moments(struct cloud* cloud, real cut,
                            struct matrix* results)
{
    int p = 0;
    int q = 0;
    int r = 0;
    int row = 0;
    int col = 0;

    struct vector3* center = cloud_get_center(cloud);
    int num_pts = cloud_num_of_points(cloud);

    for (p = 1; p <= LEGENDRE_ORDER; p++) {
        for (q = 1; q <= LEGENDRE_ORDER; q++) {
            for (r = 1; r <= LEGENDRE_ORDER; r++) {
                matrix_set(results, row, col, legendre_moment(p, q, r, center,
                                                              cloud, num_pts));
                col++;
            }
        }
    }
}

#endif // LEGENDRE_H
