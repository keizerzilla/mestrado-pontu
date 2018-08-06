/*
 * @file extraction.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo as funções reponsáveis pelo pipeline de extraçao de
 * atributos das nuvens através dos diversos algoritmos implementados.
 */

#ifndef EXTRACTION_H
#define EXTRACTION_H

#include "hu.h"
#include "legendre.h"
#include "tchebychev.h"
#include "zernike.h"

#define HU "hu"
#define ZERNIKE "zernike"
#define TCHEBY "tchebychev"
#define LEGENDRE "legendre"

#define HU_MOMENTS 21
#define TCHEBYCHEV_MOMENTS 36
#define LEGENDRE_MOMENTS 27
#define ZERNIKE_MOMENTS 16

/*
 * @brief run_extractor Executa um dado algoritmo de extração de atributos
 * @param num_of_moments Quantos momentos devem ser calculados
 * @param input_cloud A nuvem alvo
 * @param result_file Arquivo onde os momentos serão salvos
 * @return 0 se ocorreu algum erro, 1 caso-contrário
 */
int run_extractor(void (*ext_function)(struct cloud*, real cut, struct matrix*),
                  int num_of_moments,
                  real cut,
                  const char* input_cloud,
                  const char* result_file)
{

    struct cloud* cloud = cloud_load_from_file(input_cloud);
    struct cloud* sub = NULL;

    if (cut < 0)
        sub = cloud;
    else
        sub = cloud_subcloud(cloud, cut);

    if (cloud == NULL || sub == NULL) {
        printf("EXT: ERRO AO CARREGAR A NUVEM %s\n", input_cloud);
        return 0;
    }

    struct matrix* feat_matrix = matrix_new(1, num_of_moments);

    (*ext_function)(sub, cut, feat_matrix);
    matrix_save_to_file(feat_matrix, result_file, "a+", ',');

    matrix_free(feat_matrix);
    cloud_free(sub);

    return 1;
}

/*
 * @brief program_interface Função de inteface para extração de atributos
 * @param argc Número de argumentos passados pela linha de comando
 * @param argv Lista dos argumentos passados pela linha de comando
 * @return 0 se a execução terminou com sucesso, 1 caso-contrário
 */
int program_interface(int argc, char** argv) {
    if (argc != 5) {
        printf("ERRO: PARAMETROS INCORRETOS!\n");
        printf("USO: char-ext <extrator> <corte> <nuvem> <saida>\n");
        printf("EXTRATOR: hu, zernike, tchebychev, legendre\n");
        printf("CORTE: valor inteiro em milimetros (distancia ponto-centro)\n");
        printf("NUVEM: arquivo da nuvem com pontos no formato x y z\n");
        printf("SAIDA: arquivo texto com os atributos extraidos\n");
        printf("EXEMPLO: char-ext zernike 60 sample.cloud result.txt\n\n");
        exit(1);
    }

    void (*ext_function)(struct cloud*, real, struct matrix*) = NULL;
    int num_of_moments = 0;

    if (!strcmp(argv[1], HU)) {
        ext_function = &hu_cloud_moments;
        num_of_moments = HU_MOMENTS;
    } else if (!strcmp(argv[1], ZERNIKE)) {
        ext_function = &zernike_cloud_moments;
        num_of_moments = ZERNIKE_MOMENTS;
    } else if (!strcmp(argv[1], TCHEBY)) {
        ext_function = &tchebychev_cloud_moments;
        num_of_moments = TCHEBYCHEV_MOMENTS;
    } else if (!strcmp(argv[1], LEGENDRE)) {
        ext_function = &legendre_cloud_moments;
        num_of_moments = LEGENDRE_MOMENTS;
    } else {
        printf("EXTRATOR DESCONHECIDO! ABORTANDO PROGRAMA!\n");
        exit(1);
    }

    real cut = atof(argv[2]);

    if (run_extractor(ext_function, num_of_moments, cut, argv[3], argv[4]))
        printf("[%s][%s][OK]\n", argv[1], argv[3]);
    else
        printf("ERROS DURANTE EXECUCAO! ABORTANDO!\n");

    return 0;
}

/*
 * @brief center_profile Extrái distâncias dos pontos ao centro
 * @param argc Número de argumentos passados pela linha de comando
 * @param argv Lista dos argumentos passados pela linha de comando
 * @return 0 se a execução terminou com sucesso, 1 caso-contrário
 */
int center_profile(int argc, char** argv) {
    if (argc != 3) {
        printf("ERRO: PARAMETROS INCORRETOS!\n");
        printf("USO: cloudz <nuvem> <saida>\n");
        printf("NUVEM: caminho completo para o arquivo a nuvem\n");
        printf("SAIDA: arquivo onde as distancias serao salvas\n");
        printf("EXEMPLO: cloudz bs000_N_1.cloud nuvem_d.txt\n\n");
        exit(1);
    }

    struct cloud* cloud = cloud_load_from_file(argv[1]);
    if (cloud == NULL) {
        printf("ERRO: NUVEM [%s]\nNAO ENCONTRADA!\n", argv[1]);
        exit(1);
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("ERRO: ARQUIVO [%s]\nNAO FOI POSSIVEL ABRIR!\n", argv[2]);
        exit(1);
    }

    printf("ANALISANDO [%s]... ", argv[1]);

    struct cloud* aux = cloud;
    struct vector3* center = cloud_get_center(cloud);
    real d = 0.0f;
    int count = 0;
    while (aux != NULL) {
        d = vector3_distance(center, aux->point);
        fprintf(output, "%.10e\n", d);

        aux = aux->next;
        d = 0.0f;
        count++;
    }

    printf("DONE! [%d]\n", count);

    fclose(output);
    vector3_free(center);
    cloud_free(cloud);

    return 0;
}

#endif // EXTRACTION_H
