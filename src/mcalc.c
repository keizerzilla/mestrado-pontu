/*
 * \file mcalc.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Interface de programa para extração de momentos
 */

/*
 * ================================
 *  Parametros de linha de comando
 * ================================
 *
 * -m: qual momento extrair (hu, zernike, legendre, tchebychev)
 * -f: arquivo csv com nuvem
 * -o: arquivo onde salvar resultados
 * -c: tamanho de corte central em milimetros
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "include/hu.h"
#include "include/zernike.h"
#include "include/legendre.h"
#include "include/tchebychev.h"
#include "include/matrix.h"
#include "include/cloud.h"
#include "include/util.h"

#define HU "hu"
#define ZERNIKE "zernike"
#define LEGENDRE "legendre"
#define TCHEBYCHEV "tchebychev"

int main(int argc, char** argv)
{
    struct matrix* (*mfunc)(struct cloud*); // m
    struct cloud* cloud; // f
    char output[256]; // o
    real cut = 0.0f; // c
	
    if (argc == 1) {
        printf("essa eh a msg de ajuda\n");
        printf("melhorar depois\n");
        printf("muitos bugs! yaay!\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0' && i < argc - 1) {
			switch (argv[i][1]) {
				case 'm':
                    if (!strcmp(argv[i+1], HU))
                        mfunc = &hu_cloud_moments;
                    else if (!strcmp(argv[i+1], ZERNIKE))
                        mfunc = &zernike_cloud_moments;
                    else if (!strcmp(argv[i+1], TCHEBYCHEV))
                        mfunc = &tchebychev_cloud_moments;
                    else if (!strcmp(argv[i+1], LEGENDRE))
                        mfunc = &legendre_cloud_moments;
                    else
                        util_error("momento desconhecido");

					break;
				case 'f':
                    cloud = cloud_load_csv(argv[i+1]);
                    if (cloud == NULL)
                        util_error("erro carregando nuvem");
					break;
				case 'o':
                    memcpy(output, argv[i+1], 256);
					break;
				case 'c':
                    cut = atof(argv[i+1]);
					break;
				default:
					printf("parametro desconhecido!\n");
					break;
			}
		}
	}

    if (cut != 0.0f)
        cloud = cloud_subcloud(cloud, cut);

    struct matrix* moments = (*mfunc)(cloud);
    matrix_save_to_file(moments, output, "w", ',');
    matrix_free(moments);
    cloud_free(cloud);

	return 0;
}
