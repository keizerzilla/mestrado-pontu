/**
 * \file plane.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Estrutura de dados que manipula planos em 3 dimenções.
 */

#ifndef PLANE_H
#define PLANE_H

#include "vector3.h"

/**
 * \brief Estrutura que guarda plano de 3 dimenções na memória.
 */
struct plane {
	struct vector3* normal;
	struct vector3* point;
	real d;
};

/**
 * \brief Cria um novo plano
 * \param normal O vetor normal ao plano
 * \param point O ponto de referência contido no plano
 * \return Um plano alocado em memória
 */
struct plane* plane_new(struct vector3* normal, struct vector3* point)
{
	struct plane* plane = malloc(sizeof(struct plane));
	if (plane == NULL) {
		util_error("%s: erro alocando memoria plane", __FUNCTION__);
		return NULL;
	}
	
	plane->normal = vector3_from_vector(normal);
	plane->point = vector3_from_vector(point);
	plane->d = -1* ((normal->x * point->x) +
	                (normal->y * point->y) +
	                (normal->z * point->z));
	
	return plane;
}

/**
 * \brief Libera a memória alocada para um plano
 * \param plane O plano a ser liberado
 */
void plane_free(struct plane* plane)
{
	vector3_free(plane->normal);
	vector3_free(plane->point);
	free(plane);
	plane = NULL;
}

/**
 * \brief Calcula a distância absoluta entre um plano e um ponto
 * \param plane O plano
 * \param point O ponto
 * \return A distância entre plane e point
 */
real plane_distance2point(struct plane* plane, struct vector3* point)
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
uint plane_on_direction(struct plane* plane, struct vector3* point)
{
	struct vector3* proj = vector3_sub(point, plane->point);
	real d = vector3_dot(proj, plane->normal);
	vector3_free(proj);
	return (d >= 0.0f) ? 1 : 0;
}

/**
 * \brief Debuga um plano em um arquivo
 * \param plane O plano a ser debugado
 * \param out O arquivo aonde exibir a mensagem
 */
void plane_debug(struct plane* plane, FILE* out)
{
	if (plane == NULL) {
		fprintf(out, "!!! plano vazia !!!\n");
		return;
	}
	
	fprintf(out, "P(%.4f, %.4f, %.4f)|N(%.4f, %.4f, %.4f)\n", plane->point->x,
	                                                          plane->point->y,
	                                                          plane->point->z,
	                                                          plane->normal->x,
	                                                          plane->normal->y,
	                                                          plane->normal->z);
}

#endif // PLANE_H

