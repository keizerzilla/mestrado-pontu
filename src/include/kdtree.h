/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Estrutura de dados kdtree.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "plane3.h"
#include "cloud.h"

/**
 * \brief Estrutura que guarda uma kdtree
 */
struct kdtree {
	struct plane3* plane;
	struct kdtree* front;
	struct kdtree* back;
};

/**
 * \brief Cria uma kdtree vazia
 * \param normal O vetor normal da partição
 * \param point O ponto de referência da partição
 * \return Ponteiro para o nó raiz
 */
struct kdtree* kdtree_new(struct vector3* normal, struct vector3* point)
{
	struct kdtree* kdt = malloc(sizeof(struct kdtree));
	if (kdt == NULL) {
		util_error("%s: erro alocando memoria kdtree", __FUNCTION__);
		return NULL;
	}
	
	kdt->plane = plane3_new(normal, point);
	kdt->front = NULL;
	kdt->back = NULL;
	
	return kdt;
}

/**
 * \brief Checa se uma kdtree está vazia
 * \param kdt A kdtree a ser testada
 * \return 1 se estiver vazia, 0 caso contrário
 */
int kdtree_empty(struct kdtree* kdt)
{
	return (kdt == NULL) ? 1 : 0;
}

/**
 * \brief Particiona uma nuvem usando kdtree
 * \param cloud A nuvem a ser particionada
 * \param depth A profundiade do particionamento
 * \return Ponteiro para a a kdtree fruto do particionamento
 */
struct kdtree* kdtree_cloud_partition(struct cloud* cloud, uint depth)
{
	if (depth == 0)
		return NULL;
	
	struct vector3* dir = vector3_new((depth%3)==0, (depth%3)==1, (depth%3)==2);
	struct vector3* center = cloud_get_center(cloud);
	
	struct kdtree* kdt = kdtree_new(dir, center);
	if (kdt == NULL)
		return NULL;
	
	struct cloud* front_cloud = cloud_cut_plane(cloud, kdt->plane);
	kdt->front = kdtree_cloud_partition(front_cloud, depth - 1);
	cloud_free(front_cloud);
	
	vector3_invert(kdt->plane->normal);
	struct cloud* back_cloud = cloud_cut_plane(cloud, kdt->plane);
	kdt->back = kdtree_cloud_partition(back_cloud, depth - 1);
	cloud_free(back_cloud);
	
	vector3_free(dir);
	
	return kdt;
}

/**
 * \brief Armazena os pontos formadores da kdtree numa nuvem de pontos
 * \param kdt A kdtree fonte dos pontos
 * \param cloud A nuvem que armazenará os pontos
 */
void kdtree_kdtree2cloud(struct kdtree* kdt, struct cloud* cloud)
{
	if (!kdtree_empty(kdt)) {
		cloud_add_point_cpy(cloud, kdt->plane->point);
		kdtree_kdtree2cloud(kdt->front, cloud);
		kdtree_kdtree2cloud(kdt->back, cloud);
	}
}

/**
 * \brief Libera memória alocada para uma kdtree
 * \param kdt A kdtree a ser liberada
 */
void kdtree_free(struct kdtree* kdt)
{
	if (!kdtree_empty(kdt)) {
		kdtree_free(kdt->front);
		kdtree_free(kdt->back);
		plane3_free(kdt->plane);
		free(kdt);
	}
}

/**
 * \brief Debuga uma kdtree
 * \param kdt A kdtree a ser debugada
 */
void kdtree_debug(struct kdtree* kdt)
{
	if (!kdtree_empty(kdt)) {
		vector3_debug(kdt->plane->point, "kdtree debug", stdout);
		kdtree_debug(kdt->front);
		kdtree_debug(kdt->back);
	}
	
}

#endif // KDTREE_H

