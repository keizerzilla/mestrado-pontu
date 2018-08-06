/*
 * @file vector3.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo uma implementação que manipula vetores em espaços de
 * três dimensões. Diversas funções matemáticas são fornecidas e forma
 * otimizadas ao máximo.
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/*
 * @brief A estrutura vector3.
 *
 * Contem três campos auto-explicativos: as coordenadas x, y e z.
 */
struct vector3 {
    real x;
    real y;
    real z;
};

/*
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

/*
 * @brief vector3_zero Cria um vetor vazio
 * @param x A coordenada x
 * @param y A coordenada y
 * @param z A coordenada z
 * @return Um ponteiro para um novo vetor
 */
struct vector3* vector3_zero()
{
    return vector3_new(0.0f, 0.0f, 0.0f);
}

/*
 * @brief vector3_copy Faz uma cópia de um vetor
 * @param v O vetor a ser copiado
 * @return Um vetor com as mesmas coordenadas de v
 */
struct vector3* vector3_copy(struct vector3* v)
{
	return vector3_new(v->x, v->y, v->z);
}

/*
 * @brief vector3_free Libera espaço de memória alocado para um vetor
 * @param v O vetor a ser liberado
 */
void vector3_free(struct vector3* v)
{
    free(v);
    v = NULL;
}

/*
 * @brief vector3_add Soma dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O vetor resultante da soma de a com b
 */
struct vector3* vector3_add(struct vector3* a, struct vector3* b)
{
    return vector3_new(a->x + b->x, a->y + b->y, a->z + b->z);
}

/*
 * @brief vector3_sub Subtrái dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O vetor resultante da subtração de a com b
 */
struct vector3* vector3_sub(struct vector3* a, struct vector3* b)
{
    return vector3_new(a->x - b->x, a->y - b->y, a->z - b->z);
}

/*
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

/*
 * @brief vector3_invert Inverte um vetor
 * @param v O vetor a ser invertido
 */
void vector3_invert(struct vector3* v)
{
	v->x = -1 * v->x;
	v->y = -1 * v->y;
	v->z = -1 * v->z;
}

/*
 * @brief vector3_distance Calcula a distância euclidiana entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return A distância euclidiana entra a e b
 */
real vector3_distance(struct vector3* a, struct vector3* b)
{
    return calc_length3(a->x - b->x, a->y - b->y, a->z - a->z);
}

/*
 * @brief vector3_manhattan Calcula a distância Manhattan entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return A distância Manhattan entra a e b
 */
real vector3_manhattan(struct vector3* a, struct vector3* b)
{
	return abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z);
}

/*
 * @brief vector3_squared_length Calcula o tamanho quadrático de um vetor
 * @param v O vetor alvo
 * @return O tamanho quadrático de v
 */
real vector3_squared_length(struct vector3* v)
{
    return calc_squared_length3(v->x, v->y, v->z);
}

/*
 * @brief vector3_length Calcula o tamanho de um vetor
 * @param v O vetor alvo
 * @return O tamanho de v
 */
real vector3_length(struct vector3* v)
{
    return calc_length3(v->x, v->y, v->z);
}

/*
 * @brief vector3_squared_length Calcula o tamanho quadrático (2D) de um vetor
 * @param v O vetor alvo
 * @return O tamanho quadrático de v
 */
real vector3_squared_length2(struct vector3* v)
{
    return calc_squared_length2(v->x, v->y);
}

/*
 * @brief vector3_length Calcula o tamanho (2D) de um vetor
 * @param v O vetor alvo
 * @return O tamanho de v
 */
real vector3_length2(struct vector3* v)
{
    return calc_length2(v->x, v->y);
}

/*
 * @brief vector3_setlength Muda tamanho de um vetor sem alterar a sua direção
 * @param v O vetor a ser modificado
 * @param l O novo tamanho do vetor
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

/*
 * @brief vector3_unit Retorna o vetor unitário de um vetor
 * @param v O vetor alvo
 * @return Um novo vetor de tamanho 1 e direção igual a v
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

/*
 * @brief vector3_normalize Normaliza um vetor
 * @param v O vetor alvo
 */
void vector3_normalize(struct vector3* v)
{
	real l = vector3_length(v);
	
	v->x = v->x / l;
	v->y = v->y / l;
	v->z = v->z / l;
}

/*
 * @brief vector3_scale Escala um vetor através de um fator numérico
 * @param v O vetor alvo
 * @param factor O fator de escalamento
 */
void vector3_scale(struct vector3* v, real f)
{
    v->x *= f;
    v->y *= f;
    v->z *= f;
}

/*
 * @brief vector3_dot Calcula o produto escalar entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O produto escalar entre a e b
 */
real vector3_dot(struct vector3* a, struct vector3* b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

/*
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

/*
 * @brief vector3_cross2 Calcula o produto vetorial 2D entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O produto vetorial 2D entre a e b
 */
real vector3_cross2(struct vector3* a, struct vector3* b)
{
    return (a->x * b->y) - (b->x * a->y);
}

/*
 * @brief vector3_push2D Zera o eixo z do vetor
 * @param v O vetor alvo
 */
void vector3_push2(struct vector3* v) {
    v->z = 0.0f;
}

/*
 * @brief vector3_lerp Efetua interpolação linear em um vetor
 * @param s O vetor a ser interpolado
 * @param t O vetor alvo/objetivo
 * @param v A velocidade da interpolação
 */
void vector3_lerp(struct vector3* s, struct vector3* t, real v)
{
    s->x = s->x + ((t->x - s->x) * v);
    s->y = s->y + ((t->y - s->y) * v);
    s->z = s->z + ((t->z - s->z) * v);
}

/*
 * @brief vector3_volume Calcula o volume de uma caixa com dimensões x, y, z
 * @param v O vetor que guarda as dimensões da caixa
 * @return O volume
 */
real vector3_volume(struct vector3* v)
{
	return v->x * v->y * v->z;
}

/*
 * @brief vector3_projection Calcula a projeção de um vetor sobre outro
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O vetor projeção de a sobre b
 */
struct vector3* vector3_projection(struct vector3* a, struct vector3* b)
{
    struct vector3* p = vector3_new(b->x, b->y, b->z);

    real factor = vector3_dot(a, b) / vector3_length(b);
    vector3_scale(p, factor);

    return p;
}

/*
 * @brief vector3_reflection Retorna a reflexão de um vetor em um plano
 * @param v O vetor sendo refletido
 * @param n O vetor normal ao plano
 * @return O vetor reflexão de v em n
 */
struct vector3* vector3_reflection(struct vector3* v, struct vector3* n)
{
	struct vector3* r = vector3_copy(n);
	
    vector3_scale(r, 2 * vector3_dot(v, n));
	r = vector3_sub(v, r);
	
	return r;
}

/*
 * @brief vector3_angle Calcula o ângulo entre dois vetores
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @return O ângulo entre a e b
 */
real vector3_angle(struct vector3* a, struct vector3* b)
{
    return acos(vector3_dot(a, b) / vector3_length(a) * vector3_length(b));
}

/*
 * @brief vector3_normal Acha a normal à superfície de um triângulo
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @param c O terceiro vetor
 * @return O vetor normal à superfície definida
 */
struct vector3* vector3_normal(struct vector3* a,
                               struct vector3* b,
                               struct vector3* c)
{
	return vector3_cross(vector3_sub(b, a), vector3_sub(c, a));
}

/*
 * @brief vector3_area Acha a área da superfície de um triângulo
 * @param a O primeiro vetor
 * @param b O segundo vetor
 * @param c O terceiro vetor
 * @return O área da superfície definida
 */
real vector3_area(struct vector3* a,
                  struct vector3* b,
                  struct vector3* c)
{
	return 0.5f * vector3_length(vector3_normal(a, b, c));
}

/*
 * @brief vector3_debug Debuga um vetor exibindo suas coordenadas e uma mensagem
 * @param v O vetor a ser debugado
 * @param msg Uma mensagem a ser exibida no topo do debug
 * @param output O arquivo aonde exibir a mensagem
 */
void vector3_debug(struct vector3* v, const char* msg, FILE* output)
{
    fprintf(output, "vector3 (%le, %le, %le): %s\n", v->x, v->y, v->z, msg);
}

#endif // VECTOR3_H

