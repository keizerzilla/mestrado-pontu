#include "../src/include/hu.h"
#include "../src/include/golden.h"
#include "../src/include/matrix.h"
#include "../src/include/spheric.h"
#include "../src/include/legendre.h"
#include "../src/include/chebyshev.h"
#include "../src/include/tutu.h"
#include "../src/include/extraction.h"

void extraction_tshape(struct cloud* cloud)
{
	
}

int main(int argc, char** argv)
{
	struct cloud* cloud = cloud_load_xyz("../dump/bs000_raw.xyz");
	
	extraction_tshape(cloud);
	
	cloud_free(cloud);
	
	return 0;
}

