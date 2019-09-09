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

real algebra_det(struct matrix *mat)
{
    real det = 0;
    struct matrix *aux;
    int signal = 1;
    if (mat->rows == 2 && mat->cols == 2) {
        det += matrix_get(mat, 0, 0) * matrix_get(mat, 1, 1);
        det -= matrix_get(mat, 0, 1) * matrix_get(mat, 1, 0);
    } else if ((mat->rows == mat->cols) && (mat->rows > 2)) {
        aux = matrix_remove_row(mat, 0);
        for (uint i = 0; i < mat->cols; i++) {
            det += signal * matrix_get(mat, 0, i) *
                  algebra_det(matrix_remove_col(aux, i));
            signal *= -1;
        }
    }

    return det;
}

struct matrix *algebra_bhaskara(real a, real b, real c)
{
	struct matrix *roots = matrix_new(2, 2);
	if (roots == NULL) {
		return NULL;
	}

	real delta = pow(b, 2.0f) - (4 * a *c);
	
	if (delta > 0.0f) {
		matrix_set(roots, 0, 1, 0.0f);
		matrix_set(roots, 1, 1, 0.0f);

		matrix_set(roots, 0, 0, (-b + sqrt(delta)) / (2 * a));
		matrix_set(roots, 1, 0, (-b - sqrt(delta)) / (2 * a));
	} else if (delta == 0.0f) {
		matrix_set(roots, 0, 1, 0.0f);
		matrix_set(roots, 1, 1, 0.0f);

		matrix_set(roots, 0, 0, (-b) / (2 * a));
		matrix_set(roots, 1, 0, matrix_get(roots, 0, 0));
	} else {
		matrix_set(roots, 0, 1, sqrt(fabs(delta))/(2 * a));
		matrix_set(roots, 1, 1, - sqrt(fabs(delta))/(2 * a));
		matrix_set(roots, 0, 0, -b/(2 * a));
		matrix_set(roots, 1, 0, -b/(2 * a));
	}

	return roots;
}

// 3rd degree
// 4th degree

// struct matrix *algebra_eigen_val(struct matrix *mat)
// {
// 	if (matrix->rows != matrix->cols) {
// 		return NULL;
// 	}

// 	switch (matrix->rows) {
// 		case 2:

// 			break;
// 		default:
// 			return NULL;
// 	}
// }
