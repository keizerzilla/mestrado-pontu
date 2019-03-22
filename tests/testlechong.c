#define _GNU_SOURCE

#include "../src/include/lechong.h"
#include "../src/include/legendre.h"
#include "../src/include/spheric.h"
#include "../src/include/matrix.h"
#include "../src/include/plane.h"

/**
 * \brief 7 segmentos
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return 7 segmentos
 */
struct matrix* extraction_7(struct cloud* cloud,
                            struct matrix* (*mfunc)(struct cloud*))
{
	// depois, faz um corte sagital e extrai das duas metades
	struct vector3* norm_sagit = vector3_new(1, 0, 0); //ok
	struct vector3* center = cloud_get_center(cloud); //ok
	struct plane* plane_sagit = plane_new(norm_sagit, center); //ok
	struct cloud* left = cloud_empty();
	struct cloud* right = cloud_empty();
	
	cloud_plane_partition(cloud, plane_sagit, left, right);
	
	struct matrix* ans_left = (*mfunc)(left);
	struct matrix* ans_right = (*mfunc)(right);
	
	plane_free(plane_sagit);
	vector3_free(norm_sagit);
	
	// por fim, corta-se left e right transversalmente, e extrai dos 4 pedacos
	struct vector3* norm_trans = vector3_new(0, 1, 0); //ok
	struct cloud* left_1 = cloud_empty();
	struct cloud* left_2 = cloud_empty();
	struct cloud* right_1 = cloud_empty();
	struct cloud* right_2 = cloud_empty();
	struct plane* plane_left = plane_new(norm_trans, center); //ok
	struct plane* plane_right = plane_new(norm_trans, center); //ok
	
	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);
	
	struct matrix* ans_left_1 = (*mfunc)(left_1);
	struct matrix* ans_left_2 = (*mfunc)(left_2);
	struct matrix* ans_right_1 = (*mfunc)(right_1);
	struct matrix* ans_right_2 = (*mfunc)(right_2);
	
	plane_free(plane_right);
	plane_free(plane_left);
	vector3_free(norm_trans);
	
	struct matrix* ans_full = (*mfunc)(cloud);
	struct matrix* sub = matrix_concat_hor(left, right);
	struct matrix* subleft = matrix_concat_hor(ans_left_1, ans_left_2);
	struct matrix* subright = matrix_concat_hor(ans_right_1, ans_right_2);
	
	// CONTINUAR AQUI
	
	vector3_free(center);
	
	return NULL;
}

int main(int argc, char** argv)
{
	char *clouds[] = {"../dump/bunny_raw.xyz", "../dump/bunny_trans.xyz", "../dump/bunny_scale.xyz", "../dump/bunny_rotate.xyz"};
	//char *clouds[] = {"../dump/bs000_raw.xyz", "../dump/bs000_trans.xyz", "../dump/bs000_scale.xyz", "../dump/bs000_rotate.xyz"};
	
	for (int i = 0; i < 4; i++) {
		struct cloud* cloud = cloud_load_xyz(clouds[i]);
		
		struct matrix* ans = lechong_cloud_moments(cloud);
		//struct matrix* ans = legendre_cloud_moments(cloud);
		//struct matrix* ans = spheric_cloud_moments(cloud);
		
		printf("> %s\n", clouds[i]);
		matrix_debug(ans, stdout);
		printf("\n");
		
		matrix_free(ans);
		cloud_free(cloud);
	}
	
	return 0;
}

