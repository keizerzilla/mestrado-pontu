/**
 * @file vector3.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo uma implementação que manipula vetores em espaços de
 * três dimensões
 *
 * Diversas funções matemáticas são fornecidas e otimizadas ao máximo.
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/**
 * @brief A estrutura vector3.
 *
 * Contém três campos autoexplicativos: as coordenadas x, y e z.
 */
struct vector3 {
    real x;
    real y;
    real z;
};

/**
 * @brief vector3_new Cria um novo vetor
 * @param x A coordenada x
 * @param y A coordenada y
 * @param z A coordenada z
 * @return Um ponteiro para um novo vetor
 */
struct vector3* vector3_new(real x, real y, real z)
{
    struct vector3* v = malloc(sizeof(struct vector3));

    if (v == NULL)
        return NULL;

    v->x = x;
    v->y = y;
    v->z = z;

    return v;
}

/**
 * @brief vector3_free Libera espaço de memória alocado para um vetor
 * @param v O vetor a ser liberado
 */
void vector3_free(struct vector3* v)
{
    free(v);
    v = NULL;
}

/**
 * @brief vector3_add Soma dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O vetor resultante da soma de a com b
 */
struct vector3* vector3_add(struct vector3* a, struct vector3* b)
{
    return vector3_new(a->x + b->x, a->y + b->y, a->z + b->z);
}

/**
 * @brief vector3_sub Subtrái dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O vetor resultante da subtração de a com b
 */
struct vector3* vector3_sub(struct vector3* a, struct vector3* b)
{
    return vector3_new(a->x - b->x, a->y - b->y, a->z - b->z);
}

/**
 * @brief vector3_set Seta os valores de um vetor
 * @param v O vetor alvo
 * @param x O valor da coordenada x
 * @param y O valor da coordenada y
 * @param z O valor da coordenada z
 */
void vector3_set(struct vector3* v, real x, real y, real z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

/**
 * @brief vector3_distance Calcula a distância entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return A distância heuclidiana entra a e b
 */
real vector3_distance(struct vector3* a, struct vector3* b)
{
    return calc_length3(a->x - b->x, a->y - b->y, a->z - a->z);
}

/**
 * @brief vector3_riemann RIEMANN TODO
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return RIEMANN
 */
real vector3_riemann(struct vector3* a, struct vector3* b)
{
    return 0.0f;
}

/**
 * @brief vector3_squared_length Calcula o tamanho quadrático de um vetor
 * @param v O vetor alvo
 * @return O tamanho quadrático de v
 */
real vector3_squared_length(struct vector3* v)
{
    return calc_squared_length3(v->x, v->y, v->z);
}

/**
 * @brief vector3_length Calcula o tamanho de um vetor
 * @param v O vetor alvo
 * @return O tamanho de v
 */
real vector3_length(struct vector3* v)
{
    return calc_length3(v->x, v->y, v->z);
}

/**
 * @brief vector3_squared_length Calcula o tamanho quadrático (2D) de um vetor
 * @param v O vetor alvo
 * @return O tamanho quadrático de v
 */
real vector3_squared_length2(struct vector3* v)
{
    return calc_squared_length2(v->x, v->y);
}

/**
 * @brief vector3_length Calcula o tamanho (2D) de um vetor
 * @param v O vetor alvo
 * @return O tamanho de v
 */
real vector3_length2(struct vector3* v)
{
    return calc_length2(v->x, v->y);
}

/**
 * @brief vector3_scale Escala um vetor através de um fator numérico
 * @param v O vetor alvo
 * @param factor O fator de escalamento
 * @return Um novo vetor resultante do escalamento de v
 */
struct vector3* vector3_scale(struct vector3* v, real f)
{
    struct vector3* p = vector3_new(v->x, v->y, v->z);

    p->x *= f;
    p->y *= f;
    p->z *= f;

    return p;
}

/**
 * @brief vector3_dot Calcula o produto escalar entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O produto escalar entre a e b
 */
real vector3_dot(struct vector3* a, struct vector3* b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

/**
 * @brief vector3_cross Calcula o produto vetorial entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O produto vetorial entre a e b
 */
struct vector3* vector3_cross(struct vector3* a, struct vector3* b)
{
    return vector3_new((a->y * b->z) - (a->z * b->y),
                       (a->z * b->x) - (a->x * b->z),
                       (a->x * b->y) - (a->y * b->x));
}

/**
 * @brief vector3_cross2 Calcula o produto vetorial 2D entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O produto vetorial 2D entre a e b
 */
real vector3_cross2(struct vector3* a, struct vector3* b)
{
    return (a->x * b->y) - (b->x * a->y);
}

/**
 * @brief vector3_push2D Zera o eixo z do vetor
 * @param v O vetor alvo
 */
void vector3_push2D(struct vector3* v) {
    v->z = 0.0f;
}

/**
 * @brief vector3_depth A profundidade relativa a z entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return >0 se o vetor a está mais a frente de b, <0 caso-contrário
 */
int vector3_depth(struct vector3* a, struct vector3* b)
{
    return (a->z) > (b->z);
}

/**
 * @brief vector3_relative Calcula a posição relativa entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @param p O vetor referência
 * @return 1 se o vetor a estiver à direita de b, 0 caso-contrário
 */
int vector3_relative(struct vector3* a, struct vector3* b, struct vector3* p)
{
    struct vector3* pa = vector3_sub(a, p);
    struct vector3* pb = vector3_sub(b, p);

    if (pa->x >= 0 && pb->x < 0)
        return 1;

    if (pa->x < 0 && pb->x >= 0)
        return 0;

    if (pa->x == 0 && pb->x == 0) {
        if(pa->y >= 0 || pb->y >= 0)
            return a->y > b->y;
        else
            return b->y > a->y;
    }

    real cross2 = vector3_cross2(pa, pb);

    if (cross2 < 0)
        return 1;

    if (cross2 > 0)
        return 0;


    return vector3_squared_length(pa) > vector3_squared_length(pb);
}

/**
 * @brief vector3_projection Calcula a projeção de um vetor sobre outro
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O vetor projeção de a sobre b
 */
struct vector3* vector3_projection(struct vector3* a, struct vector3* b)
{
    struct vector3* p = vector3_new(b->x, b->y, b->z);

    real factor = vector3_dot(a, b) / vector3_length(b);
    p = vector3_scale(p, factor);

    return p;
}

/**
 * @brief vector3_angle Calcula o ângulo entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O ângulo entre a e b
 */
real vector3_angle(struct vector3* a, struct vector3* b)
{
    return acos(vector3_dot(a, b) / vector3_length(a) * vector3_length(b));
}

/**
 * @brief vector3_debug Debuga um vetor exibindo o valor de suas coordenadas
 * @param v O vetor a ser debugado
 * @param message Uma mensagem a ser exibida no topo do debug
 */
void vector3_debug(struct vector3* v, const char* message)
{
    printf("VECTOR3D DEBUG: %s:\n(%le, %le, %le)\n", message, v->x, v->y, v->z);
}

#endif // VECTOR3_H
