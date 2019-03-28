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
	struct vector3* norm_sagit = vector3_new(1, 0, 0); //ok
	struct vector3* center = cloud_get_center(cloud); //ok
	struct plane* plane_sagit = plane_new(norm_sagit, center); //ok
	struct cloud* left = cloud_empty(); //ok
	struct cloud* right = cloud_empty(); //ok
	
	cloud_plane_partition(cloud, plane_sagit, left, right);
	
	struct matrix* ans_left = (*mfunc)(left); //ok
	struct matrix* ans_right = (*mfunc)(right); //ok
	
	struct vector3* norm_trans = vector3_new(0, 1, 0); //ok
	struct cloud* left_1 = cloud_empty(); //ok
	struct cloud* left_2 = cloud_empty(); //ok
	struct cloud* right_1 = cloud_empty(); //ok
	struct cloud* right_2 = cloud_empty(); //ok
	struct plane* plane_left = plane_new(norm_trans, center); //ok
	struct plane* plane_right = plane_new(norm_trans, center); //ok
	
	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);
	
	struct matrix* ans_left_1 = (*mfunc)(left_1); //ok
	struct matrix* ans_left_2 = (*mfunc)(left_2); //ok
	struct matrix* ans_right_1 = (*mfunc)(right_1); //ok
	struct matrix* ans_right_2 = (*mfunc)(right_2); //ok
	
	struct matrix* ans_full = (*mfunc)(cloud); //ok
	struct matrix* halves = matrix_concat_hor(ans_left, ans_right); //ok
	struct matrix* subleft = matrix_concat_hor(ans_left_1, ans_left_2); //ok
	struct matrix* subright = matrix_concat_hor(ans_right_1, ans_right_2); //ok
	struct matrix* subs = matrix_concat_hor(subleft, subright); //ok
	struct matrix* tris = matrix_concat_hor(ans_full, halves); //ok
	
	struct matrix* ret = matrix_concat_hor(subs, tris);
	
	cloud_save_pcd(cloud, "cloud.pcd");
	cloud_save_pcd(left, "left.pcd");
	cloud_save_pcd(right, "right.pcd");
	cloud_save_pcd(left_1, "left_1.pcd");
	cloud_save_pcd(left_2, "left_2.pcd");
	cloud_save_pcd(right_1, "right_1.pcd");
	cloud_save_pcd(right_2, "right_2.pcd");
	
	vector3_free(norm_sagit);
	vector3_free(center);
	plane_free(plane_sagit);
	cloud_free(left);
	cloud_free(right);
	matrix_free(ans_left);
	matrix_free(ans_right);
	cloud_free(left_1);
	cloud_free(left_2);
	cloud_free(right_1);
	cloud_free(right_2);
	matrix_free(ans_left_1);
	matrix_free(ans_left_2);
	matrix_free(ans_right_1);
	matrix_free(ans_right_2);
	plane_free(plane_right);
	plane_free(plane_left);
	vector3_free(norm_trans);
	matrix_free(ans_full);
	matrix_free(halves);
	matrix_free(subleft);
	matrix_free(subright);
	matrix_free(subs);
	matrix_free(tris);
	
	return ret;
}

int main(int argc, char** argv)
{
	/**
	char *bunnies[] = {"../dump/bunny_raw.xyz",
	                   "../dump/bunny_trans.xyz",
	                   "../dump/bunny_scale.xyz",
	                   "../dump/bunny_rotate.xyz"};
	*/
	char *faces[] = {"../dump/bs000_raw.xyz",
	                 "../dump/bs000_trans.xyz",
	                 "../dump/bs000_scale.xyz",
	                 "../dump/bs000_rotate.xyz"};
	
	for (int i = 0; i < 4; i++) {
		const char* cloud_file = faces[i];
		
		struct cloud* cloud = cloud_load_xyz(cloud_file);
		struct vector3* avg = cloud_average_direction(cloud);
		
		printf("> %s\n", cloud_file);
		vector3_debug(avg, stdout);
		printf("\n");
		
		vector3_free(avg);
		cloud_free(cloud);
	}
	
	return 0;
}

