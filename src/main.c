/*
 * @file char-ext.c
 * @author Artur Rodrigues Rocha Neto
 * @date 2018
 * @brief Testando melhorias nas estruturas de dados e funções de extração.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/hu.h"
#include "include/legendre.h"
#include "include/tchebychev.h"
#include "include/zernike.h"
#include "include/extraction.h"

#ifndef PSIZE
#define PSIZE 40960
#endif

float randf(float max)
{
    return (float)rand() / (float)(RAND_MAX / max);
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "mising arguments\n");
        exit(1);
    }

    srand(time(NULL));

    struct cloud* cloud = cloud_new(PSIZE);
    for (uint i = 0; i < PSIZE; i++)
        cloud_set_point(cloud, i, randf(256), randf(256), randf(256));

    cloud_save_csv(cloud, argv[1]);

    struct matrix* results = matrix_new(1, 21);
    hu_cloud_moments(cloud, results);
    matrix_save_to_file(results, argv[2], "w", ',');
    matrix_free(results);

    cloud_free(cloud);

    return 0;
}
