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
#include "util.h"

/**
 * \brief Estrutura que guarda uma nuvem de pontos em memória.
 */
struct cloud {
    struct vector3* points;
    struct vector3* centroid;
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

    memset(cloud->points, 0, num_pts * sizeof(struct vector3));

    cloud->centroid = NULL;
    cloud->num_pts = num_pts;

    return cloud;
}

/**
 * \brief Libera a memória alocada para uma nuvem
 * \param cloud A nuvem a ser liberada
 */
void cloud_free(struct cloud* cloud)
{
    free(cloud->points);

    if (cloud->centroid != NULL) {
        vector3_free(cloud->centroid);
        cloud->centroid = NULL;
    }

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
struct vector3* cloud_set_point(struct cloud* cloud,
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

    return &cloud->points[index];
}

/**
 * \brief Seta os valores de um dos pontos da nuvem
 * \param cloud A nuvem alvo
 * \param index O índice do ponto a ser setado
 * \param point O valor das coordenadas a serem setadas
 * \return O endereço para o ponto setado
 */
struct vector3* cloud_set_point_cpy(struct cloud* cloud,
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
struct vector3* cloud_add_point(struct cloud* cloud,
                                real x,
                                real y,
                                real z)
{
    uint new_size = (cloud->num_pts + 1) * sizeof(struct vector3);

    struct vector3* new_points = realloc(cloud->points, new_size);
    if (new_points == NULL) {
        util_error("%s: erro adicionando novo ponto", __FUNCTION__);
        return NULL;
    }

    cloud->points = new_points;
    cloud->num_pts++;

    vector3_set(&cloud->points[cloud->num_pts - 1], x, y, z);

    return &cloud->points[cloud->num_pts - 1];
}

/**
 * \brief Adiciona um novo ponto a uma nuvem
 * \param cloud A nuvem alvo
 * \param point O ponto a ser adicionado
 * \return O endereço do novo ponto alocado ou NULL se não houver memória
 */
struct vector3* cloud_add_point_cpy(struct cloud* cloud, struct vector3* point)
{
    return cloud_add_point(cloud, point->x, point->y, point->z);
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
 * \brief Conta quantos pontos constituem uma nuvem
 * \param cloud A nuvem alvo
 * \return O número de pontos de cloud
 */
uint cloud_size(struct cloud* cloud)
{
    return cloud->num_pts;
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
        return cloud->centroid;

    return cloud_calc_center(cloud);
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
 * @brief Efetua translação na nuvem a partir de um vetor alvo
 * @param cloud A nuvem a ser transformada
 * @param t O vetor transformação
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
 * @brief Efetua translação na nuvem a partir das coordenadas alvo
 * @param cloud A nuvem a ser transformada
 * @param x Coordenada x
 * @param y Coordenada y
 * @param z Coordenada z
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
 * @brief Rotaciona nuvem em torno do eixo x
 * @param cloud A nuvem a ser rotacionada
 * @param d O ângulo de rotação em graus
 */
void cloud_rotate_x(struct cloud* cloud, real d)
{
    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_x(&cloud->points[i], d);
}

/**
 * @brief Rotaciona nuvem em torno do eixo y
 * @param cloud A nuvem a ser rotacionada
 * @param d O ângulo de rotação em graus
 */
void cloud_rotate_y(struct cloud* cloud, real d)
{
	for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_y(&cloud->points[i], d);
}

/**
 * @brief Rotaciona nuvem em torno do eixo z
 * @param cloud A nuvem a ser rotacionada
 * @param d O ângulo de rotação em graus
 */
void cloud_rotate_z(struct cloud* cloud, real d)
{
	for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_z(&cloud->points[i], d);
}

/**
 * \brief Função utilitária para a função de ordenação
 * \param p1 O primeiro ponto para comparação
 * \param p2 O segundo ponto para comparação
 */
int cloud_compare(const void* p1, const void* p2)
{
    struct vector3* dp1 = (struct vector3*)p1;
    struct vector3* dp2 = (struct vector3*)p2;

    return dp1->z - dp2->z;
}

/**
 * \brief Ordena uma cloud em profundidade
 * \param cloud A cloud alvo
 */
void cloud_sort(struct cloud* cloud)
{
    qsort(cloud->points, cloud->num_pts, sizeof(struct vector3), cloud_compare);
}

/**
 * @brief Concatena (soma) duas nuvens
 * @param c1 A primeira nuvem
 * @param c2 A segunda nuvem
 * @return Uma nuvem com os pontos de c1 e c2
 */
struct cloud* cloud_add(struct cloud* c1, struct cloud* c2)
{
    uint size_c1 = cloud_size(c1);
    uint size_c2 = cloud_size(c2);

    struct cloud* c3 = cloud_new(size_c1 + size_c2);

    for (uint i = 0; i < size_c1; i++)
        cloud_set_point_cpy(c3, i, &c1->points[i]);

    for (uint j = 0; j < size_c2; j++)
        cloud_set_point_cpy(c3, j + size_c1, &c2->points[j]);

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
struct cloud* cloud_subcloud(struct cloud* cloud, real cut) {
    struct cloud* sub = cloud_new(0);
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++) {
        if (vector3_distance(center, &cloud->points[i]) <= cut)
            cloud_add_point_cpy(sub, &cloud->points[i]);
    }

    return sub;
}

/**
 * \brief Debuga uma nuvem ponto a ponto na saída padrão
 * \param cloud A nuvem a ser debugada
 * \param message Uma mensagem opcional para ser exibida no início do debug
 * \param output O arquivo aonde exibir a mensagem
 */
void cloud_debug(struct cloud* cloud, const char* message, FILE* output)
{
    fprintf(output, "cloud: %s\n", message);
    for (uint i = 0; i < cloud->num_pts; i++)
        fprintf(output, "%le %le %le\n", cloud->points[i].x,
                                         cloud->points[i].y,
                                         cloud->points[i].z);
}

#endif // CLOUD_H
