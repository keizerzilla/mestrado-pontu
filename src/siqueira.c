/**
 * \file siqueira.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Interface de programa para extração de momentos segundo Siqueira.
 */

#include <time.h>
#include <unistd.h>
#include "include/siqueira.h"
#include "include/matrix.h"
#include "include/cloud.h"
#include "include/util.h"

#define CUT_WHOLE		"w"
#define CUT_FRONTAL		"f"
#define CUT_SAGITTAL	"s"
#define CUT_TRANSVERSAL	"t"

/**
 * \brief Exibe mensagem ao usuário informando como usar o extrator de momentos
 */
void extraction_help()
{
	printf("mcalc: Calculador de Momentos 3D (siqueira style)\n");
	printf("autor: Artur Rodrigues Rocha Neto (UFC/LATIN/INTERFACES)\n");
    printf("faltando argumentos! obrigatorios: [ -i | -o | -c ]\n");
    
    printf(" -i: nuvem de entrada no formato XYZ\n");
    printf("     > ../data/bunny.xyz, face666.xyz, ~/bs/bs001.xyz, etc\n");
    
    printf(" -o: arquivo aonde os momentos serao salvos\n");
    printf("     > path para arquivo texto\n");
    printf("     > stdout para saida padrão (normalmente console)\n");
    
    printf(" -c: tipo de corte\n");
    printf("     > f: frontal\n");
    printf("     > s: sagital\n");
    printf("     > t: transversal\n");
    
    printf("EX1: siqueira -i ../data/cloud1.xyz -o dump.txt -c t\n");
    printf("EX2: siqueira -i ../dataset/bunny.xyz -o stdout -c f\n\n");
}

/**
 * \brief Função principal: lê parâmetros de linha de comando e efetua extração
 * \param argc Número de parâmetros passados pela linha de comando
 * \param argv Parâmetros passados por linha de comando
 */
int main(int argc, char** argv)
{
	char* input = NULL;
    char* output = NULL;
    char* cut = NULL;
	
    int opt;
    while ((opt = getopt(argc, argv, "i:o:c:")) != -1) {
        switch (opt) {
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
	
    if (input == NULL || output == NULL || cut == NULL) {
        extraction_help();
        return 1;
    }
	
    struct cloud* cloud = cloud_load_xyz(input);
    if (input == NULL) {
        util_seg("abortando");
        exit(1);
    }
	
	struct vector3* nose = cloud_min_z(cloud);
	
	struct matrix* results = NULL;
	if (!strcmp(cut, CUT_SAGITTAL))
		results = siqueira_sagittal(cloud, nose);
	else if (!strcmp(cut, CUT_TRANSVERSAL))
		results = siqueira_transversal(cloud, nose);
	else
		results = siqueira_frontal(cloud, nose);
	
	if (!strcmp(output, "stdout")) {
		matrix_debug(results, stdout);
	} else {
		matrix_save_to_file(results, output, "w");
	}
	
	vector3_free(nose);
    matrix_free(results);
    cloud_free(cloud);
    
    return 0;
}

