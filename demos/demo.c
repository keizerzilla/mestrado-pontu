#include "../src/include/hu.h"
#include "../src/include/golden.h"
#include "../src/include/matrix.h"
#include "../src/include/spheric.h"
#include "../src/include/legendre.h"
#include "../src/include/chebyshev.h"

//------------------------------------------------------------------------------

real refined_moment(int p,
                    int q,
                    int r,
                    struct cloud* cloud,
                    real (*mfunc)(int, int, int, struct cloud*))
{
	/**
	real central = (*mfunc)(p, q, r, cloud);
	real zero = (*mfunc)(0, 0, 0, cloud);
	real x = (*mfunc)(p, 0, 0, cloud);
	real y = (*mfunc)(0, q, 0, cloud);
	real z = (*mfunc)(0, 0, r, cloud);
	
	return (central * pow(zero, 2)) / (x * y * z); // t+s
	**/
	
	real central = (*mfunc)(p, q, r, cloud);
	real zero = (*mfunc)(0, 0, 0, cloud);
	
	return central / pow(zero, 3); // t+s+r
	
	//real norm = 1.0f;
    //return (*mfunc)(p, q, r, cloud) / norm; // t+r
}

struct matrix* invariant_moments(struct cloud* cloud,
                                 real (*mfunc)(int, int, int, struct cloud*))
{
	struct matrix* results = matrix_new(1, 3);
	
    real m200 = refined_moment(2, 0, 0, cloud, mfunc);
    real m020 = refined_moment(0, 2, 0, cloud, mfunc);
    real m002 = refined_moment(0, 0, 2, cloud, mfunc);
    real m110 = refined_moment(1, 1, 0, cloud, mfunc);
    real m101 = refined_moment(1, 0, 1, cloud, mfunc);
    real m011 = refined_moment(0, 1, 1, cloud, mfunc);

    real i1 = m200 + m020 + m002;
    real i2 = m200*m020 + m200*m002 + m020*m002 -
              m110*m110 - m101*m101 - m011*m011;
    real i3 = m200*m020*m002 + 2*m110*m101*m011 -
              m002*m110*m110 - m020*m101*m101 - m200*m011*m011;
	
    matrix_set(results, 0, 0, i1);
    matrix_set(results, 0, 1, i2);
    matrix_set(results, 0, 2, i3);

    return results;
}

//------------------------------------------------------------------------------

int main()
{
	char* clouds[4] = {"../dump/bunny_raw.xyz",
	                   "../dump/bunny_trans.xyz",
	                   "../dump/bunny_scale_centroid.xyz",
	                   "../dump/bunny_rotate.xyz"};
	
	for (int i = 0; i < 4; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		struct matrix* ans = invariant_moments(cloud, golden_moment);
		//struct matrix* ans = chebyshev_invariant_moments(cloud);
		//struct matrix* ans = legendre_invariant_moments(cloud);
		//struct vector3* avg = cloud_average_direction(cloud);
		
		printf("%s\n", clouds[i]);
		matrix_debug(ans, stdout);
		//vector3_debug(avg, stdout);
		printf("\n");
		
		//vector3_free(avg);
		matrix_free(ans);
		cloud_free(cloud);
	}
	
	return 0;
}

