#include "../../src/include/cloud.h"
#include "../../src/include/matrix.h"

int main(int argc, char** argv)
{
	struct matrix* m1 = matrix_new(1, 3);
	struct matrix* m2 = matrix_new(1, 3);
	
	matrix_set(m1, 0, 0, 5);
	matrix_set(m1, 0, 2, 69);
	matrix_debug(m1, stdout);
	
	matrix_set(m2, 0, 0, 6);
	matrix_set(m2, 0, 1, 13);
	matrix_set(m2, 0, 2, 66);
	matrix_debug(m2, stdout);
	
	printf("\n");
	struct matrix* m3 = matrix_concat_ver(m1, m2);
	matrix_debug(m3, stdout);
	
	matrix_free(m3);
	matrix_free(m2);
	matrix_free(m1);
	
	return 0;
}

