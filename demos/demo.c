#include "../src/include/siqueira.h"

int main(int argc, char** argv)
{
	const char* clouds[13] = {"face0.xyz", "face30.xyz", "face60.xyz",
	                          "face90.xyz", "face120.xyz", "face150.xyz",
	                          "face180.xyz", "face210.xyz", "face240.xyz",
	                          "face270.xyz", "face300.xyz", "face330.xyz"};
	
	for (int i = 0; i < 12; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		struct vector3* nose = cloud_min_z(cloud);
		struct matrix* ans = siqueira_cloud_moments(cloud, nose);
		
		matrix_debug(ans, stdout);
		
		matrix_free(ans);
		vector3_free(nose);
		cloud_free(cloud);
	}
	
	return 0;
}

