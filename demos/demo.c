#include "../src/include/hu.h"
#include "../src/include/golden.h"
#include "../src/include/matrix.h"
#include "../src/include/spheric.h"
#include "../src/include/legendre.h"
#include "../src/include/chebyshev.h"

int main(int argc, char** argv)
{
	
	struct vector3* v1 = vector3_new(2, 1, 0);
	struct vector3* v2 = vector3_new(0, 1, 2);
	struct vector3* v3 = vector3_new(3, 4, 0);
	
	printf("v1 dot v2 = %.4f\n", vector3_dot(v1, v2));
	printf("v1 dot v3 = %.4f\n", vector3_dot(v1, v3));
	printf("v2 dot v3 = %.4f\n", vector3_dot(v2, v3));
	
	struct vector3* u1 = v1;
	struct vector3* u2 = vector3_sub(v2, vector3_projection(v2, u1));
	struct vector3* v31 = vector3_sub(v3, vector3_projection(v3, u1));
	struct vector3* u3 = vector3_sub(v31, vector3_projection(v31, u2));
	
	vector3_normalize(u1);
	vector3_normalize(u2);
	vector3_normalize(u3);
	
	printf("u1 dot u2 = %.4f\n", vector3_dot(u1, u2));
	printf("u1 dot u3 = %.4f\n", vector3_dot(u1, u3));
	printf("u2 dot u3 = %.4f\n", vector3_dot(u2, u3));
	
	return 0;
}

