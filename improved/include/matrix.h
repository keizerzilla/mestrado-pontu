/*
 * @file matrix.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo uma implementação simples de matrizes bidimensionais
 * que ajudam no armazenamento de diversos resultados. Alguns funcionalidades
 * foram criadas em função das necessidades do trabalho.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/*
 * @brief A estrutura matrix
 *
 * Consiste de um ponteiro bidimensional de valores ponto-flutuante e mais dois
 * campos que guardam a quantidade de linhas e colunas presente.
 */
struct matrix {
    real** data;
    uint rows;
    uint cols;
};

/*
 * @brief matrix_new Aloca memória para uma nova matriz
 * @param rows Quantidade de linhas da matriz
 * @param cols Quantidade de colunas da matriz
 * @return Um ponteiro para o endereço aonde a matriz está na memória
 */
struct matrix* matrix_new(uint rows, uint cols)
{
    struct matrix* mat = malloc(sizeof(struct matrix));

    if (mat == NULL)
        return NULL;

    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * sizeof(real*));

    uint i, j;
    for (i = 0; i < rows; i++) {
        mat->data[i] = malloc(cols * sizeof(real));

        for (j = 0; j < cols; j++)
            mat->data[i][j] = 0.0f;
    }

    return mat;
}

/*
 * @brief matrix_add_row Adiciona uma nova linha a uma matriz
 * @param mat A matriz alvo
 * @return A nova quantidade de linhas da matriz se sucesso, 0 caso-contrário
 */
int matrix_add_row(struct matrix* mat)
{
    real** newline = realloc(mat->data, (mat->rows + 1) * sizeof(real*));
    if (newline != NULL) {
        mat->data = newline;
        mat->rows++;
        mat->data[mat->rows - 1] = malloc(mat->cols * sizeof(real));

        uint j;
        for (j = 0; j < mat->cols; j++)
            mat->data[mat->rows - 1][j] = 0.0f;

        return mat->rows;
    } else {
        return 0;
    }
}

/*
 * @brief matrix_add_column Adiciona uma nova coluna a uma matriz
 * @param mat A matriz alvo
 * @return A nova quantidade de colunas da matriz se sucesso. 0 caso-contrário
 */
int matrix_add_column(struct matrix* mat)
{
    uint i;
    for (i = 0; i < mat->rows; i++) {
        real* newcolumn = realloc(mat->data[i], (mat->cols + 1) * sizeof(real));

        if (newcolumn != NULL) {
            mat->data[i] = newcolumn;
            mat->data[i][mat->cols] = 0.0f;
        } else {
            return 0;
        }
    }

    mat->cols++;
    return mat->cols;
}

/*
 * @brief matrix_set Seta o valor em uma dada célula de uma matriz
 * @param mat A matriz alvo
 * @param i O índice da linha
 * @param j O índice da coluna
 * @param value O valor a ser salvo
 */
void matrix_set(struct matrix* mat, uint i, uint j, real value)
{
    if (i >= mat->rows || j >= mat->cols)
        fprintf(stderr, "%s: index fora de posicao na matriz\n", __FUNCTION__);
    else
        mat->data[i][j] = value;
}

/*
 * @brief matrix_get Recupera um valor salvo na matriz
 * @param mat A matriz alvo
 * @param i O índice da linha
 * @param j O índice da coluna
 * @return O valor da posição(i,j) da matriz mat
 */
real matrix_get(struct matrix* mat, uint i, uint j)
{
    if (i >= mat->rows || j >= mat->cols) {
        fprintf(stderr, "%s: index fora de posicao na matriz\n", __FUNCTION__);
        return 0;
    } else {
        return mat->data[i][j];
    }
}

/*
 * @brief matrix_save_to_file Salva uma matriz em arquivo
 * @param mat A matriz a ser salva
 * @param filename O caminho para o arquivo destino
 * @param sep O caractér seperador entre cada elemento das linhas
 * @return 1 se a matriz for salva com sucesso, 0 caso-contrário
 */
int matrix_save_to_file(struct matrix* mat,
                        const char* filename,
                        const char* mode,
                        char sep)
{
    FILE* dump = fopen(filename, mode);
    if (dump == NULL) {
        fprintf(stderr, "%s: erro com arquivo %s\n", __FUNCTION__, filename);
        return 0;
    }

    uint rows = 0;
    uint cols = 0;
    for (rows = 0; rows < mat->rows; rows++) {
        for (cols = 0; cols < mat->cols; cols++) {
            fprintf(dump, "%le", matrix_get(mat, rows, cols));
            if (cols + 1 < mat->cols)
                fprintf(dump, "%c", sep);
        }

        fprintf(dump, "\n");
    }

    return 1;
}

/*
 * @brief matrix_free Libera espaço alocado para uma matriz
 * @param mat A matriz a ser desalocada
 */
void matrix_free(struct matrix* mat)
{
    uint i = 0;
    for (i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
        mat->data[i] = NULL;
    }

    free(mat);
    mat = NULL;
}

/*
 * @brief matrix_debug Debuga uma matriz imprimindo todos os seus valores
 * @param mat A matriz a ser debugada
 * @param message Uma mensagem a ser exibida no topo do debug
 * @param output Arquivo aonde a mensagem será salva
 */
void matrix_debug(struct matrix* mat, const char* msg, FILE* output)
{
    fprintf(output, "matrix: %s\n", msg);
    uint i;
    uint j;
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++)
            fprintf(output, "%le\t", mat->data[i][j]);

        fprintf(output, "\n");
    }
}

#endif // MATRIX_H
