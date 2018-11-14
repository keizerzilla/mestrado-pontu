/**
 * \file bsptree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Estrutura de dados bsptree.
 */

#ifndef BSPTREE_H
#define BSPTREE_H

#include "plane.h"
#include "cloud.h"

/**
 * \brief Estrutura que guarda uma bsptree
 */
struct bsptree {
	struct plane* plane;
	struct bsptree* front;
	struct bsptree* back;
};

/**
 * \brief Cria uma bsptree vazia
 * \param normal O vetor normal da partição
 * \param point O ponto de referência da partição
 * \return Ponteiro para o nó raiz
 */
struct bsptree* bsptree_new(struct vector3* normal, struct vector3* point)
{
	struct bsptree* bsp = malloc(sizeof(struct bsptree));
	if (bsp == NULL) {
		util_error("%s: erro alocando memoria bsptree", __FUNCTION__);
		return NULL;
	}
	
	bsp->plane = plane_new(normal, point);
	bsp->front = NULL;
	bsp->back = NULL;
	
	return bsp;
}

/**
 * \brief Checa se uma bsptree está vazia
 * \param bsp A bsptree a ser testada
 * \return 1 se estiver vazia, 0 caso contrário
 */
int bsptree_empty(struct bsptree* bsp)
{
	return (bsp == NULL) ? 1 : 0;
}

/**
 * \brief Particiona uma nuvem usando bsptree (ARRUMAR ORDEM DA RECURSÃO)
 * \param cloud A nuvem a ser particionada
 * \param depth A profundiade do particionamento
 * \return Ponteiro para a a bsptree fruto do particionamento
 */
struct bsptree* bsptree_cloud_partition(struct cloud* cloud, uint depth)
{
	if (depth == 0)
		return NULL;
	
	struct vector3* dir = vector3_new((depth%3)==0, (depth%3)==1, (depth%3)==2);
	struct vector3* center = cloud_get_center(cloud);
	
	struct bsptree* bsp = bsptree_new(dir, center);
	if (bsp == NULL)
		return NULL;
	
	struct cloud* front_cloud = cloud_cut_plane(cloud, bsp->plane);
	bsp->front = bsptree_cloud_partition(front_cloud, depth - 1);
	cloud_free(front_cloud);
	
	vector3_invert(bsp->plane->normal);
	struct cloud* back_cloud = cloud_cut_plane(cloud, bsp->plane);
	bsp->back = bsptree_cloud_partition(back_cloud, depth - 1);
	cloud_free(back_cloud);
	
	vector3_free(dir);
	
	return bsp;
}

/**
 * \brief Armazena os pontos formadores da bsptree numa nuvem de pontos
 * \param bsp A bsptree fonte dos pontos
 * \param cloud A nuvem que armazenará os pontos
 */
void bsptree_bsptree2cloud(struct bsptree* bsp, struct cloud* cloud)
{
	if (!bsptree_empty(bsp)) {
		cloud_add_point(cloud, bsp->plane->point);
		bsptree_bsptree2cloud(bsp->front, cloud);
		bsptree_bsptree2cloud(bsp->back, cloud);
	}
}

/**
 * \brief Libera memória alocada para uma bsptree
 * \param bsp A bsptree a ser liberada
 */
void bsptree_free(struct bsptree* bsp)
{
	if (!bsptree_empty(bsp)) {
		bsptree_free(bsp->front);
		bsptree_free(bsp->back);
		plane_free(bsp->plane);
		free(bsp);
		bsp = NULL;
	}
}

/**
 * \brief Debuga uma bsptree
 * \param bsp A bsptree a ser debugada
 */
void bsptree_debug(struct bsptree* bsp)
{
	if (!bsptree_empty(bsp)) {
		vector3_debug(bsp->plane->point, stdout);
		bsptree_debug(bsp->front);
		bsptree_debug(bsp->back);
	}
}

#endif // BSPTREE_H

