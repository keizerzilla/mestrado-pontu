/**
 * \file siqcal.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Interface de programa para extração de momentos segundo Siqueira.
 */

#include <unistd.h>
#include "include/siqueira.h"
#include "include/matrix.h"
#include "include/cloud.h"

#define CUT_WHOLE							"w"
#define CUT_FRONTAL							"f"
#define CUT_SAGITTAL						"s"
#define CUT_TRANSVERSAL						"t"
#define CUT_FRONTAL_SAGITTAL				"fs"
#define CUT_FRONTAL_TRANSVERSAL				"ft"
#define CUT_SAGITTAL_TRANSVERSAL			"st"
#define CUT_FRONTAL_SAGITTAL_TRANSVERSAL	"fst"

/**
 * \brief Exibe mensagem ao usuário informando como usar o extrator de momentos
 */
void extraction_help()
{
	printf("siqcalc: Calculador de Momentos 3D (siqueira style)\n");
	printf("autor: Artur Rodrigues Rocha Neto (UFC/LATIN/INTERFACES)\n");
    printf("FALTANDO ARGUMENTOS! obrigatorios: [ -i | -n | -o | -c ]\n");
        
    printf(" -i: nuvem de entrada no formato xyz\n");
    printf("     > ../data/bunny.xyz, face666.xyz, ~/bs/bs001.xyz, etc\n");
    
    printf(" -o: arquivo aonde os momentos serao salvos\n");
    printf("     > path para arquivo texto\n");
    printf("     > stdout para saida padrão (normalmente console)\n");
    
    printf(" -c: tipo de corte\n");
    printf("     > w: nuvem inteira\n");
    printf("     > f: frontal\n");
    printf("     > s: sagital\n");
    printf("     > t: transversal\n");
    printf("     > fs: frontal + sagital\n");
    printf("     > ft: frontal + transversal\n");
    printf("     > st: sagital + transversal\n");
    printf("     > fst: frontal + sagital + transversal\n");
    
    printf("EX1: siqueira -i ../data/face1.xyz -o dump.txt -c t\n");
    printf("EX2: siqueira -i ../data/face2.xyz -o stdout -c f\n\n");
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
        util_seg("abortando..\n");
        exit(1);
    }
	
	cloud_scale(cloud, 0.001);
	
	struct vector3* nose = vector3_from_vector(&cloud->points[0]);
	
	struct matrix* results = NULL;
	if (!strcmp(cut, CUT_WHOLE)) {
		results = siqueira_cloud_moments(cloud, nose);
	} else if (!strcmp(cut, CUT_SAGITTAL)) {
		results = siqueira_sagittal(cloud, nose);
	} else if (!strcmp(cut, CUT_TRANSVERSAL)) {
		results = siqueira_transversal(cloud, nose);
	} else if (!strcmp(cut, CUT_FRONTAL)) {
		results = siqueira_frontal(cloud, nose);
	} else if (!strcmp(cut, CUT_FRONTAL_SAGITTAL)) {
		struct matrix* frontal = siqueira_frontal(cloud, nose);
		struct matrix* sagittal = siqueira_sagittal(cloud, nose);
		
		results = matrix_concat_hor(frontal, sagittal);
		
		matrix_free(frontal);
		matrix_free(sagittal);
	} else if (!strcmp(cut, CUT_FRONTAL_TRANSVERSAL)) {
		struct matrix* frontal = siqueira_frontal(cloud, nose);
		struct matrix* transversal = siqueira_transversal(cloud, nose);
		
		results = matrix_concat_hor(frontal, transversal);
		
		matrix_free(frontal);
		matrix_free(transversal);
	} else if (!strcmp(cut, CUT_SAGITTAL_TRANSVERSAL)) {
		struct matrix* sagittal = siqueira_sagittal(cloud, nose);
		struct matrix* transversal = siqueira_transversal(cloud, nose);
		
		results = matrix_concat_hor(sagittal, transversal);
		
		matrix_free(sagittal);
		matrix_free(transversal);
	} else if (!strcmp(cut, CUT_FRONTAL_SAGITTAL_TRANSVERSAL)) {
		struct matrix* frontal = siqueira_frontal(cloud, nose);
		struct matrix* sagittal = siqueira_sagittal(cloud, nose);
		struct matrix* transversal = siqueira_transversal(cloud, nose);
		struct matrix* fs = matrix_concat_hor(frontal, sagittal);
		
		results = matrix_concat_hor(fs, transversal);
		
		matrix_free(frontal);
		matrix_free(sagittal);
		matrix_free(transversal);
		matrix_free(fs);
	}
	
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

