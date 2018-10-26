/**
 * \file radialcut.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Programa fatiador de nuvens a partir do centróide
 */

#include "include/cloud.h"

int main(int argc, char** argv)
{
	if (argc != 4) {
		printf("radialcut - corte radial de nuvens\n");
		printf("autor - Artur Rodrigues Rocha Neto\n");
		printf("faltando argumentos! [ <input> <cutsize> <output> ]\n");
		printf("ex: radialcut bunny.xyz 50 radcutbunny.xyz\n");
		printf("\n");
		exit(1);
	}
	
	struct cloud* input = cloud_load_xyz(argv[1]);
	if (input == NULL) exit(1);
	cloud_save_xyz(cloud_cut_center(input, atof(argv[2])), argv[3]);
	cloud_free(input);
	
	return 0;
}

