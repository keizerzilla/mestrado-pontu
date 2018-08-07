/*
 * @file char-ext.c
 * @author Artur Rodrigues Rocha Neto
 * @date 2018
 * @brief Testando melhorias nas estruturas de dados e funções de extração.
 */

#include <time.h>
#include "include/matrix.h"

#define DATA_SIZE 100

int main()
{
    fprintf(stdout, "OPTMIZING matrix\n");

    clock_t t;
    srand(time(NULL));

    t = clock();
    for (int i = 0; i < DATA_SIZE; i++) {
        // ???
    }
    t = clock() - t;

    printf("%lu ticks (%.4f seconds)\n", t, (float)t/CLOCKS_PER_SEC);

    return 0;
}
