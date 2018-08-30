/**
 * \file matrix.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo uma implementação simples de matrizes bidimensionais
 * que ajudam no armazenamento de diversos resultados. Alguns funcionalidades
 * foram criadas em função das necessidades do trabalho.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/**
 * \brief Estrutura matrix bidimensional.
 */
struct matrix {
    uint rows;
    uint cols;
    real* data;
};

/**
 * \brief Aloca memória para uma nova matriz
 * \param rows Quantidade de linhas da matriz
 * \param cols Quantidade de colunas da matriz
 * \return Um ponteiro para o endereço aonde a matriz está na memória
 */
struct matrix* matrix_new(uint rows, uint cols)
{
    struct matrix* mat = malloc(sizeof(struct matrix));
    if (mat == NULL)
        return NULL;

    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * cols * sizeof(real));

    for (uint i = 0; i < rows; i++)
        for (uint j = 0; j < cols; j++)
            mat->data[(i * cols) + j] = 0.0f;

    return mat;
}

/**
 * \brief Libera espaço alocado para uma matriz
 * \param mat A matriz a ser desalocada
 */
void matrix_free(struct matrix* mat)
{
    free(mat->data);
    mat->data = NULL;

    free(mat);
    mat = NULL;
}

/**
 * \brief Adiciona uma nova linha a uma matriz
 * \param mat A matriz alvo
 * \return A nova quantidade de linhas da matriz se sucesso, 0 caso-contrário
 */
int matrix_add_row(struct matrix* mat)
{
    real* row = realloc(mat->data, mat->cols * (mat->rows + 1) * sizeof(real));

    if (row != NULL) {
        mat->data = row;
        mat->rows++;

        for (uint j = 0; j < mat->cols; j++)
            mat->data[((mat->rows - 1) * mat->cols) + j] = 0.0f;

        return mat->rows;
    } else {
        return 0;
    }
}

/**
 * \brief Seta o valor em uma dada célula de uma matriz
 * \param mat A matriz alvo
 * \param i O índice da linha
 * \param j O índice da coluna
 * \param value O valor a ser salvo
 */
void matrix_set(struct matrix* mat, uint i, uint j, real value)
{
    if (i >= mat->rows || j >= mat->cols)
        fprintf(stderr, "%s: index fora de posicao na matriz\n", __FUNCTION__);
    else
        mat->data[(i * mat->cols) + j] = value;
}

/**
 * \brief Recupera um valor salvo na matriz
 * \param mat A matriz alvo
 * \param i O índice da linha
 * \param j O índice da coluna
 * \return O valor da posição(i,j) da matriz mat
 */
real matrix_get(struct matrix* mat, uint i, uint j)
{
    if (i >= mat->rows || j >= mat->cols) {
        fprintf(stderr, "%s: index fora de posicao na matriz\n", __FUNCTION__);
        return 0;
    } else {
        return mat->data[(i * mat->cols) + j];
    }
}

/**
 * \brief Salva uma matriz em arquivo
 * \param mat A matriz a ser salva
 * \param filename O caminho para o arquivo destino
 * \return 1 se a matriz for salva com sucesso, 0 caso-contrário
 */
int matrix_save_to_file(struct matrix* mat, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "%s: erro abrir arquivo %s\n", __FUNCTION__, filename);
        return 0;
    }

    for (uint rows = 0; rows < mat->rows; rows++) {
        for (uint cols = 0; cols < mat->cols; cols++) {
            fprintf(file, "%le", matrix_get(mat, rows, cols));

            if (cols + 1 < mat->cols)
                fprintf(file, "%c", ',');
        }

        fprintf(file, "\n");
    }

    fclose(file);

    return 1;
}

/**
 * \brief Debuga uma matriz imprimindo todos os seus valores
 * \param mat A matriz a ser debugada
 * \param message Uma mensagem a ser exibida no topo do debug
 * \param output Arquivo aonde a mensagem será salva
 */
void matrix_debug(struct matrix* mat, const char* msg, FILE* output)
{
    fprintf(output, "matrix: %s\n", msg);

    for (uint i = 0; i < mat->rows; i++) {
        for (uint j = 0; j < mat->cols; j++)
            fprintf(output, "%le  ", mat->data[(i * mat->cols) + j]);

        fprintf(output, "\n");
    }
}

#endif // MATRIX_H
