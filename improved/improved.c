/*
 * @file char-ext.c
 * @author Artur Rodrigues Rocha Neto
 * @date 2018
 * @brief Testando melhorias nas estruturas de dados e funções de extração.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/vector3.h"

#define DATA_SIZE 658

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

    FILE* csv = fopen(argv[1], "w");
    if (csv == NULL) {
        fprintf(stderr, "error loading csv\n");
        exit(1);
    }

    srand(time(NULL));
    for (int i = 0; i < DATA_SIZE; i++) {
        fprintf(csv, "%.2f,%.2f,%.2f\n", randf(256), randf(256), randf(256));
    }

    fclose(csv);

    FILE* reader = fopen(argv[1], "r");
    if (reader == NULL) {
        fprintf(stderr, "error loading csv\n");
        exit(1);
    }

    // contar linhas aqui



    return 0;
}
