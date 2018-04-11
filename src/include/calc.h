/**
 * @file calc.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo com funções matemátias utilitárias e máscaras de tipo de dado.
 */

#ifndef CALC_H
#define CALC_H

#include <math.h>

/**
 * @brief real Uma máscara para o tipo de ponto flutuante mais preciso
 * disponível no sistema (double na maioria dos casos)
 */
typedef double real;

/**
 * @brief calc_squared_length3 Calcula o tamanho quadrático em 3 dimensões
 * @param x Coordenada x
 * @param y Coordenada y
 * @param z Coordenada z
 * @return O tamanho quadrático 3D do ponto(x,y,z)
 */
real calc_squared_length3(real x, real y, real z)
{
    return ((x * x) + (y * y) + (z * z));
}

/**
 * @brief calc_length3 Calcula o tamanho em 3 dimensões
 * @param x Coordenada x
 * @param y Coordenada y
 * @param z Coordenada z
 * @return O tamanho 3D do ponto(x,y,z)
 */
real calc_length3(real x, real y, real z)
{
    return sqrt((x * x) + (y * y) + (z * z));
}

/**
 * @brief calc_squared_length2 Calcula o tamanho quadrático em 2 dimensões
 * @param x Coordenada x
 * @param y Coordenada y
 * @return  O tamanho quadrático 2D do ponto(x,y)
 */
real calc_squared_length2(real x, real y)
{
    return ((x * x) + (y * y));
}

/**
 * @brief calc_length2 Calcula o tamanho em 2 dimensões
 * @param x Coordenada x
 * @param y Coordenada y
 * @return  O tamanho 2D do ponto(x,y)
 */
real calc_length2(real x, real y)
{
    return sqrt((x * x) + (y * y));
}

#endif // CALC_H

