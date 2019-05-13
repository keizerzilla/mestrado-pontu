/**
 * \file chebyshev.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de chebyshev. Os momentos de chebyshev são calculados com base no
 * próprio espaço dos pontos da nuvem. Ou seja, diferente de outros momentos,
 * não há necessidade de efetuar uma transformação das coordenadas dos pontos de
 * um domínio para outro. Momentos invarintes apenas à translação.
 */

#ifndef CHEBYSHEV_H
#define CHEBYSHEV_H

#ifndef CHEBYSHEV_ORDER_X
#define CHEBYSHEV_ORDER_X 2
#endif

#ifndef CHEBYSHEV_ORDER_Y
#define CHEBYSHEV_ORDER_Y 2
#endif

#ifndef CHEBYSHEV_ORDER_Z
#define CHEBYSHEV_ORDER_Z 2
#endif

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
 * \brief Calcula um momento de chebyshev
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento de chebyshev de ordem p+q+r
 */
real chebyshev_moment(int p, int q, int r, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	int n = cloud_size(cloud);
    real moment = 0.0f;

    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += chebyshev_poly(p, n, cloud->points[i].x - center->x) *
                  chebyshev_poly(q, n, cloud->points[i].y - center->y) *
                  chebyshev_poly(r, n, cloud->points[i].z - center->z) *
                  vector3_distance(&cloud->points[i], center);
	}
	
	vector3_free(center);
	
    return moment;
}

struct cloud* chebyshev_reconstruction(struct matrix* moments, int numpts)
{
	struct cloud* cloud = cloud_new(numpts);
	
	for (int i = 0; i < numpts; i++) {
		// asdf
	}
	
	return cloud;
}

/**
 * \brief Calcula todos os momentos de chebyshev
 * \param cloud A nuvem alvo
 * \param cut O corte da nuvem
 * \return A matriz aonde os resultados serão salvos
 */
struct matrix* chebyshev_cloud_moments(struct cloud* cloud)
{
	int nmoments = (CHEBYSHEV_ORDER_X + 1) *
	               (CHEBYSHEV_ORDER_Y + 1) *
	               (CHEBYSHEV_ORDER_Z + 1);
    struct matrix* results = matrix_new(1, nmoments);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= CHEBYSHEV_ORDER_X; p++) {
        for (q = 0; q <= CHEBYSHEV_ORDER_Y; q++) {
            for (r = 0; r <= CHEBYSHEV_ORDER_Z; r++) {
                matrix_set(results, 0, col, chebyshev_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // CHEBYSHEV_H

