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
#include "tchebychev.h"
#include "matrix.h"
#include "cloud.h"
#include "util.h"

#define HU "hu"
#define ZERNIKE "zernike"
#define LEGENDRE "legendre"
#define TCHEBYCHEV "tchebychev"

/**
 * \brief Interface para análise de nuvens de pontos
 * \param argc Número de parâmetros passados pela linha de comando
 * \param argv Parâmetros passados por linha de comando
 */
void extraction_interface(int argc, char** argv)
{
    char* optm = NULL;
    char* opti = NULL;
    char* opto = NULL;
    char* optc = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "m:i:o:c:")) != -1) {
        switch (opt) {
            case 'm':
                optm = optarg;
                break;
            case 'i':
                opti = optarg;
                break;
            case 'o':
                opto = optarg;
                break;
            case 'c':
                optc = optarg;
                break;
            default:
                abort();
        }
    }

    if (optm == NULL || opti == NULL || opto == NULL) {
        util_error("faltando argumentos!");
        util_info("argumentos obrigatorios: -m | -i | -o\n \
-m: extrator (hu, zernike, legendre ou tchebychev)\n \
-i: nuvem de entrada no formato CSV\n \
-o: arquivo de saida aonde os momentos serao salvos");
        exit(1);
    }

    struct matrix* (*mfunc)(struct cloud*) = &hu_cloud_moments;
    if (!strcmp(optm, HU))
        mfunc = &hu_cloud_moments;
    else if (!strcmp(optm, LEGENDRE))
        mfunc = &legendre_cloud_moments;
    else if (!strcmp(optm, TCHEBYCHEV))
        mfunc = &tchebychev_cloud_moments;
    else if (!strcmp(optm, ZERNIKE))
        mfunc = &zernike_cloud_moments;

    if (optc == NULL) {
        struct cloud* cloud = cloud_load_csv(opti);
        if (cloud == NULL) {
            util_seg("abortando");
            exit(1);
        }

        struct matrix* results = (*mfunc)(cloud);

        matrix_save_to_file(results, opto);
        matrix_free(results);
        cloud_free(cloud);
    } else {
        struct cloud* cloud = cloud_load_csv(opti);
        if (cloud == NULL) {
            util_seg("abortando");
            exit(1);
        }

        struct cloud* sub = cloud_subcloud(cloud, atof(optc));
        if (sub == NULL) {
            util_seg("abortando");
            exit(1);
        }

        struct matrix* results = (*mfunc)(sub);

        matrix_save_to_file(results, opto);
        matrix_free(results);
        cloud_free(cloud);
        cloud_free(sub);
    }
}
