/*
 * @file zernike.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos de Zernike.
 *
 * Os momentos de Zernike mapeam um conjunto de dados em um domínio complexo,
 * minimizando redundâncias ou sobreposição de informações.
 */

#ifndef ZERNIKE_H
#define ZERNIKE_H

#include "cloud.h"
#include "matrix.h"

/*
 * @brief A estrutura zsphere
 *
 * Uma zspehere nada mais é que o subconjunto de uma nuvem delimitada por um
 * raio em torno de um ponto central.
 */
struct zsphere {
    struct cloud* cloud;
    struct vector3* center;
    real r;
    unsigned int norm;
};

/*
 * @brief zsphere_new Aloca espaço para uma nova zsphere
 * @param cloud A nuvem a ser delimitada
 * @param radius O raio da zsphere
 * @return O ponteiro para a zsphere alocada, NULL caso não haja memória
 */
struct zsphere* zsphere_new(struct cloud* cloud, real radius)
{
    struct zsphere* zsphere = malloc(sizeof(struct zsphere));

    if (zsphere == NULL)
        return NULL;

    zsphere->cloud = cloud;
    zsphere->center = cloud_get_center(cloud);
    zsphere->r = radius;
    zsphere->norm = 0;

    return zsphere;
}

/*
 * @brief zsphere_free Libera a memória alocada para uma zsphere (nunca usada)
 * @param zsphere A zsphere alvo
 */
void zsphere_free(struct zsphere* zsphere)
{
    cloud_free(zsphere->cloud);
    vector3_free(zsphere->center);
    free(zsphere);

    zsphere = NULL;
}

/*
 * @brief zernike_factorial_recursive Calcula o fatorial de um número inteiro
 * @param n O número que ser quer calcular o fatorial
 * @return O fatorial de n
 */
int zernike_factorial_recursive(int n)
{
    if (n >= 1)
        return n * zernike_factorial_recursive(n - 1);
    else
        return 1;
}

/*
 * @brief zernike_factorial_loop Calcula o fatorial de um número inteiro
 * @param n O número que ser quer calcular o fatorial
 * @return O fatorial de n
 */
int zernike_factorial_loop(int n)
{
    int result = 1;

    int c;
    for (c = 1; c <= n; c++)
        result = result * c;

    return result;
}

/*
 * @brief zernike_core Calcula o core do polinômio de Zernike
 * @param n A ordem do polinômio
 * @param m A repetição do polinômio
 * @param s O indíce do somatório
 * @return O core de ordem(n,m) e índice(s)
 */
real zernike_core(int n, int m, int s)
{
    real num = pow(-1, s) * (zernike_factorial_loop(n - s));
    real den = zernike_factorial_loop(s) *
               zernike_factorial_loop(((n + m) / 2) - s) *
               zernike_factorial_loop(((n - m) / 2) - s);

    return (num / den);
}

/*
 * @brief zernike_radial_distance Distância radial entre um ponto e o centro
 * @param zsphere A zsphere alvo
 * @param point O ponto alvo
 * @return A distância radial (normalizada)
 */
real zernike_radial_distance(struct zsphere* zsphere, struct vector3* point)
{
    return vector3_distance(zsphere->center, point) / zsphere->r;
}

/*
 * @brief zernike_conditions As condições de contorno dos polinômios de Zernike
 * @param n A ordem do polinômio
 * @param m A repetição
 * @return 1 se as condições forem satisfeitas, 0 caso-contrário
 */
int zernike_conditions(int n, int m)
{
    if(n < m || (n-m)%2)
        return 0;
    else
        return 1;
}

/*
 * @brief zernike_polyn Calcula um polinômio regular de Zernike
 * @param n A ordem do polinômio
 * @param m A repetição
 * @param distance A distância em questão
 * @return O polinômio regular de Zernike de ordem(n) e repetição(m)
 */
real zernike_poly(int n, int m, real distance)
{
    real radpoly = 0.0f;

    int s = 0;
    for (s = 0; s <= (n - m) / 2; s++)
        radpoly += (zernike_core(n, m, s) * pow(distance, n - (2 * s)));

    return radpoly;
}

/*
 * @brief zernike_azimuth Calcula o ângulo azimutal de um vetor (ponto)
 * @param point O ponto alvo
 * @return O ângulo do vetor no plano(x,y)
 */
real zernike_azimuth(struct vector3* point)
{
    return atan2(point->y, point->x);
}

/*
 * @brief zernike_even A parte par de um polinômio radial de Zernike
 * @param radpoly O polinômio radial
 * @param m A repeticao do polinomio
 * @param angle O ângulo azimutal do polinômio associado
 * @return A parte par do polinômio complexo
 */
real zernike_even(real radpoly, int m, real angle)
{
    return radpoly * cos(angle*m);
}

/*
 * @brief zernike_odd A parte impar de um polinômio radial de Zernike
 * @param radpoly O polinômio radial
 * @param m A repeticao do polinomio
 * @param angle O ângulo azimutal do polinômio associado
 * @return A parte ímpar do polinômio complexo
 */
real zernike_odd(real radpoly, int m, real angle)
{
    return radpoly * sin(angle*m);
}

/*
 * @brief zernike_moment Calcula um momento de Zernike
 * @param n A ordem do polinômio
 * @param m A repetição do polinômio
 * @param zsphere A zsphere alvo
 * @return O momento de Zernike de ordem(n) e repetição(m)
 */
real zernike_moment(int n, int m, struct zsphere* zsphere)
{
    real moment = 0.0f;
    real distance = 0.0f;
    real angle = 0.0f;
    real even = 0.0f;
    real odd = 0.0f;
    real poly = 0.0f;

    struct cloud* cloud = zsphere->cloud;
    while (cloud != NULL) {
        distance = zernike_radial_distance(zsphere, cloud->points);

        if (distance <= 1.0f) {
            poly = zernike_poly(n, m, distance);
            angle = zernike_azimuth(cloud->points);
            even = zernike_even(poly, m, angle);
            odd = zernike_odd(poly, m, angle);
            moment += sqrt(pow(even, 2) + pow(odd, 2));

            zsphere->norm += 1;
        }

        cloud = cloud->next;
    }

    return (moment * (n + 1.0f)) / zsphere->norm;
}

/*
 * @brief zernike_cloud_moments Calcula todos os momentos de Zernike deuma nuvem
 * @param cloud A nuvem alvo
 * @param cut O corte da nuvem
 * @param results A matriz onde os resultados serão salvos
 */
void zernike_cloud_moments(struct cloud* cloud, real cut,
                           struct matrix* results)
{
    struct zsphere* zsphere = zsphere_new(cloud, cut);

    int n = 0;
    int m = 0;
    int row = 0;
    int col = 0;

    for (n = 0; n <= 6; n++) {
        for (m = 0; m <= 6; m++) {
            if (zernike_conditions(n, m)) {
                matrix_set(results, row, col, zernike_moment(n, m, zsphere));
                col++;
            }
        }
    }
}

#endif // ZERNIKE_H
