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
#include "include/bsptree.h"
#include "include/hashtable.h"

#define HU_TUTU			"hututu"
#define HU_1980			"hu1980"
#define HU_SIQ			"husiq"
#define ZERNIKE			"zernike"
#define LEGENDRE		"legendre"
#define CHEBYSHEV		"chebyshev"
#define CUT_WHOLE		"w"
#define CUT_SAGITTAL	"s"
#define CUT_TRANSVERSAL	"t"
#define CUT_FRONTAL		"f"
#define CUT_RADIAL		"r"
#define CUT_UPPER		"u"
#define CUT_LOWER		"l"

/**
 * \brief Exibe mensagem ao usuário informando como usar o extrator de momentos
 */
void extraction_help()
{
	printf("mcalc - Calculador de Momentos 3D\n");
	printf("autor - Artur Rodrigues Rocha Neto (UFC/LATIN/INTERFACES)\n");
    printf("faltando argumentos! obrigatorios: [ -m | -i | -o | -c ]\n");
    
    printf(" -m: momento usado para extracao de atributos\n");
    printf("     hututu, hu1980, husiq, zernike, legendre ou chebyshev\n");
    
    printf(" -i: nuvem de entrada no formato XYZ\n");
    printf("     ../data/bunny.xyz, face666.xyz, ~/bs/bs001.xyz, etc\n");
    
    printf(" -o: arquivo aonde os momentos serao salvos\n");
    printf("     path para arquivo texto ou stdout para saida padrao\n");
    
    printf(" -c: tipo de corte\n");
    printf("     w: toda, s: sagital, t: transversal, f: frontal, r: radial\n");
    printf("     u: parte superior, l: parte inferior\n");
    
    printf("ex1: mcalc -m hu_1980 -i ../data/cloud1.xyz -o hu1.txt -c t\n");
    printf("ex2: mcalc -m legendre -i ../dataset/bunny.xyz -o stdout -c w\n\n");
}

/**
 * \brief Extrái momentos usando cortes a partir de planos recursivos
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \param O vetor normal ao plano de corte
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_plane(struct cloud* cloud,
                                struct matrix* (*mfunc)(struct cloud*),
                                struct vector3* norm)
{
	struct cloud* par1 = cloud_empty();
	struct cloud* par2 = cloud_empty();
	struct vector3* pt = cloud_get_center(cloud);
	struct plane* plane = plane_new(norm, pt);
	
	cloud_plane_partition(cloud, plane, par1, par2);
	
	struct cloud* par1_fh = cloud_empty();
	struct cloud* par2_fh = cloud_empty();
	struct vector3* pt_fh = cloud_get_center(par1);
	struct plane* plane_fh = plane_new(norm, pt_fh);
	
	cloud_plane_partition(par1, plane_fh, par1_fh, par2_fh);
	
	struct matrix* r1 = matrix_concat_hor((*mfunc)(par1_fh), (*mfunc)(par2_fh));
	struct cloud* par1_sh = cloud_empty();
	struct cloud* par2_sh = cloud_empty();
	struct vector3* pt_sh = cloud_get_center(par2);
	struct plane* plane_sh = plane_new(norm, pt_sh);
	
	cloud_plane_partition(par2, plane_sh, par1_sh, par2_sh);
	
	struct matrix* r2 = matrix_concat_hor((*mfunc)(par1_sh), (*mfunc)(par2_sh));
	struct matrix* ans = matrix_concat_hor(r1, r2);
	
	plane_free(plane_fh);
	vector3_free(pt_fh);
	cloud_free(par2_fh);
	cloud_free(par1_fh);
	plane_free(plane_sh);
	vector3_free(pt_sh);
	cloud_free(par2_sh);
	cloud_free(par1_sh);
	matrix_free(r2);
	matrix_free(r1);
	plane_free(plane);
	vector3_free(pt);
	vector3_free(norm);
	cloud_free(par2);
	cloud_free(par1);
	
	return ans;
}

/**
 * \brief Extrái momentos usando cortes sagitais
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_sagittal(struct cloud* cloud,
                                   struct matrix* (*mfunc)(struct cloud*))
{
	return extraction_plane(cloud, mfunc, vector3_new(1, 0, 0));
}

/**
 * \brief Extrái momentos usando cortes transversais
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_transversal(struct cloud* cloud,
                                      struct matrix* (*mfunc)(struct cloud*))
{
	return extraction_plane(cloud, mfunc, vector3_new(0, 1, 0));
}

/**
 * \brief Extrái momentos usando cortes frontais
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_frontal(struct cloud* cloud,
                                  struct matrix* (*mfunc)(struct cloud*))
{
	return extraction_plane(cloud, mfunc, vector3_new(0, 0, 1));
}

/**
 * \brief Extrái momentos usando cortes radiais
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_radial(struct cloud* cloud,
                                 struct matrix* (*mfunc)(struct cloud*))
{
	real d = 0.0f;
	real maxd = cloud_max_distance_from_center(cloud);
	
	real slice1 = maxd / 4;
	real slice2 = slice1 * 2;
	real slice3 = slice1 * 3;
	
	struct vector3* center = cloud_get_center(cloud);
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	struct cloud* sub3 = cloud_empty();
	struct cloud* sub4 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		d = vector3_distance(center, &cloud->points[i]);
		
		if (d <= slice1)
			cloud_add_point(sub1, &cloud->points[i]);
		else if (d > slice1 && d <= slice2)
			cloud_add_point(sub2, &cloud->points[i]);
		else if (d > slice2 && d <= slice3)
			cloud_add_point(sub3, &cloud->points[i]);
		else
			cloud_add_point(sub4, &cloud->points[i]);
	}
	
	struct matrix* r1 = matrix_concat_hor((*mfunc)(sub1), (*mfunc)(sub2));
	struct matrix* r2 = matrix_concat_hor((*mfunc)(sub3), (*mfunc)(sub4));
	struct matrix* ans = matrix_concat_hor(r1, r2);
	
	matrix_free(r2);
	matrix_free(r1);
	cloud_free(sub3);
	cloud_free(sub2);
	cloud_free(sub1);
	vector3_free(center);
	
	return ans;
}

/**
 * \brief Extrái momentos usando a parte superior da nuvem (já alinhada)
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_upper(struct cloud* cloud,
                                struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm = vector3_new(0, 1, 0);
	struct vector3* point = cloud_min_z(cloud);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	struct matrix* ans = (*mfunc)(sub);
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(point);
	vector3_free(norm);
	
	return ans;
}

/**
 * \brief Extrái momentos usando a parte inferior da nuvem (já alinhada)
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_lower(struct cloud* cloud,
                                struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm = vector3_new(0, -1, 0);
	struct vector3* point = cloud_min_z(cloud);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	struct matrix* ans = (*mfunc)(sub);
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(point);
	vector3_free(norm);
	
	return ans;
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
    else if (!strcmp(moment, HU_SIQ))
        mfunc = &hu_cloud_moments_husiq;
    else if (!strcmp(moment, LEGENDRE))
        mfunc = &legendre_cloud_moments;
    else if (!strcmp(moment, CHEBYSHEV))
        mfunc = &chebyshev_cloud_moments;
    else if (!strcmp(moment, ZERNIKE))
        mfunc = &zernike_cloud_moments;
    else
    	mfunc = &hu_cloud_moments_hututu;
	
    struct cloud* cloud = cloud_load_xyz(input);
    if (input == NULL) {
        util_seg("abortando");
        exit(1);
    }
	
	struct matrix* results = NULL;
	if (!strcmp(cut, CUT_WHOLE))
		results = (*mfunc)(cloud);
	else if (!strcmp(cut, CUT_SAGITTAL))
		results = extraction_sagittal(cloud, mfunc);
	else if (!strcmp(cut, CUT_TRANSVERSAL))
		results = extraction_transversal(cloud, mfunc);
	else if (!strcmp(cut, CUT_FRONTAL))
		results = extraction_frontal(cloud, mfunc);
	else if (!strcmp(cut, CUT_RADIAL))
		results = extraction_radial(cloud, mfunc);
	else if (!strcmp(cut, CUT_UPPER))
		results = extraction_upper(cloud, mfunc);
	else if (!strcmp(cut, CUT_LOWER))
		results = extraction_lower(cloud, mfunc);
	else
		results = (*mfunc)(cloud);
	
	if (!strcmp(output, "stdout")) {
		matrix_debug(results, stdout);
	} else {
		matrix_save_to_file(results, output);
	}
	
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

