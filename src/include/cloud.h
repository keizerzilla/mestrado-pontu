/**
 * \file cloud.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Implementação para manipulação de nuvens de pontos. Uma estrutura de
 * dados e várias funções de manipulação foram fornecidas.
 */

#ifndef CLOUD_H
#define CLOUD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector3.h"
#include "plane.h"
#include "util.h"
#include "hashtable.h"

/**
 * \brief Estrutura que guarda uma nuvem de pontos em memória
 */
struct cloud {
    struct vector3* points;
    struct vector3* centroid;
    uint* indexes;
    uint num_pts;
};

/**
 * \brief Inicializa uma estrutura do tipo cloud
 * \param cloud A estrutura que salvará a nuvem
 * \param num_of_points Quantos pontos possui a nuvem
 * \return O ponteiro para uma nuvem alocada ou NULL se houver algo erro
 */
struct cloud* cloud_new(uint num_pts)
{
    struct cloud* cloud = malloc(sizeof(struct cloud));
    if (cloud == NULL) {
        util_error("%s: erro alocando memoria cloud", __FUNCTION__);
        return NULL;
    }

    cloud->points = malloc(num_pts * sizeof(struct vector3));
    if (cloud->points == NULL) {
        util_error("%s: erro alocando memoria points", __FUNCTION__);
        return NULL;
    }
	
	cloud->indexes = malloc(num_pts * sizeof(uint));
	if (cloud->indexes == NULL) {
		util_error("%s: erro alocando memoria indexes", __FUNCTION__);
        return NULL;
	}
	
    memset(cloud->points, 0, num_pts * sizeof(struct vector3));
	memset(cloud->indexes, 0, num_pts * sizeof(uint));
	
    cloud->centroid = NULL;
    cloud->num_pts = num_pts;

    return cloud;
}

/**
 * \brief Cria nuvem vazia
 * \return Uma nuvem com 0 pontos
 */
struct cloud* cloud_empty()
{
	return cloud_new(0);
}

/**
 * \brief Libera a memória alocada para uma nuvem
 * \param cloud A nuvem a ser liberada
 */
void cloud_free(struct cloud* cloud)
{
	if (cloud == NULL)
		return;
	
	free(cloud->points);

    if (cloud->centroid != NULL) {
        vector3_free(cloud->centroid);
        cloud->centroid = NULL;
    }
	
	free(cloud->indexes);
	
    free(cloud);
    cloud = NULL;
}

/**
 * \brief Seta os valores de um dos pontos da nuvem
 * \param cloud A nuvem alvo
 * \param index O índice do ponto a ser setado
 * \param x O valor da coordenada x
 * \param y O valor da coordenada y
 * \param z O valor da coordenada z
 * \return O endereço para o ponto setado
 */
static struct vector3* cloud_set_point(struct cloud* cloud,
                                       uint index,
                                       real x,
                                       real y,
                                       real z)
{
    if (index >= cloud->num_pts) {
        util_error("%s: indice fora de intervalo", __FUNCTION__);
        return NULL;
    }

    vector3_set(&cloud->points[index], x, y, z);
    cloud->indexes[index] = index;

    return &cloud->points[index];
}

/**
 * \brief Seta os valores de um dos pontos da nuvem
 * \param cloud A nuvem alvo
 * \param index O índice do ponto a ser setado
 * \param point O valor das coordenadas a serem setadas
 * \return O endereço para o ponto setado
 */
struct vector3* cloud_set_vector(struct cloud* cloud,
                                    uint index,
                                    struct vector3* point)
{
    return cloud_set_point(cloud, index, point->x, point->y, point->z);
}

/**
 * \brief Adiciona um novo ponto a uma nuvem
 * \param cloud A nuvem alvo
 * \param x A coordenada x do ponto
 * \param y A coordenada y do ponto
 * \param z A coordenada z do ponto
 * \return O endereço do novo ponto alocado ou NULL se não houver memória
 */
struct vector3* cloud_new_point(struct cloud* cloud,
                                real x,
                                real y,
                                real z)
{
    uint new_size = (cloud->num_pts + 1) * sizeof(struct vector3);

    struct vector3* new_points = realloc(cloud->points, new_size);
    if (new_points == NULL) {
        util_error("%s: erro adicionando novo ponto [points]", __FUNCTION__);
        return NULL;
    }
	
	uint* new_indexes = realloc(cloud->indexes, new_size);
	if (new_indexes == NULL) {
        util_error("%s: erro adicionando novo ponto [indexes]", __FUNCTION__);
        return NULL;
    }
    
    cloud->points = new_points;
    cloud->indexes = new_indexes;
    cloud->num_pts++;

    vector3_set(&cloud->points[cloud->num_pts - 1], x, y, z);
    cloud->indexes[cloud->num_pts - 1] = cloud->num_pts - 1;

    return &cloud->points[cloud->num_pts - 1];
}

/**
 * \brief Adiciona um novo ponto a uma nuvem
 * \param cloud A nuvem alvo
 * \param point O ponto a ser adicionado
 * \return O endereço do novo ponto alocado ou NULL se não houver memória
 */
struct vector3* cloud_add_point(struct cloud* cloud, struct vector3* point)
{
    return cloud_new_point(cloud, point->x, point->y, point->z);
}

/**
 * \brief Conta quantos pontos constituem uma nuvem
 * \param cloud A nuvem alvo
 * \return O número de pontos de cloud
 */
uint cloud_size(struct cloud* cloud)
{
    return cloud->num_pts;
}

/**
 * \brief Cria cópia de uma nuvem
 * \param cloud A nuvem a ser copiada
 * \return Uma cópia de cloud
 */
struct cloud* cloud_copy(struct cloud* cloud)
{
	struct cloud* cpy = cloud_new(cloud_size(cloud));
	
	for (uint index = 0; index < cloud->num_pts; index++) {
        cloud_set_point(cpy,
                        index,
                        cloud->points[index].x,
                        cloud->points[index].y,
                        cloud->points[index].z);
    }
	
	return cpy;
}

/**
 * \brief Carrega uma nuvem a partir de um arquivo XYZ
 * \param filename O arquivo onde a nuvem está guardada
 * \return Um estrutura cloud carregada em memória ou NULL caso ocorra erro
 */
struct cloud* cloud_load_xyz(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return NULL;
    }

    uint num_pts = 0;
    while (!feof(file) && (fscanf(file, "%*s %*s %*s\n") != EOF))
        num_pts++;
	
    rewind(file);

    struct cloud* cloud = cloud_new(num_pts);
    real x = 0;
    real y = 0;
    real z = 0;
    uint index = 0;
    while (!feof(file) && (fscanf(file, "%le %le %le\n", &x, &y, &z) != EOF)) {
        cloud_set_point(cloud, index, x, y, z);
        index++;
    }

    fclose(file);

    return cloud;
}

/**
 * \brief Carrega uma nuvem a partir de um arquivo CSV
 * \param filename O arquivo onde a nuvem está guardada
 * \return Um estrutura cloud carregada em memória ou NULL caso ocorra erro
 */
struct cloud* cloud_load_csv(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return NULL;
    }

    uint num_pts = 0;
    while (!feof(file) && (fscanf(file, "%*s,%*s,%*s\n") != EOF))
        num_pts++;
	
    rewind(file);

    struct cloud* cloud = cloud_new(num_pts);
    real x = 0;
    real y = 0;
    real z = 0;
    uint index = 0;
    while (!feof(file) && (fscanf(file, "%le,%le,%le\n", &x, &y, &z) != EOF)) {
        cloud_set_point(cloud, index, x, y, z);
        index++;
    }

    fclose(file);

    return cloud;
}

/**
 * \brief Carrega uma nuvem em arquivo PLY (x y z sempre primeiras properties!)
 * \param filename A nuvem a ser carregada
 * \return A nuvem carregada em memória ou NULL caso algum erro tenha ocorrido
 */
struct cloud* cloud_load_ply(const char* filename)
{
	uint num_pts = 0;
	char buffer[80];
	
	FILE* file = fopen(filename, "r");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return NULL;
    }
	
	if (fgets(buffer, 80, file)) {
		if (strcmp(buffer, "ply\n")) {
			util_error("%s: arquivo ply invalido [%s]", __FUNCTION__, filename);
			fclose(file);
			return NULL;
		}
	} else {
		util_error("%s: erro na leitura do arquivo %s", __FUNCTION__, filename);
		fclose(file);
		return NULL;
	}
	
	if (fgets(buffer, 80, file)) {
		if (strcmp(buffer, "format ascii 1.0\n")) {
			util_error("%s: formato ply invalido [%s]", __FUNCTION__, filename);
			fclose(file);
			return NULL;
		}
	} else {
		util_error("%s: erro na leitura do arquivo %s", __FUNCTION__, filename);
		fclose(file);
		return NULL;
	}
	
	while (fgets(buffer, 80, file)) {
		if (!strcmp(buffer, "end_header\n"))
			break;
		
		if (!strncmp(buffer, "element vertex", 14))
			sscanf(buffer, "element vertex %d\n", &num_pts);
	}
	
	struct cloud* cloud = cloud_new(num_pts);
	if (cloud == NULL) {
		util_error("%s: memoria insuficiente para cloud", __FUNCTION__);
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	for (uint i = 0; i < num_pts; i++) {
		if (fgets(buffer, 80, file)) {
			sscanf(buffer, "%le %le %le %*s\n", &x, &y, &z);
			cloud_set_point(cloud, index, x, y, z);
			index++;
		} else {
			util_error("%s: erro no parse [%s]", __FUNCTION__, filename);
			break;
		}
	}
	
	fclose(file);
	
	return cloud;
}

/**
 * \brief Carrega uma nuvem em arquivo PCD (soment DATA ascii!!!)
 * \param filename A nuvem a ser carregada
 * \return A nuvem carregada em memória ou NULL caso algum erro tenha ocorrido
 */
struct cloud* cloud_load_pcd(const char* filename)
{
	uint num_pts = 0;
	char buffer[80];
	
	FILE* file = fopen(filename, "r");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return NULL;
    }
	
	while (fgets(buffer, 80, file)) {
		if (!strcmp(buffer, "DATA ascii\n"))
			break;
		
		if (!strncmp(buffer, "POINTS", 6))
			sscanf(buffer, "POINTS %d\n", &num_pts);
	}
	
	struct cloud* cloud = cloud_new(num_pts);
	if (cloud == NULL) {
		util_error("%s: memoria insuficiente para cloud", __FUNCTION__);
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	for (uint i = 0; i < num_pts; i++) {
		if (fgets(buffer, 80, file)) {
			sscanf(buffer, "%le %le %le %*s\n", &x, &y, &z);
			cloud_set_point(cloud, index, x, y, z);
			index++;
		} else {
			util_error("%s: erro no parse [%s]", __FUNCTION__, filename);
			break;
		}
	}
	
	fclose(file);
	
	return cloud;
}

/**
 * \brief Salva uma nuvem em arquivo XYZ
 * \param cloud A nuvem a ser salva
 * \param filename O arquivo destino
 * \return 0 se ocorreu algum erro, 1 caso-contrário
 */
int cloud_save_xyz(struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return 0;
    }

    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le %le %le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

/**
 * \brief Salva uma nuvem em arquivo CSV
 * \param cloud A nuvem a ser salva
 * \param filename O arquivo destino
 * \return 0 se ocorreu algum erro, 1 caso-contrário
 */
int cloud_save_csv(struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return 0;
    }

    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le,%le,%le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

/**
 * \brief Salva uma nuvem em arquivo PLY
 * \param cloud A nuvem a ser salva
 * \param filename O arquivo destino
 * \return 0 se ocorreu algum erro, 1 caso-contrário
 */
int cloud_save_ply(struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return 0;
    }
	
	fprintf(file, "ply\n");
	fprintf(file, "format ascii 1.0\n");
	fprintf(file, "comment dumped by artur@interfaces-ufc-br\n");
	fprintf(file, "element vertex %d\n", cloud_size(cloud));
	fprintf(file, "property float x\n");
	fprintf(file, "property float y\n");
	fprintf(file, "property float z\n");
	fprintf(file, "end_header\n");
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le %le %le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

/**
 * \brief Salva uma nuvem em arquivo PCD (somente DATA ascii!!!)
 * \param cloud A nuvem a ser salva
 * \param filename O arquivo destino
 * \return 0 se ocorreu algum erro, 1 caso-contrário
 */
int cloud_save_pcd(struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s", __FUNCTION__, filename);
        return 0;
    }
	
	fprintf(file, "VERSION .7\n");
	fprintf(file, "FIELDS x y z\n");
	fprintf(file, "SIZE 4 4 4\n");
	fprintf(file, "TYPE F F F\n");
	fprintf(file, "COUNT 1 1 1\n");
	fprintf(file, "WIDTH %d\n", cloud_size(cloud));
	fprintf(file, "HEIGHT 1\n");
	fprintf(file, "VIEWPOINT 0 0 0 1 0 0 0\n");
	fprintf(file, "POINTS %d\n", cloud_size(cloud));
	fprintf(file, "DATA ascii\n");
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le %le %le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

/**
 * \brief Calcula o centro geométrico de uma nuvem de pontos
 * \param cloud A Nuvem alvo
 * \return Um ponto com as coordenadas do centro geométrico da nuvem
 */
struct vector3* cloud_calc_center(struct cloud* cloud)
{
	if (cloud->centroid != NULL)
		vector3_free(cloud->centroid);
	
	cloud->centroid = vector3_zero();

    for (uint i = 0; i < cloud->num_pts; i++) {
        cloud->centroid->x += cloud->points[i].x;
        cloud->centroid->y += cloud->points[i].y;
        cloud->centroid->z += cloud->points[i].z;
    }

    cloud->centroid->x /= cloud->num_pts;
    cloud->centroid->y /= cloud->num_pts;
    cloud->centroid->z /= cloud->num_pts;

    return cloud->centroid;
}

/**
 * \brief Retorna o centro geométrico de uma nuvem de pontos
 * \param cloud A Nuvem alvo
 * \return Um ponto com as coordenadas do centro geométrico da nuvem
 */
struct vector3* cloud_get_center(struct cloud* cloud)
{
    if (cloud->centroid != NULL)
        return vector3_from_vector(cloud->centroid);

    return vector3_from_vector(cloud_calc_center(cloud));
}

/**
 * \brief Escala uma nuvem a partir de um fator
 * \param cloud A nuvem alvo
 * \param f O fator de escalamento
 */
void cloud_scale(struct cloud* cloud, real f)
{
    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_scale(&cloud->points[i], f);
}

/**
 * \brief Efetua translação na nuvem a partir de um vetor alvo
 * \param cloud A nuvem a ser transformada
 * \param t O vetor transformação
 */
void cloud_translate_vector(struct cloud* cloud, struct vector3* dest)
{
    struct vector3* t = vector3_sub(dest, cloud_get_center(cloud));

    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_increase(&cloud->points[i], t);

    vector3_free(t);
	cloud_calc_center(cloud);
}

/**
 * \brief Efetua translação na nuvem a partir das coordenadas alvo
 * \param cloud A nuvem a ser transformada
 * \param x Coordenada x
 * \param y Coordenada y
 * \param z Coordenada z
 */
void cloud_translate_real(struct cloud* cloud, real x, real y, real z)
{
    struct vector3* dest = vector3_new(x, y, z);
    struct vector3* t = vector3_sub(dest, cloud_get_center(cloud));

    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_increase(&cloud->points[i], t);

    vector3_free(dest);
    vector3_free(t);
    cloud_calc_center(cloud);
}

/**
 * \brief Rotaciona nuvem em torno do eixo x
 * \param cloud A nuvem a ser rotacionada
 * \param d O ângulo de rotação em graus
 */
void cloud_rotate_x(struct cloud* cloud, real d)
{
	util_seg("%s: funcionalidade incompleta", __FUNCTION__);
    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_x(&cloud->points[i], d);
}

/**
 * \brief Rotaciona nuvem em torno do eixo y
 * \param cloud A nuvem a ser rotacionada
 * \param d O ângulo de rotação em graus
 */
void cloud_rotate_y(struct cloud* cloud, real d)
{
	util_seg("%s: funcionalidade incompleta", __FUNCTION__);
	for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_y(&cloud->points[i], d);
}

/**
 * \brief Rotaciona nuvem em torno do eixo z
 * \param cloud A nuvem a ser rotacionada
 * \param d O ângulo de rotação em graus
 */
void cloud_rotate_z(struct cloud* cloud, real d)
{
	util_seg("%s: funcionalidade incompleta", __FUNCTION__);
	for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_z(&cloud->points[i], d);
}

/**
 * \brief Função utilitária para a função de ordenação qsort()
 * \param p1 O primeiro ponto para comparação
 * \param p2 O segundo ponto para comparação
 */
int cloud_compare(const void* p1, const void* p2)
{
    struct vector3* dp1 = (struct vector3*)p1;
    struct vector3* dp2 = (struct vector3*)p2;
	
    return dp2->z - dp1->z;
}

/**
 * \brief Função utilitária para a função de ordenação qsort_r()
 * \param p1 O primeiro ponto para comparação
 * \param p2 O segundo ponto para comparação
 */
int cloud_compare_r(const void* p1, const void* p2, void* arg)
{
    struct vector3* dp1 = (struct vector3*)p1;
    struct vector3* dp2 = (struct vector3*)p2;
    struct vector3* pivot = (struct vector3*)arg;
	real d1 = vector3_distance(dp1, pivot);
	real d2 = vector3_distance(dp2, pivot);
	
    return (int)d1 - (int)d2;
}

/**
 * \brief Ordena uma cloud em profundidade usando qsort()
 * \param cloud A cloud alvo
 */
void cloud_sort(struct cloud* cloud)
{
    qsort(cloud->points, cloud->num_pts, sizeof(struct vector3), cloud_compare);
}

/**
 * \brief Ordena uma cloud em profundidade usando qsort_r()
 * \param cloud A cloud alvo
 */
void cloud_sort_r(struct cloud* cloud, struct vector3* arg)
{
    qsort_r(cloud->points, cloud->num_pts, sizeof(struct vector3),
            cloud_compare_r, arg);
}

/**
 * \brief Concatena ("soma") duas nuvens
 * \param c1 A primeira nuvem
 * \param c2 A segunda nuvem
 * \return Uma nuvem com os pontos de c1 e c2
 */
struct cloud* cloud_concat(struct cloud* c1, struct cloud* c2)
{
    uint size_c1 = cloud_size(c1);
    uint size_c2 = cloud_size(c2);

    struct cloud* c3 = cloud_new(size_c1 + size_c2);

    for (uint i = 0; i < size_c1; i++)
        cloud_set_vector(c3, i, &c1->points[i]);

    for (uint j = 0; j < size_c2; j++)
        cloud_set_vector(c3, j + size_c1, &c2->points[j]);

    return c3;
}

/**
 * \brief Calcula os tamanhos dos 3 eixos de uma nuvem
 * \param cloud A nuvem alvo
 * \return Um ponto onde cada coordenada é o tamanho da dimensão em questão
 */
struct vector3* cloud_axis_size(struct cloud* cloud)
{
    if (cloud->num_pts == 0) {
        util_error("%s: nuvem vazia", __FUNCTION__);
        return vector3_zero();
    }

    real max_x = cloud->points[0].x;
    real min_x = cloud->points[0].x;
    real max_y = cloud->points[0].y;
    real min_y = cloud->points[0].y;
    real max_z = cloud->points[0].z;
    real min_z = cloud->points[0].z;

    for (uint i = 1; i < cloud->num_pts; i++) {
        if (cloud->points[i].x > max_x)
            max_x = cloud->points[i].x;
        else if (cloud->points[i].x < min_x)
            min_x = cloud->points[i].x;

        if (cloud->points[i].y > max_y)
            max_y = cloud->points[i].y;
        else if (cloud->points[i].y < min_y)
            min_y = cloud->points[i].y;

        if (cloud->points[i].z > max_z)
            max_z = cloud->points[i].z;
        else if (cloud->points[i].z < min_z)
            min_z = cloud->points[i].z;
    }

    return vector3_new(max_x - min_x, max_y - min_y, max_z - min_z);
}

/**
 * \brief Secciona uma nuvem com base em corte a partir do centro
 * \param cloud A nuvem alvo
 * \param cut Valor do corte em milimetros
 * \return A subnuvem cortada
 */
struct cloud* cloud_cut_center(struct cloud* cloud, real cut)
{
    struct cloud* sub = cloud_empty();
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++) {
        if (vector3_distance(center, &cloud->points[i]) <= cut)
            cloud_add_point(sub, &cloud->points[i]);
    }

    return sub;
}

/**
 * \brief Secciona uma nuvem ao longo de uma direção com base em uma referência
 * \param cloud A nuvem alvo
 * \param plane O plano de corte
 * \return A subnuvem cortada
 */
struct cloud* cloud_cut_plane(struct cloud* cloud, struct plane* plane)
{
    struct cloud* sub = cloud_empty();
    
    for (uint i = 0; i < cloud->num_pts; i++)
		if (plane_on_direction(plane, &cloud->points[i]))
			cloud_add_point(sub, &cloud->points[i]);
	
    return sub;
}

/**
 * \brief Particiona a nuvem em duas a partir de um plano
 * \param src A nuvem a ser dividida
 * \param plane O plano de corte
 * \param par1 A primeira partição (nuvem vazia)
 * \param par2 A segunda partição (nuvem vazia)
 * \return 1 se o particionamento ocorrer sem problemas, 0 caso-contrário
 */
int cloud_plane_partition(struct cloud* src,
                          struct plane* plane,
                          struct cloud* par1,
                          struct cloud* par2)
{
	if (cloud_size(par1) || cloud_size(par2))
		return 0;
	
	for (uint i = 0; i < src->num_pts; i++) {
		if (plane_on_direction(plane, &src->points[i]))
			cloud_add_point(par1, &src->points[i]);
		else
			cloud_add_point(par2, &src->points[i]);
	}
	
	return 1;
}

/**
 * \brief Subamostra pontos da nuvem inseridos em um cilindro
 * \param cloud A nuvem alvo
 * \param ref O ponto de referência
 * \param dir A direção da altura do cilindro
 * \param radius O raio do cilindro
 * \return A subnuvem inserida no cilindro
 */
struct cloud* cloud_cut_cylinder(struct cloud* cloud,
                                 struct vector3* ref,
                                 struct vector3* dir,
                                 real radius)
{
	struct cloud* sub = cloud_empty();
	real dirl = vector3_length(dir);
	real dist = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		struct vector3* dot = vector3_sub(ref, &cloud->points[i]);
		struct vector3* cross = vector3_cross(dot, dir);
		
		dist = vector3_length(cross) / dirl;
		if (dist <= radius)
			cloud_add_point(sub, &cloud->points[i]);
		
		vector3_free(dot);
		vector3_free(cross);
	}
	
	return sub;
}

/**
 * \brief Extrái segmento de pontos ao longo da nuvem
 * \param cloud A nuvem alvo
 * \param ref O ponto de referência
 * \param dir A direção do caminho de segmentação
 * \param epslon Largura do segmento em mm
 * \return Subnuvem contendo os pontos formadores do segmento
 */
struct cloud* cloud_segment(struct cloud* cloud,
                            struct vector3* ref,
                            struct vector3* dir,
                            real epslon)
{
	struct cloud* sub = cloud_empty();
	struct plane* plane = plane_new(dir, ref);
	
	for (uint i = 0; i < cloud->num_pts; i++)
		if (plane_distance2point(plane, &cloud->points[i]) <= epslon)
			cloud_add_point(sub, &cloud->points[i]);
	
	plane_free(plane);
	
	return sub;
}

/**
 * \brief Verifica se um ponto pertence a uma nuvem
 * \param cloud A nuvem alvo
 * \param p O ponto sendo buscado
 * \return 1 se p estiver em cloud, 0 caso-contrário
 */
int cloud_has_point(struct cloud* cloud, struct vector3* p)
{
	real d = 0.0f;
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		d = vector3_distance(p, &cloud->points[i]);
		if (d <= 0.1f)
			return 1;
	}
	
	return 0;
}

/**
 * \brief Descobre o ponto da nuvem mais próximo a um ponto arbitrário
 * \param cloud A nuvem alvo
 * \param point O ponto alvo
 * \return Endereço do ponto mais próximo
 */
struct vector3* cloud_closest_point(struct cloud* cloud, struct vector3* point)
{
	uint index = 0;
	real tempd = 0;
	real d = vector3_distance(point, &cloud->points[0]);
	
	for (uint i = 1; i < cloud->num_pts; i++) {
        tempd = vector3_distance(point, &cloud->points[i]);
        if (tempd < d) {
            d = tempd;
            index = i;
		}
    }
    
    return &cloud->points[index];
}

/**
 * \brief Segmento pseudo-riemanniana (dependência com GNU_SOURCE_)
 * \param cloud A nuvem alvo
 * \param start Partida
 * \param end Chegada
 * \param slice Segmento pseudo-riemanniano
 */
void cloud_sudo_riemann_segment(struct cloud* cloud,
                                struct vector3* start,
                                struct vector3* end,
                                struct cloud* slice)
{
	struct vector3* avg = vector3_average(start, end);
	struct vector3* close = cloud_closest_point(cloud, avg);
	
	vector3_free(avg);
	
	if (cloud_has_point(slice, close))
		return;
	
	cloud_add_point(slice, close);
	cloud_sudo_riemann_segment(cloud, start, close, slice);
	cloud_sudo_riemann_segment(cloud, end, close, slice);
}

/**
 * \brief Distância pseudo-riemanniana (dependência com GNU_SOURCE_)
 * \param cloud A nuvem alvo
 * \param start Partida
 * \param end Chegada
 * \return Distância do segmento començando em start e terminando em end
 */
real cloud_sudo_riemann_distance(struct cloud* cloud,
                                 struct vector3* start,
                                 struct vector3* end)
{
	real riemann = 0.0f;
	struct cloud* slice = cloud_empty();
	
	cloud_sudo_riemann_segment(cloud, start, end, slice);
	cloud_sort_r(slice, start);
	
	for (uint i = 0; i < cloud_size(slice)-1; i++) {
		struct vector3* p1 = &slice->points[i];
		struct vector3* p2 = &slice->points[i+1];
		riemann += vector3_distance(p1, p2);
	}
	
	riemann += vector3_distance(&slice->points[cloud_size(slice)-1], end);
	cloud_free(slice);
	
	return riemann;
}

/**
 * \brief Descobre o ponto da nuvem mais próximo ao seu centro de massa
 * \param cloud A nuvem alvo
 * \return Endereço do ponto mais próximo
 */
struct vector3* cloud_closest_to_center(struct cloud* cloud)
{
	return cloud_closest_point(cloud, cloud_get_center(cloud));
}

/**
 * \brief Descobre o ponto com menor coordenada X
 * \param cloud A nuvem alvo
 * \return Endereço do ponto com menor X
 */
struct vector3* cloud_min_x(struct cloud* cloud)
{
	uint index = 0;
	real min_x = cloud->points[0].x;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (min_x < cloud->points[i].x) {
            min_x = cloud->points[i].x;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

/**
 * \brief Descobre o ponto com menor coordenada Y
 * \param cloud A nuvem alvo
 * \return Endereço do ponto com menor Y
 */
struct vector3* cloud_min_y(struct cloud* cloud)
{
	uint index = 0;
	real min_y = cloud->points[0].y;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (min_y < cloud->points[i].y) {
            min_y = cloud->points[i].y;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

/**
 * \brief Descobre o ponto com menor coordenada Z
 * \param cloud A nuvem alvo
 * \return Endereço do ponto com menor Z
 */
struct vector3* cloud_min_z(struct cloud* cloud)
{
	uint index = 0;
	real minz = cloud->points[0].z;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (minz < cloud->points[i].z) {
            minz = cloud->points[i].z;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

/**
 * \brief Descobre o ponto com maior coordenada X
 * \param cloud A nuvem alvo
 * \return Endereço do ponto com maior X
 */
struct vector3* cloud_max_x(struct cloud* cloud)
{
	uint index = 0;
	real max_x = cloud->points[0].x;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (max_x > cloud->points[i].x) {
            max_x = cloud->points[i].x;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

/**
 * \brief Descobre o ponto com maior coordenada Y
 * \param cloud A nuvem alvo
 * \return Endereço do ponto com maior Y
 */
struct vector3* cloud_max_y(struct cloud* cloud)
{
	uint index = 0;
	real max_y = cloud->points[0].y;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (max_y > cloud->points[i].y) {
            max_y = cloud->points[i].y;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

/**
 * \brief Descobre o ponto com maior coordenada Z
 * \param cloud A nuvem alvo
 * \return Endereço do ponto com maior Z
 */
struct vector3* cloud_max_z(struct cloud* cloud)
{
	uint index = 0;
	real max_z = cloud->points[0].z;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (max_z > cloud->points[i].z) {
            max_z = cloud->points[i].z;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

/**
 * \brief Calcula a maior distância entre um ponto qualquer e os pontos da nuvem
 * \param cloud A nuvem alvo
 * \param p O ponto em questão
 * \return A maior distância possível a partir do ponto da nuvem
 */
real cloud_max_distance(struct cloud* cloud, struct vector3* p)
{
	real d = vector3_distance(p, &cloud->points[0]);
	real temp = 0.0f;
	
	for (uint i = 1; i < cloud->num_pts; i++) {
		temp = vector3_distance(p, &cloud->points[i]);
		
		if (temp > d)
			d = temp;
	}
	
	return d;
}

/**
 * \brief Cálcula a distância entre o centro e o ponto mais distante dele
 * \param cloud A nuvem alvo
 * \return A maior distância possível a partir do centro da nuvem
 */
real cloud_max_distance_from_center(struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real d = cloud_max_distance(cloud, center);
	
	vector3_free(center);
	
	return d;
}

/**
 * \brief Ainda não sei interpretar isso, mas conserva certas curvaturas...
 * \param cloud A nuvem alvo
 * \return A máscara da nuvem
 */
struct cloud* cloud_binary_mask(struct cloud* cloud)
{
	cloud_sort(cloud);
	
	struct cloud* mask = cloud_empty();
	struct vector3* refpoint = cloud_min_z(cloud);
	cloud_add_point(mask, refpoint);
	
	real d1 = 0;
	real d2 = 0;
	for (uint i = 0; i < cloud->num_pts; i++) {
		if (cloud->points[i].y > refpoint->y) {
			d1 = vector3_distance(refpoint, &cloud->points[i+1]);
			d2 = vector3_distance(refpoint, &cloud->points[i+2]);
			
			if (d1 < d2)
				refpoint = &cloud->points[i + 1];
			else if (d2 < d1)
				refpoint = &cloud->points[i + 2];
			
			cloud_add_point(mask, refpoint);
		}
	}
	
	return mask;
}

/**
 * \brief (???) Retorna direção média da distribuição de pontos (???)
 * \param cloud A nuvem alvo
 * \return Direção média da nuvem cloud
 */
struct vector3* cloud_average_direction(struct cloud* cloud)
{
	struct vector3* ans = vector3_zero();
	
	for (uint i = 0; i < cloud->num_pts; i++)
		vector3_increase(ans, &cloud->points[i]);
	
	vector3_normalize(ans);
	
	return ans;
}

/**
 * \brief Debuga uma nuvem ponto a ponto na saída padrão
 * \param cloud A nuvem a ser debugada
 * \param output O arquivo aonde exibir a mensagem
 */
void cloud_debug(struct cloud* cloud, FILE* output)
{
	if (cloud == NULL) {
		fprintf(output, "!!! nuvem vazia !!!\n");
		return;
	}
	
    for (uint i = 0; i < cloud->num_pts; i++)
        fprintf(output, "%le %le %le\n", cloud->points[i].x,
                                         cloud->points[i].y,
                                         cloud->points[i].z);
}

#endif // CLOUD_H

