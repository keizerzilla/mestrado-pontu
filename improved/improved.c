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
#define PSIZE 10
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

    struct cloud* cloud = cloud_load_csv(argv[1]);

    cloud_debug(cloud, "testando cloud_load_csv", stdout);
    cloud_sort(cloud);
    cloud_debug(cloud, "testando cloud_sort", stdout);
    vector3_debug(cloud_get_center(cloud), "testando cloud_get_center", stdout);
    vector3_debug(cloud_axis_size(cloud), "testando cloud_axis, size", stdout);

    cloud_free(cloud);

    return 0;
}
