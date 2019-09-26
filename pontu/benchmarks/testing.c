#include "../pontu_core.h"
#include "../pontu_features.h"
#include "../pontu_sampling.h"
#include "../pontu_registration.h"

int main()
{
	struct cloud *source = cloud_load_xyz("../samples/bunny.xyz");
	
	if (source != NULL) {
		printf("Dados ok!\n");
	} else {
		printf("Erro ao carregar...\n");
		return 1;
	}
	
	cloud_free(&source); // ok
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	cloud_free(&source); // eh pra xiar
	
	return 0;
}

