/**
 * \file plane3.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Estrutura de dados que manipula planos em 3 dimenções.
 */

#ifndef PLANE3_H
#define PLANE3_H

#include "vector3.h"

/**
 * \brief Estrutura que guarda plano de 3 dimenções na memória.
 */
struct plane3 {
	struct vector3* normal;
	struct vector3* point;
	real d;
};

/**
 * \brief Cria um novo plano
 * \param n O vetor normal ao plano
 * \param p O ponto de referência contido no plano
 * \return Um plano alocado em memória
 */
struct plane3* plane3_new(struct vector3* n, struct vector3* p)
{
	struct plane3* plane = malloc(sizeof(struct plane3));
	if (plane == NULL) {
		util_error("%s: erro alocando memoria plane3", __FUNCTION__);
		return NULL;
	}
	
	plane->normal = vector3_from_vector(n);
	plane->point = vector3_from_vector(p);
	plane->d = -1* ((n->x * p->x) + (n->y * p->y) + (n->z * p->z));
	
	return plane;
}

/**
 * \brief Cálcula distância absoluta entre um plano e um ponto
 * \param plane O plano
 * \param point O ponto
 * \return A distância entre plane e point
 */
real plane3_distance2point(struct plane3* plane, struct vector3* point)
{
	struct vector3* proj = vector3_sub(point, plane->point);
	real d = vector3_dot(proj, plane->normal) / vector3_length(plane->normal);
	vector3_free(proj);
	
	return fabs(d);
}

/**
 * \brief Indica se um ponto está ou não na região positiva de um plano
 * \param plane O plano alvo
 * \param point O ponto em questão
 * \return 1 se point estiver na região positiva da normal de plane, 0 c-c
 */
uint plane3_on_direction(struct plane3* plane, struct vector3* point)
{
	struct vector3* proj = vector3_sub(point, plane->point);
	real d = vector3_dot(proj, plane->normal);
	vector3_free(proj);
	
	return (d >= 0.0f) ? 1 : 0;
}

/**
 * \brief Libera a memória alocada para um plano
 * \param plane O plano a ser liberado
 */
void plane3_free(struct plane3* plane)
{
	vector3_free(plane->normal);
	vector3_free(plane->point);
	free(plane);
	plane = NULL;
}

#endif // PLANE3_H

