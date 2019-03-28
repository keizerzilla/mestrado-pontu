/**
 * \file lechong.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementações necessárias para o cálculo dos momentos de Lechong
 * para nuvens de pontos. O momentos de Lechong são derivados da relação
 * recursiva dos polinômios de Lechong.
 */

#ifndef LECHONG_H
#define LECHONG_H

#define LECHONG_ORDER 2
#define LECHONG_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o polinômio de Lechong
 * \param n A ordem do polinômio
 * \param x O argumento do polinômio
 * \return O polinômio de x de ordem n
 */
real lechong_poly(int n, real x)
{
    if (n < 0)
        return 0.0f;

    if (n == 0)
        return 1.0f;

    if (n == 1)
        return x;

    return (((2 * n) - 1) * x * lechong_poly(n-1, x) - (n - 1) *
            lechong_poly(n - 2, x)) / (1.0f * n);
}

/**
 * \brief Calcula a constante de normalização
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param num_pts Número de pontos da nuvem
 * \return A constante de normalização(p,q,r)
 */
real lechong_norm(int p, int q, int r, int num_pts)
{
    return (((2 * p) + 1) * ((2 * q) + 1) * ((2 * r) + 1)) / (1.0f * num_pts);
}

/**
 * \brief Normaliza uma coordenada no intervalo [-1,+1]
 * \param c Coordenada a ser normalizada
 * \param num_pts Número de pontos da nuvem
 * \return A coordenada normalizada
 */
real lechong_scale_coord(real c, int num_pts)
{
	return ((2 * c) - num_pts - 1)/(num_pts - 1);
}

/**
 * \brief Calcula o momento de lechong
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento de ordem p+q+r da nuvem cloud
 */
real lechong_moment(int p, int q, int r, struct cloud* cloud)
{
    struct vector3* center = cloud_get_center(cloud);
    center->x = lechong_scale_coord(center->x, cloud->num_pts);
    center->y = lechong_scale_coord(center->y, cloud->num_pts);
    center->z = lechong_scale_coord(center->z, cloud->num_pts);
    
    real moment = 0.0f;
	real xn = 0.0f;
	real yn = 0.0f;
	real zn = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
    	xn = lechong_scale_coord(cloud->points[i].x, cloud->num_pts);
    	yn = lechong_scale_coord(cloud->points[i].y, cloud->num_pts);
    	zn = lechong_scale_coord(cloud->points[i].z, cloud->num_pts);
    	
    	struct vector3* temp = vector3_new(xn, yn, zn);
    	
        moment += lechong_poly(p, xn - center->x) *
                  lechong_poly(q, yn - center->y) *
                  lechong_poly(r, zn - center->z) *
                  vector3_distance(temp, center);
		
		vector3_free(temp);
	}
	
	vector3_free(center);
	
    return lechong_norm(p, q, r, cloud->num_pts) * moment;
}

/**
 * \brief Calcula o momento escalado (???) de Lechong -- matematicamente errado!
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento escalado (???) p+q+r da nuvem cloud
 */
real lechong_psi(int p, int q, int r, struct cloud* cloud)
{
	real xi = 2;
	real psi = lechong_moment(p, q, r, cloud);
	real zero = lechong_moment(0, 0, 0, cloud);
	
	real xord = lechong_moment(p + xi, 0, 0, cloud);
	real yord = lechong_moment(0, q + xi, 0, cloud);
	real zord = lechong_moment(0, 0, r + xi, cloud);
	
	return (psi * pow(zero, xi)) / (xord * yord * zord);
}

/**
 * \brief Calcula os momentos de Lechong de uma nuvem
 * \param cloud A nuvem alvo
 * \return A matrix aonde os resultados serão salvos
 */
struct matrix* lechong_cloud_moments(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, LECHONG_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= LECHONG_ORDER; p++) {
        for (q = 0; q <= LECHONG_ORDER; q++) {
            for (r = 0; r <= LECHONG_ORDER; r++) {
                matrix_set(results, 0, col, lechong_psi(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // LECHONG_H

