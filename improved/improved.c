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
        fprintf(csv, "%e,%e,%e\n", randf(256), randf(256), randf(256));
    }

    fprintf(csv, "\n\n\n\n\n\n\n\n\n\n");
    fprintf(csv, "%e,%e,%e\n", randf(256), randf(256), randf(256));

    fclose(csv);

    FILE* reader = fopen(argv[1], "r");
    if (reader == NULL) {
        fprintf(stderr, "error loading csv\n");
        exit(1);
    }

    int k = 0;
    while (!feof(reader)) {
        fscanf(reader, "%*s\n");
        k++;
    }

    printf("linhas = %d\n", k);

    return 0;
}
