/**
 * \file shapenet.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Interface de programa para extração de momentos (formato ShapeNetCore)
 */

#include "include/hu.h"
#include "include/zernike.h"
#include "include/legendre.h"
#include "include/chebyshev.h"
#include "include/matrix.h"
#include "include/cloud.h"
#include "include/util.h"
#include "include/spheric.h"

#define HUTUTU			"hututu"
#define HU1980			"hu1980"
#define HUSIQ			"husiq"
#define ZERNIKE			"zernike"
#define LEGENDRE		"legendre"
#define CHEBYSHEV		"chebyshev"
#define SPHERIC			"spheric"

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("shapenet: argumentos insuficientes!\n");
		return 1;
	}
	
	char* moment = argv[1];
	char* cloud_path = argv[2];
	
	struct matrix* (*mfunc)(struct cloud*) = &hu_cloud_moments_hututu;
    
    if (!strcmp(moment, HUTUTU)) {
        mfunc = &hu_cloud_moments_hututu;
    } else if (!strcmp(moment, HU1980)) {
        mfunc = &hu_cloud_moments_hu1980;
    } else if (!strcmp(moment, HUSIQ)) {
        mfunc = &hu_cloud_moments_husiq;
    } else if (!strcmp(moment, LEGENDRE)) {
        mfunc = &legendre_cloud_moments;
    } else if (!strcmp(moment, CHEBYSHEV)) {
        mfunc = &chebyshev_cloud_moments;
    } else if (!strcmp(moment, ZERNIKE)) {
        mfunc = &zernike_cloud_moments;
    } else if (!strcmp(moment, SPHERIC)) {
        mfunc = &spheric_cloud_moments;
    } else {
    	mfunc = &hu_cloud_moments_hututu;
	}
	
	struct cloud* cloud = cloud_load_obj(cloud_path);
	struct matrix* results = (*mfunc)(cloud);
	
	matrix_debug(results, stdout);
	
	matrix_free(results);
	cloud_free(cloud);
	
	return 0;
}

