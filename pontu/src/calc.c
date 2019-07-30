#include "../include/calc.h"

uint calc_factorial(uint n)
{
	uint result = 1;

	for (uint c = 1; c <= n; c++)
		result *= c;

	return result;
}

int calc_combination(int n, int p)
{
	return calc_factorial(n) / (calc_factorial(p) * calc_factorial(n - p));
}

int calc_kronecker(int i, int j)
{
	return i == j ? 1 : 0;
}

real calc_squared_length3(real x, real y, real z)
{
	return ((x * x) + (y * y) + (z * z));
}

real calc_length3(real x, real y, real z)
{
	return sqrt((x * x) + (y * y) + (z * z));
}

real calc_squared_length2(real x, real y)
{
	return ((x * x) + (y * y));
}

real calc_length2(real x, real y)
{
	return sqrt((x * x) + (y * y));
}

real calc_randr(real max)
{
	return (real) rand() / (real) (RAND_MAX / max);
}

real calc_max3(real a, real b, real c)
{
	real t = a;

	if (b >= t)
		t = b;

	if (c >= t)
		t = c;

	return t;
}

real calc_sign(real a)
{
	return (a >= 0.0f) ? 1.0f : -1.0f;
}

real calc_logt(real a)
{
	return -1.0f * calc_sign(a) * log(fabs(a));
}

real calc_gaussian(real x, real s)
{
	real num = exp((-1.0f * x * x) / (2.0f * s * s));
	real den = s * sqrt(2.0f * CALC_PI);
	
	return num / den;
}

real calc_gaussian3(real x, real y, real z, real s)
{
	real num = exp(-1.0f * ((x * x) + (y * y) + (z * z)) / (2.0f * s * s));
	real den = pow(2.0f * CALC_PI * s * s, 1.5f);
	
	return num / den;
}

