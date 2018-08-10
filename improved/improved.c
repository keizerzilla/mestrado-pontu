/*
 * @file char-ext.c
 * @author Artur Rodrigues Rocha Neto
 * @date 2018
 * @brief Testando melhorias nas estruturas de dados e funções de extração.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/cloud.h"

#ifndef PSIZE
#define PSIZE 4096
#endif

float randf(float max)
{
    return (float)rand() / (float)(RAND_MAX / max);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "mising arguments\n");
        exit(1);
    }

    srand(time(NULL));

    struct cloud* cloud = cloud_new(PSIZE);
    for (uint i = 0; i < PSIZE; i++) {
        cloud_set_point(cloud, i, randf(256), randf(256), randf(256));
    }

    cloud_sort(cloud);
    struct vector3* center = cloud_get_center(cloud);
    struct vector3* axis = cloud_axis_size(cloud);
    cloud_save_csv(cloud, argv[1]);

    vector3_free(center);
    vector3_free(axis);
    cloud_free(cloud);

    return 0;
}
