#include "../src/include/hu.h"
#include "../src/include/matrix.h"
#include "../src/include/spheric.h"
#include "../src/include/legendre.h"
#include "../src/include/chebyshev.h"
#include "../src/include/tutu.h"
#include "../src/include/extraction.h"
#include "../src/include/kdtree.h"

int main(int argc, char** argv)
{
	struct cloud* cloud = cloud_load_xyz(argv[1]);
	
	struct cloud* seg = extraction_vshape_base(cloud);
	cloud_save_pcd(seg, "seg.pcd");
	
	cloud_free(seg);
	cloud_free(cloud);
	
	return 0;
}

