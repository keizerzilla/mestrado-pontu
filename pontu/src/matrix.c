#include "../include/matrix.h"

struct matrix *matrix_new(uint rows, uint cols)
{
	struct matrix *mat = malloc(sizeof(struct matrix));
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

void matrix_free(struct matrix *mat)
{
	if (mat == NULL)
		return;

	free(mat->data);
	mat->data = NULL;
	free(mat);
	mat = NULL;
}

int matrix_add_row(struct matrix *mat)
{
	real *row = realloc(mat->data, mat->cols * (mat->rows + 1) * sizeof(real));

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

int matrix_add_col(struct matrix *mat)
{
	real *new_mat = malloc(mat->rows * (mat->cols + 1) * sizeof(real));
	if (new_mat != NULL)
		return 0;

	for (uint i = 0; i < mat->rows; i++)
		for (uint j = 0; j < mat->cols + 1; j++)
			new_mat[(i * (mat->cols + 1)) + j] = 0.0f;

	for (uint i = 0; i < mat->rows; i++)
		for (uint j = 0; j < mat->cols; j++)
			new_mat[(i * (mat->cols + 1)) + j] =
			    mat->data[(i * mat->cols) + j];

	free(mat->data);
	
	mat->data = new_mat;
	mat->cols++;

	return mat->cols;
}

struct matrix *matrix_remove_row(struct matrix *mat, uint row)
{
	if (row >= mat->rows) {
		return NULL;
	}

	struct matrix *new_mat = matrix_new(mat->rows - 1, mat->cols);
	if (new_mat == NULL)
		return NULL;
	
	uint k = -1;

	for (uint i = 0; i < new_mat->rows; i++) {
		k += row == i ? 2 : 1;
		for (uint j = 0; j < new_mat->cols; j++)
			matrix_set(new_mat, i, j, matrix_get(mat, k, j));
	}

	return new_mat;
}

struct matrix *matrix_remove_col(struct matrix *mat, uint col)
{
	if (col >= mat->cols) {
		return NULL;
	}

	struct matrix *new_mat = matrix_new(mat->rows, mat->cols - 1);
	if (new_mat == NULL)
		return NULL;
	
	uint k;

	for (uint i = 0; i < new_mat->rows; i++) {
		k = -1;
		for (uint j = 0; j < new_mat->cols; j++) {
			k += col == j ? 2 : 1;
			matrix_set(new_mat, i, j, matrix_get(mat, i, k));
		}
	}

	return new_mat;
}

real *matrix_set(struct matrix *mat, uint i, uint j, real value)
{
	if (i >= mat->rows || j >= mat->cols) {
		return NULL;
	} else {
		mat->data[(i * mat->cols) + j] = value;
		return &mat->data[(i * mat->cols) + j];
	}
}

real matrix_get(struct matrix *mat, uint i, uint j)
{
	if (i >= mat->rows || j >= mat->cols)
		return 0.0f;
	else
		return mat->data[(i * mat->cols) + j];
}

struct matrix *matrix_concat_hor(struct matrix *m1, struct matrix *m2)
{
	if (m1->rows != m2->rows)
		return NULL;

	uint ncols = m1->cols + m2->cols;

	struct matrix *ans = matrix_new(m1->rows, ncols);
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

struct matrix *matrix_concat_ver(struct matrix *m1, struct matrix *m2)
{
	if (m1->cols != m2->cols)
		return NULL;

	uint nrows = m1->rows + m2->rows;

	struct matrix *ans = matrix_new(nrows, m1->cols);
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

int matrix_save_to_file(struct matrix *mat, const char *filename, const char *m)
{
	FILE *file = fopen(filename, m);
	if (file == NULL) {
		printf("%s: erro abrir arquivo %s\n", __FUNCTION__, filename);
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

void matrix_debug(struct matrix *mat, FILE *output)
{
	if (mat == NULL) {
		fprintf(output, "!!! matrix empty !!!\n");
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

