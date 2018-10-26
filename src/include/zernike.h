/**
 * \file zernike.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de Zernike. Os momentos de Zernike mapeam um conjunto de dados no
 * domínio complexo, minimizando redundâncias e sobreposição de informações
 * (eu nunca estive tão errado...).
 */

#ifndef ZERNIKE_H
#define ZERNIKE_H

#define ZERNIKE_PARAM 12
#define ZERNIKE_MOMENTS 49

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o fatorial de um número inteiro
 * \param n O número que ser quer calcular o fatorial
 * \return O fatorial de n
 */
int zernike_factorial(int n)
{
    int result = 1;
    
    for (uint c = 1; c <= n; c++)
        result *= c;

    return result;
}

/**
 * \brief As condições de contorno dos polinômios de Zernike
 * \param n A ordem do polinômio
 * \param m A repetição
 * \return 1 se as condições forem satisfeitas, 0 caso-contrário
 */
int zernike_conditions(int n, int m)
{
	return !(n < m || (n - m) % 2);
}

/**
 * \brief Calcula um polinômio radial de Zernike
 * \param n A ordem do polinômio
 * \param m A repetição
 * \param distance A distância em radial
 * \return O polinômio radial de Zernike de ordem(n) e repetição(m)
 */
real zernike_radpoly(int n, int m, real distance)
{
    real radpoly = 0.0f;
    
    for (uint s = 0; s <= (n - m) / 2; s++) {
    	real num = pow(-1, s) * (zernike_factorial(n - s));
    	
	    real den = zernike_factorial(s) *
                   zernike_factorial(((n + m) / 2) - s) *
                   zernike_factorial(((n - m) / 2) - s);
        
        radpoly += (num * pow(distance, n - (2 * s))) / den;
	}
	
    return radpoly;
}

/**
 * \brief zernike_azimuth Calcula o ângulo azimutal
 * \param point O ponto alvo
 * \return O ângulo do vetor
 */
real zernike_azimuth(struct vector3* point)
{
    return atan2(point->z, point->y);
}

/**
 * \brief Calcula um momento de Zernike
 * \param n A ordem do polinômio
 * \param m A repetição do polinômio
 * \param r O raio de atuação
 * \param cloud A nuvem alvo
 * \return O momento de Zernike de ordem(n) e repetição(m)
 */
real zernike_moment(int n, int m, real r, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
    
    real dist = 0.0f;
    real poly = 0.0f;
    real angle = 0.0f;
    real ef = 0.0f;
    real moment = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        dist = vector3_distance(center, &cloud->points[i]) / r;
        poly = zernike_radpoly(n, m, dist);
        angle = zernike_azimuth(&cloud->points[i]);
        ef = cos(m * angle);
        moment += poly * ef;
    }
	
	vector3_free(center);
	
    return (moment * (n + 1.0f)) / CALC_PI;
}

/**
 * \brief Calcula todos os momentos de Zernike deuma nuvem
 * \param cloud A nuvem alvo
 * \param cut O corte da nuvem
 * \return A matriz onde os resultados serão salvos
 */
struct matrix* zernike_cloud_moments(struct cloud* cloud)
{
	struct matrix* results = matrix_new(1, ZERNIKE_MOMENTS);
    real r = cloud_max_distance(cloud);

    int n = 0;
    int m = 0;
    int row = 0;
    int col = 0;
	
    for (n = 0; n <= ZERNIKE_PARAM; n++) {
        for (m = 0; m <= ZERNIKE_PARAM; m++) {
            if (zernike_conditions(n, m)) {
                matrix_set(results, row, col, zernike_moment(n, m, r, cloud));
                col++;
            }
        }
    }
    
    return results;
}

#endif // ZERNIKE_H

