#include "../src/include/chebyshev.h"
#include "../src/include/hu.h"

#define MAX_ORDER 1

real regular(int p, int q, int r, struct cloud* cloud)
{
    real moment = 0.0f;
    
    struct vector3* center = cloud_get_center(cloud);
    
    for (uint i = 0; i < cloud->num_pts; i++) {
        moment += pow(cloud->points[i].x, p)
                * pow(cloud->points[i].y, q)
                * pow(cloud->points[i].z, r)
                * vector3_distance(&cloud->points[i], center);
	}
	
	vector3_free(center);
	
    return moment;
}

real gmi(int p, int q, int r, struct cloud* cloud)
{
	real m = 0;
	
	real zero = regular(0, 0, 0, cloud);
	
	real u011 = hu_central_moment(0, 1, 1, cloud);
	real u020 = hu_central_moment(0, 2, 0, cloud);
	real u002 = hu_central_moment(0, 0, 2, cloud);
	real u101 = hu_central_moment(1, 0, 1, cloud);
	real u200 = hu_central_moment(2, 0, 0, cloud);
	real u110 = hu_central_moment(1, 1, 0, cloud);
	
	real theta = 0.5f * atan2(2*u011, u020 - u002);
	real phi = 0.5f * atan2(2*u101, u200 - u002);
	real psi = 0.5f * atan2(2*u110, u200 - u020);
	
	real cos_theta = cos(theta);
	real sin_theta = sin(theta);
	real cos_phi = cos(phi);
	real sin_phi = sin(phi);
	real cos_psi = cos(psi);
	real sin_psi = sin(psi);
	
	struct vector3* center = cloud_get_center(cloud);
	
	for (int i = 0; i < cloud_size(cloud); i++) {
		real x = cloud->points[i].x - center->x;
		real y = cloud->points[i].y - center->y;
		real z = cloud->points[i].z - center->z;
		
		real x_theta = x;
		real y_theta = cos_theta*y + sin_theta*z;
		real z_theta = cos_theta*z - sin_theta*y;
		
		real x_phi = cos_phi*x - sin_phi*z;
		real y_phi = y;
		real z_phi = sin_phi*x + cos_phi*z;
		
		real x_psi = cos_psi*x + sin_psi*y;
		real y_psi = cos_psi*y - sin_psi*x;
		real z_psi = z;
		
		real distance = vector3_distance(&cloud->points[i], center);
		
		m += pow(x_theta * y_theta * z_theta, p)
		   * pow(x_phi * y_phi * z_phi, q)
		   * pow(x_psi * y_psi * z_psi, r)
		   * distance;
	}
	
	vector3_free(center);
	
	return m / pow(zero, (p + q + r)/3.0f);
}

int main(int argc, char** argv)
{
	const char* clouds[2] = {"../dump/bunny_raw.xyz",
	                         "../dump/bunny_rotate.xyz"};
	
	for (int i = 0; i < 2; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		
		printf("%s\n", clouds[i]);
		
		for (int p = 0; p <= MAX_ORDER; p++)
			for (int q = 0; q <= MAX_ORDER; q++)
				for (int r = 0; r <= MAX_ORDER; r++)
					printf("%le ", gmi(p, q, r, cloud));
		
		printf("\n");
		
		cloud_free(cloud);
	}
	
	return 0;
}

