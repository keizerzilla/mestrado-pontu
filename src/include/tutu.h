/**
 * \file tutu.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief TUTU
 */

#ifndef TUTU_H
#define TUTU_H

#define TUTU_ORDER 2
#define TUTU_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

real tutu_quad(real x, real y, real z, int p, int q, int r)
{
	return sqrt(pow(x, 2*p) + pow(y, 2*q) + pow(z, 2*r));
}

real tutu_geometric(int p, int q, int r, struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	
	real geometric = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		center_x = cloud->points[i].x - center->x;
		center_y = cloud->points[i].y - center->y;
		center_z = cloud->points[i].z - center->z;
		
		geometric += pow(center_x, p)
                   * pow(center_y, q)
                   * pow(center_z, r);
                   //* tutu_quad(center_x, center_y, center_z, p, q, r);
                   //* vector3_distance(center, &cloud->points[i]);
                   //* vector3_chebyshev(center, &cloud->points[i]);
                   //* vector3_mse(center, &cloud->points[i]);
                   //* vector3_sse(center, &cloud->points[i]);
	}
	
	vector3_free(center);
	
	return geometric;
}

real tutu_normalized(int p, int q, int r, struct cloud* cloud)
{
	/** SOLUCAO PRA ESCALA - NESTED LOOP DE 0 A ORDEM
	real central = tutu_geometric(p, q, r, cloud);
	real zero = tutu_geometric(0, 0, 0, cloud);
	real mx = tutu_geometric(p, 0, 0, cloud);
	real my = tutu_geometric(0, q, 0, cloud);
	real mz = tutu_geometric(0, 0, r, cloud);
	
	return (central * pow(zero, 2)) / (mx * my * mz);
	*/
	
	return tutu_geometric(p, q, r, cloud) / cloud_boundingbox_volume(cloud);
}

real tutu_moment(int p, int q, int r, struct cloud* cloud)
{
	/** SOLUCAO PRA ESCALA - NESTED LOOP DE 0 A ORDEM
	real moment = 0.0f;
		
	for (uint i = 0; i <= p; i++) {
		for (uint j = 0; j <= q; j++) {
			for (uint k = 0; k <= r; k++) {
				moment += tutu_normalized(i, j, k, cloud);
			}
		}
	}
	
	return moment;
	*/
	
	return tutu_normalized(p, q, r, cloud);
}

struct matrix* tutu_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, TUTU_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= TUTU_ORDER; p++) {
        for (q = 0; q <= TUTU_ORDER; q++) {
            for (r = 0; r <= TUTU_ORDER; r++) {
                matrix_set(results, 0, col, tutu_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

struct matrix* tutu_invariant_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, 5);
	
	real m200 = tutu_moment(2, 0, 0, cloud);
    real m020 = tutu_moment(0, 2, 0, cloud);
    real m002 = tutu_moment(0, 0, 2, cloud);
    real m110 = tutu_moment(1, 1, 0, cloud);
    real m101 = tutu_moment(1, 0, 1, cloud);
    real m011 = tutu_moment(0, 1, 1, cloud);
	
    real i1 = m200 + m020 + m002;
    real i2 = m200*m020 + m200*m002 + m020*m002 -
              m110*m110 - m101*m101 - m011*m011;
    real i3 = m200*m020*m002 + 2*m110*m101*m011 -
              m002*m110*m110 - m020*m101*m101 - m200*m011*m011;
	
	real j2 = i1*i1 - 2*i2;
	real j3 = i1*i1*i1 - 3*i1*i2 + 3*i3;
	
    matrix_set(results, 0, 0, i1);
    matrix_set(results, 0, 1, i2);
    matrix_set(results, 0, 2, i3);
    matrix_set(results, 0, 3, j2);
    matrix_set(results, 0, 4, j3);
    
    return results;
}

#endif // TUTU_H

