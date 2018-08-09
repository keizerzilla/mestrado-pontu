/*
 * @file cloud.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo com implementações para manipulação de nuvens de pontos.
 *
 * Uma estrutura de dados e várias funções de manipulação foram fornecidas.
 */

#ifndef CLOUD_H
#define CLOUD_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector3.h"

/*
 * @brief A estrutura que guarda uma nuvem de dados em memória.
 *
 * A estrutura dessa versão é um array simples mais o número de pontos.
 */
struct cloud {
    struct vector3* points;
    uint num_pts;
};

/*
 * @brief cloud_new Inicializa uma estrutura do tipo cloud
 * @param cloud A estrutura que salvará a nuvem
 * @param num_of_points Quantos pontos possui a nuvem
 * @return O ponteiro para uma nuvem alocada ou NULL se houver algo erro
 */
struct cloud* cloud_new(uint num_pts)
{
    struct cloud* cloud = malloc(sizeof(struct cloud));

    if (cloud == NULL) {
        fprintf(stderr, "%s: erro alocando memória para nuvem\n", __FUNCTION__);
        return NULL;
    }

    cloud->points = malloc(num_pts * sizeof(struct vector3));
    if (cloud->points == NULL) {
        fprintf(stderr, "%s: erro alocando memória para ponts\n", __FUNCTION__);
        return NULL;
    }

    memset(cloud->points, 0, num_pts * sizeof(struct vector3));

    cloud->num_pts = num_pts;

    return cloud;
}

/*
 * @brief cloud_free Libera a memória alocada para uma nuvem
 * @param cloud A nuvem a ser liberada
 */
void cloud_free(struct cloud* cloud)
{
    free(cloud->points);
    free(cloud);
    cloud = NULL;
}

/*
 * @brief cloud_set_point Seta os valores de um dos pontos da nuvem
 * @param cloud A nuvem alvo
 * @param index O índice do ponto a ser setado
 * @param x O valor da coordenada x
 * @param y O valor da coordenada y
 * @param z O valor da coordenada z
 * @return O endereço para o ponto setado
 */
struct vector3* cloud_set_point(struct cloud* cloud,
                                uint index,
                                real x,
                                real y,
                                real z)
{
    if (index >= cloud->num_pts) {
        fprintf(stderr, "%s: index out of bounds\n", __FUNCTION__);
        return NULL;
    }

    vector3_set(&cloud->points[index], x, y, z);

    return &cloud->points[index];
}

/*
 * @brief cloud_set_point Seta os valores de um dos pontos da nuvem
 * @param cloud A nuvem alvo
 * @param index O índice do ponto a ser setado
 * @param point O valor das coordenadas a serem setadas
 * @return O endereço para o ponto setado
 */
struct vector3* cloud_set_point_cpy(struct cloud* cloud,
                                    uint index,
                                    struct vector3* point)
{
    return cloud_set_point(cloud, index, point->x, point->y, point->z);
}

/*
 * @brief cloud_add_point Adiciona um novo ponto a uma nuvem
 * @param cloud A nuvem alvo
 * @param x A coordenada x do ponto
 * @param y A coordenada y do ponto
 * @param z A coordenada z do ponto
 * @return O endereço do novo ponto alocado ou NULL se não houver memória
 */
struct vector3* cloud_add_point(struct cloud* cloud,
                                real x,
                                real y,
                                real z)
{
    uint new_size = (cloud->num_pts + 1) * sizeof(struct vector3);

    struct vector3* new_points = realloc(cloud->points, new_size);
    if (new_points == NULL) {
        fprintf(stderr, "%s: erro adicionando novo ponto\n", __FUNCTION__);
        return NULL;
    }

    cloud->points = new_points;
    cloud->num_pts++;

    vector3_set(&cloud->points[cloud->num_pts - 1], x, y, z);

    return &cloud->points[cloud->num_pts - 1];
}

/*
 * @brief cloud_add_point_cpy Adiciona um novo ponto a uma nuvem
 * @param cloud A nuvem alvo
 * @param point O ponto a ser adicionado
 * @return O endereço do novo ponto alocado ou NULL se não houver memória
 */
struct vector3* cloud_add_point_cpy(struct cloud* cloud, struct vector3* point)
{
    return cloud_add_point(cloud, point->x, point->y, point->z);
}

/*
 * @brief cloud_load_csv Carrega uma nuvem a partir de um arquivo CSV
 * @param filename O arquivo onde a nuvem está guardada
 * @return Um estrutura cloud carregada em memória ou NULL caso ocorra erro
 */
struct cloud* cloud_load_csv(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "%s: erro com arquivo %s\n", __FUNCTION__, filename);
        return NULL;
    }

    uint num_pts = 0;
    while (!feof(file)) {
        fscanf(file, "%*s\n");
        num_pts++;
    }
    rewind(file);

    struct cloud* cloud = cloud_new(num_pts);
    real x = 0;
    real y = 0;
    real z = 0;
    uint index = 0;
    while (!feof(file)) {
        fscanf(file, "%le,%le,%le\n", &x, &y, &z);
        cloud_set_point(cloud, index, x, y, z);
        index++;
    }

    fclose(file);

    return cloud;
}

/*
 * @brief cloud_save_to_file Salva uma nuvem em arquivo
 * @param cloud A nuvem a ser salva
 * @param filename O arquivo destino
 * @return 0 se ocorreu algum erro, 1 caso-contrário
 */
int cloud_save_csv(struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "%s: erro com arquivo %s\n", __FUNCTION__, filename);
        return 0;
    }

    int print = 0;
    for (uint i = 0; i < cloud->num_pts; i++) {
        print = fprintf(file, "%le,%le,%le\n", cloud->points[i].x,
                                               cloud->points[i].y,
                                               cloud->points[i].z);
        if (print < 0)
            break;
    }

    return 1;
}

/*
 * @brief cloud_num_of_points Conta quantos pontos constituem uma nuvem
 * @param cloud A nuvem alvo
 * @return O número de pontos de cloud
 */
uint cloud_num_of_points(struct cloud* cloud)
{
    return cloud->num_pts;
}

/*
 * @brief cloud_get_center Calcula o centro geométrico de uma nuvem de pontos
 * @param cloud A Nuvem alvo
 * @return Um ponto com as coordenadas do centro geométrico da nuvem
 */
struct vector3* cloud_get_center(struct cloud* cloud)
{
    struct vector3* center = vector3_zero();

    uint k = 0;
    for (uint i = 0; i < cloud->num_pts; i++) {
        center->x += cloud->points[i].x;
        center->y += cloud->points[i].y;
        center->z += cloud->points[i].z;

        k++;
    }

    center->x /= k;
    center->y /= k;
    center->z /= k;

    return center;
}

/*
 * @brief cloud_scale Escala uma nuvem a partir de um fator
 * @param cloud A nuvem alvo
 * @param f O fator de escalamento
 */
void cloud_scale(struct cloud* cloud, real f)
{
    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_scale(&cloud->points[i], f);
}

/*
 * @brief cloud_compare Função utilitária para a função de ordenação
 * @param p1 O primeiro ponto para comparação
 * @param p2 O segundo ponto para comparação
 */
int cloud_compare(const void* p1, const void* p2)
{
    struct vector3* dp1 = (struct vector3*)p1;
    struct vector3* dp2 = (struct vector3*)p2;

    return dp1->z - dp2->z;
}

/*
 * @brief cloud_sort Ordena uma cloud em profundidade
 * @param cloud A cloud alvo
 */
void cloud_sort(struct cloud* cloud)
{
    qsort(cloud->points, cloud->num_pts, sizeof(struct vector3), cloud_compare);
}

/*
 * @brief cloud_axis_size Calcula os tamanhos dos 3 eixos de uma nuvem
 * @param cloud A nuvem alvo
 * @return Um ponto onde cada coordenada é o tamanho da dimensão em questão
 */
struct vector3* cloud_axis_size(struct cloud* cloud)
{
    if (cloud->num_pts == 0) {
        fprintf(stderr, "%s: nuvem vazia\n", __FUNCTION__);
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

/*
 * @brief cloud_subcloud Secciona uma nuvem com base em corte a partir do centro
 * @param cloud A nuvem alvo
 * @param cut Valor do corte em milimetros
 * @return A subnuvem
 */
struct cloud* cloud_subcloud(struct cloud* cloud, real cut) {
    struct cloud* sub = cloud_new(0);
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++) {
        if (vector3_distance(center, &cloud->points[i]) <= cut)
            cloud_add_point_cpy(sub, &cloud->points[i]);
    }

    vector3_free(center);

    return sub;
}

/*
 * @brief cloud_debug Debuga uma nuvem ponto a ponto na saída padrão
 * @param cloud A nuvem a ser debugada
 * @param message Uma mensagem opcional para ser exibida no início do debug
 * @param output O arquivo aonde exibir a mensagem
 */
void cloud_debug(struct cloud* cloud, const char* message, FILE* output)
{
    fprintf(output, "cloud: %s\n", message);
    for (uint i = 0; i < cloud->num_pts; i++)
        fprintf(output, "%le, %le, %le\n", cloud->points[i].x,
                                           cloud->points[i].y,
                                           cloud->points[i].z);
}

#endif // CLOUD_H
