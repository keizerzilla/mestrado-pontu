#include "../include/algebra.h"

struct matrix *algebra_affine_root(real a, real b)
{
	if (a == 0.0)
		return NULL;

	struct matrix *root = matrix_new(1, 1);
	if (root == NULL)
		return NULL;
	
	matrix_set(root, 0, 0, -b / a);

	return root;
}

struct matrix *algebra_quadratic_roots(real a, real b, real c)
{
	if (a == 0.0)
		return algebra_affine_root(b, c);

	struct matrix *roots = matrix_new(2, 1);
	if (roots == NULL)
		return NULL;

	real delta = (b * b) - (4.0 * a *c);

	matrix_set(roots, 0, 0, (-b + csqrt(delta)) / (2 * a));
	matrix_set(roots, 1, 0, delta == 0.0 ? matrix_get(roots, 0, 0) :
	                                         (-b - csqrt(delta)) / (2 * a));

	if (delta >= 0) {
		matrix_set(roots, 0, 0, creal(matrix_get(roots, 0, 0)));
		matrix_set(roots, 1, 0, creal(matrix_get(roots, 1, 0)));
	}
	return roots;
}

struct matrix *algebra_cubic_roots(real a, real b, real c, real d)
{
	if (a == 0.0)
		return algebra_quadratic_roots(b, c, d);
	
	struct matrix *roots;

	if (b != 0.0 && c != 0.0 && d == 0.0) {
		roots = algebra_quadratic_roots(a, b, c);
		matrix_add_row(roots);
		return roots;
	}

	roots = matrix_new(3, 1);

	if (roots == NULL)
		return NULL;

	real delta = 18.0 * a * b * c * d -
				 4.0 * b * b * b * d +
				 b * b * c * c -
				 4.0 * a * c * c * c -
				 27.0 * a * a * d * d;

	real delta_0 = b * b - 3.0 * a * c;

	if (delta == 0.0) {
		if (delta_0 == 0.0) {
			matrix_set(roots, 0, 0, -b / (3.0 * a));
			matrix_set(roots, 1, 0, matrix_get(roots, 0, 0));
			matrix_set(roots, 2, 0, matrix_get(roots, 0, 0));
		} else {
			matrix_set(roots, 0, 0, (9.0 * a * d - b * c) / (2.0 * delta_0));
			matrix_set(roots, 1, 0, matrix_get(roots, 0, 0));
			matrix_set(roots, 2, 0, (4.0 * a * b * c -
									 9.0 * a * a * d -
									 b * b * b) /
									 (a * delta_0));
		}
	} else {
		real delta_1 = 2.0 * b * b * b - 
					   9.0 * a * b * c + 
					   27.0 * a * a * d;

		real p = -1.0 / (3.0 * a);
		cnum q = cpow((delta_1 + csqrt(-27.0 * a * a * delta)) / 2.0,
					  1.0 / 3.0);

		cnum ep = -0.5 + (0.5 * CALC_SQRT_3 * I);
		cnum r = ep * q;
		cnum s = delta_0 / r;
		matrix_set(roots, 0, 0, p * (b + q + delta_0/q));
		matrix_set(roots, 1, 0, p * (b + r + s));
		r *= ep;
		s /= ep;
		matrix_set(roots, 2, 0, p * (b + r + s));

		if (delta > 0.0) {
			matrix_set(roots, 0, 0, creal(matrix_get(roots, 0, 0)));
			matrix_set(roots, 1, 0, creal(matrix_get(roots, 1, 0)));
			matrix_set(roots, 2, 0, creal(matrix_get(roots, 2, 0)));
		}
	}

	return roots;
}

struct matrix *algebra_quartic_roots(real a, real b, real c, real d, real e)
{
	if (a == 0.0)
		return algebra_cubic_roots(b, c, d, e);
	
	struct matrix *roots;

	if (b != 0.0 && c == d  && d == e && e == 0.0) {
		roots = algebra_affine_root(a, b);

		matrix_add_row(roots);
		matrix_add_row(roots);
		matrix_add_row(roots);

		return roots;
	}

	if (b != 0.0 && c != 0.0 && d == e && e == 0.0) {
		roots = algebra_quadratic_roots(a, b, c);
		
		matrix_add_row(roots);
		matrix_add_row(roots);

		return roots;
	}

	if (b != 0.0 && c != 0.0 && d != 0.0 && e == 0.0) {
		roots = algebra_cubic_roots(a, b, c, d);
		
		matrix_add_row(roots);

		return roots;
	}

	roots = matrix_new(4, 1);
	if (roots == NULL)
		return NULL;

	real delta = (256.0 * a * a * a * e * e * e) -
				 (192.0 * a * a * b * d * e * e) -
				 (128.0 * a * a * c * c * e * e) +
				 (144.0 * a * a * c * d * d * e) -
				 (27.0 * a * a * d * d * d * d) +
				 (144.0 * a * b * b * c * e * e) -
				 (6.0 * a * b * b * d * d * e) -
				 (80.0 * a * b * c * c * d * e) +
				 (18.0 * a * b * c * d * d * d) +
				 (16.0 * a * c * c * c * c * e) -
				 (4.0 * a * c * c * c * d * d) -
				 (27.0 * b * b * b * b * e * e) +
				 (18.0 * b * b * b * c * d * e) -
				 (4.0 * b * b * b * d * d * d) -
				 (4.0 * b * b * c * c * c * e) +
				 (b * b * c * c * d * d);

	real delta_0 = c * c - 3.0 * b * d + 12.0 * a * e;

	real delta_1 = 2.0 * c * c * c -
				   9.0 * b * c * d +
				   27.0 * b * b * e +
				   27.0 * a * d * d -
				   72.0 * a * c * e;
	
	real p = (8.0 * a * c - 3.0 * b * b) / (8.0 * a * a);

	real q = (b * b * b - 4.0 * a * b * c + 8.0 * a * a * d) /
			 (8.0 * a * a * a);

	cnum r = cpow((delta_1 + csqrt(-27.0 * delta)) / 2.0, 1.0 / 3.0);

	cnum s = 0.5 * csqrt(-2.0 / 3.0 * p + (r + delta_0 / r) / (3.0 * a));

	real t = -b / (4.0 * a);
	cnum u = -4 * s * s - 2 * p;
	cnum v = 0.5 * csqrt(u + q / s);

	real f = p * 8.0 * a * a;
	real g = q * 8.0 * a * a * a;
	real h = 64.0 * a * a * a * e -
			 16.0 * a * a * c * c +
			 16.0 * a * b * b * c -
			 16.0 * a * a * b * d -
			 3.0 * b * b * b * b;

	matrix_set(roots, 0, 0, t - s + v);
	matrix_set(roots, 1, 0, t - s - v);

	v = 0.5 * csqrt(u - q / s);

	matrix_set(roots, 2, 0, t + s + v);
	matrix_set(roots, 3, 0, t + s - v);

	real x0, x1, x2, x3;

	if (delta < 0.0) {
		x0 = fabs(cimag(matrix_get(roots, 0, 0)));
		x1 = fabs(cimag(matrix_get(roots, 1, 0)));
		x2 = fabs(cimag(matrix_get(roots, 2, 0)));
		x3 = fabs(cimag(matrix_get(roots, 3, 0)));

		
		if (x0 <= x1 && x0 <= x2 && x0 <= x3) {
			matrix_set(roots, 0, 0, creal(matrix_get(roots, 0, 0)));
		}

		if (x1 <= x0 && x1 <= x2 && x1 <= x3) {
			matrix_set(roots, 1, 0, creal(matrix_get(roots, 1, 0)));
		}

		if (x2 <= x1 && x2 <= x0 && x2 <= x3) {
			matrix_set(roots, 2, 0, creal(matrix_get(roots, 2, 0)));
		}

		if (x3 <= x1 && x3 <= x2 && x3 <= x0) {
			matrix_set(roots, 3, 0, creal(matrix_get(roots, 3, 0)));
		}
		
	} else if (delta > 0.0) {
		if (f < 0.0 && h < 0.0) {
			matrix_set(roots, 0, 0, creal(matrix_get(roots, 0, 0)));
			matrix_set(roots, 1, 0, creal(matrix_get(roots, 1, 0)));
			matrix_set(roots, 2, 0, creal(matrix_get(roots, 2, 0)));
			matrix_set(roots, 3, 0, creal(matrix_get(roots, 3, 0)));
		}
	} else {
		if ((f < 0.0 && h < 0.0 && delta_0 != 0.0) ||
		   (h != 0.0 && delta_0 == 0.0) ||
		   (h == 0.0 && f < 0.0) ||
		   (h == 0.0 && delta_0 == 0.0)) {
			matrix_set(roots, 0, 0, creal(matrix_get(roots, 0, 0)));
			matrix_set(roots, 1, 0, creal(matrix_get(roots, 1, 0)));
			matrix_set(roots, 2, 0, creal(matrix_get(roots, 2, 0)));
			matrix_set(roots, 3, 0, creal(matrix_get(roots, 3, 0)));
		} else if (h > 0.0 || (f > 0.0 && (h != 0.0 || g != 0.0))) {
			x0 = fabs(cimag(matrix_get(roots, 0, 0)));
			x1 = fabs(cimag(matrix_get(roots, 1, 0)));
			x2 = fabs(cimag(matrix_get(roots, 2, 0)));
			x3 = fabs(cimag(matrix_get(roots, 3, 0)));

			if (x0 <= x1 && x0 <= x2 && x0 <= x3) {
				matrix_set(roots, 0, 0, creal(matrix_get(roots, 0, 0)));
			}

			if (x1 <= x0 && x1 <= x2 && x1 <= x3) {
				matrix_set(roots, 1, 0, creal(matrix_get(roots, 1, 0)));
			}

			if (x2 <= x1 && x2 <= x0 && x2 <= x3) {
				matrix_set(roots, 2, 0, creal(matrix_get(roots, 2, 0)));
			}

			if (x3 <= x1 && x3 <= x2 && x3 <= x0) {
				matrix_set(roots, 3, 0, creal(matrix_get(roots, 3, 0)));
			}
		}
	}

	return roots;	
}
struct matrix *algebra_gauss_elim_pivot(struct matrix *mat)
{
	if (mat->rows != mat->cols)
		return NULL;

	struct matrix *eq_sys = matrix_copy(mat);

	if (eq_sys == NULL)
		return NULL;

	cnum temp, diag_el;

	for (uint i = 0; i < (eq_sys->rows - 1); i++) {
		for (uint k = i + 1; k < eq_sys->rows; k++)
			if (cabs(matrix_get(eq_sys, i, i)) <
				cabs(matrix_get(eq_sys, k, i)))
				for (uint j = 0; j < eq_sys->cols; j++) {
					temp = matrix_get(eq_sys, i, j);
					matrix_set(eq_sys, i, j, matrix_get(eq_sys, k, j));
					matrix_set(eq_sys, k, j, temp);
				}
		for (uint k = i + 1; k < eq_sys->rows; k++) {
			if (matrix_get(eq_sys, i, i) != 0.0 &&
				matrix_get(eq_sys, k, i) != 0.0) {
				temp = matrix_get(eq_sys, k, i) / matrix_get(eq_sys, i, i);
				for (uint j = 0; j < eq_sys->cols; j++)
					matrix_set(eq_sys, k, j,
								matrix_get(eq_sys, k, j) - temp *
								matrix_get(eq_sys, i, j));
			}
			
		}
	}

	for (uint i = 0; i < eq_sys->rows; i++)
		for (uint j = 0; j < eq_sys->rows; j++)
			if (j < i)
				matrix_set(eq_sys, i, j, 0.0);

	for (uint i = 0; i < eq_sys->rows; i++){
		diag_el = matrix_get(eq_sys, i, i);
		if ((cabs(diag_el) < ALGEBRA_NUM_ERR))
			matrix_set(eq_sys, i, i, 0.0);
		else
			for (uint j = 0; j < eq_sys->cols; j++)
				if (diag_el != 0.0)
					matrix_set(eq_sys, i, j,
								matrix_get(eq_sys, i, j) / diag_el);
	}

	return eq_sys;
}

struct matrix *algebra_gauss_elim(struct matrix *mat)
{
	if (mat->rows + 1 != mat->cols)
		return NULL;

	struct matrix *eq_sys = matrix_copy(mat);

	if (eq_sys == NULL)
		return NULL;

	struct matrix *solution = matrix_new(mat->rows, 1);

	if (solution == NULL) {
		matrix_free(&eq_sys);
		return NULL;
	}

	cnum temp;

	for (uint i = 0; i < (eq_sys->rows - 1); i++) {
		for (uint k = i + 1; k < eq_sys->rows; k++)
			if (cabs(matrix_get(eq_sys, i, i)) <
				cabs(matrix_get(eq_sys, k, i)))
				for (uint j = 0; j < eq_sys->cols; j++) {
					temp = matrix_get(eq_sys, i, j);
					matrix_set(eq_sys, i, j, matrix_get(eq_sys, k, j));
					matrix_set(eq_sys, k, j, temp);
				}
		for (uint k = i + 1; k < eq_sys->rows; k++) {
			temp = matrix_get(eq_sys, k, i) / matrix_get(eq_sys, i, i);
			for (uint j = 0; j < eq_sys->cols; j++)
				matrix_set(eq_sys, k, j,
							matrix_get(eq_sys, k, j) - temp *
							matrix_get(eq_sys, i, j));
		}
	}

	for (int l = (eq_sys->rows - 1); l >= 0; l--) {
		matrix_set(solution, l, 0, matrix_get(eq_sys, l, eq_sys->cols - 1));
		for (uint j = l + 1; j < (eq_sys->cols - 1); j++)
			matrix_set(solution, l, 0, 
						matrix_get(solution, l, 0) -
						matrix_get(eq_sys, l, j) *
						matrix_get(solution, j, 0));
		matrix_set(solution, l, 0, 
					matrix_get(solution, l, 0) / matrix_get(eq_sys, l, l));
	}

	matrix_free(&eq_sys);
	eq_sys = NULL;

	return solution;
}

struct matrix *algebra_mat_sum(struct matrix *a, struct matrix *b)
{
    if (a->cols != b->cols || a->rows != b->rows)
        return NULL;
    
    struct matrix *c = matrix_new(a->rows, a->cols);

    if (c == NULL)
        return NULL;

    for (uint i = 0; i < c->rows; i++)
        for (uint j = 0; j < c->cols; j++)
            matrix_set(c, i, j, matrix_get(a, i, j) + matrix_get(b, i, j));
    
    return c;
}

struct matrix *algebra_mat_sub(struct matrix *a, struct matrix *b)
{
    if (a->cols != b->cols || a->rows != b->rows)
        return NULL;
    
    struct matrix *c = matrix_new(a->rows, a->cols);

    if (c == NULL)
        return NULL;

    for (uint i = 0; i < c->rows; i++)
        for (uint j = 0; j < c->cols; j++)
            matrix_set(c, i, j, matrix_get(a, i, j) - matrix_get(b, i, j));
    
    return c;
}

struct matrix *algebra_mat_prod(struct matrix *a, struct matrix *b)
{
    if (a->cols != b->rows)
        return NULL;
    
    struct matrix *c = matrix_new(a->rows, b->cols);
    
    if (c == NULL)
        return NULL;
    
    cnum sum = 0.0;

    for (uint i = 0; i < a->rows; i++)
        for (uint j = 0; j < b->cols; j++) {
            for (uint k = 0; k < a->cols; k++)
                sum += matrix_get(a, i, k) * matrix_get(b, k, j);
            matrix_set(c, i, j, sum);
            sum = 0.0;
        }

    return c;
}

struct matrix *algebra_mat_vs_scalar(struct matrix *mat, cnum scalar)
{
    struct matrix *nmat = matrix_new(mat->rows, mat->cols);
    
    if (nmat == NULL)
        return NULL;

    for (uint i = 0; i < mat->rows; i++)
        for (uint j = 0; j < mat->cols; j++)
            matrix_set(nmat, i, j, matrix_get(mat, i, j) * scalar);

    return nmat;
}

struct matrix *algebra_mat_transpose(struct matrix *mat)
{
    struct matrix *mat_trans = matrix_new(mat->cols, mat->rows);

    if (mat_trans == NULL)
        return NULL;

    for (uint i = 0; i < mat->rows; i++)
        for (uint j = 0; j < mat->cols; j++)
            matrix_set(mat_trans, j, i, matrix_get(mat, i, j));

    return mat_trans;
}

cnum algebra_mat_trace(struct matrix *mat)
{
	if (mat->rows != mat->cols) {
		return 0.0;
	}

	cnum trace = 0.0;
	for (uint i = 0; i < mat->rows; i++) {
		trace += matrix_get(mat, i, i);
	}

	return trace;
}

cnum algebra_mat_det(struct matrix *mat)
{
	cnum det = 0.0;
	struct matrix *aux;
	short signal = 1;
	if (mat->rows == 2 && mat->cols == 2) {
		det += matrix_get(mat, 0, 0) * matrix_get(mat, 1, 1);
		det -= matrix_get(mat, 0, 1) * matrix_get(mat, 1, 0);
	} else if (mat->rows == mat->cols && mat->rows > 2) {
		aux = matrix_remove_row(mat, 0);
		for (uint i = 0; i < mat->cols; i++) {
			det += signal * matrix_get(mat, 0, i) *
				   algebra_mat_det(matrix_remove_col(aux, i));
			signal *= -1;
		}
	}

	matrix_free(&aux);
	return det;
}

struct matrix *algebra_mat_eigen_val(struct matrix *mat)
{
	if (mat->rows != mat->cols && mat->rows > 4)
		return NULL;

	if (mat->rows == 2) {
		real a = creal(matrix_get(mat, 0, 0));
		real b = creal(matrix_get(mat, 0, 1));
		real c = creal(matrix_get(mat, 1, 0));
		real d = creal(matrix_get(mat, 1, 1));
		return algebra_quadratic_roots(1.0,-a - d, a * d - b * c);
	} else if (mat->rows == 3) {
		real a = creal(matrix_get(mat, 0, 0));
		real b = creal(matrix_get(mat, 0, 1));
		real c = creal(matrix_get(mat, 0, 2));
		real d = creal(matrix_get(mat, 1, 0));
		real e = creal(matrix_get(mat, 1, 1));
		real f = creal(matrix_get(mat, 1, 2));
		real g = creal(matrix_get(mat, 2, 0));
		real h = creal(matrix_get(mat, 2, 1));
		real i = creal(matrix_get(mat, 2, 2));

		return algebra_cubic_roots(-1.0,
								  a + e + i,
								  -a * e - a * i - e * i +
								  f * h + b * d + c * g,
								  a * e * i - a * f * h -
								  b * d * i + b * g * f +
								  c * d * h - c * g * e);
	} else if (mat->rows == 4) {
		real a = creal(matrix_get(mat, 0, 0));
		real b = creal(matrix_get(mat, 0, 1));
		real c = creal(matrix_get(mat, 0, 2));
		real d = creal(matrix_get(mat, 0, 3));
		real e = creal(matrix_get(mat, 1, 0));
		real f = creal(matrix_get(mat, 1, 1));
		real g = creal(matrix_get(mat, 1, 2));
		real h = creal(matrix_get(mat, 1, 3));
		real i = creal(matrix_get(mat, 2, 0));
		real j = creal(matrix_get(mat, 2, 1));
		real k = creal(matrix_get(mat, 2, 2));
		real l = creal(matrix_get(mat, 2, 3));
		real m = creal(matrix_get(mat, 3, 0));
		real n = creal(matrix_get(mat, 3, 1));
		real o = creal(matrix_get(mat, 3, 2));
		real p = creal(matrix_get(mat, 3, 3));


		real coefc = a * (k + f + p) - (-p * (k + f) -
					 f * k + n * h + o * l + j * g) -
					 b * e +
					 c * (-i) -
					 d * m;
		real coefd = a * (-p * (k + f) - f * k + n * h + o * l + j * g) -
					 (p * f * k + g * l * n + h * j * o - n * h * k -
					 o * l * f - j * g * p) -
					 b * (-e * (k + p) + m * h + i * g) +
					 c * (-e * j - l * m + i * p + i * f) -
					 d * (-m * f - m * k + n * e + o * i);
		real coefe = a * (p * f * k + g * l * n + h * j * o - n * h * k -
					 o * l * f - j * g * p) -
					 b * (e * k * p + g * l * m + h * i * o -
					 m * k * h - o * l * e - p * i * g) +
					 c * (e * j * p + l * m * f + h * i * n -
					 m * j * h - n * l * e - i * p * f) -
					 d * (e * j * o + m * f * k + g * i * n - m * j * g -
					 n * k * e - o * i * f);

		return algebra_quartic_roots(1.0,
									-a - f - k - p,
									coefc,
									coefd,
									coefe);
	}

	return NULL;
}

struct matrix *algebra_mat_eigen_vec(struct matrix *mat, 
									  struct matrix *eig_val)
{
	struct matrix *mat_cpy;

	if (eig_val == NULL)
		return NULL;

	if (eig_val->rows != mat->rows || eig_val->rows != mat->cols)
		return NULL;

	struct matrix *eig_vec = matrix_new(mat->rows, eig_val->rows);

	if (eig_vec == NULL)
		return NULL;

	struct matrix *used_eig_val = matrix_new(1, 2);
	
	if(used_eig_val == NULL) {
		matrix_free(&eig_vec);
		return NULL;
	}

	matrix_set(used_eig_val, 0, 0, matrix_get(eig_val, 0, 0));
	matrix_set(used_eig_val, 0, 1, -1);

	real aux_flag;
	uint val_idx;

	struct matrix *red_mat;
	struct matrix *temp_vec;
	for (uint i = 0; i < eig_val->rows; i++) {
		mat_cpy = matrix_copy(mat);
		
		if (mat_cpy == NULL) {
			matrix_free(&used_eig_val);
			matrix_free(&eig_vec);
			return NULL;
		}

		for (uint k = 0; k < mat_cpy->rows; k++)
			matrix_set(mat_cpy, k, k,
						matrix_get(mat_cpy, k, k) -
						matrix_get(eig_val, i, 0));

		red_mat = algebra_gauss_elim_pivot(mat_cpy);

		if (red_mat == NULL) {
			matrix_free(&mat_cpy);
			matrix_free(&used_eig_val);
			matrix_free(&eig_vec);
			return NULL;
		}

		aux_flag = 1.0;

		for (uint k = 0; k < used_eig_val->rows; k++)
			if (matrix_get(used_eig_val, k, 0) == matrix_get(eig_val, i, 0)) {
				matrix_set(used_eig_val, k, 1, 
							matrix_get(used_eig_val, k, 1) + 1);
				aux_flag = 0.0;
				val_idx = k;
				break;
			}

		if (aux_flag) {
			if (!matrix_add_row(used_eig_val)) {
				matrix_free(&mat_cpy);
				matrix_free(&used_eig_val);
				matrix_free(&eig_vec);
				matrix_free(&red_mat);

				return NULL;
			}
			val_idx = used_eig_val->rows - 1;
			matrix_set(used_eig_val, val_idx, 0, matrix_get(eig_val, i , 0));
		}

		matrix_add_col(red_mat);
		for(uint j = red_mat->rows - 1; j > 0; j--) {
			if (matrix_get(red_mat, j, j) == 0.0) {
				matrix_set(red_mat, j, j, 1.0);
				if (j == red_mat->rows - 1 -
				    matrix_get(used_eig_val, val_idx, 1)) {
					matrix_set(red_mat, j, red_mat->rows, 1.0);
				} else {
					matrix_set(red_mat, j, red_mat->rows, 0.0);
				}
			}
		}

		
		temp_vec = algebra_gauss_elim(red_mat);

		if (temp_vec == NULL) {
			matrix_free(&mat_cpy);
			matrix_free(&used_eig_val);
			matrix_free(&eig_vec);
			matrix_free(&red_mat);
			return NULL;
		}

		for (uint j = 0; j < eig_vec->rows; j++)
			matrix_set(eig_vec, j, i, matrix_get(temp_vec, j, 0));

		matrix_free(&red_mat);
		matrix_free(&mat_cpy);
		matrix_free(&temp_vec);
	}

	matrix_free(&used_eig_val);

	return eig_vec;
}

