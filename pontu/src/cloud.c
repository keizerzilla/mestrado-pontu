#include "../include/cloud.h"

struct cloud *cloud_new()
{
	struct cloud *cloud = malloc(sizeof(struct cloud));
	if (cloud == NULL)
		return NULL;

	cloud->points = pointset_new();
	cloud->numpts = 0;
	cloud->centroid = vector3_zero();
	cloud->tree = NULL;
	
	return cloud;
}

void cloud_free(struct cloud **cloud)
{
	if (*cloud == NULL)
		return;
	
	pointset_free(&(*cloud)->points);
	vector3_free(&(*cloud)->centroid);
	octree_free(&(*cloud)->tree);
	
	free(*cloud);
	*cloud = NULL;
}

struct vector3 *cloud_insert_real(struct cloud *cloud, real x, real y, real z)
{
	struct vector3 *i = pointset_insert(&cloud->points, x, y, z);
	
	if (i != NULL)
		cloud->numpts++;
	else
		return NULL;
	
	return i;
}

struct vector3 *cloud_insert_vector3(struct cloud *cloud, struct vector3 *p)
{
	return cloud_insert_real(cloud, p->x, p->y, p->z);
}

uint cloud_size(struct cloud *cloud)
{
	return cloud->numpts;
}

struct cloud *cloud_load_xyz(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;
	
	struct cloud *cloud = cloud_new();
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	while (!feof(file) && (fscanf(file, "%le %le %le\n", &x, &y, &z) != EOF))
		cloud_insert_real(cloud, x, y, z);
	
	fclose(file);
	
	return cloud;
}

struct cloud *cloud_load_csv(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	struct cloud *cloud = cloud_new();
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	while (!feof(file) && (fscanf(file, "%le,%le,%le\n", &x, &y, &z) != EOF))
		cloud_insert_real(cloud, x, y, z);

	fclose(file);

	return cloud;
}

struct cloud *cloud_load_ply(const char *filename)
{
	uint numpts = 0;
	char buffer[CLOUD_MAXBUFFER];

	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
		if (strcmp(buffer, "ply\n")) {
			fclose(file);
			return NULL;
		}
	} else {
		fclose(file);
		return NULL;
	}

	if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
		if (strcmp(buffer, "format ascii 1.0\n")) {
			fclose(file);
			return NULL;
		}
	} else {
		fclose(file);
		return NULL;
	}

	while (fgets(buffer, CLOUD_MAXBUFFER, file)) {
		if (!strcmp(buffer, "end_header\n"))
			break;

		if (!strncmp(buffer, "element vertex", 14))
			sscanf(buffer, "element vertex %d\n", &numpts);
	}

	struct cloud *cloud = cloud_new();
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}

	real x = 0;
	real y = 0;
	real z = 0;
	for (uint i = 0; i < numpts; i++) {
		if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
			sscanf(buffer, "%le %le %le %*s\n", &x, &y, &z);
			cloud_insert_real(cloud, x, y, z);
		} else {
			break;
		}
	}

	fclose(file);

	return cloud;
}

struct cloud *cloud_load_pcd(const char *filename)
{
	uint numpts = 0;
	char buffer[CLOUD_MAXBUFFER];

	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	while (fgets(buffer, CLOUD_MAXBUFFER, file)) {
		if (!strcmp(buffer, "DATA ascii\n"))
			break;

		if (!strncmp(buffer, "POINTS", 6))
			sscanf(buffer, "POINTS %d\n", &numpts);
	}

	struct cloud *cloud = cloud_new();
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}

	real x = 0;
	real y = 0;
	real z = 0;
	for (uint i = 0; i < numpts; i++) {
		if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
			sscanf(buffer, "%le %le %le %*s\n", &x, &y, &z);
			cloud_insert_real(cloud, x, y, z);
		} else {
			break;
		}
	}

	fclose(file);

	return cloud;
}

struct cloud *cloud_load_obj(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	struct cloud *cloud = cloud_new();
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	while (!feof(file)) {
		char buffer[CLOUD_MAXBUFFER];
		fgets(buffer, CLOUD_MAXBUFFER, file);

		if (buffer[0] == 'v' && buffer[1] == ' ') {
			sscanf(buffer, "v %lf %lf %lf\n", &x, &y, &z);
			cloud_insert_real(cloud, x, y, z);
		}
	}

	fclose(file);

	return cloud;
}

int cloud_save_xyz(struct cloud *cloud, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		return 0;


	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		fprintf(file, "%le %le %le\n", set->point->x,
			                           set->point->y,
			                           set->point->z);
	}

	fclose(file);

	return 1;
}

int cloud_save_csv(struct cloud *cloud, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		return 0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		fprintf(file, "%le,%le,%le\n", set->point->x,
			                           set->point->y,
			                           set->point->z);
	}

	fclose(file);

	return 1;
}

int cloud_save_ply(struct cloud *cloud, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		return 0;

	fprintf(file, "ply\n");
	fprintf(file, "format ascii 1.0\n");
	fprintf(file, "comment dumped by libpontu\n");
	fprintf(file, "element vertex %d\n", cloud->numpts);
	fprintf(file, "property float x\n");
	fprintf(file, "property float y\n");
	fprintf(file, "property float z\n");
	fprintf(file, "end_header\n");

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		fprintf(file, "%le %le %le\n", set->point->x,
			                           set->point->y,
			                           set->point->z);
	}

	fclose(file);

	return 1;
}

int cloud_save_pcd(struct cloud *cloud, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		return 0;

	fprintf(file, "VERSION .7\n");
	fprintf(file, "FIELDS x y z\n");
	fprintf(file, "SIZE 4 4 4\n");
	fprintf(file, "TYPE F F F\n");
	fprintf(file, "COUNT 1 1 1\n");
	fprintf(file, "WIDTH %d\n", cloud->numpts);
	fprintf(file, "HEIGHT 1\n");
	fprintf(file, "VIEWPOINT 0 0 0 1 0 0 0\n");
	fprintf(file, "POINTS %d\n", cloud->numpts);
	fprintf(file, "DATA ascii\n");

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		fprintf(file, "%le %le %le\n", set->point->x,
			                           set->point->y,
			                           set->point->z);
	}

	fclose(file);

	return 1;
}

struct cloud *cloud_copy(struct cloud *cloud)
{
	struct cloud *cpy = cloud_new();
	if (cpy == NULL)
		return NULL;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next)
		cloud_insert_real(cpy, set->point->x, set->point->y, set->point->z);
	
	return cpy;
}

void cloud_partitionate(struct cloud *cloud)
{
	if (cloud->tree == NULL) {
		cloud->tree = octree_new(cloud->points, cloud->numpts, 5);
		octree_partitionate(cloud->tree);
	}
}

struct vector3 *cloud_calc_centroid(struct cloud *cloud)
{
	vector3_free(&cloud->centroid);
	cloud->centroid = vector3_zero();
	cloud->numpts = 0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		cloud->centroid->x += set->point->x;
		cloud->centroid->y += set->point->y;
		cloud->centroid->z += set->point->z;
		
		cloud->numpts++;
	}

	cloud->centroid->x /= cloud->numpts;
	cloud->centroid->y /= cloud->numpts;
	cloud->centroid->z /= cloud->numpts;

	return cloud->centroid;
}

struct vector3 *cloud_get_centroid(struct cloud *cloud)
{
	if (cloud->centroid != NULL)
		return vector3_from_vector(cloud->centroid);
	else
		return vector3_from_vector(cloud_calc_centroid(cloud));
}

void cloud_scale(struct cloud *cloud, real f)
{
	for (struct pointset *set = cloud->points; set != NULL; set = set->next)
		vector3_scale(set->point, f);
}

void cloud_translate_vector_dir(struct cloud *cloud,
				                struct vector3 *source,
				                struct vector3 *target)
{
	struct vector3 *t = vector3_sub(target, source);
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next)
		vector3_increase(set->point, t);
	
	vector3_free(&t);
	cloud_calc_centroid(cloud);
}

void cloud_translate_vector(struct cloud *cloud, struct vector3 *dest)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct vector3 *t = vector3_sub(dest, centroid);

	for (struct pointset *set = cloud->points; set != NULL; set = set->next)
		vector3_increase(set->point, t);

	vector3_free(&t);
	vector3_free(&centroid);
	cloud_calc_centroid(cloud);
}

void cloud_translate_real(struct cloud *cloud, real x, real y, real z)
{
	struct vector3 *dest = vector3_new(x, y, z);
	struct vector3 *t = vector3_sub(dest, cloud_get_centroid(cloud));

	for (struct pointset *set = cloud->points; set != NULL; set = set->next)
		vector3_increase(set->point, t);

	vector3_free(&dest);
	vector3_free(&t);
	cloud_calc_centroid(cloud);
}

void cloud_transform(struct cloud *cloud, struct matrix* rt)
{
	struct matrix *cloud_mat = matrix_new(4, cloud->numpts);
	if (cloud_mat == NULL) {
		cloud_free(&cloud);
		return;
	}

	struct matrix *output_mat = matrix_new(4, cloud->numpts);
	if (output_mat == NULL) {
		matrix_free(&cloud_mat);
		cloud_free(&cloud);
		return;
	}
	
	uint i = 0;
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		matrix_set(cloud_mat, 0, i, set->point->x);
		matrix_set(cloud_mat, 1, i, set->point->y);
		matrix_set(cloud_mat, 2, i, set->point->z);
		matrix_set(cloud_mat, 3, i, 1.0);
		
		i++;
	}

	output_mat = algebra_mat_prod(rt, cloud_mat);

	matrix_free(&cloud_mat);
	
	i = 0;
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		set->point->x = matrix_get(output_mat, 0, i);
		set->point->y = matrix_get(output_mat, 1, i);
		set->point->z = matrix_get(output_mat, 2, i);
		
		i++;
	}

	matrix_free(&output_mat);
}

void cloud_sort(struct cloud *cloud, int axis)
{
	pointset_sort(cloud->points, axis);
}

struct cloud *cloud_concat(struct cloud *c1, struct cloud *c2)
{
	struct cloud *cat = cloud_new();
	if (cat == NULL)
		return NULL;

	for (struct pointset *s1 = c1->points; s1 != NULL; s1 = s1->next)
		cloud_insert_real(cat, s1->point->x, s1->point->y, s1->point->z);

	for (struct pointset *s2 = c2->points; s2 != NULL; s2 = s2->next)
		cloud_insert_real(cat, s2->point->x, s2->point->y, s2->point->z);

	return cat;
}

struct vector3 *cloud_axis_size(struct cloud *cloud)
{
	if (cloud->numpts == 0)
		return vector3_zero();
	
	real max_x = INFINITY;
	real max_y = INFINITY;
	real max_z = INFINITY;
	real min_x = -INFINITY;
	real min_y = -INFINITY;
	real min_z = -INFINITY;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (set->point->x > max_x)
			max_x = set->point->x;
		else if (set->point->x < min_x)
			min_x = set->point->x;

		if (set->point->y > max_y)
			max_y = set->point->y;
		else if (set->point->y < min_y)
			min_y = set->point->y;

		if (set->point->z > max_z)
			max_z = set->point->z;
		else if (set->point->z < min_z)
			min_z = set->point->z;
	}

	return vector3_new(max_x - min_x, max_y - min_y, max_z - min_z);
}

real cloud_boundingbox_area(struct cloud *cloud)
{
	struct vector3 *axis = cloud_axis_size(cloud);
	
	real area = (2.0 * axis->x * axis->y) +
	            (2.0 * axis->y * axis->z) +
	            (2.0 * axis->x * axis->z);
	
	vector3_free(&axis);

	return area;
}

real cloud_boundingbox_volume(struct cloud *cloud)
{
	struct vector3 *axis = cloud_axis_size(cloud);
	real vol = vector3_volume(axis);
	
	vector3_free(&axis);

	return vol;
}

real cloud_function_volume(struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	real vol = 0.0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next)
		vol += vector3_distance(centroid, set->point);

	vector3_free(&centroid);

	return vol;
}

struct cloud *cloud_cut_radius(struct cloud *cloud, struct vector3 *p, real r)
{
	struct cloud *sub = cloud_new();
	if (sub == NULL)
		return NULL;
	
	real sq_r = r * r;
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (vector3_squared_distance(p, set->point) <= sq_r)
			cloud_insert_real(sub, set->point->x, set->point->y, set->point->z);
	}

	return sub;
}

struct cloud *cloud_cut_centroid(struct cloud *cloud, real r)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct cloud *sub = cloud_cut_radius(cloud, centroid, r);
	
	vector3_free(&centroid);
	
	return sub;
}

struct cloud *cloud_cut_plane(struct cloud *cloud, struct plane *plane)
{
	struct cloud *sub = cloud_new();
	if (sub == NULL)
		return NULL;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (plane_on_direction(plane, set->point))
			cloud_insert_real(sub, set->point->x, set->point->y, set->point->z);
	}
	
	return sub;
}

int cloud_plane_partition(struct cloud *src,
			              struct plane *plane,
			              struct cloud *p1,
			              struct cloud *p2)
{
	if (p1->numpts != 0 || p2->numpts != 0)
		return 0;
	
	for (struct pointset *set = src->points; set != NULL; set = set->next) {
		if (plane_on_direction(plane, set->point))
			cloud_insert_real(p1, set->point->x, set->point->y, set->point->z);
		else
			cloud_insert_real(p2, set->point->x, set->point->y, set->point->z);
	}

	return 1;
}

struct vector3 *cloud_max_distance_from_plane(struct cloud *cloud,
					                          struct plane *plane)
{
	struct vector3 *p = NULL;
	real dist = 0.0;
	real temp = 0.0;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (plane_on_direction(plane, set->point)) {
			temp = plane_distance2point(plane, set->point);
			
			if (temp >= dist) {
				dist = temp;
				p = set->point;
			}
		}
	}

	return vector3_from_vector(p);
}

struct cloud *cloud_cut_cylinder(struct cloud *cloud,
				                 struct vector3 *ref,
				                 struct vector3 *dir,
				                 real radius)
{
	struct cloud *sub = cloud_new();
	if (sub == NULL)
		return NULL;
	
	real dirl = vector3_length(dir);
	real dist = 0.0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		struct vector3 *dot = vector3_sub(ref, set->point);
		struct vector3 *cross = vector3_cross(dot, dir);

		dist = vector3_length(cross) / dirl;
		if (dist <= radius)
			cloud_insert_vector3(sub, set->point);

		vector3_free(&dot);
		vector3_free(&cross);
	}

	return sub;
}

struct cloud *cloud_segment(struct cloud *cloud,
			                struct vector3 *ref,
			                struct vector3 *dir,
			                real epslon)
{
	struct cloud *sub = cloud_new();
	if (sub == NULL)
		return NULL;
	
	struct plane *plane = plane_new(dir, ref);

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (plane_distance2point(plane, set->point) <= epslon)
			cloud_insert_real(sub, set->point->x, set->point->y, set->point->z);
	}
	
	plane_free(&plane);

	return sub;
}

struct vector3 *cloud_closest_point(struct cloud *cloud, struct vector3 *point)
{
	struct vector3* closest = NULL;
	real temp = 0.0;
	real dist = INFINITY;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		temp = vector3_squared_distance(point, set->point);
		
		if (temp < dist) {
			dist = temp;
			closest = set->point;
		}
	}

	return closest;
}

struct pointset *cloud_closest_point_set(struct cloud *cloud,
                                         struct vector3 *point)
{
	struct pointset *closest = NULL;
	real temp = 0.0;
	real dist = INFINITY;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		temp = vector3_squared_distance(point, set->point);
		
		if (temp < dist) {
			dist = temp;
			closest = set;
		}
	}

	return closest;
}

struct vector3 *cloud_closest_to_centroid(struct cloud *cloud)
{
	return cloud_closest_point(cloud, cloud_get_centroid(cloud));
}

real cloud_nearest_neighbors_bruteforce(struct cloud* source,
                                        struct cloud* target,
                                        struct vector3 **src_pt,
                                        struct vector3 **tgt_pt)
{
	real dist = INFINITY;
	real temp = 0.0;
	
	for (struct pointset *s = source->points; s != NULL; s = s->next) {
		for (struct pointset *t = target->points; t != NULL; t = t->next) {
			temp = vector3_squared_distance(s->point, t->point);
			
			if (temp < dist) {
				dist = temp;
				*src_pt = s->point;
				*tgt_pt = t->point;
			}
		}
	}
	
	return vector3_distance(*src_pt, *tgt_pt);
}

struct vector3 *cloud_min_x(struct cloud *cloud)
{
	struct vector3 *v = NULL;
	real min_x = -INFINITY;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (min_x > set->point->x) {
			min_x = set->point->x;
			v = set->point;
		}
	}

	return v;
}

struct vector3 *cloud_min_y(struct cloud *cloud)
{
	struct vector3 *v = NULL;
	real min_y = -INFINITY;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (min_y > set->point->y) {
			min_y = set->point->y;
			v = set->point;
		}
	}

	return v;
}

struct vector3 *cloud_min_z(struct cloud *cloud)
{
	struct vector3 *v = NULL;
	real min_z = -INFINITY;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (min_z > set->point->z) {
			min_z = set->point->z;
			v = set->point;
		}
	}

	return v;
}

struct vector3 *cloud_max_x(struct cloud *cloud)
{
	struct vector3 *v = NULL;
	real max_x = INFINITY;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (max_x < set->point->x) {
			max_x = set->point->x;
			v = set->point;
		}
	}

	return v;
}

struct vector3 *cloud_max_y(struct cloud *cloud)
{
	struct vector3 *v = NULL;
	real max_y = INFINITY;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (max_y < set->point->y) {
			max_y = set->point->y;
			v = set->point;
		}
	}

	return v;
}

struct vector3 *cloud_max_z(struct cloud *cloud)
{
	struct vector3 *v = NULL;
	real max_z = INFINITY;
	
	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		if (max_z < set->point->z) {
			max_z = set->point->z;
			v = set->point;
		}
	}

	return v;
}

real cloud_max_distance(struct cloud *cloud, struct vector3 *p)
{
	real dist = INFINITY;
	real temp = 0.0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		temp = vector3_squared_distance(p, set->point);

		if (temp > dist)
			dist = temp;
	}

	return sqrt(dist);
}

real cloud_max_distance_from_centroid(struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	
	real d = cloud_max_distance(cloud, centroid);
	
	vector3_free(&centroid);

	return d;
}

struct plane *cloud_dispersion_plane(struct cloud *cloud, struct vector3 *ref)
{
	if (cloud->numpts < 3)
		return NULL;

	real xx = 0.0;
	real xy = 0.0;
	real xz = 0.0;
	real yy = 0.0;
	real yz = 0.0;
	real zz = 0.0;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		struct vector3 *r = vector3_sub(set->point, ref);

		xx += r->x * r->x;
		xy += r->x * r->y;
		xz += r->x * r->z;
		yy += r->y * r->y;
		yz += r->y * r->z;
		zz += r->z * r->z;

		vector3_free(&r);
	}

	real det_x = yy * zz - yz * yz;
	real det_y = xx * zz - xz * xz;
	real det_z = xx * yy - xy * xy;

	real det_max = calc_max3(det_x, det_y, det_z);

	if (det_max <= 0.0)
		return NULL;

	real x = 0.0;
	real y = 0.0;
	real z = 0.0;

	if (det_max == det_x) {
		x = det_x;
		y = xz * yz - xy * zz;
		z = xy * yz - xz * yy;
	} else if (det_max == det_y) {
		x = xz * yz - xy * zz;
		y = det_y;
		z = xy * xz - yz * xx;
	} else {
		x = xy * yz - xz * yy;
		y = xy * xz - yz * xx;
		z = det_z;
	}

	struct vector3 *normal = vector3_new(x, y, z);
	vector3_normalize(normal);

	struct plane *plane = plane_new(normal, ref);

	vector3_free(&normal);

	return plane;
}

struct vector3 *cloud_normal(struct cloud *cloud, struct vector3 *ref)
{
	struct plane *bestfit = cloud_dispersion_plane(cloud, ref);
	struct vector3 *normal = vector3_from_vector(bestfit->normal);

	plane_free(&bestfit);

	return normal;
}

struct plane *cloud_plane_fitting(struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct plane *plane = cloud_dispersion_plane(cloud, centroid);

	vector3_free(&centroid);

	return plane;
}

struct vector3 *cloud_point_faraway_bestfit(struct cloud *cloud)
{
	struct plane *bestfit = cloud_plane_fitting(cloud);
	struct vector3 *faraway = cloud_max_distance_from_plane(cloud, bestfit);

	plane_free(&bestfit);

	return faraway;
}

real cloud_curvature(struct cloud *cloud)
{
	struct vector3 *centroid = cloud_get_centroid(cloud);
	struct vector3 *p = NULL;
	uint size = cloud->numpts;
	real radius = 0.0;
	real a = centroid->x;
	real b = centroid->y;
	real c = centroid->z;

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		p = set->point;

		a -= p->x;
		b -= p->y;
		c -= p->z;
	}

	a = centroid->x + (a / size);
	b = centroid->y + (b / size);
	c = centroid->z + (c / size);

	for (struct pointset *set = cloud->points; set != NULL; set = set->next) {
		p = set->point;
		
		radius += sqrt(pow(p->x - a, 2) + pow(p->y - b, 2) + pow(p->z - c, 2));
	}

	radius /= size;

	vector3_free(&centroid);

	return 1.0 / radius;
}

real cloud_rmse(struct cloud *source, struct cloud *target)
{
	if (source->numpts < target->numpts)
		return -1.0;

	real rmse = 0.0;
	struct pointset *s = source->points;
	struct pointset *t = target->points;
	
	while (s != NULL && t != NULL) {
		rmse += sqrt((s->point->x - t->point->x) *
					 (s->point->x - t->point->x) +
					 (s->point->y - t->point->y) *
					 (s->point->y - t->point->y) +
					 (s->point->z - t->point->z) *
					 (s->point->z - t->point->z));
		
		s = s->next;
		t = t->next;
	}
	
	return rmse / (real)source->numpts;
}

void cloud_debug(struct cloud *cloud, FILE *output)
{
	if (cloud == NULL) {
		fprintf(output, "!!! cloud empty !!!\n");
		return;
	}

	pointset_debug(cloud->points, output);
}

