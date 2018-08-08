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
    uint num_of_points;
};

/*
 * @brief cloud_new Inicializa uma estrutura do tipo cloud
 * @param cloud A estrutura que salvará a nuvem
 * @param num_of_points Quantos pontos possui a nuvem
 * @return O ponteiro para uma nuvem alocada ou NULL se houver algo erro
 */
struct cloud* cloud_new(uint num_of_points)
{
    struct cloud* cloud = malloc(sizeof(struct cloud));

    if (cloud == NULL) {
        fprintf(stderr, "%s: erro alocando memória para nuvem\n", __FUNCTION__);
        return NULL;
    }

    cloud->points = malloc(num_of_points * sizeof(struct vector3));
    if (cloud->points == NULL) {
        fprintf(stderr, "%s: erro alocando memória para ponts\n", __FUNCTION__);
        return NULL;
    }

    cloud->num_of_points = num_of_points;

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
 * @brief cloud_add_point Adiciona um novo ponto a uma nuvem
 * @param cloud A nuvem alvo
 * @param point O ponto a ser adicionado
 * @return O endereço do novo ponto alocado ou NULL se não houver memória
 */
struct vector3* cloud_add_point(struct cloud* cloud,
                                real x,
                                real y,
                                real z)
{
    uint new_size = (cloud->num_of_points + 1) * sizeof(struct vector3);

    struct vector3* new_points = realloc(cloud->points, new_size);
    if (new_points == NULL) {
        fprintf(stderr, "%s: erro adicionando novo ponto\n", __FUNCTION__);
        return NULL;
    }

    cloud->points = new_points;
    cloud->num_of_points++;

    vector3_set(&cloud->points[cloud->num_of_points - 1], x, y, z);

    return &cloud->points[cloud->num_of_points - 1];
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
    for (uint i = 0; i < cloud->num_of_points; i++) {
        center->x += cloud->points->x;
        center->y += cloud->points->y;
        center->z += cloud->points->z;

        k++;
    }

    center->x /= k;
    center->y /= k;
    center->z /= k;

    return center;
}

/*
 * @brief cloud_swap_points Troca a posição em memória entre dois pontos
 * @param cloud A nuvem que guarda os pontos a serem trocados
 * @param p1 O index do primeiro ponto
 * @param p2 O index do segundo ponto
 */
void cloud_swap_points(struct cloud* cloud, uint p1, uint p2)
{
    struct vector3 temp = cloud->points[p1];
    cloud->points[p1] = cloud->points[p2];
    cloud->points[p2] = temp;
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

    struct cloud* cloud = cloud_new();
    int scan = 0;
    real x = 0;
    real y = 0;
    real z = 0;
    while (!feof(file)) {
        scan = fscanf(file, "%le %le %le\n", &x, &y, &z);

        if (scan == EOF)
            break;
        else
            cloud_add_point_xyz(&cloud, x, y, z);
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
    FILE* dump = fopen(filename, "w");
    if (dump == NULL)
        return 0;

    struct cloud* aux = cloud;
    while (aux != NULL) {
        fprintf(dump, "%le %le %le\n", aux->points->x,
                                       aux->points->y,
                                       aux->points->z);

        aux = aux->next;
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
    return cloud->num_of_points;
}

/*
 * @brief cloud_scale Escala uma nuvem a partir de um fator
 * @param cloud A nuvem alvo
 * @param f O fator de escalamento
 */
void cloud_scale(struct cloud* cloud, real f)
{
    struct cloud* aux = cloud;
    while (aux != NULL) {
        vector3_scale(aux->points, f);
        
        aux = aux->next;
    }
}

/*
 * @brief cloud_sort Ordena uma cloud em profundidade
 * @param cloud A cloud alvo
 */
void cloud_sort(struct cloud* cloud)
{
    struct cloud* pivot = cloud;
    while (pivot != NULL) {
        struct cloud* aux = pivot->next;

        while(aux != NULL) {
            struct vector3* p1 = pivot->points;
            struct vector3* p2 = aux->points;

            if (p1->x > p2->x)
                cloud_swap_points(pivot, aux);

            aux = aux->next;
        }

        pivot = pivot->next;
    }
}

/*
 * @brief cloud_axis_size Calcula os tamanhos dos 3 eixos de uma nuvem
 * @param cloud A nuvem alvo
 * @return Um ponto onde cada coordenada é o tamanho da dimensão em questão
 */
struct vector3* cloud_axis_size(struct cloud* cloud)
{
    struct cloud* aux = cloud;

    if (aux == NULL) {
        printf("ERRO NUVEM VAZIA\n");
        return NULL;
    }

    real max_x = aux->points->x;
    real min_x = aux->points->x;

    real max_y = aux->points->y;
    real min_y = aux->points->y;

    real max_z = aux->points->z;
    real min_z = aux->points->z;

    while (aux != NULL) {
        if (aux->points->x > max_x)
            max_x = aux->points->x;
        else if (aux->points->x < min_x)
            min_x = aux->points->x;

        if (aux->points->y > max_y)
            max_y = aux->points->y;
        else if (aux->points->y < min_y)
            min_y = aux->points->y;

        if (aux->points->z > max_z)
            max_z = aux->points->z;
        else if (aux->points->z < min_z)
            min_z = aux->points->z;

        aux = aux->next;
    }

    struct vector3* length = vector3_new(max_x - min_x,
                                         max_y - min_y,
                                         max_z - min_z);
    return length;
}

/*
 * @brief cloud_subcloud Secciona uma nuvem com base em corte a partir do centro
 * @param cloud A nuvem alvo
 * @param cut Valor do corte em milimetros
 * @return A subnuvem
 */
struct cloud* cloud_subcloud(struct cloud* cloud, real cut) {
    struct cloud* sub = cloud_new();
    struct vector3* center = cloud_get_center(cloud);
    struct cloud* aux = cloud;

    while (aux != NULL) {
        if (vector3_distance(center, aux->points) <= cut)
            cloud_add_point_copy(&sub, aux->points);

        aux = aux->next;
    }

    vector3_free(center);
    return sub;
}

/*
 * @brief cloud_debug Debuga uma nuvem ponto a ponto na saída padrão
 * @param cloud A nuvem a ser debugada
 * @param message Uma mensagem opcional para ser exibida no início do debug
 */
void cloud_debug(struct cloud* cloud, const char* message)
{
    printf("CLOUD DEBUG: %s\n", message);

    struct cloud* aux = cloud;
    while (aux != NULL) {
        printf("(%le, %le, %le)\n", aux->points->x,
                                    aux->points->y,
                                    aux->points->z);
        aux = aux->next;
    }

    printf("\n");
}

#endif // CLOUD_H
