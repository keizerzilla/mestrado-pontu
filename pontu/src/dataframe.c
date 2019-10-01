#include "../include/dataframe.h"

struct dataframe *dataframe_new(uint rows, uint cols)
{
	struct dataframe *mat = malloc(sizeof(struct dataframe));
	if (mat == NULL)
		return NULL;

	mat->rows = rows;
	mat->cols = cols;
	
	mat->data = malloc(rows * cols * sizeof(real));
	if (mat->data == NULL)
		return NULL;

	for (uint i = 0; i < rows; i++)
		for (uint j = 0; j < cols; j++)
			mat->data[(i * cols) + j] = 0.0;

	return mat;
}

void dataframe_free(struct dataframe **mat)
{
	if (*mat == NULL)
		return;

	free((*mat)->data);
	(*mat)->data = NULL;
	free(*mat);
	*mat = NULL;
}

int dataframe_add_row(struct dataframe *mat)
{
	real *row = realloc(mat->data, mat->cols * (mat->rows + 1) * sizeof(real));

	if (row != NULL) {
		mat->data = row;
		mat->rows++;

		for (uint j = 0; j < mat->cols; j++)
			mat->data[((mat->rows - 1) * mat->cols) + j] = 0.0;

		return mat->rows;
	} else {
		return 0;
	}
}

int dataframe_add_col(struct dataframe *mat)
{
	real *new_mat = malloc(mat->rows * (mat->cols + 1) * sizeof(real));
	if (new_mat == NULL)
		return 0;

	for (uint i = 0; i < mat->rows; i++)
		for (uint j = 0; j < mat->cols + 1; j++)
			new_mat[(i * (mat->cols + 1)) + j] = 0.0;

	for (uint i = 0; i < mat->rows; i++)
		for (uint j = 0; j < mat->cols; j++)
			new_mat[(i * (mat->cols + 1)) + j] =
			    mat->data[(i * mat->cols) + j];

	free(mat->data);
	
	mat->data = new_mat;
	mat->cols++;

	return mat->cols;
}

struct dataframe *dataframe_remove_row(struct dataframe *mat, uint row)
{
	if (row >= mat->rows) {
		return NULL;
	}

	struct dataframe *new_mat = dataframe_new(mat->rows - 1, mat->cols);
	if (new_mat == NULL)
		return NULL;
	
	uint k = -1;

	for (uint i = 0; i < new_mat->rows; i++) {
		k += row == i ? 2 : 1;
		for (uint j = 0; j < new_mat->cols; j++)
			dataframe_set(new_mat, i, j, dataframe_get(mat, k, j));
	}

	return new_mat;
}

struct dataframe *dataframe_remove_col(struct dataframe *mat, uint col)
{
	if (col >= mat->cols) {
		return NULL;
	}

	struct dataframe *new_mat = dataframe_new(mat->rows, mat->cols - 1);
	if (new_mat == NULL)
		return NULL;
	
	uint k;

	for (uint i = 0; i < new_mat->rows; i++) {
		k = -1;
		for (uint j = 0; j < new_mat->cols; j++) {
			k += col == j ? 2 : 1;
			dataframe_set(new_mat, i, j, dataframe_get(mat, i, k));
		}
	}

	return new_mat;
}

real *dataframe_set(struct dataframe *mat, uint i, uint j, real value)
{
	if (i >= mat->rows || j >= mat->cols) {
		return NULL;
	} else {
		mat->data[(i * mat->cols) + j] = value;
		return &mat->data[(i * mat->cols) + j];
	}
}

real dataframe_get(struct dataframe *mat, uint i, uint j)
{
	if (i >= mat->rows || j >= mat->cols)
		return 0.0;
	else
		return mat->data[(i * mat->cols) + j];
}

struct dataframe *dataframe_concat_hor(struct dataframe *m1,
                                       struct dataframe *m2)
{
	if (m1->rows != m2->rows)
		return NULL;

	uint ncols = m1->cols + m2->cols;

	struct dataframe *ans = dataframe_new(m1->rows, ncols);
	if (ans == NULL)
		return NULL;

	for (uint i = 0; i < m1->rows; i++)
		for (uint j = 0; j < m1->cols; j++)
			dataframe_set(ans, i, j, dataframe_get(m1, i, j));

	for (uint i = 0; i < m2->rows; i++)
		for (uint j = m1->cols; j < ncols; j++)
			dataframe_set(ans, i, j, dataframe_get(m2, i, j - m1->cols));

	return ans;
}

struct dataframe *dataframe_concat_ver(struct dataframe *m1,
                                       struct dataframe *m2)
{
	if (m1->cols != m2->cols)
		return NULL;

	uint nrows = m1->rows + m2->rows;

	struct dataframe *ans = dataframe_new(nrows, m1->cols);
	if (ans == NULL)
		return NULL;

	for (uint i = 0; i < m1->rows; i++)
		for (uint j = 0; j < m1->cols; j++)
			dataframe_set(ans, i, j, dataframe_get(m1, i, j));

	for (uint i = m1->rows; i < nrows; i++)
		for (uint j = 0; j < m2->cols; j++)
			dataframe_set(ans, i, j, dataframe_get(m2, i - m1->rows, j));

	return ans;
}

int dataframe_save_to_file(struct dataframe *mat,
                           const char *filename,
                           const char *m)
{
	FILE *file = fopen(filename, m);
	if (file == NULL) {
		printf("%s: erro abrir arquivo %s\n", __FUNCTION__, filename);
		return 0;
	}

	for (uint row = 0; row < mat->rows; row++) {
		for (uint col = 0; col < mat->cols; col++) {
			fprintf(file, "%le", dataframe_get(mat, row, col));
			if (col + 1 < mat->cols)
				fprintf(file, "%c", ',');
		}

		fprintf(file, "\n");
	}

	fclose(file);

	return 1;
}

void dataframe_debug(struct dataframe *mat, FILE *output)
{
	if (mat == NULL) {
		fprintf(output, "!!! dataframe empty !!!\n");
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

