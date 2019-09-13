#include "../include/cmatrix.h"

struct cmatrix *cmatrix_new(uint rows, uint cols)
{
	struct cmatrix *mat = malloc(sizeof(struct cmatrix));
	if (mat == NULL)
		return NULL;

	mat->rows = rows;
	mat->cols = cols;
	
	mat->data = malloc(rows * cols * sizeof(cnum));
	if (mat->data == NULL)
		return NULL;

	for (uint i = 0; i < rows; i++)
		for (uint j = 0; j < cols; j++)
			mat->data[(i * cols) + j] = 0.0f;

	return mat;
}

void cmatrix_free(struct cmatrix *mat)
{
	if (mat == NULL)
		return;

	free(mat->data);
	mat->data = NULL;
	free(mat);
	mat = NULL;
}

int cmatrix_add_row(struct cmatrix *mat)
{
	cnum *row = realloc(mat->data, mat->cols * (mat->rows + 1) * sizeof(cnum));

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

int cmatrix_add_col(struct cmatrix *mat)
{
	cnum *new_mat = malloc(mat->rows * (mat->cols + 1) * sizeof(cnum));
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

struct cmatrix *cmatrix_remove_row(struct cmatrix *mat, uint row)
{
	if (row >= mat->rows) {
		return NULL;
	}

	struct cmatrix *new_mat = cmatrix_new(mat->rows - 1, mat->cols);
	if (new_mat == NULL)
		return NULL;
	
	uint k = -1;

	for (uint i = 0; i < new_mat->rows; i++) {
		k += row == i ? 2 : 1;
		for (uint j = 0; j < new_mat->cols; j++)
			cmatrix_set(new_mat, i, j, cmatrix_get(mat, k, j));
	}

	return new_mat;
}

struct cmatrix *cmatrix_remove_col(struct cmatrix *mat, uint col)
{
	if (col >= mat->cols) {
		return NULL;
	}

	struct cmatrix *new_mat = cmatrix_new(mat->rows, mat->cols - 1);
	if (new_mat == NULL)
		return NULL;
	
	uint k;

	for (uint i = 0; i < new_mat->rows; i++) {
		k = -1;
		for (uint j = 0; j < new_mat->cols; j++) {
			k += col == j ? 2 : 1;
			cmatrix_set(new_mat, i, j, cmatrix_get(mat, i, k));
		}
	}

	return new_mat;
}

cnum *cmatrix_set(struct cmatrix *mat, uint i, uint j, cnum value)
{
	if (i >= mat->rows || j >= mat->cols) {
		return NULL;
	} else {
		mat->data[(i * mat->cols) + j] = value;
		return &mat->data[(i * mat->cols) + j];
	}
}

cnum cmatrix_get(struct cmatrix *mat, uint i, uint j)
{
	if (i >= mat->rows || j >= mat->cols)
		return 0.0f;
	else
		return mat->data[(i * mat->cols) + j];
}

struct cmatrix *cmatrix_concat_hor(struct cmatrix *m1, struct cmatrix *m2)
{
	if (m1->rows != m2->rows)
		return NULL;

	uint ncols = m1->cols + m2->cols;

	struct cmatrix *ans = cmatrix_new(m1->rows, ncols);
	if (ans == NULL)
		return NULL;

	for (uint i = 0; i < m1->rows; i++)
		for (uint j = 0; j < m1->cols; j++)
			cmatrix_set(ans, i, j, cmatrix_get(m1, i, j));

	for (uint i = 0; i < m2->rows; i++)
		for (uint j = m1->cols; j < ncols; j++)
			cmatrix_set(ans, i, j, cmatrix_get(m2, i, j - m1->cols));

	return ans;
}

struct cmatrix *cmatrix_concat_ver(struct cmatrix *m1, struct cmatrix *m2)
{
	if (m1->cols != m2->cols)
		return NULL;

	uint nrows = m1->rows + m2->rows;

	struct cmatrix *ans = cmatrix_new(nrows, m1->cols);
	if (ans == NULL)
		return NULL;

	for (uint i = 0; i < m1->rows; i++)
		for (uint j = 0; j < m1->cols; j++)
			cmatrix_set(ans, i, j, cmatrix_get(m1, i, j));

	for (uint i = m1->rows; i < nrows; i++)
		for (uint j = 0; j < m2->cols; j++)
			cmatrix_set(ans, i, j, cmatrix_get(m2, i - m1->rows, j));

	return ans;
}

struct cmatrix *cmatrix_sum(struct cmatrix *a, struct cmatrix *b)
{
    if (a->cols != b->cols || a->rows != b->rows)
        return NULL;
    
    struct cmatrix *c = cmatrix_new(a->rows, a->cols);

    if (c == NULL)
        return NULL;

    for (uint i = 0; i < c->rows; i++)
        for (uint j = 0; j < c->cols; j++)
            cmatrix_set(c, i, j, cmatrix_get(a, i, j) + cmatrix_get(b, i, j));
    
    return c;
}

struct cmatrix *cmatrix_sub(struct cmatrix *a, struct cmatrix *b)
{
    if (a->cols != b->cols || a->rows != b->rows)
        return NULL;
    
    struct cmatrix *c = cmatrix_new(a->rows, a->cols);

    if (c == NULL)
        return NULL;

    for (uint i = 0; i < c->rows; i++)
        for (uint j = 0; j < c->cols; j++)
            cmatrix_set(c, i, j, cmatrix_get(a, i, j) - cmatrix_get(b, i, j));
    
    return c;
}

struct cmatrix *cmatrix_prod(struct cmatrix *a, struct cmatrix *b)
{
    if (a->cols != b->rows)
        return NULL;
    
    struct cmatrix *c = cmatrix_new(a->rows, b->cols);
    
    if (c == NULL)
        return NULL;
    
    cnum sum = 0.0f;

    for (uint i = 0; i < a->rows; i++)
        for (uint j = 0; j < b->cols; j++) {
            for (uint k = 0; k < a->cols; k++)
                sum += cmatrix_get(a, i, k) * cmatrix_get(b, k, j);
            cmatrix_set(c, i, j, sum);
            sum = 0.0f;
        }

    return c;
}

struct cmatrix *cmatrix_vs_scalar(struct cmatrix *mat, cnum scalar)
{
    struct cmatrix *nmat = cmatrix_new(mat->rows, mat->cols);
    
    if (nmat == NULL)
        return NULL;

    for (uint i = 0; i < mat->rows; i++)
        for (uint j = 0; j < mat->cols; j++)
            cmatrix_set(nmat, i, j, cmatrix_get(mat, i, j) * scalar);

    return nmat;
}

struct cmatrix *cmatrix_transpose(struct cmatrix *mat)
{
    struct cmatrix *mat_trans = cmatrix_new(mat->cols, mat->rows);

    if (mat_trans == NULL)
        return NULL;

    for (uint i = 0; i < mat->rows; i++)
        for (uint j = 0; j < mat->cols; j++)
            cmatrix_set(mat_trans, j, i, cmatrix_get(mat, i, j));

    return mat_trans;
}

cnum cmatrix_det(struct cmatrix *mat)
{
	cnum det = 0.0f;
	struct cmatrix *aux;
	short signal = 1;
	if (mat->rows == 2 && mat->cols == 2) {
		det += cmatrix_get(mat, 0, 0) * cmatrix_get(mat, 1, 1);
		det -= cmatrix_get(mat, 0, 1) * cmatrix_get(mat, 1, 0);
	} else if (mat->rows == mat->cols && mat->rows > 2) {
		aux = cmatrix_remove_row(mat, 0);
		for (uint i = 0; i < mat->cols; i++) {
			det += signal * cmatrix_get(mat, 0, i) *
				   cmatrix_det(cmatrix_remove_col(aux, i));
			signal *= -1;
		}
	}

	return det;
}

int cmatrix_save_to_file(struct cmatrix *mat, const char *filename, const char *m)
{
	FILE *file = fopen(filename, m);
	if (file == NULL) {
		printf("%s: erro abrir arquivo %s\n", __FUNCTION__, filename);
		return 0;
	}

	for (uint row = 0; row < mat->rows; row++) {
		for (uint col = 0; col < mat->cols; col++) {
			fprintf(file, "%le + (%le * I)", creal(cmatrix_get(mat, row, col)), 
                                      cimag(cmatrix_get(mat, row, col)));
			if (col + 1 < mat->cols)
				fprintf(file, "%c", ',');
		}

		fprintf(file, "\n");
	}

	fclose(file);

	return 1;
}

void cmatrix_debug(struct cmatrix *mat, FILE *output)
{
	if (mat == NULL) {
		fprintf(output, "!!! matrix empty !!!\n");
		return;
	}

	for (uint i = 0; i < mat->rows; i++) {
		for (uint j = 0; j < mat->cols; j++) {
			if (j == mat->cols - 1)
				fprintf(output, "%le + (%le * I)", creal(mat->data[(i * mat->cols) + j]),
                                                   cimag(mat->data[(i * mat->cols) + j]));
			else
				fprintf(output, "%le + (%le * I), ", creal(mat->data[(i * mat->cols) + j]),
                                                    cimag(mat->data[(i * mat->cols) + j]));
		}

		fprintf(output, "\n");
	}
}

