#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

#define ZERNIKE_ODD		"odd"
#define ZERNIKE_EVEN	"even"
#define ZERNIKE_MAG		"mag"

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("! numero de parametros incorreto !\n");
		printf("uso: shapenet <arquivo_shape>");
		exit(1);
	}
	
	struct cloud *cloud = cloud_load_obj(argv[1]);
	if (cloud == NULL) {
		printf("! ops, kd a nuvem? !");
		exit(1);
	}
	
	struct matrix *moments = NULL;
	if (!strcmp(argv[2], ZERNIKE_ODD))
		moments = zernike_cloud_moments_odd(cloud);
	else if (!strcmp(argv[2], ZERNIKE_EVEN))
		moments = zernike_cloud_moments_even(cloud);
	else if (!strcmp(argv[2], ZERNIKE_MAG))
		moments = zernike_cloud_moments_mag(cloud);
	else
		moments = zernike_cloud_moments_mag(cloud);
	
	if (moments == NULL) {
		printf("! ops, momentos vazios !");
		exit(1);
	}
	
	matrix_debug(moments, stdout);
	
	matrix_free(moments);
	cloud_free(cloud);
	
	return 0;
}

