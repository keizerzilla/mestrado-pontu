/**
 * @file cloud.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo com implementações para manipulação de nuvens de pontos.
 *
 * Uma estrutura de dados e várias funções de manipulação dos pontos de uma
 * nuvem foram fornecidas.
 */

#ifndef CLOUD_H
#define CLOUD_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector3.h"

/**
 * @brief A estrutura que guarda uma nuvem de dados em memória (linked list)
 */
struct cloud {
    struct vector3* point;
    struct cloud* next;
};

/**
 * @brief cloud_new Inicializa uma estrutura do tipo cloud
 * @return Ponteiro para uma nova nuvem de pontos
 */
struct cloud* cloud_new()
{
    return NULL;
}

/**
 * @brief cloud_add_point Adiciona um novo ponto a uma nuvem (1 vetor)
 * @param cloud A nuvem alvo
 * @param point O ponto a ser adicionado
 * @return E endereço do novo ponto alocado ou NULL se não houver memória
 */
struct cloud* cloud_add_point(struct cloud** cloud, struct vector3* point)
{
    struct cloud* node = malloc(sizeof(struct cloud));

    if (node == NULL)
        return NULL;

    node->point = point;
    node->next = *cloud;
    *cloud = node;

    return node;
}

/**
 * @brief cloud_add_point_xyz Adiciona um novo ponto a uma nuvem (3 reais)
 * @param cloud A nuvem alvo
 * @param point O ponto a ser adicionado
 * @return E endereço do novo ponto alocado ou NULL se não houver memória
 */
struct cloud* cloud_add_point_xyz(struct cloud** cloud,
                                  real x, real y, real z)
{
    struct cloud* node = malloc(sizeof(struct cloud));

    if (node == NULL)
        return NULL;

    node->point = vector3_new(x, y, z);
    node->next = *cloud;
    *cloud = node;

    return node;
}

/**
 * @brief cloud_add_point Adiciona um novo ponto a uma nuvem (copia de vetor)
 * @param cloud A nuvem alvo
 * @param point O ponto a ser adicionado
 * @return E endereço do novo ponto alocado ou NULL se não houver memória
 */
struct cloud* cloud_add_point_copy(struct cloud** cloud, struct vector3* point)
{
    struct cloud* node = malloc(sizeof(struct cloud));

    if (node == NULL)
        return NULL;

    node->point = vector3_new(point->x, point->y, point->z);
    node->next = *cloud;
    *cloud = node;

    return node;
}

/**
 * @brief cloud_get_center Calcula o centro de uma nuvem de pontos
 * @param cloud A Nuvem alvo
 * @return Um ponto com as coordenadas do centro da nuvem
 */
struct vector3* cloud_get_center(struct cloud* cloud)
{
    struct vector3* center = vector3_new(0.0f, 0.0f, 0.0f);

    int k = 0;
    struct cloud* aux = cloud;
    while (aux != NULL) {
        center->x += aux->point->x;
        center->y += aux->point->y;
        center->z += aux->point->z;

        k++;
        aux = aux->next;
    }

    center->x /= k;
    center->y /= k;
    center->z /= k;

    return center;
}

/**
 * @brief cloud_swap_points Troca a posição em memória entre dois pontos
 * @param node1 O primeiro nó da lista
 * @param node2 O segundo nó da lista
 */
void cloud_swap_points(struct cloud* node1, struct cloud* node2)
{
    struct vector3* temp = node1->point;
    node1->point = node2->point;
    node2->point = temp;
}

/**
 * @brief cloud_load_from_file Carrega uma nuvem a partir de um arquivo CSV
 * @param filename O arquivo onde a nuvem está guardada
 * @return Um estrutura cloud carregada em memória
 */
struct cloud* cloud_load_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return NULL;

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

/**
 * @brief cloud_save_to_file Salva uma nuvem em arquivo
 * @param cloud A nuvem a ser salva
 * @param filename O arquivo destino
 * @return 0 se ocorreu algum erro, 1 caso-contrário
 */
int cloud_save_to_file(struct cloud* cloud, const char* filename)
{
    FILE* dump = fopen(filename, "w");
    if (dump == NULL)
        return 0;

    struct cloud* aux = cloud;
    while (aux != NULL) {
        fprintf(dump, "%le %le %le\n", aux->point->x,
                                       aux->point->y,
                                       aux->point->z);

        aux = aux->next;
    }

    return 1;
}

/**
 * @brief cloud_num_of_points Conta quantos pontos constituem uma nuvem
 * @param cloud A nuvem alvo
 * @return O número de pontos dentro de cloud
 */
int cloud_num_of_points(struct cloud* cloud)
{
    int size = 0;
    struct cloud* aux = cloud;
    while (aux != NULL) {
        size++;
        aux = aux->next;
    }

    return size;
}

/**
 * @brief cloud_scale Escala uma nuvem a partir de um fator
 * @param cloud A nuvem alvo
 * @param f O fator de escalamento
 */
void cloud_scale(struct cloud* cloud, real f)
{
    struct cloud* aux = cloud;
    while (aux != NULL) {
        vector3_scale(aux->point, f);
        
        aux = aux->next;
    }
}

/**
 * @brief cloud_sort Ordena uma cloud em profundidade
 * @param cloud A cloud alvo
 */
void cloud_sort(struct cloud* cloud)
{
    struct cloud* pivot = cloud;
    while (pivot != NULL) {
        struct cloud* aux = pivot->next;

        while(aux != NULL) {
            struct vector3* p1 = pivot->point;
            struct vector3* p2 = aux->point;

            if (p1->x > p2->x)
                cloud_swap_points(pivot, aux);

            aux = aux->next;
        }

        pivot = pivot->next;
    }
}

/**
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

    real max_x = aux->point->x;
    real min_x = aux->point->x;

    real max_y = aux->point->y;
    real min_y = aux->point->y;

    real max_z = aux->point->z;
    real min_z = aux->point->z;

    while (aux != NULL) {
        if (aux->point->x > max_x)
            max_x = aux->point->x;
        else if (aux->point->x < min_x)
            min_x = aux->point->x;

        if (aux->point->y > max_y)
            max_y = aux->point->y;
        else if (aux->point->y < min_y)
            min_y = aux->point->y;

        if (aux->point->z > max_z)
            max_z = aux->point->z;
        else if (aux->point->z < min_z)
            min_z = aux->point->z;

        aux = aux->next;
    }

    struct vector3* length = vector3_new(max_x - min_x,
                                         max_y - min_y,
                                         max_z - min_z);
    return length;
}

/**
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
        if (vector3_distance(center, aux->point) <= cut)
            cloud_add_point_copy(&sub, aux->point);

        aux = aux->next;
    }

    vector3_free(center);
    return sub;
}

/**
 * @brief cloud_debug Debuga uma nuvem ponto a ponto na saída padrão
 * @param cloud A nuvem a ser debugada
 * @param message Uma mensagem opcional para ser exibida no início do debug
 */
void cloud_debug(struct cloud* cloud, const char* message)
{
    printf("CLOUD DEBUG: %s\n", message);

    struct cloud* aux = cloud;
    while (aux != NULL) {
        printf("(%le, %le, %le)\n", aux->point->x,
                                    aux->point->y,
                                    aux->point->z);
        aux = aux->next;
    }

    printf("\n");
}

/**
 * @brief cloud_free Libera a memória alocada para uma nuvem
 * @param cloud A nuvem a ser liberada
 */
void cloud_free(struct cloud* cloud)
{
    struct cloud* aux = cloud;
    while (aux != NULL) {
        struct cloud* next = aux->next;

        vector3_free(aux->point);
        free(aux);

        aux = next;
    }

    cloud = NULL;
}

#endif // CLOUD_H
