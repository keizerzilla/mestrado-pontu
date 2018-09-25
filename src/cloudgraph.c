/**
 * \file cloudgraph.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Tentativas de sub-amostragem e segmentação
 */

#include "include/cloud.h"

int main(int argc, char** argv)
{
	if (argc != 4) {
		util_error("numero incorreto de parametros!");
		util_error("uso: cloudgraph <nuvem.xyz> <graph.xyz> <cut>");
		exit(1);
	}
	
	struct cloud* c = cloud_load_xyz(argv[1]);
	struct plane3* p = plane3_new(vector3_new(0, -1, 0), cloud_min_z(c));
	struct cloud* cut = cloud_new(0);
	real leafsize = atof(argv[3]);
	
	for (uint i = 0; i < c->num_pts; i++)
		if (plane3_distance2point(p, &c->points[i]) <= leafsize)
			cloud_add_point_cpy(cut, &c->points[i]);
	
	cloud_save_xyz(cut, argv[2]);
	
	cloud_free(cut);
	plane3_free(p);
	cloud_free(c);
	
	return 0;
}

