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
	
	struct cmatrix *roots = cmatrix_new(3, 1);

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
									 a * delta_0);
		}
	} else {
		real delta_1 = 2.0f * b * b * b - 
					   9.0f * a * b * c + 
					   27.0f * a * a * d;

		real p = -1.0f / (3.0f * a);
		cnum q = cpow((delta_1 + csqrt(-27.0f * a * a * delta)) / 2.0f,
					  1.0f / 3.0f);

		cnum ep = -0.5f + (0.5f * ALGEBRA_SQRT_3 * I);
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
	
	struct cmatrix *roots = cmatrix_new(4, 1);

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
// struct matrix *algebra_quartic_roots(real a, real b, real c, real d, real e)
// {
// 	if (a == 0.0f) {
// 		return NULL;
// 	}

// 	struct matrix *roots = matrix_new(4, 2);
	
// 	if (roots == NULL) {
// 		return NULL;
// 	}

// 	a = 1.0f;
// 	b = b / a;
// 	c = c / a;
// 	d = d / a;
// 	e = e / a;

// 	real delta = (256.0f * a * a * a * e * e * e) -
// 				 (192.0f * a * a * b * d * e * e) -
// 				 (128.0f * a * a * c * c * e * e) +
// 				 (144.0f * a * a * c * d * d * e) -
// 				 (27.0f * a * a * d * d * d * d) +
// 				 (144.0f * a * b * b * c * e * e) -
// 				 (6.0f * a * b * b * d * d * e) -
// 				 (80.0f * a * b * c * c * d * e) +
// 				 (18.0f * a * b * c * d * d * d) +
// 				 (16.0f * a * c * c * c * c e) -
// 				 (4.0f * a * c * c * c * d * d) -
// 				 (27.0f * b * b * b * b * e * e) +
// 				 (18.0f * b * b * b * c * d * e) -
// 				 (4.0f * b * b * b * d * d * d) -
// 				 (4.0f * b * b * c * c * c * e) +
// 				 (b * b * c * c * d * d);

// 	if (delta < 0.0f) {
// 		real f = c - (3 * b * b / 8.0f);
// 		real g = d + (b * b * b / 8.0f) - (b * c / 2.0f);
// 		real h = e - (3 * b * b * b * b / 256.0f) + (b * b * c / 16.0f) - (b * d / 4.0f);

// 		struct matrix *cubic_roots = algebra_cubic_roots(1.0f,
// 														 f / 2.0f,
// 														 ((f * f) - (4.0f * h)) / 16.0f,
// 														 b * d / 4.0f);

// 		if (cubic_roots == NULL) {
// 			matrix_free(roots);
// 			return NULL;
// 		}
// 		// check if it has 2 complex roots
// 		real y1, y2;
// 		real y_i1, y_i2;

// 		// if it has complex roots
// 		if (matrix_get(cubic_roots, 1, 1) != 0.0) {
// 			y1 = matrix_get(cubic_roots, 1, 0);
// 			y_i1 = matrix_get(cubic_roots, 1, 1);
// 			y2 = matrix_get(cubic_roots, 2, 0);
// 			y_i2 = matrix_get(cubic_roots, 2, 1);
// 		} else {
// 			y1 = matrix_get(cubic_roots, 0, 0) != 0.0f ? matrix_get(cubic_roots, 0, 0) : 0.0f;

// 			if (y1 == 0.0f) {
// 				y1 = matrix_get(cubic_roots, 1, 0);
// 				y2 = matrix_get(cubic_roots, 2, 0);
// 			} else {
// 				y2 = matrix_get(cubic_roots, 1, 0) != 0.0f ? matrix_get(cubic_roots, 1, 0) : matrix_get(cubic_roots, 2, 0);
// 			}

// 			real p = sqrt(y1);
// 			real q = sqrt(y2);
// 			real r = -g / (8.0f * p * q);
// 			real s = b / (4 * a);


// 			matrix_set(roots, 0, 0, p + q + r - s);
// 			matrix_set(roots, 0, 1, 0.0f);
// 			matrix_set(roots, 1, 0, -p - q + r - s);
// 			matrix_set(roots, 1, 1, 0.0f);

// 			matrix_set(roots)

// 			return 
// 		}

// 	} else if (delta == 0.0f) {

// 	} else {

// 	}
// 	// real f = c - (3.0f * b * b / 8.0f);
// 	// real g = d + (b * b * b / 8.0f) - (b * c / 2.0f);
// 	// real h = e - (3 * b * b * b * b / 256.0f) + (b * b * c / 16.0f) - (b * d / 4.0f);

// 	// struct matrix *cubic_roots = algebra_cubic_roots(1.0f,
// 	// 												 f / 2.0f,
// 	// 												 ((f * f) - (4 * h)) / 16.0f,
// 	// 												 - (g * g) / 64.0f
// 	// 												);

// 	// real s = b / (4.0f * a);


// 	// real p = c - 3.0f/ 8.0f * b * b;
// 	// real r = - b * b * b / 8.0f + (b * d) / 4.0f + e;
	
// 	// real p = 8.0f * a * c - 3 * b *b;
// 	// real r = b * b * b + 8.0f * d * a * a - 4 * a * b * c;

// 	// real delta_0 = c * c - 3 * b * d + 12.0f * a * e;
// 	// real dd = 64.0f * a * a *  a * e -
// 	// 		  16.0f * a * a * c * c +
// 	// 		  16.0f * a * b * b * c -
// 	// 		  16.0f * a * a * b * d -
// 	// 		  3 * b * b * b * b;



	

// 	// real bb = b*b;
// 	// real i = -3.0f * bb * 0.125f + c;
// 	// real j = bb * b * 0.125 - b * c * 0.5 + d;
// 	// real k = -3 * bb * bb / 256.0f + c * bb / 16.0f - b * d * 0.25 + e;

// 	// struct matrix *cubic_roots = algebra_cubic_roots(a,
// 	// 												 i + i,
// 	// 												 i * i - 4 * k,
// 	// 												 -(j*j));

// 	// if (cubic_roots == NULL) {
// 	// 	return NULL;
// 	// }

// 	// matrix_debug(cubic_roots, stdout);
// 	// real z = 1000000.0f;
// 	// if (matrix_get(cubic_roots, 0, 1) == 0.0f &&  matrix_get(cubic_roots, 0, 0) >= 0.0f)
// 	// 	z = matrix_get(cubic_roots, 0, 0);
	
// 	// if (matrix_get(cubic_roots, 1, 1) == 0.0f && matrix_get(cubic_roots, 1, 0) >= 0.0f)
// 	// 	if (matrix_get(cubic_roots, 1, 0) < z)
// 	// 		z = matrix_get(cubic_roots, 1, 0);

// 	// if (matrix_get(cubic_roots, 2, 1) == 0.0f && matrix_get(cubic_roots, 2, 0) >= 0.0f)
// 	// 	if (matrix_get(cubic_roots, 1, 0) < z)
// 	// 		z = matrix_get(cubic_roots, 1, 0);

// 	// real val = z * z * z + z * z * (i + i) + z * (i* i - 4 * k) - j * j;

// 	// real p = sqrt(z);
// 	// real r = -p;
// 	// real q = (i + z - j/p) / 2.0f;
// 	// real s = (i + z + j/p) / 2.0f;

// 	// struct matrix *quadratic_roots = 

// 	return NULL;
// 	// real f = c - (3.0f *b * b / 8.0f);
// 	// real g = d + (b * b * b / 8.0f) - (b * c / 2.0f);
// 	// real h = e - (3 * b * b * b * b / 256.0f) + ((b * b * c / 16.0f) - (b * d / 4.0f));


// 	// 

// 	// if ()
// }

// // 3rd degree
// // 4th degree

// // struct matrix *algebra_eigen_val(struct matrix *mat)
// // {
// // 	if (matrix->rows != matrix->cols) {
// // 		return NULL;
// // 	}

// // 	switch (matrix->rows) {
// // 		case 2:

// // 			break;
// // 		default:
// // 			return NULL;
// // 	}
// // }
