#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("! numero de parametros incorreto !\n");
		printf("uso: shapenet <arquivo_shape>");
		exit(1);
	}
	
	struct cloud *cloud = cloud_load_obj(argv[1]);
	if (cloud == NULL) {
		printf("! ops, kd a nuvem? !");
		exit(1);
	}
	
	struct matrix *moments = spheric_cloud_moments(cloud);
	if (moments == NULL) {
		printf("! ops, momentos vazios !");
		exit(1);
	}
	
	matrix_debug(moments, stdout);
	
	matrix_free(moments);
	cloud_free(cloud);
	
	return 0;
}

