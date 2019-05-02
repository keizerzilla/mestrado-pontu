/**
 * \file zernike.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de Zernike. Os momentos de Zernike mapeam um conjunto de dados no
 * domínio complexo, minimizando redundâncias e sobreposição de informações.
 */

#ifndef ZERNIKE_H
#define ZERNIKE_H

#ifndef ZERNIKE_ORDER
#define ZERNIKE_ORDER 12
#endif

#ifndef ZERNIKE_REPETITION
#define ZERNIKE_REPETITION 8
#endif

#include "cloud.h"
#include "matrix.h"

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
 * \brief As condições de contorno dos polinômios ímpares de Zernike
 * \param n A ordem do polinômio
 * \param m A repetição
 * \return 1 se as condições forem satisfeitas, 0 caso-contrário
 */
int zernike_conditions_odd(int n, int m)
{
	return (!((n - m) % 2) && (m <= n) && (m != 0));
}

/**
 * \brief Função auxiliar para contagem de momentos válidos
 * \param ord Ordem
 * \param rep Repetição
 * \return A quantidade de momentos válidos para param
 */
int zernike_num_moments(int ord, int rep)
{
	int count = 0;
	
	for (int n = 0; n <= ord; n++)
        for (int m = 0; m <= rep; m++)
            if (zernike_conditions(n, m))
                count++;
	
	return count;
}

/**
 * \brief Função auxiliar para contagem de momentos ímpares válidos
 * \param ord Ordem
 * \param rep Repetição
 * \return A quantidade de momentos válidos para param
 */
int zernike_num_moments_odd(int ord, int rep)
{
	int count = 0;
	
	for (int n = 0; n <= ord; n++)
        for (int m = 0; m <= rep; m++)
            if (zernike_conditions_odd(n, m))
                count++;
	
	return count;
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
    	real num = pow(-1, s) * (calc_factorial(n - s));
    	
	    real den = calc_factorial(s) *
                   calc_factorial(((n + m) / 2) - s) *
                   calc_factorial(((n - m) / 2) - s);
        
        radpoly += (num * pow(distance, n - (2 * s))) / den;
	}
	
    return radpoly;
}

/**
 * \brief zernike_azimuth Calcula o ângulo azimutal
 * \param y A coordenada y
 * \param x A coordenada x
 * \return O ângulo do vetor
 */
real zernike_azimuth(real y, real x)
{
    return atan2(y, x);
}

/**
 * \brief zernike_azimuth Calcula o ângulo zenite
 * \param z A coordenada z
 * \param r O raio de zernike
 * \return Zenite
 */
real zernike_zenith(real z, real r)
{
	return acos(z / r);
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
    
	real center_y = 0.0f;
	real center_z = 0.0f;
	real d = 0.0f;
    real dist = 0.0f;
    real poly = 0.0f;
    real azimuth = 0.0f;
	real moment = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
		center_y = cloud->points[i].y - center->y;
        center_z = cloud->points[i].z - center->z;
        
        d = vector3_distance(center, &cloud->points[i]);
        dist = d / r;
        poly = zernike_radpoly(n, m, dist);
        azimuth = zernike_azimuth(center_y, center_z);
        
        moment += poly * (sin(m * azimuth) / CALC_PI);
    }
	
	vector3_free(center);
	
    return ((n + 1.0f) / CALC_PI) * moment;
}

/**
 * \brief Calcula todos os momentos de Zernike deuma nuvem
 * \param cloud A nuvem alvo
 * \return A matriz onde os resultados serão salvos
 */
struct matrix* zernike_cloud_moments(struct cloud* cloud)
{
	int n_moments = zernike_num_moments_odd(ZERNIKE_ORDER, ZERNIKE_REPETITION);
	struct matrix* results = matrix_new(1, n_moments);
    real r = cloud_max_distance_from_center(cloud);

    int n = 0;
    int m = 0;
    int col = 0;
	
    for (n = 0; n <= ZERNIKE_ORDER; n++) {
        for (m = 0; m <= ZERNIKE_REPETITION; m++) {
            if (zernike_conditions_odd(n, m)) {
                matrix_set(results, 0, col, zernike_moment(n, m, r, cloud));
                col++;
            }
        }
    }
    
    return results;
}

/**
 * \brief Superset de momentos de zernike selecionados
 * \param ord A ordem
 * \param rep A repetição
 * \param cloud A nuvem alvo
 * \return A matriz onde os resultados serão salvos
 */
struct matrix* zernike_superset(int ord, int rep, struct cloud* cloud)
{
	real r = cloud_max_distance_from_center(cloud);
	int num_moments = zernike_num_moments(ord, rep);
	struct matrix* results = matrix_new(1, num_moments);
	
    int n = 0;
    int m = 0;
    int col = 0;
	
    for (n = 0; n <= ord; n++) {
        for (m = 0; m <= rep; m++) {
            if (zernike_conditions(n, m)) {
                matrix_set(results, 0, col, zernike_moment(n, m, r, cloud));
                col++;
            }
        }
    }
    
    return results;
}

#endif // ZERNIKE_H


