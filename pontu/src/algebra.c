#include "../include/algebra.h"

struct cmatrix *algebra_affine_root(real a, real b)
{
	if (a == 0.0f)
		return NULL;

	struct cmatrix *root = cmatrix_new(1, 1);
	if (root == NULL)
		return NULL;
	
	cmatrix_set(root, 0, 0, -b / a);

	return root;
}

struct cmatrix *algebra_quadratic_roots(real a, real b, real c)
{
	if (a == 0.0f)
		return algebra_affine_root(b, c);

	struct cmatrix *roots = cmatrix_new(2, 1);
	if (roots == NULL)
		return NULL;

	real delta = (b * b) - (4.0f * a *c);

	cmatrix_set(roots, 0, 0, (-b + csqrt(delta)) / (2 * a));
	cmatrix_set(roots, 1, 0, delta == 0.0f ? cmatrix_get(roots, 0, 0) : (-b - csqrt(delta)) / (2 * a));

	if (delta >= 0) {
		cmatrix_set(roots, 0, 0, creal(cmatrix_get(roots, 0, 0)));
		cmatrix_set(roots, 1, 0, creal(cmatrix_get(roots, 1, 0)));
	}
	return roots;
}

struct cmatrix *algebra_cubic_roots(real a, real b, real c, real d)
{
	if (a == 0.0f)
		return algebra_quadratic_roots(b, c, d);
	
	struct cmatrix *roots;

	if (b != 0.0f && c != 0.0f && d == 0.0f) {
		roots = algebra_quadratic_roots(a, b, c);
		cmatrix_add_row(roots);
		return roots;
	}

	roots = cmatrix_new(3, 1);

	if (roots == NULL)
		return NULL;

	real delta = 18.0f * a * b * c * d -
				 4.0f * b * b * b * d +
				 b * b * c * c -
				 4.0f * a * c * c * c -
				 27.0f * a * a * d * d;

	real delta_0 = b * b - 3.0f * a * c;

	if (delta == 0.0f) {
		if (delta_0 == 0.0f) {
			cmatrix_set(roots, 0, 0, -b / (3.0f * a));
			cmatrix_set(roots, 1, 0, cmatrix_get(roots, 0, 0));
			cmatrix_set(roots, 2, 0, cmatrix_get(roots, 0, 0));
		} else {
			cmatrix_set(roots, 0, 0, (9.0f * a * d - b * c) / (2.0f * delta_0));
			cmatrix_set(roots, 1, 0, cmatrix_get(roots, 0, 0));
			cmatrix_set(roots, 2, 0, (4.0f * a * b * c -
									 9.0f * a * a * d -
									 b * b * b) /
									 (a * delta_0));
		}
	} else {
		real delta_1 = 2.0f * b * b * b - 
					   9.0f * a * b * c + 
					   27.0f * a * a * d;

		real p = -1.0f / (3.0f * a);
		cnum q = cpow((delta_1 + csqrt(-27.0f * a * a * delta)) / 2.0f,
					  1.0f / 3.0f);

		cnum ep = -0.5f + (0.5f * CALC_SQRT_3 * I);
		cnum r = ep * q;
		cnum s = delta_0 / r;
		cmatrix_set(roots, 0, 0, p * (b + q + delta_0/q));
		cmatrix_set(roots, 1, 0, p * (b + r + s));
		r *= ep;
		s /= ep;
		cmatrix_set(roots, 2, 0, p * (b + r + s));

		if (delta > 0.0f) {
			cmatrix_set(roots, 0, 0, creal(cmatrix_get(roots, 0, 0)));
			cmatrix_set(roots, 1, 0, creal(cmatrix_get(roots, 1, 0)));
			cmatrix_set(roots, 2, 0, creal(cmatrix_get(roots, 2, 0)));
		}
	}

	return roots;
}

struct cmatrix *algebra_quartic_roots(real a, real b, real c, real d, real e)
{
	if (a == 0.0f)
		return algebra_cubic_roots(b, c, d, e);
	
	struct cmatrix *roots;

	if (b != 0.0f && c == d  && d == e && e == 0.0f) {
		roots = algebra_affine_root(a, b);

		cmatrix_add_row(roots);
		cmatrix_add_row(roots);
		cmatrix_add_row(roots);

		return roots;
	}

	if (b != 0.0f && c != 0.0f && d == e && e == 0.0f) {
		roots = algebra_quadratic_roots(a, b, c);
		
		cmatrix_add_row(roots);
		cmatrix_add_row(roots);

		return roots;
	}

	if (b != 0.0f && c != 0.0f && d != 0.0f && e == 0.0f) {
		roots = algebra_cubic_roots(a, b, c, d);
		
		cmatrix_add_row(roots);

		return roots;
	}

	roots = cmatrix_new(4, 1);
	if (roots == NULL)
		return NULL;

	real delta = (256.0f * a * a * a * e * e * e) -
				 (192.0f * a * a * b * d * e * e) -
				 (128.0f * a * a * c * c * e * e) +
				 (144.0f * a * a * c * d * d * e) -
				 (27.0f * a * a * d * d * d * d) +
				 (144.0f * a * b * b * c * e * e) -
				 (6.0f * a * b * b * d * d * e) -
				 (80.0f * a * b * c * c * d * e) +
				 (18.0f * a * b * c * d * d * d) +
				 (16.0f * a * c * c * c * c * e) -
				 (4.0f * a * c * c * c * d * d) -
				 (27.0f * b * b * b * b * e * e) +
				 (18.0f * b * b * b * c * d * e) -
				 (4.0f * b * b * b * d * d * d) -
				 (4.0f * b * b * c * c * c * e) +
				 (b * b * c * c * d * d);

	real delta_0 = c * c - 3.0f * b * d + 12.0f * a * e;

	real delta_1 = 2.0f * c * c * c -
				   9.0f * b * c * d +
				   27.0f * b * b * e +
				   27.0f * a * d * d -
				   72.0f * a * c * e;
	
	real p = (8.0f * a * c - 3.0f * b * b) / (8.0f * a * a);

	real q = (b * b * b - 4.0f * a * b * c + 8.0f * a * a * d) /
			 (8.0f * a * a * a);

	cnum r = cpow((delta_1 + csqrt(-27.0f * delta)) / 2.0f, 1.0f / 3.0f);

	cnum s = 0.5f * csqrt(-2.0f / 3.0f * p + (r + delta_0 / r) / (3.0f * a));

	real t = -b / (4.0f * a);
	cnum u = -4 * s * s - 2 * p;
	cnum v = 0.5f * csqrt(u + q / s);

	real f = p * 8.0f * a * a;
	real g = q * 8.0f * a * a * a;
	real h = 64.0f * a * a * a * e -
			 16.0f * a * a * c * c +
			 16.0f * a * b * b * c -
			 16.0f * a * a * b * d -
			 3.0f * b * b * b * b;

	cmatrix_set(roots, 0, 0, t - s + v);
	cmatrix_set(roots, 1, 0, t - s - v);

	v = 0.5f * csqrt(u - q / s);

	cmatrix_set(roots, 2, 0, t + s + v);
	cmatrix_set(roots, 3, 0, t + s - v);

	real x0, x1, x2, x3;

	if (delta < 0.0f) {
		x0 = fabs(cimag(cmatrix_get(roots, 0, 0)));
		x1 = fabs(cimag(cmatrix_get(roots, 1, 0)));
		x2 = fabs(cimag(cmatrix_get(roots, 2, 0)));
		x3 = fabs(cimag(cmatrix_get(roots, 3, 0)));

		
		if (x0 <= x1 && x0 <= x2 && x0 <= x3) {
			cmatrix_set(roots, 0, 0, creal(cmatrix_get(roots, 0, 0)));
		}

		if (x1 <= x0 && x1 <= x2 && x1 <= x3) {
			cmatrix_set(roots, 1, 0, creal(cmatrix_get(roots, 1, 0)));
		}

		if (x2 <= x1 && x2 <= x0 && x2 <= x3) {
			cmatrix_set(roots, 2, 0, creal(cmatrix_get(roots, 2, 0)));
		}

		if (x3 <= x1 && x3 <= x2 && x3 <= x0) {
			cmatrix_set(roots, 3, 0, creal(cmatrix_get(roots, 3, 0)));
		}
		
	} else if (delta > 0.0f) {
		if (f < 0.0f && h < 0.0f) {
			cmatrix_set(roots, 0, 0, creal(cmatrix_get(roots, 0, 0)));
			cmatrix_set(roots, 1, 0, creal(cmatrix_get(roots, 1, 0)));
			cmatrix_set(roots, 2, 0, creal(cmatrix_get(roots, 2, 0)));
			cmatrix_set(roots, 3, 0, creal(cmatrix_get(roots, 3, 0)));
		}
	} else {
		if ((f < 0.0f && h < 0.0f && delta_0 != 0.0f) ||
		   (h != 0.0f && delta_0 == 0.0f) ||
		   (h == 0.0f && f < 0.0f) ||
		   (h == 0.0f && delta_0 == 0.0f)) {
			cmatrix_set(roots, 0, 0, creal(cmatrix_get(roots, 0, 0)));
			cmatrix_set(roots, 1, 0, creal(cmatrix_get(roots, 1, 0)));
			cmatrix_set(roots, 2, 0, creal(cmatrix_get(roots, 2, 0)));
			cmatrix_set(roots, 3, 0, creal(cmatrix_get(roots, 3, 0)));
		} else if (h > 0.0f || (f > 0.0f && (h != 0.0f || g != 0.0f))) {
			x0 = fabs(cimag(cmatrix_get(roots, 0, 0)));
			x1 = fabs(cimag(cmatrix_get(roots, 1, 0)));
			x2 = fabs(cimag(cmatrix_get(roots, 2, 0)));
			x3 = fabs(cimag(cmatrix_get(roots, 3, 0)));

			if (x0 <= x1 && x0 <= x2 && x0 <= x3) {
				cmatrix_set(roots, 0, 0, creal(cmatrix_get(roots, 0, 0)));
			}

			if (x1 <= x0 && x1 <= x2 && x1 <= x3) {
				cmatrix_set(roots, 1, 0, creal(cmatrix_get(roots, 1, 0)));
			}

			if (x2 <= x1 && x2 <= x0 && x2 <= x3) {
				cmatrix_set(roots, 2, 0, creal(cmatrix_get(roots, 2, 0)));
			}

			if (x3 <= x1 && x3 <= x2 && x3 <= x0) {
				cmatrix_set(roots, 3, 0, creal(cmatrix_get(roots, 3, 0)));
			}
		}
	}

	return roots;	
}
struct cmatrix *algebra_gauss_elim_pivot(struct cmatrix *mat)
{
	if (mat->rows != mat->cols)
		return NULL;

	struct cmatrix *eq_sys = cmatrix_copy(mat);

	if (eq_sys == NULL)
		return NULL;

	cnum temp, diag_el;

	for (uint i = 0; i < (eq_sys->rows - 1); i++) {
		for (uint k = i + 1; k < eq_sys->rows; k++)
			if (cabs(cmatrix_get(eq_sys, i, i)) <
				cabs(cmatrix_get(eq_sys, k, i)))
				for (uint j = 0; j < eq_sys->cols; j++) {
					temp = cmatrix_get(eq_sys, i, j);
					cmatrix_set(eq_sys, i, j, cmatrix_get(eq_sys, k, j));
					cmatrix_set(eq_sys, k, j, temp);
				}
		for (uint k = i + 1; k < eq_sys->rows; k++) {
			if (cmatrix_get(eq_sys, i, i) != 0.0f &&
				cmatrix_get(eq_sys, k, i) != 0.0f) {
				temp = cmatrix_get(eq_sys, k, i) / cmatrix_get(eq_sys, i, i);
				for (uint j = 0; j < eq_sys->cols; j++)
					cmatrix_set(eq_sys, k, j,
								cmatrix_get(eq_sys, k, j) - temp *
								cmatrix_get(eq_sys, i, j));
			}
			
		}
	}

	for (uint i = 0; i < eq_sys->rows; i++)
		for (uint j = 0; j < eq_sys->rows; j++)
			if (j < i)
				cmatrix_set(eq_sys, i, j, 0.0f);

	for (uint i = 0; i < eq_sys->rows; i++){
		diag_el = cmatrix_get(eq_sys, i, i);
		if ((cabs(diag_el) < ALGEBRA_NUM_ERR))
			cmatrix_set(eq_sys, i, i, 0.0f);
		else
			for (uint j = 0; j < eq_sys->cols; j++)
				if (diag_el != 0.0f)
					cmatrix_set(eq_sys, i, j,
								cmatrix_get(eq_sys, i, j) / diag_el);
	}

	return eq_sys;
}

struct cmatrix *algebra_gauss_elim(struct cmatrix *mat)
{
	if (mat->rows + 1 != mat->cols)
		return NULL;

	struct cmatrix *eq_sys = cmatrix_copy(mat);

	if (eq_sys == NULL)
		return NULL;

	struct cmatrix *solution = cmatrix_new(mat->rows, 1);

	if (solution == NULL) {
		cmatrix_free(eq_sys);
		return NULL;
	}

	cnum temp;

	for (uint i = 0; i < (eq_sys->rows - 1); i++) {
		for (uint k = i + 1; k < eq_sys->rows; k++)
			if (cabs(cmatrix_get(eq_sys, i, i)) <
				cabs(cmatrix_get(eq_sys, k, i)))
				for (uint j = 0; j < eq_sys->cols; j++) {
					temp = cmatrix_get(eq_sys, i, j);
					cmatrix_set(eq_sys, i, j, cmatrix_get(eq_sys, k, j));
					cmatrix_set(eq_sys, k, j, temp);
				}
		for (uint k = i + 1; k < eq_sys->rows; k++) {
			temp = cmatrix_get(eq_sys, k, i) / cmatrix_get(eq_sys, i, i);
			for (uint j = 0; j < eq_sys->cols; j++)
				cmatrix_set(eq_sys, k, j,
							cmatrix_get(eq_sys, k, j) - temp *
							cmatrix_get(eq_sys, i, j));
		}
	}

	for (int l = (eq_sys->rows - 1); l >= 0; l--) {
		cmatrix_set(solution, l, 0, cmatrix_get(eq_sys, l, eq_sys->cols - 1));
		for (uint j = l + 1; j < (eq_sys->cols - 1); j++)
			cmatrix_set(solution, l, 0, 
						cmatrix_get(solution, l, 0) -
						cmatrix_get(eq_sys, l, j) *
						cmatrix_get(solution, j, 0));
		cmatrix_set(solution, l, 0, 
					cmatrix_get(solution, l, 0) / cmatrix_get(eq_sys, l, l));
	}

	cmatrix_free(eq_sys);
	eq_sys = NULL;

	return solution;
}

struct cmatrix *algebra_mat_sum(struct cmatrix *a, struct cmatrix *b)
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

struct cmatrix *algebra_mat_sub(struct cmatrix *a, struct cmatrix *b)
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

struct cmatrix *algebra_mat_prod(struct cmatrix *a, struct cmatrix *b)
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

struct cmatrix *algebra_mat_vs_scalar(struct cmatrix *mat, cnum scalar)
{
    struct cmatrix *nmat = cmatrix_new(mat->rows, mat->cols);
    
    if (nmat == NULL)
        return NULL;

    for (uint i = 0; i < mat->rows; i++)
        for (uint j = 0; j < mat->cols; j++)
            cmatrix_set(nmat, i, j, cmatrix_get(mat, i, j) * scalar);

    return nmat;
}

struct cmatrix *algebra_mat_transpose(struct cmatrix *mat)
{
    struct cmatrix *mat_trans = cmatrix_new(mat->cols, mat->rows);

    if (mat_trans == NULL)
        return NULL;

    for (uint i = 0; i < mat->rows; i++)
        for (uint j = 0; j < mat->cols; j++)
            cmatrix_set(mat_trans, j, i, cmatrix_get(mat, i, j));

    return mat_trans;
}

cnum algebra_mat_trace(struct cmatrix *mat)
{
	if (mat->rows != mat->cols) {
		return 0.0f;
	}

	cnum trace = 0.0f;
	for (uint i = 0; i < mat->rows; i++) {
		trace += cmatrix_get(mat, i, i);
	}

	return trace;
}

cnum algebra_mat_det(struct cmatrix *mat)
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
				   algebra_mat_det(cmatrix_remove_col(aux, i));
			signal *= -1;
		}
	}

	cmatrix_free(aux);
	return det;
}

struct cmatrix *algebra_mat_eigen_val(struct cmatrix *mat)
{
	if (mat->rows != mat->cols && mat->rows > 4)
		return NULL;

	if (mat->rows == 2) {
		real a = creal(cmatrix_get(mat, 0, 0));
		real b = creal(cmatrix_get(mat, 0, 1));
		real c = creal(cmatrix_get(mat, 1, 0));
		real d = creal(cmatrix_get(mat, 1, 1));
		return algebra_quadratic_roots(1.0f,-a - d, a * d - b * c);
	} else if (mat->rows == 3) {
		real a = creal(cmatrix_get(mat, 0, 0));
		real b = creal(cmatrix_get(mat, 0, 1));
		real c = creal(cmatrix_get(mat, 0, 2));
		real d = creal(cmatrix_get(mat, 1, 0));
		real e = creal(cmatrix_get(mat, 1, 1));
		real f = creal(cmatrix_get(mat, 1, 2));
		real g = creal(cmatrix_get(mat, 2, 0));
		real h = creal(cmatrix_get(mat, 2, 1));
		real i = creal(cmatrix_get(mat, 2, 2));

		return algebra_cubic_roots(-1.0f,
								  a + e + i,
								  -a * e - a * i - e * i +
								  f * h + b * d + c * g,
								  a * e * i - a * f * h -
								  b * d * i + b * g * f +
								  c * d * h - c * g * e);
	} else if (mat->rows == 4) {
		real a = creal(cmatrix_get(mat, 0, 0));
		real b = creal(cmatrix_get(mat, 0, 1));
		real c = creal(cmatrix_get(mat, 0, 2));
		real d = creal(cmatrix_get(mat, 0, 3));
		real e = creal(cmatrix_get(mat, 1, 0));
		real f = creal(cmatrix_get(mat, 1, 1));
		real g = creal(cmatrix_get(mat, 1, 2));
		real h = creal(cmatrix_get(mat, 1, 3));
		real i = creal(cmatrix_get(mat, 2, 0));
		real j = creal(cmatrix_get(mat, 2, 1));
		real k = creal(cmatrix_get(mat, 2, 2));
		real l = creal(cmatrix_get(mat, 2, 3));
		real m = creal(cmatrix_get(mat, 3, 0));
		real n = creal(cmatrix_get(mat, 3, 1));
		real o = creal(cmatrix_get(mat, 3, 2));
		real p = creal(cmatrix_get(mat, 3, 3));


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

		return algebra_quartic_roots(1.0f,
									-a - f - k - p,
									coefc,
									coefd,
									coefe);
	}

	return NULL;
}

struct cmatrix *algebra_mat_eigen_vec(struct cmatrix *mat, 
									  struct cmatrix *eig_val)
{
	struct cmatrix *mat_cpy;

	if (eig_val == NULL)
		return NULL;

	if (eig_val->rows != mat->rows || eig_val->rows != mat->cols)
		return NULL;

	struct cmatrix *eig_vec = cmatrix_new(mat->rows, eig_val->rows);

	if (eig_vec == NULL)
		return NULL;

	struct cmatrix *used_eig_val = cmatrix_new(1, 2);
	
	if(used_eig_val == NULL) {
		cmatrix_free(eig_vec);
		return NULL;
	}

	cmatrix_set(used_eig_val, 0, 0, cmatrix_get(eig_val, 0, 0));
	cmatrix_set(used_eig_val, 0, 1, -1);

	real aux_flag;
	uint val_idx;

	struct cmatrix *red_mat;
	struct cmatrix *temp_vec;
	for (uint i = 0; i < eig_val->rows; i++) {
		mat_cpy = cmatrix_copy(mat);
		
		if (mat_cpy == NULL) {
			cmatrix_free(used_eig_val);
			cmatrix_free(eig_vec);
			return NULL;
		}

		for (uint k = 0; k < mat_cpy->rows; k++)
			cmatrix_set(mat_cpy, k, k,
						cmatrix_get(mat_cpy, k, k) -
						cmatrix_get(eig_val, i, 0));

		red_mat = algebra_gauss_elim_pivot(mat_cpy);

		if (red_mat == NULL) {
			cmatrix_free(mat_cpy);
			cmatrix_free(used_eig_val);
			cmatrix_free(eig_vec);
			return NULL;
		}

		aux_flag = 1.0f;

		for (uint k = 0; k < used_eig_val->rows; k++)
			if (cmatrix_get(used_eig_val, k, 0) == cmatrix_get(eig_val, i, 0)) {
				cmatrix_set(used_eig_val, k, 1, 
							cmatrix_get(used_eig_val, k, 1) + 1);
				aux_flag = 0.0f;
				val_idx = k;
				break;
			}

		if (aux_flag) {
			if (!cmatrix_add_row(used_eig_val)) {
				cmatrix_free(mat_cpy);
				cmatrix_free(used_eig_val);
				cmatrix_free(eig_vec);
				cmatrix_free(red_mat);

				return NULL;
			}
			val_idx = used_eig_val->rows - 1;
			cmatrix_set(used_eig_val, val_idx, 0, cmatrix_get(eig_val, i , 0));
		}

		cmatrix_add_col(red_mat);
		for(uint j = red_mat->rows - 1; j > 0; j--) {
			if (cmatrix_get(red_mat, j, j) == 0.0f) {
				cmatrix_set(red_mat, j, j, 1.0f);
				if (j == red_mat->rows - 1 -
				    cmatrix_get(used_eig_val, val_idx, 1)) {
					cmatrix_set(red_mat, j, red_mat->rows, 1.0f);
				} else {
					cmatrix_set(red_mat, j, red_mat->rows, 0.0f);
				}
			}
		}

		
		temp_vec = algebra_gauss_elim(red_mat);

		if (temp_vec == NULL) {
			cmatrix_free(mat_cpy);
			cmatrix_free(used_eig_val);
			cmatrix_free(eig_vec);
			cmatrix_free(red_mat);
			return NULL;
		}

		for (uint j = 0; j < eig_vec->rows; j++)
			cmatrix_set(eig_vec, j, i, cmatrix_get(temp_vec, j, 0));

		cmatrix_free(red_mat);
		cmatrix_free(mat_cpy);
		cmatrix_free(temp_vec);
	}

	cmatrix_free(used_eig_val);

	return eig_vec;
}
