#include "./pontu_core.h"
#include "./pontu_features.h"
#include "./pontu_sampling.h"
#include "./pontu_registration.h"

int main (unsigned argc, char **argv)
{
    struct matrix *m = matrix_new(3, 3);
    struct matrix *n = matrix_new(2, 2);
    matrix_set(m, 0, 0, 1);
    matrix_set(m, 1, 0, 2);
    matrix_set(m, 2, 0, 3);

    matrix_set(m, 0, 1, 4);
    matrix_set(m, 1, 1, 5);
    matrix_set(m, 2, 1, 9);

    matrix_set(m, 0, 2, 7);
    matrix_set(m, 1, 2, 8);
    matrix_set(m, 2, 2, 9);

    matrix_set(n, 0, 0, 11);
    matrix_set(n, 1, 0, 12);

    matrix_set(n, 0, 1, 14);
    matrix_set(n, 1, 1, 15);

    matrix_set(n, 0, 2, 17);
    matrix_set(n, 1, 2, 18);
    
    matrix_debug(m, stdout);
    // printf("\n");
    // struct matrix * new_mat = matrix_remove_col(m, 0);
    // matrix_debug(new_mat, stdout);
    printf("\n");
    printf("%f", algebra_det(m));
    
    return 0;
}