#include "../include/cloud.h"

struct cloud *cloud_new(uint numpts)
{
	struct cloud *cloud = malloc(sizeof(struct cloud));
	if (cloud == NULL)
		return NULL;

	cloud->points = malloc(numpts * sizeof(struct vector3));
	if (cloud->points == NULL) {
		free(cloud);
		cloud = NULL;
		return NULL;
	}

	memset(cloud->points, 0, numpts * sizeof(struct vector3));

	cloud->centroid = NULL;
	cloud->numpts = numpts;
	cloud->kdt = NULL;
	
	return cloud;
}

struct cloud *cloud_empty()
{
	return cloud_new(0);
}

void cloud_free(struct cloud *cloud)
{
	if (cloud != NULL) {
		free(cloud->points);
		vector3_free(cloud->centroid);
		kdtree_free(cloud->kdt);
		
		free(cloud);
		cloud = NULL;
	}
}

struct vector3 *cloud_set_point_real(struct cloud *cloud,
				                     uint index,
				                     real x,
				                     real y,
				                     real z)
{
	if (index >= cloud->numpts)
		return NULL;

	vector3_set(&cloud->points[index], x, y, z);

	return &cloud->points[index];
}

struct vector3 *cloud_set_point_vector(struct cloud *cloud,
				                       uint index,
				                       struct vector3 *point)
{
	return cloud_set_point_real(cloud, index, point->x, point->y, point->z);
}

struct vector3 *cloud_add_point_real(struct cloud *cloud,
				                     real x,
				                     real y,
				                     real z)
{
	uint new_size = (cloud->numpts + 1) * sizeof(struct vector3);

	struct vector3 *new_points = realloc(cloud->points, new_size);
	if (new_points == NULL)
		return NULL;

	cloud->points = new_points;
	cloud->numpts++;

	vector3_set(&cloud->points[cloud->numpts - 1], x, y, z);

	return &cloud->points[cloud->numpts - 1];
}

struct vector3 *cloud_add_point_vector(struct cloud *cloud, struct vector3 *p)
{
	return cloud_add_point_real(cloud, p->x, p->y, p->z);
}

uint cloud_size(struct cloud *cloud)
{
	return cloud->numpts;
}

void cloud_partitionate(struct cloud *cloud)
{
	if (cloud->kdt != NULL)
		return;
	
	cloud->kdt = kdtree_new(cloud->points, cloud->numpts, VECTOR3_AXIS_X);
	kdtree_partitionate(cloud->kdt, VECTOR3_AXIS_X);
}

struct cloud *cloud_load_xyz(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	uint numpts = 0;
	while (!feof(file) && (fscanf(file, "%*s %*s %*s\n") != EOF))
		numpts++;

	rewind(file);

	struct cloud *cloud = cloud_new(numpts);
	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	while (!feof(file) && (fscanf(file, "%le %le %le\n", &x, &y, &z) != EOF)) {
		cloud_set_point_real(cloud, index, x, y, z);
		index++;
	}

	fclose(file);

	return cloud;
}

struct cloud *cloud_load_csv(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	uint numpts = 0;
	while (!feof(file) && (fscanf(file, "%*s,%*s,%*s\n") != EOF))
		numpts++;

	rewind(file);

	struct cloud *cloud = cloud_new(numpts);
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	while (!feof(file) && (fscanf(file, "%le,%le,%le\n", &x, &y, &z) != EOF)) {
		cloud_set_point_real(cloud, index, x, y, z);
		index++;
	}

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

	struct cloud *cloud = cloud_new(numpts);
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}

	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	for (uint i = 0; i < numpts; i++) {
		if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
			sscanf(buffer, "%le %le %le %*s\n", &x, &y, &z);
			cloud_set_point_real(cloud, index, x, y, z);
			index++;
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

	struct cloud *cloud = cloud_new(numpts);
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
			cloud_set_point_real(cloud, i, x, y, z);
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

	struct cloud *cloud = cloud_empty();
	real x = 0;
	real y = 0;
	real z = 0;

	while (!feof(file)) {
		char buffer[CLOUD_MAXBUFFER];
		fgets(buffer, CLOUD_MAXBUFFER, file);

		if (buffer[0] == 'v' && buffer[1] == ' ') {
			sscanf(buffer, "v %lf %lf %lf\n", &x, &y, &z);
			cloud_add_point_real(cloud, x, y, z);
		}
	}

	fclose(file);

	return cloud;
}

struct cloud *cloud_load_off(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;
	
	uint numpts = 0;
	char buffer[CLOUD_MAXBUFFER];
	
	if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
		if (strcmp(buffer, "OFF\n")) {
			fclose(file);
			return NULL;
		}
	} else {
		fclose(file);
		return NULL;
	}
	
	if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
		sscanf(buffer, "%u %*u %*u\n", &numpts);
	} else {
		fclose(file);
		return NULL;
	}
	
	struct cloud *cloud = cloud_new(numpts);
	if (cloud == NULL) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	for (uint i = 0; i < numpts; i++) {
		if (fgets(buffer, CLOUD_MAXBUFFER, file)) {
			sscanf(buffer, "%le %le %le\n", &x, &y, &z);
			cloud_set_point_real(cloud, i, x, y, z);
		} else {
			break;
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

	for (uint i = 0; i < cloud->numpts; i++) {
		fprintf(file, "%le %le %le\n", cloud->points[i].x,
			                           cloud->points[i].y,
			                           cloud->points[i].z);
	}

	fclose(file);

	return 1;
}

int cloud_save_csv(struct cloud *cloud, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		return 0;

	for (uint i = 0; i < cloud->numpts; i++) {
		fprintf(file, "%le,%le,%le\n", cloud->points[i].x,
			                           cloud->points[i].y,
			                           cloud->points[i].z);
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

	for (uint i = 0; i < cloud->numpts; i++) {
		fprintf(file, "%le %le %le\n", cloud->points[i].x,
			                           cloud->points[i].y,
			                           cloud->points[i].z);
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

	for (uint i = 0; i < cloud->numpts; i++) {
		fprintf(file, "%le %le %le\n", cloud->points[i].x,
			                           cloud->points[i].y,
			                           cloud->points[i].z);
	}

	fclose(file);

	return 1;
}

struct cloud *cloud_copy(struct cloud *cloud)
{
	struct cloud *cpy = cloud_new(cloud->numpts);
	if (cpy == NULL)
		return NULL;
	
	for (uint index = 0; index < cloud->numpts; index++) {
		cloud_set_point_real(cpy,
				             index,
				             cloud->points[index].x,
				             cloud->points[index].y,
				             cloud->points[index].z);
	}
	
	return cpy;
}

struct vector3 *cloud_calc_center(struct cloud *cloud)
{
	vector3_free(cloud->centroid);

	cloud->centroid = vector3_zero();

	for (uint i = 0; i < cloud->numpts; i++) {
		cloud->centroid->x += cloud->points[i].x;
		cloud->centroid->y += cloud->points[i].y;
		cloud->centroid->z += cloud->points[i].z;
	}

	cloud->centroid->x /= cloud->numpts;
	cloud->centroid->y /= cloud->numpts;
	cloud->centroid->z /= cloud->numpts;

	return cloud->centroid;
}

struct vector3 *cloud_get_center(struct cloud *cloud)
{
	if (cloud->centroid)
		return vector3_from_vector(cloud->centroid);
	else
		return vector3_from_vector(cloud_calc_center(cloud));
}

void cloud_scale(struct cloud *cloud, real f)
{
	for (uint i = 0; i < cloud->numpts; i++)
		vector3_scale(&cloud->points[i], f);
}

void cloud_translate_vector_dir(struct cloud *cloud,
				                struct vector3 *source,
				                struct vector3 *target)
{
	struct vector3 *t = vector3_sub(target, source);
	
	for (uint i = 0; i < cloud->numpts; i++)
		vector3_increase(&cloud->points[i], t);
	
	vector3_free(t);
	
	cloud_calc_center(cloud);
}

void cloud_translate_vector(struct cloud *cloud, struct vector3 *dest)
{
	struct vector3 *center = cloud_get_center(cloud);
	struct vector3 *t = vector3_sub(dest, center);

	for (uint i = 0; i < cloud->numpts; i++)
		vector3_increase(&cloud->points[i], t);

	vector3_free(t);
	vector3_free(center);
	cloud_calc_center(cloud);
}

void cloud_translate_real(struct cloud *cloud, real x, real y, real z)
{
	struct vector3 *dest = vector3_new(x, y, z);
	struct vector3 *t = vector3_sub(dest, cloud_get_center(cloud));

	for (uint i = 0; i < cloud->numpts; i++)
		vector3_increase(&cloud->points[i], t);

	vector3_free(dest);
	vector3_free(t);
	cloud_calc_center(cloud);
}

void cloud_rotate_x(struct cloud *cloud, real d)
{
	printf("cloud_rotate_x: incomplete function\n");
	
	for (uint i = 0; i < cloud->numpts; i++)
		vector3_rotate_x(&cloud->points[i], d);
}

void cloud_rotate_y(struct cloud *cloud, real d)
{
	printf("cloud_rotate_y: incomplete function\n");
	
	for (uint i = 0; i < cloud->numpts; i++)
		vector3_rotate_y(&cloud->points[i], d);
}

void cloud_rotate_z(struct cloud *cloud, real d)
{
	printf("cloud_rotate_z: incomplete function\n");
	
	for (uint i = 0; i < cloud->numpts; i++)
		vector3_rotate_z(&cloud->points[i], d);
}

real cloud_mean_x(struct cloud *cloud)
{
	real mx = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++)
		mx += cloud->points[i].x;

	return mx / cloud->numpts;
}

real cloud_mean_y(struct cloud *cloud)
{
	real my = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++)
		my += cloud->points[i].y;

	return my / cloud->numpts;
}

real cloud_mean_z(struct cloud *cloud)
{
	real mz = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++)
		mz += cloud->points[i].z;

	return mz / cloud->numpts;
}

int cloud_compare(const void *p1, const void *p2)
{
	struct vector3 *dp1 = (struct vector3 *)p1;
	struct vector3 *dp2 = (struct vector3 *)p2;

	return dp2->z - dp1->z;
}

void cloud_sort(struct cloud *cloud)
{
	qsort(cloud->points, cloud->numpts, sizeof(struct vector3), cloud_compare);
}

struct cloud *cloud_concat(struct cloud *c1, struct cloud *c2)
{
	uint size_c1 = c1->numpts;
	uint size_c2 = c2->numpts;

	struct cloud *concat = cloud_new(size_c1 + size_c2);

	for (uint i = 0; i < size_c1; i++)
		cloud_set_point_vector(concat, i, &c1->points[i]);

	for (uint j = 0; j < size_c2; j++)
		cloud_set_point_vector(concat, j + size_c1, &c2->points[j]);

	return concat;
}

struct vector3 *cloud_axis_size(struct cloud *cloud)
{
	if (cloud->numpts == 0)
		return vector3_zero();

	real max_x = cloud->points[0].x;
	real min_x = cloud->points[0].x;
	real max_y = cloud->points[0].y;
	real min_y = cloud->points[0].y;
	real max_z = cloud->points[0].z;
	real min_z = cloud->points[0].z;

	for (uint i = 1; i < cloud->numpts; i++) {
		if (cloud->points[i].x > max_x)
			max_x = cloud->points[i].x;
		else if (cloud->points[i].x < min_x)
			min_x = cloud->points[i].x;

		if (cloud->points[i].y > max_y)
			max_y = cloud->points[i].y;
		else if (cloud->points[i].y < min_y)
			min_y = cloud->points[i].y;

		if (cloud->points[i].z > max_z)
			max_z = cloud->points[i].z;
		else if (cloud->points[i].z < min_z)
			min_z = cloud->points[i].z;
	}

	return vector3_new(max_x - min_x, max_y - min_y, max_z - min_z);
}

real cloud_boundingbox_area(struct cloud *cloud)
{
	struct vector3 *axis = cloud_axis_size(cloud);
	
	real area = (2.0f * axis->x * axis->y) +
	            (2.0f * axis->y * axis->z) +
	            (2.0f * axis->x * axis->z);
	
	vector3_free(axis);

	return area;
}

real cloud_boundingbox_volume(struct cloud *cloud)
{
	struct vector3 *axis = cloud_axis_size(cloud);
	real vol = vector3_volume(axis);
	
	vector3_free(axis);

	return vol;
}

real cloud_function_volume(struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	real vol = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++)
		vol += vector3_distance(center, &cloud->points[i]);

	vector3_free(center);

	return vol;
}

struct cloud *cloud_cut_radius(struct cloud *cloud, struct vector3 *p, real r)
{
	struct cloud *sub = cloud_empty();

	for (uint i = 0; i < cloud->numpts; i++) {
		if (vector3_distance(p, &cloud->points[i]) <= r)
			cloud_add_point_vector(sub, &cloud->points[i]);
	}

	return sub;
}

struct cloud *cloud_cut_center(struct cloud *cloud, real cut)
{
	struct cloud *sub = cloud_empty();
	struct vector3 *center = cloud_get_center(cloud);

	for (uint i = 0; i < cloud->numpts; i++) {
		if (vector3_distance(center, &cloud->points[i]) <= cut)
			cloud_add_point_vector(sub, &cloud->points[i]);
	}
	
	vector3_free(center);
	
	return sub;
}

struct cloud *cloud_cut_plane(struct cloud *cloud, struct plane *plane)
{
	struct cloud *sub = cloud_empty();

	for (uint i = 0; i < cloud->numpts; i++)
		if (plane_on_direction(plane, &cloud->points[i]))
			cloud_add_point_vector(sub, &cloud->points[i]);

	return sub;
}

int cloud_plane_partition(struct cloud *src,
			              struct plane *plane,
			              struct cloud *par1,
			              struct cloud *par2)
{
	if (par1->numpts != 0 || par2->numpts != 0)
		return 0;

	for (uint i = 0; i < src->numpts; i++) {
		if (plane_on_direction(plane, &src->points[i]))
			cloud_add_point_vector(par1, &src->points[i]);
		else
			cloud_add_point_vector(par2, &src->points[i]);
	}

	return 1;
}

struct vector3 *cloud_max_distance_from_plane(struct cloud *cloud,
					                          struct plane *plane)
{
	struct vector3 *p = NULL;
	real d = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		if (plane_on_direction(plane, &cloud->points[i])) {
			real temp = plane_distance2point(plane, &cloud->points[i]);
			if (temp >= d) {
				d = temp;
				p = &cloud->points[i];
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
	struct cloud *sub = cloud_empty();
	real dirl = vector3_length(dir);
	real dist = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		struct vector3 *dot = vector3_sub(ref, &cloud->points[i]);
		struct vector3 *cross = vector3_cross(dot, dir);

		dist = vector3_length(cross) / dirl;
		if (dist <= radius)
			cloud_add_point_vector(sub, &cloud->points[i]);

		vector3_free(dot);
		vector3_free(cross);
	}

	return sub;
}

struct cloud *cloud_segment(struct cloud *cloud,
			                struct vector3 *ref,
			                struct vector3 *dir,
			                real epslon)
{
	struct cloud *sub = cloud_empty();
	struct plane *plane = plane_new(dir, ref);

	for (uint i = 0; i < cloud->numpts; i++)
		if (plane_distance2point(plane, &cloud->points[i]) <= epslon)
			cloud_add_point_vector(sub, &cloud->points[i]);

	plane_free(plane);

	return sub;
}

struct vector3 *cloud_closest_point(struct cloud *cloud, struct vector3 *point)
{
	/**
	uint index = 0;
	real temp = 0;
	real dist = vector3_squared_distance(point, &cloud->points[0]);

	for (uint i = 1; i < cloud->numpts; i++) {
		temp = vector3_squared_distance(point, &cloud->points[i]);
		if (temp < dist) {
			dist = temp;
			index = i;
		}
	}

	return &cloud->points[index];
	*/
	
	cloud_partitionate(cloud);
	
	return kdtree_nearest_neighbor(cloud->kdt, point);
	
}

struct vector3 *cloud_closest_to_center(struct cloud *cloud)
{
	return cloud_closest_point(cloud, cloud_get_center(cloud));
}

real cloud_nearest_neighbors_bruteforce(struct cloud* source,
                                        struct cloud* target,
                                        struct vector3 **src_pt,
                                        struct vector3 **tgt_pt)
{
	*src_pt = &source->points[0];
	*tgt_pt = &target->points[0];
	real dist = vector3_squared_distance(*src_pt, *tgt_pt);
	real temp = 0.0f;
	
	for (uint i = 0; i < source->numpts; i++) {
		for (uint j = 0; j < target->numpts; j++) {
			temp = vector3_squared_distance(&source->points[i],
			                                &target->points[j]);
			if (temp < dist) {
				dist = temp;
				*src_pt = &source->points[i];
				*tgt_pt = &target->points[j];
			}
		}
	}
	
	return vector3_distance(*src_pt, *tgt_pt);
}

real cloud_nearest_neighbors_partition(struct cloud* source,
                                       struct cloud* target,
                                       struct vector3 **src_pt,
                                       struct vector3 **tgt_pt)
{
	cloud_partitionate(target);
	
	*src_pt = &source->points[0];
	*tgt_pt = &target->points[0];
	real dist = vector3_squared_distance(*src_pt, *tgt_pt);
	real temp = 0.0f;
	struct vector3 *nn = NULL;
	
	for (uint i = 0; i < source->numpts; i++) {
		nn = kdtree_nearest_neighbor(target->kdt, &source->points[i]);
		temp = vector3_squared_distance(nn, &source->points[i]);
		
		if (temp < dist) {
			dist = temp;
			*src_pt = &source->points[i];
			*tgt_pt = nn;
		}
	}
	
	return vector3_distance(*src_pt, *tgt_pt);
}

struct vector3 *cloud_min_x(struct cloud *cloud)
{
	uint index = 0;
	real min_x = cloud->points[0].x;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		if (min_x > cloud->points[i].x) {
			min_x = cloud->points[i].x;
			index = i;
		}
	}

	return vector3_from_vector(&cloud->points[index]);
}

struct vector3 *cloud_min_y(struct cloud *cloud)
{
	uint index = 0;
	real min_y = cloud->points[0].y;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		if (min_y > cloud->points[i].y) {
			min_y = cloud->points[i].y;
			index = i;
		}
	}

	return vector3_from_vector(&cloud->points[index]);
}

struct vector3 *cloud_min_z(struct cloud *cloud)
{
	uint index = 0;
	real minz = cloud->points[0].z;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		if (minz > cloud->points[i].z) {
			minz = cloud->points[i].z;
			index = i;
		}
	}

	return vector3_from_vector(&cloud->points[index]);
}

struct vector3 *cloud_max_x(struct cloud *cloud)
{
	uint index = 0;
	real max_x = cloud->points[0].x;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		if (max_x < cloud->points[i].x) {
			max_x = cloud->points[i].x;
			index = i;
		}
	}

	return vector3_from_vector(&cloud->points[index]);
}

struct vector3 *cloud_max_y(struct cloud *cloud)
{
	uint index = 0;
	real max_y = cloud->points[0].y;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		if (max_y < cloud->points[i].y) {
			max_y = cloud->points[i].y;
			index = i;
		}
	}

	return vector3_from_vector(&cloud->points[index]);
}

struct vector3 *cloud_max_z(struct cloud *cloud)
{
	uint index = 0;
	real max_z = cloud->points[0].z;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		if (max_z < cloud->points[i].z) {
			max_z = cloud->points[i].z;
			index = i;
		}
	}

	return vector3_from_vector(&cloud->points[index]);
}

real cloud_max_distance(struct cloud *cloud, struct vector3 *p)
{
	real dist = vector3_squared_distance(p, &cloud->points[0]);
	real temp = 0.0f;

	for (uint i = 1; i < cloud->numpts; i++) {
		temp = vector3_squared_distance(p, &cloud->points[i]);

		if (temp > dist)
			dist = temp;
	}

	return sqrt(dist);
}

real cloud_max_distance_from_center(struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	real d = cloud_max_distance(cloud, center);

	vector3_free(center);

	return d;
}

struct vector3 *cloud_average_direction(struct cloud *cloud)
{
	struct vector3 *ans = vector3_zero();

	for (uint i = 0; i < cloud->numpts; i++)
		vector3_increase(ans, &cloud->points[i]);

	vector3_normalize(ans);

	return ans;
}

struct plane *cloud_dispersion_plane(struct cloud *cloud, struct vector3 *ref)
{
	if (cloud->numpts < 3)
		return NULL;

	real xx = 0.0f;
	real xy = 0.0f;
	real xz = 0.0f;
	real yy = 0.0f;
	real yz = 0.0f;
	real zz = 0.0f;

	for (uint i = 0; i < cloud->numpts; i++) {
		struct vector3 *r = vector3_sub(&cloud->points[i], ref);

		xx += r->x * r->x;
		xy += r->x * r->y;
		xz += r->x * r->z;
		yy += r->y * r->y;
		yz += r->y * r->z;
		zz += r->z * r->z;

		vector3_free(r);
	}

	real det_x = yy * zz - yz * yz;
	real det_y = xx * zz - xz * xz;
	real det_z = xx * yy - xy * xy;

	real det_max = calc_max3(det_x, det_y, det_z);

	if (det_max <= 0.0f)
		return NULL;

	real x = 0.0f;
	real y = 0.0f;
	real z = 0.0f;

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

	vector3_free(normal);

	return plane;
}

struct vector3 *cloud_normal(struct cloud *cloud, struct vector3 *ref)
{
	struct plane *bestfit = cloud_dispersion_plane(cloud, ref);
	struct vector3 *normal = vector3_from_vector(bestfit->normal);

	plane_free(bestfit);

	return normal;
}

struct plane *cloud_plane_fitting(struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	struct plane *plane = cloud_dispersion_plane(cloud, center);

	vector3_free(center);

	return plane;
}

struct vector3 *cloud_point_faraway_bestfit(struct cloud *cloud)
{
	struct plane *bestfit = cloud_plane_fitting(cloud);
	struct vector3 *faraway = cloud_max_distance_from_plane(cloud, bestfit);

	plane_free(bestfit);

	return faraway;
}

real cloud_curvature(struct cloud *cloud)
{
	struct vector3 *center = cloud_get_center(cloud);
	struct vector3 *p = NULL;
	uint size = cloud->numpts;
	real radius = 0.0f;
	real a = center->x;
	real b = center->y;
	real c = center->z;

	for (uint i = 0; i < size; i++) {
		p = &cloud->points[i];

		a -= p->x;
		b -= p->y;
		c -= p->z;
	}

	a = center->x + (a / size);
	b = center->y + (b / size);
	c = center->z + (c / size);

	for (uint i = 0; i < size; i++) {
		p = &cloud->points[i];
		
		radius += sqrt(pow(p->x - a, 2) + pow(p->y - b, 2) + pow(p->z - c, 2));
	}

	radius /= size;

	vector3_free(center);

	return 1.0f / radius;
}

struct vector3 *cloud_remove_point(struct cloud *cloud, uint idx)
{
	if (idx >= cloud->numpts) {
		return NULL;
	}

	struct vector3 *removed_point;
	removed_point = vector3_from_vector(&cloud->points[idx]);

	if (cloud->numpts == 1) {
		cloud_free(cloud);
		cloud = cloud_empty();
	} else {
		cloud->numpts--;
		for (uint i = idx; i < cloud->numpts; i++) {
			vector3_copy(&cloud->points[i], &cloud->points[i + 1]);
		}

		uint new_size = cloud->numpts * sizeof(struct vector3);
		struct vector3 *new_points = realloc(cloud->points, new_size);
		if (!new_points)
			return NULL;

		cloud_calc_center(cloud);
	}

	return removed_point;
}

void cloud_ritter(struct cloud *cloud, struct vector3 **center, real *radius)
{
	struct vector3* vmin = vector3_from_vector(&cloud->points[0]);
	struct vector3* vmax = vector3_from_vector(&cloud->points[0]);
	struct vector3* p = NULL;
	
	for (uint i = 1; i < cloud->numpts; i++) {
		p = &cloud->points[i];
		
		if (p->x < vmin->x)
			vmin->x = p->x;
		if (p->y < vmin->y)
			vmin->y = p->y;
		if (p->z < vmin->z)
			vmin->z = p->z;
		
		if (p->x > vmax->x)
			vmax->x = p->x;
		if (p->y > vmax->y)
			vmax->y = p->y;
		if (p->z > vmax->z)
			vmax->z = p->z;
	}
	
	real xdiff = vmax->x - vmin->x;
	real ydiff = vmax->y - vmin->y;
	real zdiff = vmax->z - vmin->z;
	
	real diameter = calc_max3(xdiff, ydiff, zdiff);
	*radius = diameter * 0.5f;
	real sq_radius = *radius * *radius;
	*center = vector3_average(vmax, vmin);
	
	for (uint i = 0; i < cloud->numpts; i++) {
		p = &cloud->points[i];
		
		struct vector3 *direction = vector3_sub(p, *center);
		real sq_distance = vector3_length(direction);
		
		if (sq_distance > sq_radius) {
			real distance = sqrt(sq_distance);
			real difference = distance - *radius;
			
			diameter = (2.0f * *radius) + difference;
			*radius = diameter * 0.5f;
			sq_radius = *radius * *radius;
			difference *= 0.5f;
			
			vector3_scale(direction, difference);
			vector3_increase(*center, direction);
		}
		
		vector3_free(direction);
	}
}

void cloud_debug(struct cloud *cloud, FILE *output)
{
	if (cloud == NULL) {
		fprintf(output, "!!! cloud empty !!!\n");
		return;
	}

	for (uint i = 0; i < cloud->numpts; i++) {
		fprintf(output, "%le %le %le\n", cloud->points[i].x,
			                             cloud->points[i].y,
			                             cloud->points[i].z);
	}
}

