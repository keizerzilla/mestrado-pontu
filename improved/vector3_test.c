/*
 * @file char-ext.c
 * @author Artur Rodrigues Rocha Neto
 * @date 2018
 * @brief Testando melhorias nas estruturas de dados e funções de extração.
 */

#include <time.h>
#include "include/vector3.h"

#define DATA_SIZE 1000000

int main()
{
    fprintf(stdout, "OPTMIZING vector3\n");
    fprintf(stdout, "SIZEOF vector3: %ld\n", sizeof(struct vector3));

    clock_t t;
    srand(time(NULL));

    struct vector3 p1 = {1.0f, 2.0f, 3.0f};
    struct vector3 p2 = {-3.0f, -2.0f, 0.0f};

    struct vector3* array = malloc(DATA_SIZE * sizeof(struct vector3));
    struct vector3** normals = malloc(DATA_SIZE * sizeof(struct vector3*));
    real* areas = malloc(DATA_SIZE * sizeof(real));

    t = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        array[i].x = rand() % 256;
        array[i].y = rand() % 256;
        array[i].z = rand() % 256;

        normals[i] = vector3_normal(&p1, &p2, &array[i]);
        areas[i] = vector3_area(&p1, &p2, &array[i]);
    }
    t = clock() - t;

    free(array);
    for (int i = 0; i < DATA_SIZE; i++) vector3_free(normals[i]);
    free(normals);
    free(areas);

    printf("%lu ticks (%.4f seconds)\n", t, (float)t/CLOCKS_PER_SEC);

    return 0;
}

