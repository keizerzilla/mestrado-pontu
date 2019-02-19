#ifndef SPHERIC_H
#define SPHERIC_H

#include "cloud.h"
#include "matrix.h"

/**
 * \brief TODO
 * \param TODO
 * \return TODO
 */
real spheric_quad(real x, real y, real z)
{
	return (x * x) + (y * y) + (z * z);
}

/**
 * \brief TODO
 * \param TODO
 * \return TODO
 */
real spheric_moment(int p, int q, int r, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real moment = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;
		
        moment += pow(center_x, p)
                * pow(center_y, q)
                * pow(center_z, r)
                * spheric_quad(center_x, center_y, center_z)
                * vector3_distance(&cloud->points[i], center);
	}
	
	return moment / cloud_max_distance_from_center(cloud);
}

/**
 * \brief TODO
 * \param TODO
 * \return TODO
 */
real spheric_normmoment(int p, int q, int r, struct cloud* cloud)
{
	real central = spheric_moment(p, q, r, cloud);
	real zero = spheric_moment(0, 0, 0, cloud);
	
	return central / zero;
}

/**
 * \brief TODO
 * \param TODO
 * \return TODO
 */
real spheric_quadmoment(int p, int q, int r, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real moment = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;
		
        moment += spheric_quad(center_x, center_y, center_z);
	}
	
	return moment;
}

/**
 * \brief TODO
 * \param TODO
 * \return TODO
 */
struct matrix* spheric_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, 27);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= 2; p++) {
        for (q = 0; q <= 2; q++) {
            for (r = 0; r <= 2; r++) {
                matrix_set(results, 0, col, spheric_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

#endif // SPHERIC_H

