/**
 * \file cloudgraph.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Tentativas de sub-amostragem e segmentação
 */

#include "include/cloud.h"

int main(int argc, char** argv)
{
	if (argc != 3) {
		util_error("numero incorreto de parametros!");
		util_error("uso: cloudgraph <nuvem.xyz> <graph.xyz>");
		exit(1);
	}
	
	struct cloud* c = cloud_load_xyz(argv[1]);
	cloud_translate_real(c, 0, 0, 0);
	struct vector3* ref = cloud_min_z(c);
	struct vector3* dir = vector3_new(0, -1, 0);
	
	// soh mudar a chamada da funcao abaixo
	struct cloud* p = cloud_binary_mask(c);
	
	cloud_save_xyz(c, argv[1]);
	cloud_save_xyz(p, argv[2]);
	cloud_free(p);
	cloud_free(c);
	
	return 0;
}

