#include "../include/extraction.h"

struct dataframe *extraction_plane(struct cloud *cloud,
				                struct dataframe *(*mfunc) (struct cloud *),
				                struct vector3 *norm)
{
	struct cloud *par1 = cloud_new();
	struct cloud *par2 = cloud_new();
	struct vector3 *pt = cloud_get_centroid(cloud);
	struct plane *plane = plane_new(norm, pt);

	cloud_plane_partition(cloud, plane, par1, par2);

	struct dataframe *r1 = (*mfunc) (par1);
	struct dataframe *r2 = (*mfunc) (par2);
	struct dataframe *ans = dataframe_concat_hor(r1, r2);

	cloud_free(&par1);
	cloud_free(&par2);
	vector3_free(&pt);
	plane_free(&plane);
	dataframe_free(&r1);
	dataframe_free(&r2);

	return ans;
}

struct dataframe *extraction_recursive(struct cloud *cloud,
				                    struct dataframe *(*mfunc) (struct cloud *),
				                    struct vector3 *norm)
{
	struct cloud *par1 = cloud_new();
	struct cloud *par2 = cloud_new();
	struct vector3 *pt = cloud_get_centroid(cloud);
	struct plane *plane = plane_new(norm, pt);

	cloud_plane_partition(cloud, plane, par1, par2);

	struct cloud *par1_fh = cloud_new();
	struct cloud *par2_fh = cloud_new();
	struct vector3 *pt_fh = cloud_get_centroid(par1);
	struct plane *plane_fh = plane_new(norm, pt_fh);

	cloud_plane_partition(par1, plane_fh, par1_fh, par2_fh);

	struct dataframe *r1 = dataframe_concat_hor((*mfunc) (par1_fh),
	                                      (*mfunc) (par2_fh));
	struct cloud *par1_sh = cloud_new();
	struct cloud *par2_sh = cloud_new();
	struct vector3 *pt_sh = cloud_get_centroid(par2);
	struct plane *plane_sh = plane_new(norm, pt_sh);

	cloud_plane_partition(par2, plane_sh, par1_sh, par2_sh);

	struct dataframe *r2 = dataframe_concat_hor((*mfunc) (par1_sh),
	                                      (*mfunc) (par2_sh));
	struct dataframe *ans = dataframe_concat_hor(r1, r2);

	plane_free(&plane_fh);
	vector3_free(&pt_fh);
	cloud_free(&par2_fh);
	cloud_free(&par1_fh);
	plane_free(&plane_sh);
	vector3_free(&pt_sh);
	cloud_free(&par2_sh);
	cloud_free(&par1_sh);
	dataframe_free(&r2);
	dataframe_free(&r1);
	plane_free(&plane);
	vector3_free(&pt);
	vector3_free(&norm);
	cloud_free(&par2);
	cloud_free(&par1);

	return ans;
}

struct dataframe *extraction_sagittal(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *))
{
	return extraction_recursive(cloud, mfunc, vector3_new(1, 0, 0));
}

struct dataframe *extraction_transversal(struct cloud *cloud,
				                    struct dataframe *(*mfunc) (struct cloud *))
{
	return extraction_recursive(cloud, mfunc, vector3_new(0, 1, 0));
}

struct dataframe *extraction_frontal(struct cloud *cloud,
				                  struct dataframe *(*mfunc) (struct cloud *))
{
	return extraction_recursive(cloud, mfunc, vector3_new(0, 0, 1));
}

struct dataframe *extraction_radial(struct cloud *cloud,
				                 struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *nosetip = cloud_point_faraway_bestfit(cloud);
	real slice = 25.0;

	struct cloud *sub1 = cloud_new();
	struct cloud *sub2 = cloud_new();
	struct cloud *sub3 = cloud_new();
	struct cloud *sub4 = cloud_new();

	real d = 0.0;
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		d = vector3_distance(set->point, nosetip);

		if (d <= slice)
			cloud_insert_vector3(sub1, set->point);
		else if (d > slice && d <= 2.0 * slice)
			cloud_insert_vector3(sub2, set->point);
		else if (d > 2.0 * slice && d <= 3.0 * slice)
			cloud_insert_vector3(sub3, set->point);
		else
			cloud_insert_vector3(sub4, set->point);
	}

	struct dataframe *ans1 = dataframe_concat_hor((*mfunc) (sub1),
	                                              (*mfunc) (sub2));
	struct dataframe *ans2 = dataframe_concat_hor((*mfunc) (sub3),
	                                              (*mfunc) (sub4));
	struct dataframe *ans = dataframe_concat_hor(ans1, ans2);

	dataframe_free(&ans1);
	dataframe_free(&ans2);
	cloud_free(&sub4);
	cloud_free(&sub3);
	cloud_free(&sub2);
	cloud_free(&sub1);
	vector3_free(&nosetip);

	return ans;
}

struct dataframe *extraction_upper(struct cloud *cloud,
				                struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *norm = vector3_new(0, 1, 0);
	struct vector3 *point = cloud_point_faraway_bestfit(cloud);
	struct plane *plane = plane_new(norm, point);
	struct cloud *sub = cloud_cut_plane(cloud, plane);
	struct dataframe *ans = (*mfunc) (sub);

	cloud_free(&sub);
	plane_free(&plane);
	vector3_free(&point);
	vector3_free(&norm);

	return ans;
}

struct dataframe *extraction_lower(struct cloud *cloud,
				                struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *norm = vector3_new(0, -1, 0);
	struct vector3 *point = cloud_point_faraway_bestfit(cloud);
	struct plane *plane = plane_new(norm, point);
	struct cloud *sub = cloud_cut_plane(cloud, plane);
	struct dataframe *ans = (*mfunc) (sub);

	cloud_free(&sub);
	plane_free(&plane);
	vector3_free(&point);
	vector3_free(&norm);

	return ans;
}

struct dataframe *extraction_manhattan(struct cloud *cloud,
				                    struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *nosetip = cloud_point_faraway_bestfit(cloud);
	struct cloud *nose = cloud_new();
	real d = 0.0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		d = vector3_manhattan(set->point, nosetip);

		if (d <= 150.0)
			cloud_insert_vector3(nose, set->point);
	}

	struct dataframe *ans = (*mfunc) (nose);

	cloud_free(&nose);
	vector3_free(&nosetip);

	return ans;
}

struct dataframe *extraction_4(struct cloud *cloud,
			                struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *norm_sagit = vector3_new(1, 0, 0);
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct plane *plane_sagit = plane_new(norm_sagit, centroid);
	struct cloud *left = cloud_new();
	struct cloud *right = cloud_new();

	cloud_plane_partition(cloud, plane_sagit, left, right);

	struct vector3 *norm_trans = vector3_new(0, 1, 0);
	struct cloud *left_1 = cloud_new();
	struct cloud *left_2 = cloud_new();
	struct cloud *right_1 = cloud_new();
	struct cloud *right_2 = cloud_new();
	struct plane *plane_left = plane_new(norm_trans, centroid);
	struct plane *plane_right = plane_new(norm_trans, centroid);

	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);

	struct dataframe *ans_left_1 = (*mfunc) (left_1);
	struct dataframe *ans_left_2 = (*mfunc) (left_2);
	struct dataframe *ans_right_1 = (*mfunc) (right_1);
	struct dataframe *ans_right_2 = (*mfunc) (right_2);

	struct dataframe *subleft = dataframe_concat_hor(ans_left_1, ans_left_2);
	struct dataframe *subright = dataframe_concat_hor(ans_right_1, ans_right_2);
	struct dataframe *ret = dataframe_concat_hor(subleft, subright);

	vector3_free(&norm_sagit);
	vector3_free(&centroid);
	plane_free(&plane_sagit);
	cloud_free(&left);
	cloud_free(&right);
	vector3_free(&norm_trans);
	cloud_free(&left_1);
	cloud_free(&left_2);
	cloud_free(&right_1);
	cloud_free(&right_2);
	plane_free(&plane_left);
	plane_free(&plane_right);
	dataframe_free(&ans_left_1);
	dataframe_free(&ans_left_2);
	dataframe_free(&ans_right_1);
	dataframe_free(&ans_right_2);
	dataframe_free(&subleft);
	dataframe_free(&subright);

	return ret;
}

struct dataframe *extraction_6(struct cloud *cloud,
			                struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *norm_sagit = vector3_new(1, 0, 0);
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct plane *plane_sagit = plane_new(norm_sagit, centroid);
	struct cloud *left = cloud_new();
	struct cloud *right = cloud_new();

	cloud_plane_partition(cloud, plane_sagit, left, right);

	struct vector3 *norm_trans = vector3_new(0, 1, 0);
	struct cloud *left_1 = cloud_new();
	struct cloud *left_2 = cloud_new();
	struct cloud *right_1 = cloud_new();
	struct cloud *right_2 = cloud_new();
	struct plane *plane_left = plane_new(norm_trans, centroid);
	struct plane *plane_right = plane_new(norm_trans, centroid);

	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);

	struct dataframe *ans_left = (*mfunc) (left);
	struct dataframe *ans_right = (*mfunc) (right);
	struct dataframe *ans_left_1 = (*mfunc) (left_1);
	struct dataframe *ans_left_2 = (*mfunc) (left_2);
	struct dataframe *ans_right_1 = (*mfunc) (right_1);
	struct dataframe *ans_right_2 = (*mfunc) (right_2);

	struct dataframe *full = dataframe_concat_hor(ans_left, ans_right);
	struct dataframe *subleft = dataframe_concat_hor(ans_left_1, ans_left_2);
	struct dataframe *subright = dataframe_concat_hor(ans_right_1, ans_right_2);
	struct dataframe *subs = dataframe_concat_hor(subleft, subright);
	struct dataframe *ret = dataframe_concat_hor(full, subs);

	vector3_free(&norm_sagit);
	vector3_free(&centroid);
	plane_free(&plane_sagit);
	cloud_free(&left);
	cloud_free(&right);
	vector3_free(&norm_trans);
	cloud_free(&left_1);
	cloud_free(&left_2);
	cloud_free(&right_1);
	cloud_free(&right_2);
	plane_free(&plane_left);
	plane_free(&plane_right);
	dataframe_free(&ans_left);
	dataframe_free(&ans_right);
	dataframe_free(&ans_left_1);
	dataframe_free(&ans_left_2);
	dataframe_free(&ans_right_1);
	dataframe_free(&ans_right_2);
	dataframe_free(&full);
	dataframe_free(&subleft);
	dataframe_free(&subright);
	dataframe_free(&subs);

	return ret;
}

struct dataframe *extraction_7(struct cloud *cloud,
			                struct dataframe *(*mfunc) (struct cloud *))
{
	struct vector3 *norm_sagit = vector3_new(1, 0, 0);
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct plane *plane_sagit = plane_new(norm_sagit, centroid);
	struct cloud *left = cloud_new();
	struct cloud *right = cloud_new();

	cloud_plane_partition(cloud, plane_sagit, left, right);

	struct dataframe *ans_left = (*mfunc) (left);
	struct dataframe *ans_right = (*mfunc) (right);

	struct vector3 *norm_trans = vector3_new(0, 1, 0);
	struct cloud *left_1 = cloud_new();
	struct cloud *left_2 = cloud_new();
	struct cloud *right_1 = cloud_new();
	struct cloud *right_2 = cloud_new();
	struct plane *plane_left = plane_new(norm_trans, centroid);
	struct plane *plane_right = plane_new(norm_trans, centroid);

	cloud_plane_partition(left, plane_left, left_1, left_2);
	cloud_plane_partition(right, plane_right, right_1, right_2);
	
	struct dataframe *ans_left_1 = (*mfunc) (left_1);
	struct dataframe *ans_left_2 = (*mfunc) (left_2);
	struct dataframe *ans_right_1 = (*mfunc) (right_1);
	struct dataframe *ans_right_2 = (*mfunc) (right_2);

	struct dataframe *ans_full = (*mfunc) (cloud);
	struct dataframe *halves = dataframe_concat_hor(ans_left, ans_right);
	struct dataframe *subleft = dataframe_concat_hor(ans_left_1, ans_left_2);
	struct dataframe *subright = dataframe_concat_hor(ans_right_1, ans_right_2);
	struct dataframe *subs = dataframe_concat_hor(subleft, subright);
	struct dataframe *tris = dataframe_concat_hor(ans_full, halves);

	struct dataframe *ret = dataframe_concat_hor(subs, tris);

	vector3_free(&norm_sagit);
	vector3_free(&centroid);
	plane_free(&plane_sagit);
	cloud_free(&left);
	cloud_free(&right);
	dataframe_free(&ans_left);
	dataframe_free(&ans_right);
	cloud_free(&left_1);
	cloud_free(&left_2);
	cloud_free(&right_1);
	cloud_free(&right_2);
	dataframe_free(&ans_left_1);
	dataframe_free(&ans_left_2);
	dataframe_free(&ans_right_1);
	dataframe_free(&ans_right_2);
	plane_free(&plane_right);
	plane_free(&plane_left);
	vector3_free(&norm_trans);
	dataframe_free(&ans_full);
	dataframe_free(&halves);
	dataframe_free(&subleft);
	dataframe_free(&subright);
	dataframe_free(&subs);
	dataframe_free(&tris);

	return ret;
}

struct cloud *extraction_vshape_base(struct cloud *cloud)
{
	real y_margin = 15.0;
	real x_margin = 25.0;

	struct vector3 *nosetip = cloud_point_faraway_bestfit(cloud);
	nosetip->y += y_margin;
	struct vector3 *diry = vector3_new(0, 1, 0);
	struct vector3 *dirx = vector3_new(1, 0, 0);
	struct plane *plane = plane_new(diry, nosetip);
	struct cloud *upper = cloud_cut_plane(cloud, plane);
	nosetip->y -= y_margin;
	struct cloud *nose = cloud_segment(cloud, nosetip, diry, y_margin);
	struct cloud *nose_slice = cloud_segment(nose, nosetip, dirx, x_margin);

	struct cloud *concat = cloud_concat(nose_slice, upper);

	cloud_free(&nose_slice);
	cloud_free(&nose);
	cloud_free(&upper);
	plane_free(&plane);
	vector3_free(&dirx);
	vector3_free(&diry);
	vector3_free(&nosetip);

	return concat;
}

struct dataframe *extraction_vshape(struct cloud *cloud,
				                 struct dataframe *(*mfunc) (struct cloud *))
{
	struct cloud *seg = extraction_vshape_base(cloud);
	struct dataframe *ans = (*mfunc) (seg);

	cloud_free(&seg);

	return ans;
}

struct dataframe *extraction_vshape_f(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *))
{
	struct cloud *seg = extraction_vshape_base(cloud);
	struct dataframe *ans = extraction_frontal(seg, mfunc);

	cloud_free(&seg);

	return ans;
}

struct dataframe *extraction_vshape_s(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *))
{
	struct cloud *seg = extraction_vshape_base(cloud);
	struct dataframe *ans = extraction_sagittal(seg, mfunc);

	cloud_free(&seg);

	return ans;
}

struct dataframe *extraction_vshape_t(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *))
{
	struct cloud *seg = extraction_vshape_base(cloud);
	struct dataframe *ans = extraction_transversal(seg, mfunc);

	cloud_free(&seg);

	return ans;
}

