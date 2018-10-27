#include "../../src/include/cloud.h"
#include "../../src/include/matrix.h"

int main(int argc, char** argv)
{
	struct cloud* src = cloud_load_xyz(argv[1]);
	struct cloud* par1 = cloud_empty();
	struct cloud* par2 = cloud_empty();
	
	struct vector3* norm = vector3_new(1, 0, 0);
	struct vector3* pt = cloud_get_center(src);
	struct plane* plane = plane_new(norm, pt);
	
	cloud_plane_partition(src, plane, par1, par2);
	cloud_save_xyz(par1, "../../dump/par1.xyz");
	cloud_save_xyz(par2, "../../dump/par2.xyz");
	
	plane_free(plane);
	vector3_free(pt);
	vector3_free(norm);
	cloud_free(par2);
	cloud_free(par1);
	cloud_free(src);
	
	return 0;
}

