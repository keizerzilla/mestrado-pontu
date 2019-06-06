#include "../extraction.h"

struct matrix* extraction_plane (struct cloud* cloud,
                                 struct matrix* (*mfunc)(struct cloud*),
                                 struct vector3* norm)
{
	struct cloud* par1 = cloud_empty();
	struct cloud* par2 = cloud_empty();
	struct vector3* pt = cloud_get_center(cloud);
	struct plane* plane = plane_new(norm, pt);
	
	cloud_plane_partition(cloud, plane, par1, par2);
	
	struct matrix* r1 = (*mfunc)(par1);
	struct matrix* r2 = (*mfunc)(par2);
	struct matrix* ans = matrix_concat_hor(r1, r2);
	
	cloud_free(par1);
	cloud_free(par2);
	vector3_free(pt);
	plane_free(plane);
	matrix_free(r1);
	matrix_free(r2);
	
	return ans;
}

struct matrix* extraction_recursive (struct cloud* cloud,
                                     struct matrix* (*mfunc)(struct cloud*),
                                     struct vector3* norm)
{
	struct cloud* par1 = cloud_empty();
	struct cloud* par2 = cloud_empty();
	struct vector3* pt = cloud_get_center(cloud);
	struct plane* plane = plane_new(norm, pt);
	
	cloud_plane_partition(cloud, plane, par1, par2);
	
	struct cloud* par1_fh = cloud_empty();
	struct cloud* par2_fh = cloud_empty();
	struct vector3* pt_fh = cloud_get_center(par1);
	struct plane* plane_fh = plane_new(norm, pt_fh);
	
	cloud_plane_partition(par1, plane_fh, par1_fh, par2_fh);
	
	struct matrix* r1 = matrix_concat_hor((*mfunc)(par1_fh), (*mfunc)(par2_fh));
	struct cloud* par1_sh = cloud_empty();
	struct cloud* par2_sh = cloud_empty();
	struct vector3* pt_sh = cloud_get_center(par2);
	struct plane* plane_sh = plane_new(norm, pt_sh);
	
	cloud_plane_partition(par2, plane_sh, par1_sh, par2_sh);
	
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

struct matrix* extraction_sagittal (struct cloud* cloud,
                                    struct matrix* (*mfunc)(struct cloud*))
{
	return extraction_recursive(cloud, mfunc, vector3_new(1, 0, 0));
}

struct matrix* extraction_transversal (struct cloud* cloud,
                                       struct matrix* (*mfunc)(struct cloud*))
{
	return extraction_recursive(cloud, mfunc, vector3_new(0, 1, 0));
}

struct matrix* extraction_frontal (struct cloud* cloud,
                                   struct matrix* (*mfunc)(struct cloud*))
{
	return extraction_recursive(cloud, mfunc, vector3_new(0, 0, 1));
}

struct matrix* extraction_radial (struct cloud* cloud,
                                  struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* nosetip = cloud_point_faraway_bestfit(cloud);
	real slice = 100.0f / 4.0f;
	
	struct cloud* sub1 = cloud_empty();
	struct cloud* sub2 = cloud_empty();
	struct cloud* sub3 = cloud_empty();
	struct cloud* sub4 = cloud_empty();
	
	real d = 0.0f;
	for (uint i = 0; i < cloud_size(cloud); i++) {
		d = vector3_distance(&cloud->points[i], nosetip);
		
		if (d <= slice)
			cloud_add_point_vector(sub1, &cloud->points[i]);
		else if (d > slice && d <= 2.0f*slice)
			cloud_add_point_vector(sub2, &cloud->points[i]);
		else if (d > 2.0f*slice && d <= 3.0f*slice)
			cloud_add_point_vector(sub3, &cloud->points[i]);
		else
			cloud_add_point_vector(sub4, &cloud->points[i]);
	}
	
	struct matrix* ans1 = matrix_concat_hor((*mfunc)(sub1), (*mfunc)(sub2));
	struct matrix* ans2 = matrix_concat_hor((*mfunc)(sub3), (*mfunc)(sub4));
	struct matrix* ans = matrix_concat_hor(ans1, ans2);
	
	matrix_free(ans1);
	matrix_free(ans2);
	cloud_free(sub4);
	cloud_free(sub3);
	cloud_free(sub2);
	cloud_free(sub1);
	vector3_free(nosetip);
	
	return ans;
}

struct matrix* extraction_upper (struct cloud* cloud,
                                 struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm = vector3_new(0, 1, 0);
	struct vector3* point = cloud_point_faraway_bestfit(cloud);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	struct matrix* ans = (*mfunc)(sub);
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(point);
	vector3_free(norm);
	
	return ans;
}

struct matrix* extraction_lower (struct cloud* cloud,
                                 struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm = vector3_new(0, -1, 0);
	struct vector3* point = cloud_point_faraway_bestfit(cloud);
	struct plane* plane = plane_new(norm, point);
	struct cloud* sub = cloud_cut_plane(cloud, plane);
	struct matrix* ans = (*mfunc)(sub);
	
	cloud_free(sub);
	plane_free(plane);
	vector3_free(point);
	vector3_free(norm);
	
	return ans;
}

struct matrix* extraction_manhattan (struct cloud* cloud,
                                     struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* nosetip = cloud_point_faraway_bestfit(cloud);
	struct cloud* nose = cloud_empty();
	real d = 0.0f;
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		d = vector3_manhattan(&cloud->points[i], nosetip);
		
		if (d <= 150.0f)
			cloud_add_point_vector(nose, &cloud->points[i]);
	}
		
	struct matrix* ans = (*mfunc)(nose);
	
	cloud_free(nose);
	vector3_free(nosetip);
	
	return ans;
}

struct matrix* extraction_4 (struct cloud* cloud,
                             struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm_sagit = vector3_new(1, 0, 0);
	struct vector3* center = cloud_get_center(cloud);
	struct plane* plane_sagit = plane_new(norm_sagit, center);
	struct cloud* left = cloud_empty();
	struct cloud* right = cloud_empty();
	
	cloud_plane_partition(cloud, plane_sagit, left, right);
	
	struct vector3* norm_trans = vector3_new(0, 1, 0);
	struct cloud* left_1 = cloud_empty();
	struct cloud* left_2 = cloud_empty();
	struct cloud* right_1 = cloud_empty();
	struct cloud* right_2 = cloud_empty();
	struct plane* plane_left = plane_new(norm_trans, center);
	struct plane* plane_right = plane_new(norm_trans, center);
	
	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);
	
	struct matrix* ans_left_1 = (*mfunc)(left_1);
	struct matrix* ans_left_2 = (*mfunc)(left_2);
	struct matrix* ans_right_1 = (*mfunc)(right_1);
	struct matrix* ans_right_2 = (*mfunc)(right_2);
	
	struct matrix* subleft = matrix_concat_hor(ans_left_1, ans_left_2);
	struct matrix* subright = matrix_concat_hor(ans_right_1, ans_right_2);
	struct matrix* ret = matrix_concat_hor(subleft, subright);
	
	vector3_free(norm_sagit);
	vector3_free(center);
	plane_free(plane_sagit);
	cloud_free(left);
	cloud_free(right);
	vector3_free(norm_trans);
	cloud_free(left_1);
	cloud_free(left_2);
	cloud_free(right_1);
	cloud_free(right_2);
	plane_free(plane_left);
	plane_free(plane_right);
	matrix_free(ans_left_1);
	matrix_free(ans_left_2);
	matrix_free(ans_right_1);
	matrix_free(ans_right_2);
	matrix_free(subleft);
	matrix_free(subright);
	
	return ret;
}

struct matrix* extraction_6 (struct cloud* cloud,
                             struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm_sagit = vector3_new(1, 0, 0);
	struct vector3* center = cloud_get_center(cloud);
	struct plane* plane_sagit = plane_new(norm_sagit, center);
	struct cloud* left = cloud_empty();
	struct cloud* right = cloud_empty();
	
	cloud_plane_partition(cloud, plane_sagit, left, right);
	
	struct vector3* norm_trans = vector3_new(0, 1, 0);
	struct cloud* left_1 = cloud_empty();
	struct cloud* left_2 = cloud_empty();
	struct cloud* right_1 = cloud_empty();
	struct cloud* right_2 = cloud_empty();
	struct plane* plane_left = plane_new(norm_trans, center);
	struct plane* plane_right = plane_new(norm_trans, center);
	
	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);
	
	struct matrix* ans_left = (*mfunc)(left);
	struct matrix* ans_right = (*mfunc)(right);
	struct matrix* ans_left_1 = (*mfunc)(left_1);
	struct matrix* ans_left_2 = (*mfunc)(left_2);
	struct matrix* ans_right_1 = (*mfunc)(right_1);
	struct matrix* ans_right_2 = (*mfunc)(right_2);
	
	struct matrix* full = matrix_concat_hor(ans_left, ans_right);
	struct matrix* subleft = matrix_concat_hor(ans_left_1, ans_left_2);
	struct matrix* subright = matrix_concat_hor(ans_right_1, ans_right_2);
	struct matrix* subs = matrix_concat_hor(subleft, subright);
	struct matrix* ret = matrix_concat_hor(full, subs);
	
	vector3_free(norm_sagit);
	vector3_free(center);
	plane_free(plane_sagit);
	cloud_free(left);
	cloud_free(right);
	vector3_free(norm_trans);
	cloud_free(left_1);
	cloud_free(left_2);
	cloud_free(right_1);
	cloud_free(right_2);
	plane_free(plane_left);
	plane_free(plane_right);
	matrix_free(ans_left);
	matrix_free(ans_right);
	matrix_free(ans_left_1);
	matrix_free(ans_left_2);
	matrix_free(ans_right_1);
	matrix_free(ans_right_2);
	matrix_free(full);
	matrix_free(subleft);
	matrix_free(subright);
	matrix_free(subs);
	
	return ret;
}

struct matrix* extraction_7 (struct cloud* cloud,
                             struct matrix* (*mfunc)(struct cloud*))
{
	struct vector3* norm_sagit = vector3_new(1, 0, 0);
	struct vector3* center = cloud_get_center(cloud);
	struct plane* plane_sagit = plane_new(norm_sagit, center);
	struct cloud* left = cloud_empty();
	struct cloud* right = cloud_empty();
	
	cloud_plane_partition(cloud, plane_sagit, left, right);
	
	struct matrix* ans_left = (*mfunc)(left);
	struct matrix* ans_right = (*mfunc)(right);
	
	struct vector3* norm_trans = vector3_new(0, 1, 0);
	struct cloud* left_1 = cloud_empty();
	struct cloud* left_2 = cloud_empty();
	struct cloud* right_1 = cloud_empty();
	struct cloud* right_2 = cloud_empty();
	struct plane* plane_left = plane_new(norm_trans, center);
	struct plane* plane_right = plane_new(norm_trans, center);
	
	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);
	
	struct matrix* ans_left_1 = (*mfunc)(left_1);
	struct matrix* ans_left_2 = (*mfunc)(left_2);
	struct matrix* ans_right_1 = (*mfunc)(right_1);
	struct matrix* ans_right_2 = (*mfunc)(right_2);
	
	struct matrix* ans_full = (*mfunc)(cloud);
	struct matrix* halves = matrix_concat_hor(ans_left, ans_right);
	struct matrix* subleft = matrix_concat_hor(ans_left_1, ans_left_2);
	struct matrix* subright = matrix_concat_hor(ans_right_1, ans_right_2);
	struct matrix* subs = matrix_concat_hor(subleft, subright);
	struct matrix* tris = matrix_concat_hor(ans_full, halves);
	
	struct matrix* ret = matrix_concat_hor(subs, tris);
	
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

struct cloud* extraction_vshape_base (struct cloud* cloud)
{
	real y_margin = 15.0f;
	real x_margin = 25.0f;
	
	struct vector3* nosetip = cloud_point_faraway_bestfit(cloud);
	nosetip->y += y_margin;
	struct vector3* diry = vector3_new(0, 1, 0);
	struct vector3* dirx = vector3_new(1, 0, 0);
	struct plane* plane = plane_new(diry, nosetip);
	struct cloud* upper = cloud_cut_plane(cloud, plane);
	nosetip->y -= y_margin;
	struct cloud* nose = cloud_segment(cloud, nosetip, diry, y_margin);
	struct cloud* nose_slice = cloud_segment(nose, nosetip, dirx, x_margin);
	
	struct cloud* concat = cloud_concat(nose_slice, upper);
	
	cloud_free(nose_slice);
	cloud_free(nose);
	cloud_free(upper);
	plane_free(plane);
	vector3_free(dirx);
	vector3_free(diry);
	vector3_free(nosetip);
	
	return concat;
}

struct matrix* extraction_vshape (struct cloud* cloud,
                                  struct matrix* (*mfunc)(struct cloud*))
{
	struct cloud* seg = extraction_vshape_base(cloud);
	struct matrix* ans = (*mfunc)(seg);
	
	cloud_free(seg);
	
	return ans;
}

struct matrix* extraction_vshape_f (struct cloud* cloud,
                                    struct matrix* (*mfunc)(struct cloud*))
{
	struct cloud* seg = extraction_vshape_base(cloud);
	struct matrix* ans = extraction_frontal(seg, mfunc);
	
	cloud_free(seg);
	
	return ans;
}

struct matrix* extraction_vshape_s (struct cloud* cloud,
                                    struct matrix* (*mfunc)(struct cloud*))
{
	struct cloud* seg = extraction_vshape_base(cloud);
	struct matrix* ans = extraction_sagittal(seg, mfunc);
	
	cloud_free(seg);
	
	return ans;
}

struct matrix* extraction_vshape_t (struct cloud* cloud,
                                    struct matrix* (*mfunc)(struct cloud*))
{
	struct cloud* seg = extraction_vshape_base(cloud);
	struct matrix* ans = extraction_transversal(seg, mfunc);
	
	cloud_free(seg);
	
	return ans;
}

