#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main()
{
	printf("%d\n", zernike_num_moments_even(20, 20));
	
	if (!strcmp("artur", "artur")) {
		printf("equal\n");
	} else {
		printf("diff\n");
	}
	
	return 0;
}

