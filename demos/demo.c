#include "../src/include/chebyshev.h"

int main(int argc, char** argv)
{
	struct cloud* cloud = cloud_load_xyz("../dump/bunny_raw.xyz");
	struct matrix* ans = chebyshev_cloud_moments(cloud);
	struct cloud* rec = chebyshev_reconstruction(ans, cloud_size(cloud));
	
	cloud_save_xyz(rec, "../dump/bunny_rec.xyz");
	
	cloud_free(rec);
	matrix_free(ans);
	cloud_free(cloud);
	
	return 0;
}

