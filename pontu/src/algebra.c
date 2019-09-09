#include "../include/algebra.h"

struct matrix *algebra_mat_prod(struct matrix *a, struct matrix *b)
{
	struct matrix *c;
	real sum = 0.0f;

	if (a->cols != b->rows) {
		return NULL;
	}

	c = matrix_new(a->rows, b->cols);

	for (uint i = 0; i < a->rows; i++) {
		for (uint j = 0; j < b->cols; j++) {
			for (uint k = 0; k < a->cols; k++) {
				sum += matrix_get(a, i, k) * matrix_get(b, k, j);
			}
			matrix_set(c, i, j, sum);
			sum = 0.0f;
		}
	}

	return c;
}

struct matrix *algebra_mat_sum(struct matrix *a, struct matrix *b)
{
	struct matrix *c;
	if ((a->cols != b->cols) || (a->rows != b->rows)) {
		return NULL;
	}

	c = matrix_new(a->rows, a->cols);

	for (uint i = 0; i < c->rows; i++) {
		for (uint j = 0; j < c->cols; j++) {
			matrix_set(c, i, j, matrix_get(a, i, j) + matrix_get(b, i, j));
		}
	}

	return c;
}

struct matrix *algebra_mat_sub(struct matrix *a, struct matrix *b)
{
	struct matrix *c;
	if ((a->cols != b->cols) || (a->rows != b->rows)) {
		return NULL;
	}

	c = matrix_new(a->rows, a->cols);

	for (uint i = 0; i < c->rows; i++) {
		for (uint j = 0; j < c->cols; j++) {
			matrix_set(c, i, j, matrix_get(a, i, j) - matrix_get(b, i, j));
		}
	}

	return c;
}

struct matrix *algebra_mat_vs_scalar(struct matrix *mat, real scalar)
{
    struct matrix *new_mat = matrix_new(mat->rows, mat->cols);
	for (uint i = 0; i < mat->rows; i++) {
		for (uint j = 0; j < mat->cols; j++) {
			matrix_set(new_mat, i, j, matrix_get(mat, i, j) * scalar);
		}
	}

    return new_mat;
}

struct matrix *algebra_trans_mat(struct matrix *mat)
{
	struct matrix *mat_trans = matrix_new(mat->cols, mat->rows);

	for (uint i = 0; i < mat->rows; i++) {
		for (uint j = 0; j < mat->cols; j++) {
			matrix_set(mat_trans, j, i, matrix_get(mat, i, j));
		}
	}

	return mat_trans;
}
