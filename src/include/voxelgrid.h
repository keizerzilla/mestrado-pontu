/**
 * \file voxelgrid.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Implementação para tecnica de subamostragem de nuvens de pontos
 * utilizando voxel grid.
 */

 #ifndef VOXELGRID_H
 #define VOXELGRID_H

#include "cloud.h"

struct cloud* cubic_voxel_grid(struct cloud* src, float leafsize) {
    
    float apothem = leafsize / 2;
    
    struct vector3* centroid = cloud_get_center(src);
    
    struct vector3* axis_size = cloud_axis_size(src);
    
    uint num_voxels_x = (uint) ceil(axis_size->x/leafsize) + 1;
    uint num_voxels_y = (uint) ceil(axis_size->y/leafsize) + 1;
    uint num_voxels_z = (uint) ceil(axis_size->z/leafsize) + 1;

    struct vector3* voxel_o = vector3_zero();
    voxel_o->x = (cloud_min_x(src)->x + cloud_max_x(src)->x) / 2;
    voxel_o->y = (cloud_min_y(src)->y + cloud_max_y(src)->y) / 2;
    voxel_o->z = (cloud_min_z(src)->z + cloud_max_z(src)->z) / 2;
    
    voxel_o->x -= (float) floor(apothem * num_voxels_x);
    voxel_o->y -= (float) floor(apothem * num_voxels_y);
    voxel_o->z -= (float) floor(apothem * num_voxels_z);

    uint pos_x, pos_y, pos_z;
    float d_x, min_x, max_x, d_y, min_y, max_y, d_z, min_z, max_z;

    struct cloud* output = cloud_empty();

    struct cloud* clouds[num_voxels_x][num_voxels_y][num_voxels_z];
    for (uint i = 0; i < num_voxels_x; i++) {
        for (uint j = 0; j < num_voxels_y; j++) {
            for (uint k = 0; k < num_voxels_z; k++) {
                clouds[i][j][k] = cloud_empty();
            }
        }
    }

    for (uint n = 0; n < src->num_pts; n++) {
        d_x = src->points[n].x - voxel_o->x;
        pos_x = (uint) ceil(d_x / leafsize);
        min_x = voxel_o->x + (pos_x * leafsize) - apothem;
        max_x = voxel_o->x + (pos_x * leafsize) + apothem;
        if (src->points[n].x < centroid->x) {
            if (src->points[n].x < min_x) {
                pos_x--;
            } else if (src->points[n].x >= max_x) {
                pos_x++;
            }
        } else if (src->points[n].x > centroid->x) {
            if (src->points[n].x <= min_x) {
                pos_x--;
            } else if (src->points[n].x > max_x) {
                pos_x++;
            }
        }

        d_y = src->points[n].y - voxel_o->y;
        pos_y = (uint) ceil(d_y / leafsize);
        min_y = voxel_o->y + (pos_y * leafsize) - apothem;
        max_y = voxel_o->y + (pos_y * leafsize) + apothem;
        if (src->points[n].y < centroid->y) {
            if (src->points[n].y < min_y) {
                pos_y--;
            } else if (src->points[n].y >= max_y) {
                pos_y++;
            }
        } else if (src->points[n].y > centroid->y) {
            if (src->points[n].y <= min_y) {
                pos_y--;
            } else if (src->points[n].y > max_y) {
                pos_y++;
            }
        }

        d_z = src->points[n].z - voxel_o->z;
        pos_z = (uint) ceil(d_z / leafsize);
        min_z = voxel_o->z + (pos_z * leafsize) - apothem;
        max_z = voxel_o->z + (pos_z * leafsize) + apothem;
        if (src->points[n].z < centroid->z) {
            if (src->points[n].z < min_z) {
                pos_z--;
            } else if (src->points[n].z >= max_z) {
                pos_z++;
            }
        } else if (src->points[n].z > centroid->z) {
            if (src->points[n].z <= min_z) {
                pos_z--;
            } else if (src->points[n].z > max_z) {
                pos_z++;
            }
        }

        cloud_add_point_vector(clouds[pos_x][pos_y][pos_z], &src->points[n]);
    }

    for (uint i = 0; i < num_voxels_x; i++) {
        for (uint j = 0; j < num_voxels_y; j++) {
            for (uint k = 0; k < num_voxels_z; k++) {
                if (clouds[i][j][k]->num_pts != 0) {
                    cloud_add_point_vector(output, 
                                           cloud_calc_center(clouds[i][j][k]));
                }
                cloud_free(clouds[i][j][k]);
            }
        }
    }
    return output;
}

 #endif // VOXELGRID_H