/**
 * \file cbic2019.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Extrator de momentos de zernike para a base bosphorus
 */

#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"

#define ZERNIKE_ODD		"zi"
#define ZERNIKE_EVEN	"zr"
#define ZERNIKE_MAG		"zm"
#define ZERNIKE_FULL	"zf"
#define HARMONICS_MAG	"hs"
#define BOSPHORUS		"bosphorus"
#define SHAPENET		"shapenet"

int main(int argc, char** argv)
{
	if (argc != 4) {
		printf("! numero de parametros incorreto !\n");
		printf("uso: bosphorus <modo> <arquivo_nuvem> <tipo_zernike>\n");
		printf("<modo>: bosphorus, shapenet\n");
		printf("<arquivo_nuvem>: caminho completo para nuvem de pontos\n");
		printf("<tipo_zernike>: zi, zr, zm, zf, hs\n");
		printf("exemplo1: cbic2019 shapenet ../data/model.obj hs\n");
		printf("exemplo2: cbic2019 bosphorus clouds/bs000.xyz zr\n");
		exit(1);
	}
	
	struct cloud *cloud = NULL;
	if (!strcmp(argv[1], BOSPHORUS)) {
		cloud = cloud_load_xyz(argv[2]);
	} else if (!strcmp(argv[1], SHAPENET)) {
		cloud = cloud_load_obj(argv[2]);
	} else {
		printf("! formato desconhecido !\n");
		exit(1);
	}
	
	if (cloud == NULL) {
		printf("! ops, kd a nuvem? !\n");
		exit(1);
	}
	
	struct matrix *moments = NULL;
	if (!strcmp(argv[3], ZERNIKE_ODD))
		moments = zernike_cloud_moments_odd(cloud);
	else if (!strcmp(argv[3], ZERNIKE_EVEN))
		moments = zernike_cloud_moments_even(cloud);
	else if (!strcmp(argv[3], ZERNIKE_MAG))
		moments = zernike_cloud_moments_mag(cloud);
	else if (!strcmp(argv[3], ZERNIKE_FULL))
		moments = zernike_cloud_moments_full(cloud);
	else if (!strcmp(argv[3], HARMONICS_MAG))
		moments = harmonics_cloud_moments_mag(cloud);
	else
		moments = harmonics_cloud_moments_full(cloud);
	
	if (moments == NULL) {
		printf("! ops, momentos vazios !\n");
		exit(1);
	}
	
	matrix_debug(moments, stdout);
	
	matrix_free(moments);
	cloud_free(&cloud);
	
	return 0;
}

