#include "../calc.h"

int calc_factorial (int n)
{
    int result = 1;
    
    for (uint c = 1; c <= n; c++)
        result *= c;

    return result;
}

int calc_combination (int n, int p)
{
	return calc_factorial(n) / (calc_factorial(p) * calc_factorial(n - p));
}

int calc_kronecker (int i, int j)
{
	return i == j ? 1 : 0;
}

real calc_squared_length3 (real x, real y, real z)
{
    return ((x * x) + (y * y) + (z * z));
}

real calc_length3 (real x, real y, real z)
{
    return sqrt((x * x) + (y * y) + (z * z));
}

real calc_squared_length2 (real x, real y)
{
    return ((x * x) + (y * y));
}

real calc_length2 (real x, real y)
{
    return sqrt((x * x) + (y * y));
}

real calc_randr (real max)
{
    return (real)rand() / (real)(RAND_MAX / max);
}

real calc_max3 (real a, real b, real c)
{
	real t = a;
	
	if (b >= t)
		t = b;
	
	if (c >= t)
		t = c;
	
	return t;
}

