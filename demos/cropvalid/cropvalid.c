#define _GNU_SOURCE

#include "../../src/include/hu.h"
#include "../../src/include/legendre.h"
#include "../../src/include/spheric.h"
#include "../../src/include/plane.h"

/**
 * \brief Extrái momentos usando cortes a partir de planos recursivos
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \param O vetor normal ao plano de corte
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_plane(struct cloud* cloud,
                                struct matrix* (*mfunc)(struct cloud*),
                                struct vector3* norm)
{
	struct cloud* par1 = cloud_empty();
	struct cloud* par2 = cloud_empty();
	struct vector3* pt = cloud_get_center(cloud);
	struct plane* plane = plane_new(norm, pt);
	
	cloud_plane_partition(cloud, plane, par1, par2);
	cloud_save_xyz(par1, "par1.xyz");
	cloud_save_xyz(par2, "par2.xyz");
	
	struct cloud* par1_fh = cloud_empty();
	struct cloud* par2_fh = cloud_empty();
	struct vector3* pt_fh = cloud_get_center(par1);
	struct plane* plane_fh = plane_new(norm, pt_fh);
	
	cloud_plane_partition(par1, plane_fh, par1_fh, par2_fh);
	cloud_save_xyz(par1_fh, "par1_fh.xyz");
	cloud_save_xyz(par2_fh, "par2_fh.xyz");
	
	struct matrix* r1 = matrix_concat_hor((*mfunc)(par1_fh), (*mfunc)(par2_fh));
	struct cloud* par1_sh = cloud_empty();
	struct cloud* par2_sh = cloud_empty();
	struct vector3* pt_sh = cloud_get_center(par2);
	struct plane* plane_sh = plane_new(norm, pt_sh);
	
	cloud_plane_partition(par2, plane_sh, par1_sh, par2_sh);
	cloud_save_xyz(par1_sh, "par1_sh.xyz");
	cloud_save_xyz(par2_sh, "par2_sh.xyz");
	
	struct matrix* r2 = matrix_concat_hor((*mfunc)(par1_sh), (*mfunc)(par2_sh));
	struct matrix* ans = matrix_concat_hor(r1, r2);
	
	plane_free(plane_fh);
	vector3_free(pt_fh);
	cloud_free(par2_fh);
	cloud_free(par1_fh);
	plane_free(plane_sh);
	vector3_free(pt_sh);
	cloud_free(par2_sh);
	cloud_free(par1_sh);
	matrix_free(r2);
	matrix_free(r1);
	plane_free(plane);
	vector3_free(pt);
	vector3_free(norm);
	cloud_free(par2);
	cloud_free(par1);
	
	return ans;
}

// -------------------------------------------------------------------------- //

/**
 * \brief Extrái momentos usando cortes radiais
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_radial(struct cloud* cloud,
                                 struct matrix* (*mfunc)(struct cloud*))
{
	real d = 0.0f;
	//real slice = cloud_max_distance_from_center(cloud) / 2.0f;
	real slice = cloud_max_distance(cloud, cloud_min_z(cloud)) / 2.0f;
	
	struct vector3* center = cloud_get_center(cloud);
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		d = vector3_distance(&cloud->points[i], center);
		
		if (d <= slice)
			cloud_add_point(sub1, &cloud->points[i]);
		else
			cloud_add_point(sub2, &cloud->points[i]);
	}
	
	cloud_save_xyz(sub1, "sub1.xyz");
	cloud_save_xyz(sub2, "sub2.xyz");
	
	struct matrix* ans = matrix_concat_hor((*mfunc)(sub1), (*mfunc)(sub2));
	
	cloud_free(sub2);
	cloud_free(sub1);
	vector3_free(center);
	
	return ans;
}

/**
 * \brief Extrái momentos usando a parte superior da nuvem (já alinhada)
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_upper(struct cloud* cloud,
                                struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm = vector3_new(0, 1, 0);
	struct vector3* point = cloud_min_z(cloud);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	struct matrix* ans = (*mfunc)(sub);
	
	cloud_save_xyz(sub, "upper.xyz");
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(point);
	vector3_free(norm);
	
	return ans;
}

/**
 * \brief Extrái momentos usando a parte inferior da nuvem (já alinhada)
 * \param cloud A nuvem alvo
 * \param mfunc A função extratora de momentos
 * \return A matrix com os momentos extraídos
 */
struct matrix* extraction_lower(struct cloud* cloud,
                                struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm = vector3_new(0, -1, 0);
	struct vector3* point = cloud_min_z(cloud);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	struct matrix* ans = (*mfunc)(sub);
	
	cloud_save_xyz(sub, "lower.xyz");
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(point);
	vector3_free(norm);
	
	return ans;
}

int main(int argc, char** argv)
{
	struct cloud* raw = cloud_load_xyz("../../datasets/bosphorus-outlier-densit200-crop80-icp/neutral/bs000_N_N_0.xyz");
	struct matrix* (*mfunc)(struct cloud*) = &legendre_cloud_moments;
	
	//extraction_plane(raw, mfunc, vector3_new(1, 0, 0));
	extraction_plane(raw, mfunc, vector3_new(0, 1, 0));
	//extraction_plane(raw, mfunc, vector3_new(0, 0, 1));
	//matrix_debug(extraction_radial(raw, mfunc), stdout);
	//matrix_debug(extraction_upper(raw, mfunc), stdout);
	//matrix_debug(extraction_lower(raw, mfunc), stdout);
	
	cloud_free(raw);
	
	return 0;
}

