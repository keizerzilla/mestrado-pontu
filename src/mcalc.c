/**
 * \file mcalc.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Interface de programa para extração de momentos
 */

#include <unistd.h>

#include "include/hu.h"
#include "include/zernike.h"
#include "include/legendre.h"
#include "include/chebyshev.h"
#include "include/matrix.h"
#include "include/cloud.h"
#include "include/util.h"

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
    printf("faltando argumentos! obrigatorios: [ -m | -i | -o | -c ]\n");
    printf("  -m: momento usado para extracao de atributos\n");
    printf("      > hututu, hu1980, zernike, legendre ou chebyshev\n");
    printf("  -i: nuvem de entrada no formato XYZ\n");
    printf("      > ../data/bunny.xyz, face666.xyz, ~/bs/bs001.xyz, etc.\n");
    printf("  -o: arquivo aonde os momentos serao salvos\n");
    printf("      > path para arquivo texto ou stdout para saida padrao\n");
    printf("  -c: tipo de corte\n");
    printf("      > w: nuvem inteira, s: sagital, t: transverso, r: radial\n");
    printf("ex1: mcalc -m hu_1980 -i ../data/cloud1.xyz -o hu1.txt\n");
    printf("ex2: mcalc -m legendre -i ../dataset/bunny.xyz -o stdout\n");
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
    char* input = NULL;
    char* output = NULL;
    char* cut = NULL;
	
    int opt;
    while ((opt = getopt(argc, argv, "m:i:o:c:")) != -1) {
        switch (opt) {
            case 'm':
                moment = optarg;
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'c':
                cut = optarg;
                break;
            default:
                abort();
        }
    }
	
    if (moment == NULL || input == NULL || output == NULL || cut == NULL) {
        extraction_help();
        exit(1);
    }
	
    struct matrix* (*mfunc)(struct cloud*) = &hu_cloud_moments_hututu;
    if (!strcmp(moment, HU_TUTU))
        mfunc = &hu_cloud_moments_hututu;
    else if (!strcmp(moment, HU_1980))
        mfunc = &hu_cloud_moments_hu1980;
    else if (!strcmp(moment, LEGENDRE))
        mfunc = &legendre_cloud_moments;
    else if (!strcmp(moment, CHEBYSHEV))
        mfunc = &chebyshev_cloud_moments;
    else if (!strcmp(moment, ZERNIKE))
        mfunc = &zernike_cloud_moments;
	
    struct cloud* cloud = cloud_load_xyz(input);
    if (input == NULL) {
        util_seg("abortando");
        exit(1);
    }
	
	if (!strcmp(cut, "w")) {
		printf("w\n");
	} else if (!strcmp(cut, "s")) {
		printf("s\n");
	} else if (!strcmp(cut, "t")) {
		printf("t\n");
	} else if (!strcmp(cut, "r")) {
		printf("r\n");
	}
	
    struct matrix* results = (*mfunc)(cloud);
    
    if (!strcmp(output, "stdout"))
        matrix_debug(results, stdout);
    else
        matrix_save_to_file(results, output);
	
    matrix_free(results);
    cloud_free(cloud);
}

/**
 * \brief Função principal
 * \param argc Número de parâmetros passados pela linha de comando
 * \param argv Parâmetros passados por linha de comando
 */
int main(int argc, char** argv)
{
    extraction_interface(argc, argv);
    return 0;
}

