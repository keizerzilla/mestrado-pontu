#include "../include/vector3.h"

struct vector3 *vector3_new(real x, real y, real z)
{
	struct vector3 *v = malloc(sizeof(struct vector3));
	if (v == NULL)
		return NULL;

	v->x = x;
	v->y = y;
	v->z = z;

	return v;
}

struct vector3 *vector3_zero()
{
	return vector3_new(0.0f, 0.0f, 0.0f);
}

struct vector3 *vector3_from_vector(struct vector3 *src)
{
	return vector3_new(src->x, src->y, src->z);
}

void vector3_copy(struct vector3 *dst, struct vector3 *src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

void vector3_free(struct vector3 *v)
{
	if (v != NULL) {
		free(v);
		v = NULL;
	}
}

struct vector3 *vector3_add(struct vector3 *a, struct vector3 *b)
{
	return vector3_new(a->x + b->x, a->y + b->y, a->z + b->z);
}

void vector3_increase(struct vector3 *src, struct vector3 *inc)
{
	src->x += inc->x;
	src->y += inc->y;
	src->z += inc->z;
}

struct vector3 *vector3_sub(struct vector3 *a, struct vector3 *b)
{
	return vector3_new(a->x - b->x, a->y - b->y, a->z - b->z);
}

void vector3_pow(struct vector3 *p, real n)
{
	p->x = pow(p->x, n);
	p->y = pow(p->y, n);
	p->z = pow(p->z, n);
}

void vector3_decrease(struct vector3 *src, struct vector3 *dec)
{
	src->x -= dec->x;
	src->y -= dec->y;
	src->z -= dec->z;
}

void vector3_set(struct vector3 *v, real x, real y, real z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

void vector3_invert(struct vector3 *v)
{
	v->x = -1 * v->x;
	v->y = -1 * v->y;
	v->z = -1 * v->z;
}

real vector3_distance(struct vector3 *a, struct vector3 *b)
{
	return calc_length3(a->x - b->x, a->y - b->y, a->z - b->z);
}

real vector3_squared_distance(struct vector3 *a, struct vector3 *b)
{
	return calc_squared_length3(a->x - b->x, a->y - b->y, a->z - b->z);
}

real vector3_manhattan(struct vector3 *a, struct vector3 *b)
{
	return fabs(a->x - b->x) + fabs(a->y - b->y) + fabs(a->z - b->z);
}

real vector3_squared_length(struct vector3 *v)
{
	return calc_squared_length3(v->x, v->y, v->z);
}

real vector3_length(struct vector3 *v)
{
	return calc_length3(v->x, v->y, v->z);
}

real vector3_squared_length2(struct vector3 *v)
{
	return calc_squared_length2(v->x, v->y);
}

real vector3_length2(struct vector3 *v)
{
	return calc_length2(v->x, v->y);
}

void vector3_setlength(struct vector3 *v, real l)
{
	real mag = calc_length3(v->x, v->y, v->z);

	if (mag > 0.0f) {
		mag = l / mag;

		v->x *= mag;
		v->y *= mag;
		v->z *= mag;
	}
}

struct vector3 *vector3_unit(struct vector3 *v)
{
	struct vector3 *unit = vector3_zero();
	real l = vector3_length(v);

	unit->x = v->x / l;
	unit->y = v->y / l;
	unit->z = v->z / l;

	return unit;
}

void vector3_normalize(struct vector3 *v)
{
	real l = vector3_length(v);

	v->x = v->x / l;
	v->y = v->y / l;
	v->z = v->z / l;
}

void vector3_scale(struct vector3 *v, real f)
{
	v->x *= f;
	v->y *= f;
	v->z *= f;
}

void vector3_rotate_x(struct vector3 *v, real d)
{
	real rad = d * CALC_DEG2RAD;
	real rsin = sin(rad);
	real rcos = cos(rad);

	v->y = (v->y * rcos) - (v->z * rsin);
	v->z = (v->y * rsin) + (v->z * rcos);
}

void vector3_rotate_y(struct vector3 *v, real d)
{
	real rad = d * CALC_DEG2RAD;
	real rsin = sin(rad);
	real rcos = cos(rad);

	v->x = (v->x * rcos) + (v->z * rsin);
	v->z = (v->z * rcos) - (v->x * rsin);
}

void vector3_rotate_z(struct vector3 *v, real d)
{
	real rad = d * CALC_DEG2RAD;
	real rsin = sin(rad);
	real rcos = cos(rad);

	v->x = (v->x * rcos) - (v->y * rsin);
	v->y = (v->x * rsin) + (v->y * rcos);
}

real vector3_dot(struct vector3 *a, struct vector3 *b)
{
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

real vector3_angle(struct vector3 *a, struct vector3 *b)
{
	return acos(vector3_dot(a, b) / vector3_length(a) * vector3_length(b));
}

struct vector3 *vector3_cross(struct vector3 *a, struct vector3 *b)
{
	return vector3_new((a->y * b->z) - (a->z * b->y),
			           (a->z * b->x) - (a->x * b->z),
			           (a->x * b->y) - (a->y * b->x));
}

real vector3_cross2(struct vector3 *a, struct vector3 *b)
{
	return (a->x * b->y) - (b->x * a->y);
}

void vector3_push2(struct vector3 *v)
{
	v->z = 0.0f;
}

real vector3_minkowski(struct vector3 *v1, struct vector3 *v2, real m)
{
	return pow(pow(v1->x - v2->x, m) + pow(v1->y - v2->y, m), 1.0f / m);
}

real vector3_chebyshev(struct vector3 *v1, struct vector3 *v2)
{
	real fx = fabs(v1->x - v2->x);
	real fy = fabs(v1->y - v2->y);
	real fz = fabs(v1->z - v2->z);

	if (fx > fy)
		if (fx > fz)
			return fx;
		else
			return fz;
	else if (fy > fz)
		return fy;
	else
		return fz;
}

real vector3_modsse(struct vector3 *v1, struct vector3 *v2)
{
	real d = calc_squared_length3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
	real n1 = pow(v1->x, 2) + pow(v1->y, 2) + pow(v1->z, 2);
	real n2 = pow(v2->x, 2) + pow(v2->y, 2) + pow(v2->z, 2);

	return d / (n1 * n2);
}

real vector3_cosdistance(struct vector3 *v1, struct vector3 *v2)
{
	return -1.0f * vector3_angle(v1, v2);
}

real vector3_mse(struct vector3 *v1, struct vector3 *v2)
{
	real d = calc_squared_length3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
	
	return d / 3.0f;
}

real vector3_sse(struct vector3 *v1, struct vector3 *v2)
{
	return calc_squared_length3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
}

real vector3_chi_distance(struct vector3 *v1, struct vector3 *v2)
{
	real n = calc_squared_length3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
	real d = v1->x + v2->x + v1->y + v2->y + v1->z + v2->z;

	return n / d;
}

real vector3_canberra(struct vector3 *v1, struct vector3 *v2)
{
	real n = fabs(v1->x - v2->x) +
	         fabs(v1->y - v2->y) +
	         fabs(v1->z - v2->z);
	real d = fabs(v1->x) + fabs(v2->x) +
	         fabs(v1->y) + fabs(v2->y) +
	         fabs(v1->z) + fabs(v2->z);

	return n / d;
}

real vector3_modmanhattan(struct vector3 *v1, struct vector3 *v2)
{
	real n = fabs(v1->x - v2->x) + fabs(v1->y - v2->y) + fabs(v1->z - v2->z);
	real d1 = fabs(v1->x) + fabs(v1->y) + fabs(v1->z);
	real d2 = fabs(v2->x) + fabs(v2->y) + fabs(v2->z);

	return n / (d1 * d2);
}

void vector3_lerp(struct vector3 *s, struct vector3 *t, real v)
{
	s->x = s->x + ((t->x - s->x) * v);
	s->y = s->y + ((t->y - s->y) * v);
	s->z = s->z + ((t->z - s->z) * v);
}

real vector3_volume(struct vector3 *v)
{
	return v->x * v->y * v->z;
}

struct vector3 *vector3_projection(struct vector3 *a, struct vector3 *b)
{
	struct vector3 *p = vector3_new(b->x, b->y, b->z);

	real factor = vector3_dot(a, b) / vector3_length(b);
	vector3_scale(p, factor);

	return p;
}

struct vector3 *vector3_reflection(struct vector3 *v, struct vector3 *n)
{
	struct vector3 *r = vector3_zero();
	
	vector3_copy(n, r);
	vector3_scale(r, 2 * vector3_dot(v, n));
	
	r = vector3_sub(v, r);

	return r;
}

struct vector3 *vector3_normal(struct vector3 *a,
                               struct vector3 *b,
                               struct vector3 *c)
{
	struct vector3 *v1 = vector3_sub(b, a);
	struct vector3 *v2 = vector3_sub(c, a);
	struct vector3 *ret = vector3_cross(v1, v2);

	vector3_free(v1);
	vector3_free(v2);

	return ret;
}

real vector3_area(struct vector3 *a, struct vector3 *b, struct vector3 *c)
{
	struct vector3 *n = vector3_normal(a, b, c);
	real ret = 0.5f * vector3_length(n);
	
	vector3_free(n);

	return ret;
}

real vector3_menger_curvature(struct vector3 *a,
			                  struct vector3 *b,
			                  struct vector3 *c)
{
	struct vector3 *s1 = vector3_sub(a, b);
	struct vector3 *s2 = vector3_sub(b, c);
	struct vector3 *s3 = vector3_sub(c, a);

	real num = 4 * vector3_area(a, b, c);
	real den = vector3_length(s1) * vector3_length(s2) * vector3_length(s3);

	vector3_free(s1);
	vector3_free(s2);
	vector3_free(s3);

	return num / den;
}

struct vector3 *vector3_average(struct vector3 *a, struct vector3 *b)
{
	struct vector3 *avg = vector3_zero();

	avg->x = (a->x + b->x) / 2;
	avg->y = (a->y + b->y) / 2;
	avg->z = (a->z + b->z) / 2;

	return avg;
}

void vector3_debug(struct vector3 *v, FILE * output)
{
	if (v == NULL) {
		fprintf(output, "!!! vector3 empty !!!\n");
		return;
	}

	fprintf(output, "%.4f %.4f %.4f\n", v->x, v->y, v->z);
}

