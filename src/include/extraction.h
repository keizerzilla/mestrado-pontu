/**
 * \file extraction.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Funções de interface de programa para análise de nuvens de pontos
 */

#include <unistd.h>

#include "hu.h"
#include "zernike.h"
#include "legendre.h"
#include "chebyshev.h"
#include "matrix.h"
#include "cloud.h"
#include "util.h"

#define HU_TUTU "hututu"
#define HU_1980 "hu1980"
#define ZERNIKE "zernike"
#define LEGENDRE "legendre"
#define CHEBYSHEV "chebyshev"

/**
 * \brief Exibe mensagem ao usuário informando como usar o extrator de momentos
 */
void extraction_help()
{
	printf("mcalc - calculador de momentos\n");
	printf("autor - Artur Rodrigues Rocha Neto\n");
    printf("faltando argumentos! obrigatorios: [ -m | -c | -o | -s ]\n");
    printf("  -m: momento usado para extracao de atributos\n");
    printf("      > hututu, hu1980, zernike, legendre ou chebyshev\n");
    printf("  -c: nuvem de entrada no formato XYZ\n");
    printf("      > ../data/bunny.xyz, face666.xyz, ~/bs/bs001.xyz, etc.\n");
    printf("  -o: arquivo aonde os momentos serao salvos\n");
    printf("      > path para arquivo texto ou stdout para saida padrao\n");
    printf("  -s: tipo de fatiamento da nuvem ou combinacoes\n");
    printf("      > w: inteira, f: frontal, s: sagital, t: transverso\n");
    printf("ex1: mcalc -m hu_1980 -i ../data/cloud1.xyz -o hu1.txt -s w\n");
    printf("ex2: mcalc -m legendre -i ../dataset/bunny.xyz -o stdout -s s\n");
    printf("ex3: mcalc -m chebyshev -i armadillo.xyz -o cheby.dat -s fst\n");
    printf("\n");
}

/**
 * \brief Interface para análise de nuvens de pontos
 * \param argc Número de parâmetros passados pela linha de comando
 * \param argv Parâmetros passados por linha de comando
 */
void extraction_interface(int argc, char** argv)
{
    char* moment = NULL;
    char* cloud = NULL;
    char* output = NULL;
    char* slice = NULL;
	
    int opt;
    while ((opt = getopt(argc, argv, "m:c:o:s:")) != -1) {
        switch (opt) {
            case 'm':
                moment = optarg;
                break;
            case 'c':
                cloud = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 's':
            	slice = optarg;
            	break;
            default:
                abort();
        }
    }
	
    if (moment == NULL || cloud == NULL || output == NULL || slice == NULL) {
        extraction_help();
        exit(1);
    }
	
    struct matrix* (*mfunc)(struct cloud*) = &hu_cloud_moments_artur_tcc;
    if (!strcmp(moment, HU_TUTU))
        mfunc = &hu_cloud_moments_artur_tcc;
    else if(!strcmp(moment, HU_1980))
        mfunc = &hu_cloud_moments_sadjadi_hall;
    else if (!strcmp(moment, LEGENDRE))
        mfunc = &legendre_cloud_moments;
    else if (!strcmp(moment, CHEBYSHEV))
        mfunc = &chebyshev_cloud_moments;
    else if (!strcmp(moment, ZERNIKE))
        mfunc = &zernike_cloud_moments;
	
    struct cloud* input = cloud_load_xyz(cloud);
    if (input == NULL) {
        util_seg("abortando");
        exit(1);
    }
	
    struct matrix* results = (*mfunc)(input);
	
    if (!strcmp(output, "stdout"))
        matrix_debug(stdout, results);
    else
        matrix_save_to_file(results, output);
	
    matrix_free(results);
    cloud_free(input);
}

