#include "../../src/include/cloud.h"
#include "../../src/include/matrix.h"

int main(int argc, char** argv)
{
	struct cloud* c = cloud_load_xyz(argv[1]);
	printf("%d\n", cloud_size(c));
	
	struct matrix* m = matrix_new(20, 5);
	matrix_set(m, 0, 0, 5);
	matrix_debug(m, stdout);
	
	matrix_add_col(m);
	matrix_debug(m, stdout);
	
	matrix_free(m);
	cloud_free(c);
	
	return 0;
}

