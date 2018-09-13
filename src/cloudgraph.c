/**
 * \file cloudgraph.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Tentativas de sub-amostragem e segmentação
 */

#include "include/cloud.h"

void plane(char** argv)
{
	struct cloud* c = cloud_load_xyz(argv[1]);
	
	//struct vector3* ref = cloud_min_z(c);
	struct vector3* ref = vector3_new(-99.714, -54.082, 46.744);
	struct vector3* dir = vector3_new(0.4127296, -0.09994896, -0.905353215);
	
	struct cloud* p = cloud_cut_plane(c, ref, dir);
	
	cloud_save_xyz(p, argv[2]);
	
	cloud_free(p);
	cloud_free(c);
}

void line(char** argv)
{
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	cloud_sort(cloud);
	
	struct vector3* ref = cloud_min_z(cloud);
	struct vector3* dir = vector3_new(1, 1, 0);
	struct cloud* mask = cloud_cut_plane(cloud, ref, dir);
	struct cloud* path = cloud_new(0);
	
	cloud_add_point_cpy(path, ref);
	
	while (mask->num_pts) {
		ref = cloud_closest_point(mask, ref);
		cloud_add_point_cpy(path, ref);
		//cloud_free(mask); //SEGFAULT
		mask = cloud_cut_plane(cloud, ref, dir);
	}
	
	cloud_save_xyz(path, argv[2]);
	
	cloud_free(path);
	cloud_free(mask);
	cloud_free(cloud);
}

void mask(char** argv)
{
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	cloud_sort(cloud);
	
	struct cloud* path = cloud_binary_mask(cloud);
	
	cloud_save_xyz(path, argv[2]);
	
	cloud_free(path);
	cloud_free(cloud);
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		util_error("numero incorreto de parametros!");
		util_error("uso: cloudgraph <nuvem.xyz> <graph.xyz>");
		exit(1);
	}
	
	plane(argv);
	//line(argv);
	//mask(argv);
	
	return 0;
}

