/**
 * \file vector3.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo uma implementação que manipula vetores em espaços de
 * três dimensões. Diversas funções matemáticas são fornecidas e forma
 * otimizadas ao máximo.
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/**
 * \brief Estrutura que armazena pontos no espaço tridimensional.
 */
struct vector3 {
    real x;
    real y;
    real z;
};

/**
 * \brief Cria um novo vetor
 * \param x A coordenada x
 * \param y A coordenada y
 * \param z A coordenada z
 * \return Um ponteiro para um novo vetor
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
 * \brief Cria um vetor vazio
 * \param x A coordenada x
 * \param y A coordenada y
 * \param z A coordenada z
 * \return Um ponteiro para um novo vetor
 */
struct vector3* vector3_zero()
{
    return vector3_new(0.0f, 0.0f, 0.0f);
}

/**
 * \brief Faz uma cópia de um vetor
 * \param dst O vetor que guardará a cópia
 * \param src O vetor a ser copiado
 */
void vector3_copy(struct vector3* dst, struct vector3* src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

/**
 * \brief Libera espaço de memória alocado para um vetor
 * \param v O vetor a ser liberado
 */
void vector3_free(struct vector3* v)
{
    free(v);
    v = NULL;
}

/**
 * \brief Soma dois vetores gerando um novo sem mudar os operandos
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O vetor resultante da soma de a com b
 */
struct vector3* vector3_add(struct vector3* a, struct vector3* b)
{
    return vector3_new(a->x + b->x, a->y + b->y, a->z + b->z);
}

/**
 * @brief Soma dois vetores modificando um deles permanentemente
 * @param src O vetor a ser modificado (somado)
 * @param inc O vetor que servirá de incremento
 */
void vector3_increase(struct vector3* src, struct vector3* inc)
{
    src->x += inc->x;
    src->y += inc->y;
    src->z += inc->z;
}

/**
 * \brief Subtrái dois vetores gerando um novo sem modificar os operandos
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O vetor resultante da subtração de a com b
 */
struct vector3* vector3_sub(struct vector3* a, struct vector3* b)
{
    return vector3_new(a->x - b->x, a->y - b->y, a->z - b->z);
}

/**
 * @brief vector3_decrease
 * @param src O vetor a ser modificado (diminuído)
 * @param dec O vetor que servirá de decremento
 */
void vector3_decrease(struct vector3* src, struct vector3* dec)
{
    src->x -= dec->x;
    src->y -= dec->y;
    src->z -= dec->z;
}

/**
 * \brief Seta os valores de um vetor
 * \param v O vetor alvo
 * \param x O valor da coordenada x
 * \param y O valor da coordenada y
 * \param z O valor da coordenada z
 */
void vector3_set(struct vector3* v, real x, real y, real z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

/**
 * \brief Inverte um vetor
 * \param v O vetor a ser invertido
 */
void vector3_invert(struct vector3* v)
{
	v->x = -1 * v->x;
	v->y = -1 * v->y;
	v->z = -1 * v->z;
}

/**
 * \brief Calcula a distância euclidiana entre dois vetores
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return A distância euclidiana entra a e b
 */
real vector3_distance(struct vector3* a, struct vector3* b)
{
    return calc_length3(a->x - b->x, a->y - b->y, a->z - a->z);
}

/**
 * \brief Calcula a distância Manhattan entre dois vetores
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return A distância Manhattan entra a e b
 */
real vector3_manhattan(struct vector3* a, struct vector3* b)
{
	return abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z);
}

/**
 * \brief Calcula o tamanho quadrático de um vetor
 * \param v O vetor alvo
 * \return O tamanho quadrático de v
 */
real vector3_squared_length(struct vector3* v)
{
    return calc_squared_length3(v->x, v->y, v->z);
}

/**
 * \brief Calcula o tamanho de um vetor
 * \param v O vetor alvo
 * \return O tamanho de v
 */
real vector3_length(struct vector3* v)
{
    return calc_length3(v->x, v->y, v->z);
}

/**
 * \brief Calcula o tamanho quadrático (2D) de um vetor
 * \param v O vetor alvo
 * \return O tamanho quadrático de v
 */
real vector3_squared_length2(struct vector3* v)
{
    return calc_squared_length2(v->x, v->y);
}

/**
 * \brief Calcula o tamanho (2D) de um vetor
 * \param v O vetor alvo
 * \return O tamanho de v
 */
real vector3_length2(struct vector3* v)
{
    return calc_length2(v->x, v->y);
}

/**
 * \brief Muda tamanho de um vetor sem alterar a sua direção
 * \param v O vetor a ser modificado
 * \param l O novo tamanho do vetor
 */
void vector3_setlength(struct vector3* v, real l)
{
	real mag = calc_length3(v->x, v->y, v->z);
	
	if (mag > 0.0f) {
		mag = l / mag;

		v->x *= mag;
		v->y *= mag;
		v->z *= mag;
	}
}

/**
 * \brief Retorna o vetor unitário de um vetor
 * \param v O vetor alvo
 * \return Um novo vetor de tamanho 1 e direção igual a v
 */
struct vector3* vector3_unit(struct vector3* v)
{
    struct vector3* unit = vector3_zero();
	real l = vector3_length(v);
	
	unit->x = v->x / l;
	unit->y = v->y / l;
	unit->z = v->z / l;
	
	return unit;
}

/**
 * \brief Normaliza um vetor
 * \param v O vetor alvo
 */
void vector3_normalize(struct vector3* v)
{
	real l = vector3_length(v);
	
	v->x = v->x / l;
	v->y = v->y / l;
	v->z = v->z / l;
}

/**
 * \brief Escala um vetor através de um fator numérico
 * \param v O vetor alvo
 * \param factor O fator de escalamento
 */
void vector3_scale(struct vector3* v, real f)
{
    v->x *= f;
    v->y *= f;
    v->z *= f;
}

/**
 * @brief Rotaciona vetor em torno do eixo x
 * @param v O vetor alvo
 * @param d O ângulo de rotação em graus
 */
void vector3_rotate_x(struct vector3* v, real d)
{
    real rad = d * CALC_DEG2RAD;
    real rsin = sin(rad);
    real rcos = cos(rad);

    v->y = (v->y * rcos) - (v->z * rsin);
    v->z = (v->y * rsin) + (v->z * rcos);
}

/**
 * @brief Rotaciona vetor em torno do eixo y
 * @param v O vetor alvo
 * @param d O ângulo de rotação em graus
 */
void vector3_rotate_y(struct vector3* v, real d)
{
    real rad = d * CALC_DEG2RAD;
    real rsin = sin(rad);
    real rcos = cos(rad);

    v->x = (v->x * rcos) + (v->z * rsin);
    v->z = (-1 * (v->x * rsin)) + (v->z * rcos);
}

/**
 * @brief Rotaciona vetor em torno do eixo z
 * @param v O vetor alvo
 * @param d O ângulo de rotação em graus
 */
void vector3_rotate_z(struct vector3* v, real d)
{
	real rad = d * CALC_DEG2RAD;
    real rsin = sin(rad);
    real rcos = cos(rad);

    v->x = (v->x * rcos) - (v->y * rsin);
    v->y = (v->x * rsin) + (v->y * rcos);
}

/**
 * \brief Calcula o produto escalar entre dois vetores
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O produto escalar entre a e b
 */
real vector3_dot(struct vector3* a, struct vector3* b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

/**
 * \brief Calcula o produto vetorial entre dois vetores
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O produto vetorial entre a e b
 */
struct vector3* vector3_cross(struct vector3* a, struct vector3* b)
{
    return vector3_new((a->y * b->z) - (a->z * b->y),
                       (a->z * b->x) - (a->x * b->z),
                       (a->x * b->y) - (a->y * b->x));
}

/**
 * \brief Calcula o produto vetorial 2D entre dois vetores
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O produto vetorial 2D entre a e b
 */
real vector3_cross2(struct vector3* a, struct vector3* b)
{
    return (a->x * b->y) - (b->x * a->y);
}

/**
 * \brief Zera o eixo z do vetor
 * \param v O vetor alvo
 */
void vector3_push2(struct vector3* v) {
    v->z = 0.0f;
}

/**
 * @brief Calcula a distância de Minkowski entre dois vetores
 * @param v1 O primeiro vetor
 * @param v2 O segundo vetor
 * @param m A ordem da distância
 * @return A distância de ordem 'm' entre v1 e v2
 */
real vector3_minkowski(struct vector3* v1, struct vector3* v2, real m)
{
    return pow(pow(v1->x - v2->x, m) + pow(v1->y - v2->y, m), 1.0f/m);
}

/**
 * \brief Efetua interpolação linear em um vetor
 * \param s O vetor a ser interpolado
 * \param t O vetor alvo/objetivo
 * \param v A velocidade da interpolação
 */
void vector3_lerp(struct vector3* s, struct vector3* t, real v)
{
    s->x = s->x + ((t->x - s->x) * v);
    s->y = s->y + ((t->y - s->y) * v);
    s->z = s->z + ((t->z - s->z) * v);
}

/**
 * \brief Calcula o volume de uma caixa com dimensões x, y, z
 * \param v O vetor que guarda as dimensões da caixa
 * \return O volume
 */
real vector3_volume(struct vector3* v)
{
	return v->x * v->y * v->z;
}

/**
 * \brief Calcula a projeção de um vetor sobre outro
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O vetor projeção de a sobre b
 */
struct vector3* vector3_projection(struct vector3* a, struct vector3* b)
{
    struct vector3* p = vector3_new(b->x, b->y, b->z);

    real factor = vector3_dot(a, b) / vector3_length(b);
    vector3_scale(p, factor);

    return p;
}

/**
 * \brief Retorna a reflexão de um vetor em um plano
 * \param v O vetor sendo refletido
 * \param n O vetor normal ao plano
 * \return O vetor reflexão de v em n
 */
struct vector3* vector3_reflection(struct vector3* v, struct vector3* n)
{
    struct vector3* r = vector3_zero();
    vector3_copy(n, r);

    vector3_scale(r, 2 * vector3_dot(v, n));
	r = vector3_sub(v, r);
	
	return r;
}

/**
 * \brief Calcula o ângulo entre dois vetores
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \return O ângulo entre a e b
 */
real vector3_angle(struct vector3* a, struct vector3* b)
{
    return acos(vector3_dot(a, b) / vector3_length(a) * vector3_length(b));
}

/**
 * \brief Acha a normal à superfície de um triângulo
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \param c O terceiro vetor
 * \return O vetor normal à superfície definida
 */
struct vector3* vector3_normal(struct vector3* a,
                               struct vector3* b,
                               struct vector3* c)
{
    struct vector3* v1 = vector3_sub(b, a);
    struct vector3* v2 = vector3_sub(c, a);
    struct vector3* ret = vector3_cross(v1, v2);

    vector3_free(v1);
    vector3_free(v2);

    return ret;
}


/**
 * \brief Acha a área da superfície de um triângulo
 * \param a O primeiro vetor
 * \param b O segundo vetor
 * \param c O terceiro vetor
 * \return O área da superfície definida
 */
real vector3_area(struct vector3* a,
                  struct vector3* b,
                  struct vector3* c)
{
    struct vector3* n = vector3_normal(a, b, c);
    real ret = 0.5f * vector3_length(n);
    vector3_free(n);

    return ret;
}

/**
 * \brief Debuga um vetor exibindo suas coordenadas
 * \param output O arquivo aonde exibir a mensagem
 * \param v O vetor a ser debugado
 */
void vector3_debug(FILE* output, struct vector3* v)
{
    fprintf(output, "%le %le %le\n", v->x, v->y, v->z);
}

#endif // VECTOR3_H
