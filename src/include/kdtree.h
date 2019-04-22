/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Implementação da estrutura de particionamento espacial kdtree.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "cloud.h"

/**
 * \brief Estrutura que armazena uma kdtree
 */
struct kdtree {
	real median;
	int numpts;
	struct vector3** points;
	struct kdtree* left;
	struct kdtree* right;
};

/**
 * \brief Aloca memória para uma nova kdtree
 * \return Ponteiro para endereço com memória alocada para nós raíz
 */
struct kdtree* kdtree_new()
{
	struct kdtree* kdt = malloc(sizeof(struct kdtree));
	if (kdt == NULL)
		return NULL;
	
	kdt->median = 0.0f;
	kdt->numpts = 0;
	kdt->points = NULL;
	kdt->left = NULL;
	kdt->right = NULL;
	
	return kdt;
}

/**
 * \brief Libera memória alocada por uma kdtree
 * \param kdt A kdtree a ser desalocada
 */
void kdtree_free(struct kdtree* kdt)
{
	if (kdt == NULL)
		return;
	
	free(kdt->points);
	kdtree_free(kdt->left);
	kdtree_free(kdt->right);
	
	free(kdt);
}

/**
 * \brief Adiciona ponto à lista de pontos de um nó
 * \param kdt A kdtree alvo
 * \param p O novo ponto
 * \return NULL se um problema acontecer, caso-contrário endereço do nó alvo
 */
struct kdtree* kdtree_add_point(struct kdtree* kdt, struct vector3* p)
{
	uint new_size = (kdt->numpts + 1) * sizeof(struct vector3*);

    struct vector3** new_points = realloc(kdt->points, new_size);
    if (new_points == NULL)
        return NULL;
	
    kdt->points = new_points;
    kdt->numpts++;
    
    kdt->points[kdt->numpts - 1] = p;
    
    return kdt;
}

/**
 * \brief Particionamento recursivo dos pontos de uma nuvem
 * \param kdt A kdtree alvo
 * \param axis Eixo de particionamento
 * \param depth A profundidade do particionamento ("quantas vezes particionar")
 */
void kdtree_partitionate(struct kdtree* kdt, int axis, int depth)
{
	if (depth <= 0)
		return;
	
	int current_axis = axis % 3;
	real median = 0.0f;
	
	for (uint i = 0; i < kdt->numpts; i++)
		median += kdt->points[i]->coord[current_axis];
	
	median /= kdt->numpts;
	
	kdt->median = median;
	kdt->left = kdtree_new();
	kdt->right = kdtree_new();
	
	for (uint i = 0; i < kdt->numpts; i++) {
		if (kdt->points[i]->coord[axis] < median)
			kdtree_add_point(kdt->left, kdt->points[i]);
		else
			kdtree_add_point(kdt->right, kdt->points[i]);
	}
	
	kdtree_partitionate(kdt->left, axis + 1, depth - 1);
	kdtree_partitionate(kdt->right, axis + 1, depth - 1);
}

/**
 * \brief Inicia uma kdtree a partir de uma nuvem de pontos
 * \param kdt A kdtree alvo
 * \param cloud A nuvem base
 * \return NULL em caso de erro, caso-contrário o endereço de kdt
 */
struct kdtree* kdtree_init(struct kdtree* kdt, struct cloud* cloud)
{
	if (kdt == NULL)
		kdt = kdtree_new();
	
	int size = cloud_size(cloud);
	kdt->points = malloc(size * sizeof(struct vector3*));
	if (kdt->points == NULL)
		return NULL;
	
	for (int i = 0; i < size; i++)
		kdt->points[i] = &cloud->points[i];
	
	kdt->numpts = size;		
	
	return kdt;
}

/**
 * \brief Debuga uma kdtree (número de pontos nas folhas)
 * \param kdt A kdtree alvo
 */
void kdtree_debug(struct kdtree* kdt)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL) {
		printf("numpts = %d\n", kdt->numpts);
	}
	
	kdtree_debug(kdt->left);
	kdtree_debug(kdt->right);
}

/**
 * \brief Converte os pontos de um nó em uma nuvem de pontos
 * \param kdt A kdtree alvo
 * \return Uma nuvem de pontos contendo os pontos de kdt
 */
struct cloud* kdtree_tocloud(struct kdtree* kdt)
{
	struct cloud* cloud = cloud_empty();
		
	for (uint i = 0; i < kdt->numpts; i++)
		cloud_add_point_vector(cloud, kdt->points[i]);
	
	return cloud;
}

/**
 * \brief Salva os pontos das folhas em arquivos .pcd
 * \param kdt A kdtree alvo
 * \param path Caminho para pasta aonde as nuvens serão salvas
 */
void kdtree_tofile(struct kdtree* kdt, const char* path)
{
	if (kdt == NULL)
		return;
	
	if (kdt->left == NULL && kdt->right == NULL) {
		char buffer[80];
		sprintf(buffer, "%sfolha_%p.pcd", path, kdt);
		
		struct cloud* cloud = kdtree_tocloud(kdt);
		cloud_save_pcd(cloud, buffer);
		cloud_free(cloud);
		
		printf("%s ok!\n", buffer);
	}
	
	kdtree_tofile(kdt->left, path);
	kdtree_tofile(kdt->right, path);
}

#endif // KDTREE_H

