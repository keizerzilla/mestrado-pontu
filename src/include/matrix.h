/**
 * \file matrix.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Arquivo contendo uma implementação simples de matrizes bidimensionais
 * que ajudam no armazenamento de diversos resultados. Algumas funcionalidades
 * foram criadas em função das necessidades específicas do trabalho.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"
#include "util.h"

/**
 * \brief Estrutura matriz bidimensional.
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
    
    if (mat->data == NULL)
		return NULL;
	
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
	if (mat == NULL)
		return;
	
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
 * \brief Adiciona uma nova coluna a uma matriz
 * \param mat A matriz alvo
 * \return A nova quantidade de colunas da matriz se sucesso, 0 caso-contrário
 */
int matrix_add_col(struct matrix* mat)
{
	real* new_mat = malloc(mat->rows * (mat->cols + 1) * sizeof(real));
	if (new_mat == NULL)
		return 0;
	
	for (uint i = 0; i < mat->rows; i++)
		for (uint j = 0; j < mat->cols + 1; j++)
			new_mat[(i * (mat->cols + 1)) + j] = 0.0f;
	
	for (uint i = 0; i < mat->rows; i++)
		for (uint j = 0; j < mat->cols; j++)
			new_mat[(i * (mat->cols + 1)) + j] = mat->data[(i * mat->cols) + j];
	
	free(mat->data);
	mat->data = new_mat;
	mat->cols++;
	
	return mat->cols;
}

/**
 * \brief Adiciona um valor em uma posição de uma matriz
 * \param mat A matriz alvo
 * \param i O índice da linha
 * \param j O índice da coluna
 * \param value O valor a ser salvo
 * \return O endereço do valor adicionado
 */
real* matrix_set(struct matrix* mat, uint i, uint j, real value)
{
    if (i >= mat->rows || j >= mat->cols) {
		return NULL;
    } else {
        mat->data[(i * mat->cols) + j] = value;
        return &mat->data[(i * mat->cols) + j];
	}
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
    if (i >= mat->rows || j >= mat->cols)
        return 0.0f;
    else
        return mat->data[(i * mat->cols) + j];
}

/**
 * \brief Concatena duas matrizes horizontalmente (lado-a-lado)
 * \param m1 A primeira matriz
 * \param m2 A segunda matriz
 * \return A matriz resultante da concatenação horizontal de m1 com m2
 */
struct matrix* matrix_concat_hor(struct matrix* m1, struct matrix* m2)
{
	if (m1->rows != m2->rows)
		return NULL;
		
	uint ncols = m1->cols + m2->cols;
	
	struct matrix* ans = matrix_new(m1->rows, ncols);
	if (ans == NULL)
		return NULL;
	
	for (uint i = 0; i < m1->rows; i++)
		for (uint j = 0; j < m1->cols; j++)
			matrix_set(ans, i, j, matrix_get(m1, i, j));
	
	for (uint i = 0; i < m2->rows; i++)
		for (uint j = m1->cols; j < ncols; j++)
			matrix_set(ans, i, j, matrix_get(m2, i, j - m1->cols));
	
	return ans;
}

/**
 * \brief Concatena duas matrizes verticalmente (uma em cima da outra)
 * \param m1 A primeira matriz
 * \param m2 A segunda matriz
 * \return A matriz resultante da concatenação vertical de m1 com m2
 */
struct matrix* matrix_concat_ver(struct matrix* m1, struct matrix* m2)
{
	if (m1->cols != m2->cols)
		return NULL;
	
	uint nrows = m1->rows + m2->rows;
	
	struct matrix* ans = matrix_new(nrows, m1->cols);
	if (ans == NULL)
		return NULL;
	
	for (uint i = 0; i < m1->rows; i++)
		for (uint j = 0; j < m1->cols; j++)
			matrix_set(ans, i, j, matrix_get(m1, i, j));
	
	for (uint i = m1->rows; i < nrows; i++)
		for (uint j = 0; j < m2->cols; j++)
			matrix_set(ans, i, j, matrix_get(m2, i - m1->rows, j));
	
	return ans;
}

/**
 * \brief Salva uma matriz em arquivo
 * \param mat A matriz a ser salva
 * \param filename O caminho para o arquivo destino
 * \param m Modo de escrita no arquivo
 * \return 1 se a matriz for salva com sucesso, 0 caso-contrário
 */
int matrix_save_to_file(struct matrix* mat, const char* filename, const char* m)
{
    FILE* file = fopen(filename, m);
    if (file == NULL) {
        util_error("%s: erro abrir arquivo %s\n", __FUNCTION__, filename);
        return 0;
    }

    for (uint row = 0; row < mat->rows; row++) {
        for (uint col = 0; col < mat->cols; col++) {
			fprintf(file, "%le", matrix_get(mat, row, col));
            if (col + 1 < mat->cols)
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
 * \param output Arquivo aonde a matrix será exibida
 */
void matrix_debug(struct matrix* mat, FILE* output)
{
	if (mat == NULL) {
		fprintf(output, "!!! matriz vazia !!!\n");
		return;
	}
	
	for (uint i = 0; i < mat->rows; i++) {
        for (uint j = 0; j < mat->cols; j++) {
        	if (j == mat->cols - 1)
            	fprintf(output, "%le", mat->data[(i * mat->cols) + j]);
            else
            	fprintf(output, "%le ", mat->data[(i * mat->cols) + j]);
        }
        
        fprintf(output, "\n");
    }
}

#endif // MATRIX_H

