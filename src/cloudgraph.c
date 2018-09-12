#include "include/cloud.h"

int main(int argc, char** argv)
{
	util_info("cloudgraph");
	
	if (argc != 3) {
		util_error("numero incorreto de parametros!");
		util_error("uso: cloudgraph <nuvem.xyz> <lendmark.xyz>");
		exit(1);
	}
	
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	cloud_sort(cloud);
	
	struct cloud* graph = cloud_new(0);
	//struct vector3* refpoint = cloud_min_z(cloud);
	struct vector3* refpoint = &cloud->points[0];
	
	cloud_add_point_cpy(graph, refpoint);
	
	real d1 = 0;
	real d2 = 0;
	
	uint i = 0;
	while (i < cloud->num_pts) {
		if (cloud->points[i].y < refpoint->y) {
			i++;
			continue;
		}
		
		d1 = vector3_distance(refpoint, &cloud->points[i+1]);
		d2 = vector3_distance(refpoint, &cloud->points[i+2]);
		
		if (d1 < d2) {
			refpoint = &cloud->points[i+1];
			i = i+1;
		} else if (d2 < d1) {
			refpoint = &cloud->points[i+2];
			i = i+2;
		}
		
		cloud_add_point_cpy(graph, refpoint);
	}
	
	/**
	for (uint i = 0; i < cloud->num_pts; i++) {
		if (cloud->points[i].y > refpoint->y) {
			d1 = vector3_distance(refpoint, &cloud->points[i+1]);
			d2 = vector3_distance(refpoint, &cloud->points[i+2]);
			
			if (d1 < d2) {
				refpoint = &cloud->points[i+1];
				
			} else if (d2 < d1) {
				refpoint = &cloud->points[i+2];
			}
			
			cloud_add_point_cpy(graph, refpoint);
		}
	}
	*/
	
	cloud_save_xyz(graph, argv[2]);
	cloud_free(graph);
	cloud_free(cloud);
	
	return 0;
}

