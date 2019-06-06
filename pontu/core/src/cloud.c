#include "../cloud.h"

struct cloud* cloud_new (uint num_pts)
{
    struct cloud* cloud = malloc(sizeof(struct cloud));
    if (!cloud)
        return NULL;

    cloud->points = malloc(num_pts * sizeof(struct vector3));
    if (!cloud->points) {
		free(cloud);
		cloud = NULL;
        return NULL;
    }
	
    memset(cloud->points, 0, num_pts * sizeof(struct vector3));
	
    cloud->centroid = NULL;
    cloud->num_pts = num_pts;

    return cloud;
}

struct cloud* cloud_empty ()
{
	return cloud_new(0);
}

void cloud_free (struct cloud* cloud)
{
    if (cloud) {
        free(cloud->points);
        if (cloud->centroid){
            vector3_free(cloud->centroid);
            cloud->centroid = NULL;
        }
        free(cloud);
        cloud = NULL;
    }
}

struct vector3* cloud_set_point_real (struct cloud* cloud,
                                             uint index,
                                             real x,
                                             real y,
                                             real z)
{
    if (index >= cloud->num_pts)
        return NULL;

    vector3_set(&cloud->points[index], x, y, z);
    
    return &cloud->points[index];
}

struct vector3* cloud_set_point_vector (struct cloud* cloud,
                                        uint index,
                                        struct vector3* point)
{
    return cloud_set_point_real(cloud, index, point->x, point->y, point->z);
}

struct vector3* cloud_add_point_real (struct cloud* cloud,
                                     real x,
                                     real y,
                                     real z)
{
    uint new_size = (cloud->num_pts + 1) * sizeof(struct vector3);

    struct vector3* new_points = realloc(cloud->points, new_size);
    if (!new_points)
        return NULL;
	
    cloud->points = new_points;
    cloud->num_pts++;

    vector3_set(&cloud->points[cloud->num_pts - 1], x, y, z);

    return &cloud->points[cloud->num_pts - 1];
}

struct vector3* cloud_add_point_vector (struct cloud* cloud, struct vector3* p)
{
    return cloud_add_point_real(cloud, p->x, p->y, p->z);
}

uint cloud_size (struct cloud* cloud)
{
    return cloud->num_pts;
}

struct cloud* cloud_cpy (struct cloud* cloud)
{
	struct cloud* cpy = cloud_new(cloud_size(cloud));
	
	for (uint index = 0; index < cloud->num_pts; index++) {
        cloud_set_point_real(cpy,
                             index,
                             cloud->points[index].x,
                             cloud->points[index].y,
                             cloud->points[index].z);
    }
	
	return cpy;
}

struct vector3* cloud_min_alpha (struct cloud* cloud)
{
	if (cloud_size(cloud))
		return NULL;
	
	struct vector3* p = &cloud->points[0];
	real min = p->alpha;
	
	for (uint i = 1; i < cloud_size(cloud); i++) {
		if (cloud->points[i].alpha < min) {
			min = cloud->points[i].alpha;
			p = &cloud->points[i];
		}
	}
	
	return p;
}

struct vector3* cloud_max_alpha (struct cloud* cloud)
{
	if (cloud_size(cloud))
		return NULL;
	
	struct vector3* p = &cloud->points[0];
	real max = p->alpha;
	
	for (uint i = 1; i < cloud_size(cloud); i++) {
		if (cloud->points[i].alpha > max) {
			max = cloud->points[i].alpha;
			p = &cloud->points[i];
		}
	}
	
	return p;
}

struct cloud* cloud_load_xyz (const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return NULL;

    uint num_pts = 0;
    while (!feof(file) && (fscanf(file, "%*s %*s %*s\n") != EOF))
        num_pts++;
	
    rewind(file);

    struct cloud* cloud = cloud_new(num_pts);
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

struct cloud* cloud_load_csv (const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return NULL;

    uint num_pts = 0;
    while (!feof(file) && (fscanf(file, "%*s,%*s,%*s\n") != EOF))
        num_pts++;
	
    rewind(file);

    struct cloud* cloud = cloud_new(num_pts);
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

struct cloud* cloud_load_ply (const char* filename)
{
	uint num_pts = 0;
	char buffer[80];
	
	FILE* file = fopen(filename, "r");
	if (!file)
		return NULL;

	if (fgets(buffer, 80, file)) {
		if (strcmp(buffer, "ply\n")) {
			fclose(file);
			return NULL;
		}
	} else {
		fclose(file);
		return NULL;
	}
	
	if (fgets(buffer, 80, file)) {
		if (strcmp(buffer, "format ascii 1.0\n")) {
			fclose(file);
			return NULL;
		}
	} else {
		fclose(file);
		return NULL;
	}
	
	while (fgets(buffer, 80, file)) {
		if (!strcmp(buffer, "end_header\n"))
			break;
		
		if (!strncmp(buffer, "element vertex", 14))
			sscanf(buffer, "element vertex %d\n", &num_pts);
	}
	
	struct cloud* cloud = cloud_new(num_pts);
	if (!cloud) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	for (uint i = 0; i < num_pts; i++) {
		if (fgets(buffer, 80, file)) {
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

struct cloud* cloud_load_pcd (const char* filename)
{
	uint num_pts = 0;
	char buffer[80];
	
	FILE* file = fopen(filename, "r");
	if (!file)
		return NULL;

	while (fgets(buffer, 80, file)) {
		if (!strcmp(buffer, "DATA ascii\n"))
			break;
		
		if (!strncmp(buffer, "POINTS", 6))
			sscanf(buffer, "POINTS %d\n", &num_pts);
	}
	
	struct cloud* cloud = cloud_new(num_pts);
	if (!cloud) {
		fclose(file);
		return NULL;
	}
	
	real x = 0;
	real y = 0;
	real z = 0;
	uint index = 0;
	for (uint i = 0; i < num_pts; i++) {
		if (fgets(buffer, 80, file)) {
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

struct cloud* cloud_load_obj (const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return NULL;
    
    struct cloud* cloud = cloud_empty();
    real x = 0;
    real y = 0;
    real z = 0;
    
    while (!feof(file)) {
        char buffer[512];
        fgets(buffer, 512, file);
        
        if (buffer[0] == 'v' && buffer[1] == ' ') {
        	sscanf(buffer, "v %lf %lf %lf\n", &x, &y, &z);
        	cloud_add_point_real(cloud, x, y, z);
        }
    }

    fclose(file);

    return cloud;
}

int cloud_save_xyz (struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
        return 0;

    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le %le %le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

int cloud_save_csv (struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
        return 0;

    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le,%le,%le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

int cloud_save_ply (struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
	if (file)
		return 0;

	fprintf(file, "ply\n");
	fprintf(file, "format ascii 1.0\n");
	fprintf(file, "comment dumped by artur@interfaces-ufc-br\n");
	fprintf(file, "element vertex %d\n", cloud_size(cloud));
	fprintf(file, "property float x\n");
	fprintf(file, "property float y\n");
	fprintf(file, "property float z\n");
	fprintf(file, "property uchar red\n");
	fprintf(file, "property uchar green\n");
	fprintf(file, "property uchar blue\n");
	fprintf(file, "end_header\n");
	
	real max_alpha = cloud_max_alpha(cloud)->alpha;
	real min_alpha = cloud_min_alpha(cloud)->alpha;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
    	float f = cloud->points[i].alpha;
    	unsigned short x = ((f - min_alpha) / (max_alpha - min_alpha)) * 255;
    	
    	unsigned short r = x;
    	unsigned short g = x;
    	unsigned short b = x;
        fprintf(file, "%le %le %le %hu %hu %hu\n", cloud->points[i].x,
                                                   cloud->points[i].y,
                                                   cloud->points[i].z,
                                                   r, g, b);
    }
	
    fclose(file);

    return 1;
}

int cloud_save_pcd (struct cloud* cloud, const char* filename)
{
    FILE* file = fopen(filename, "w");
	if (!file)
		return 0;

	fprintf(file, "VERSION .7\n");
	fprintf(file, "FIELDS x y z\n");
	fprintf(file, "SIZE 4 4 4\n");
	fprintf(file, "TYPE F F F\n");
	fprintf(file, "COUNT 1 1 1\n");
	fprintf(file, "WIDTH %d\n", cloud_size(cloud));
	fprintf(file, "HEIGHT 1\n");
	fprintf(file, "VIEWPOINT 0 0 0 1 0 0 0\n");
	fprintf(file, "POINTS %d\n", cloud_size(cloud));
	fprintf(file, "DATA ascii\n");
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(file, "%le %le %le\n", cloud->points[i].x,
                                       cloud->points[i].y,
                                       cloud->points[i].z);
    }

    fclose(file);

    return 1;
}

struct vector3* cloud_calc_center (struct cloud* cloud)
{
	if (cloud->centroid)
		vector3_free(cloud->centroid);
	
	cloud->centroid = vector3_zero();

    for (uint i = 0; i < cloud->num_pts; i++) {
        cloud->centroid->x += cloud->points[i].x;
        cloud->centroid->y += cloud->points[i].y;
        cloud->centroid->z += cloud->points[i].z;
    }

    cloud->centroid->x /= cloud->num_pts;
    cloud->centroid->y /= cloud->num_pts;
    cloud->centroid->z /= cloud->num_pts;

    return cloud->centroid;
}

struct vector3* cloud_get_center (struct cloud* cloud)
{
    if (cloud->centroid)
        return vector3_from_vector(cloud->centroid);

    return vector3_from_vector(cloud_calc_center(cloud));
}

void cloud_scale (struct cloud* cloud, real f)
{
    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_scale(&cloud->points[i], f);
}

void cloud_translate_vector_dir (struct cloud* cloud,
                                 struct vector3* origin,
                                 struct vector3* dest)
{
	struct vector3* t = vector3_sub(origin, dest);

    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_increase(&cloud->points[i], t);

    vector3_free(t);
    
	cloud_calc_center(cloud);
}

void cloud_translate_vector (struct cloud* cloud, struct vector3* dest)
{
	struct vector3* center = cloud_get_center(cloud);
    struct vector3* t = vector3_sub(dest, center);

    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_increase(&cloud->points[i], t);

    vector3_free(t);
    vector3_free(center);
    
	cloud_calc_center(cloud);
}

void cloud_translate_real (struct cloud* cloud, real x, real y, real z)
{
    struct vector3* dest = vector3_new(x, y, z);
    struct vector3* t = vector3_sub(dest, cloud_get_center(cloud));

    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_increase(&cloud->points[i], t);

    vector3_free(dest);
    vector3_free(t);
    cloud_calc_center(cloud);
}

void cloud_rotate_x (struct cloud* cloud, real d)
{
	printf("%s: incomplete function", __FUNCTION__);
    for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_x(&cloud->points[i], d);
}

void cloud_rotate_y (struct cloud* cloud, real d)
{
	printf("%s: incomplete function", __FUNCTION__);
	for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_y(&cloud->points[i], d);
}

void cloud_rotate_z (struct cloud* cloud, real d)
{
	printf("%s: incomplete function", __FUNCTION__);
	for (uint i = 0; i < cloud->num_pts; i++)
        vector3_rotate_z(&cloud->points[i], d);
}

real cloud_mean_x (struct cloud* cloud)
{
	real mx = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++)
        mx += cloud->points[i].x;
	
	return mx / cloud_size(cloud);
}

real cloud_mean_y (struct cloud* cloud)
{
	real my = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++)
        my += cloud->points[i].y;
	
	return my / cloud_size(cloud);
}

real cloud_mean_z (struct cloud* cloud)
{
	real mz = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++)
        mz += cloud->points[i].z;
	
	return mz / cloud_size(cloud);
}

int cloud_compare (const void* p1, const void* p2)
{
    struct vector3* dp1 = (struct vector3*)p1;
    struct vector3* dp2 = (struct vector3*)p2;
	
    return dp2->z - dp1->z;
}

void cloud_sort (struct cloud* cloud)
{
    qsort(cloud->points, cloud->num_pts, sizeof(struct vector3), cloud_compare);
}

int cloud_polar_compare (const void* p1, const void* p2)
{
	struct vector3* dp1 = (struct vector3*)p1;
    struct vector3* dp2 = (struct vector3*)p2;
	
    return dp2->alpha - dp1->alpha;
}

void cloud_polar_sort (struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	struct vector3* p = NULL;
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		p = &cloud->points[i];
		p->alpha = atan2(p->y - center->y, p->x - center->x);
	}
	
	qsort(cloud->points,
	      cloud->num_pts,
	      sizeof(struct vector3),
	      cloud_polar_compare);
	
	vector3_free(center);
}

struct cloud* cloud_concat (struct cloud* c1, struct cloud* c2)
{
    uint size_c1 = cloud_size(c1);
    uint size_c2 = cloud_size(c2);

    struct cloud* c3 = cloud_new(size_c1 + size_c2);

    for (uint i = 0; i < size_c1; i++)
        cloud_set_point_vector(c3, i, &c1->points[i]);

    for (uint j = 0; j < size_c2; j++)
        cloud_set_point_vector(c3, j + size_c1, &c2->points[j]);

    return c3;
}

struct vector3* cloud_axis_size (struct cloud* cloud)
{
    if (!cloud->num_pts)
        return vector3_zero();

    real max_x = cloud->points[0].x;
    real min_x = cloud->points[0].x;
    real max_y = cloud->points[0].y;
    real min_y = cloud->points[0].y;
    real max_z = cloud->points[0].z;
    real min_z = cloud->points[0].z;

    for (uint i = 1; i < cloud->num_pts; i++) {
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

real cloud_boundingbox_area (struct cloud* cloud)
{
	struct vector3* axis = cloud_axis_size(cloud);
	real area = (2.0f * axis->x * axis->y) +
	            (2.0f * axis->y * axis->z) +
	            (2.0f * axis->x * axis->z);
	vector3_free(axis);
	
	return area;
}

real cloud_boundingbox_volume (struct cloud* cloud)
{
	struct vector3* axis = cloud_axis_size(cloud);
	real vol = vector3_volume(axis);
	vector3_free(axis);
	
	return vol;
}

real cloud_function_volume (struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real vol = 0.0f;
	
	for (uint i = 0; i < cloud_size(cloud); i++)
		vol += vector3_distance(center, &cloud->points[i]);
	
	vector3_free(center);
	
	return vol;
}

struct cloud* cloud_cut_radius (struct cloud* cloud, struct vector3* p, real r)
{
	struct cloud* sub = cloud_empty();
    
    for (uint i = 0; i < cloud->num_pts; i++) {
        if (vector3_distance(p, &cloud->points[i]) <= r)
            cloud_add_point_vector(sub, &cloud->points[i]);
    }
    
    return sub;
}

struct cloud* cloud_cut_center (struct cloud* cloud, real cut)
{
    struct cloud* sub = cloud_empty();
    struct vector3* center = cloud_get_center(cloud);

    for (uint i = 0; i < cloud->num_pts; i++) {
        if (vector3_distance(center, &cloud->points[i]) <= cut)
            cloud_add_point_vector(sub, &cloud->points[i]);
    }

    return sub;
}

struct cloud* cloud_cut_plane (struct cloud* cloud, struct plane* plane)
{
    struct cloud* sub = cloud_empty();
    
    for (uint i = 0; i < cloud->num_pts; i++)
		if (plane_on_direction(plane, &cloud->points[i]))
			cloud_add_point_vector(sub, &cloud->points[i]);
	
    return sub;
}

int cloud_plane_partition (struct cloud* src,
                           struct plane* plane,
                           struct cloud* par1,
                           struct cloud* par2)
{
	if (cloud_size(par1) || cloud_size(par2))
		return 0;
	
	for (uint i = 0; i < src->num_pts; i++) {
		if (plane_on_direction(plane, &src->points[i]))
			cloud_add_point_vector(par1, &src->points[i]);
		else
			cloud_add_point_vector(par2, &src->points[i]);
	}
	
	return 1;
}

struct vector3* cloud_max_distance_from_plane (struct cloud* cloud,
                                               struct plane* plane)
{
	struct vector3* p = NULL;
	real d = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
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

struct cloud* cloud_cut_cylinder (struct cloud* cloud,
                                  struct vector3* ref,
                                  struct vector3* dir,
                                  real radius)
{
	struct cloud* sub = cloud_empty();
	real dirl = vector3_length(dir);
	real dist = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		struct vector3* dot = vector3_sub(ref, &cloud->points[i]);
		struct vector3* cross = vector3_cross(dot, dir);
		
		dist = vector3_length(cross) / dirl;
		if (dist <= radius)
			cloud_add_point_vector(sub, &cloud->points[i]);
		
		vector3_free(dot);
		vector3_free(cross);
	}
	
	return sub;
}

struct cloud* cloud_segment (struct cloud* cloud,
                             struct vector3* ref,
                             struct vector3* dir,
                             real epslon)
{
	struct cloud* sub = cloud_empty();
	struct plane* plane = plane_new(dir, ref);
	
	for (uint i = 0; i < cloud->num_pts; i++)
		if (plane_distance2point(plane, &cloud->points[i]) <= epslon)
			cloud_add_point_vector(sub, &cloud->points[i]);
	
	plane_free(plane);
	
	return sub;
}

struct vector3* cloud_closest_point (struct cloud* cloud, struct vector3* point)
{
	uint index = 0;
	real tempd = 0;
	real d = vector3_distance(point, &cloud->points[0]);
	
	for (uint i = 1; i < cloud->num_pts; i++) {
        tempd = vector3_distance(point, &cloud->points[i]);
        if (tempd < d) {
            d = tempd;
            index = i;
		}
    }
    
    return &cloud->points[index];
}

struct vector3* cloud_closest_to_center (struct cloud* cloud)
{
	return cloud_closest_point(cloud, cloud_get_center(cloud));
}

struct vector3* cloud_min_x (struct cloud* cloud)
{
	uint index = 0;
	real min_x = cloud->points[0].x;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (min_x > cloud->points[i].x) {
            min_x = cloud->points[i].x;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

struct vector3* cloud_min_y (struct cloud* cloud)
{
	uint index = 0;
	real min_y = cloud->points[0].y;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (min_y > cloud->points[i].y) {
            min_y = cloud->points[i].y;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

struct vector3* cloud_min_z (struct cloud* cloud)
{
	uint index = 0;
	real minz = cloud->points[0].z;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (minz > cloud->points[i].z) {
            minz = cloud->points[i].z;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

struct vector3* cloud_max_x (struct cloud* cloud)
{
	uint index = 0;
	real max_x = cloud->points[0].x;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (max_x < cloud->points[i].x) {
            max_x = cloud->points[i].x;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

struct vector3* cloud_max_y (struct cloud* cloud)
{
	uint index = 0;
	real max_y = cloud->points[0].y;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (max_y < cloud->points[i].y) {
            max_y = cloud->points[i].y;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

struct vector3* cloud_max_z (struct cloud* cloud)
{
	uint index = 0;
	real max_z = cloud->points[0].z;
	for (uint i = 1; i < cloud->num_pts; i++) {
        if (max_z < cloud->points[i].z) {
            max_z = cloud->points[i].z;
            index = i;
		}
    }
    
    return vector3_from_vector(&cloud->points[index]);
}

real cloud_max_distance (struct cloud* cloud, struct vector3* p)
{
	real d = vector3_distance(p, &cloud->points[0]);
	real temp = 0.0f;
	
	for (uint i = 1; i < cloud->num_pts; i++) {
		temp = vector3_distance(p, &cloud->points[i]);
		
		if (temp > d)
			d = temp;
	}
	
	return d;
}

real cloud_max_distance_from_center (struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	real d = cloud_max_distance(cloud, center);
	
	vector3_free(center);
	
	return d;
}

struct vector3* cloud_average_direction (struct cloud* cloud)
{
	struct vector3* ans = vector3_zero();
	
	for (uint i = 0; i < cloud->num_pts; i++)
		vector3_increase(ans, &cloud->points[i]);
	
	vector3_normalize(ans);
	
	return ans;
}

struct plane* cloud_dispersion_plane (struct cloud* cloud, struct vector3* ref)
{
	if (cloud_size(cloud) < 3)
		return NULL;
	
	real xx = 0.0f;
	real xy = 0.0f;
	real xz = 0.0f;
	real yy = 0.0f;
	real yz = 0.0f;
	real zz = 0.0f;
	
	for (uint i = 0; i < cloud->num_pts; i++) {
		struct vector3* r = vector3_sub(&cloud->points[i], ref);
		
		xx += r->x * r->x;
		xy += r->x * r->y;
		xz += r->x * r->z;
		yy += r->y * r->y;
		yz += r->y * r->z;
		zz += r->z * r->z;
		
		vector3_free(r);
	}
	
	real det_x = yy*zz - yz*yz;
	real det_y = xx*zz - xz*xz;
	real det_z = xx*yy - xy*xy;
	
	real det_max = calc_max3(det_x, det_y, det_z);
	
	if (det_max <= 0.0f)
		return NULL;
	
	real x = 0.0f;
	real y = 0.0f;
	real z = 0.0f;
	
	if (det_max == det_x) {
		x = det_x;
		y = xz*yz - xy*zz;
		z = xy*yz - xz*yy;
	} else if (det_max == det_y) {
		x = xz*yz - xy*zz;
		y = det_y;
		z = xy*xz - yz*xx;
	} else {
		x = xy*yz - xz*yy;
		y = xy*xz - yz*xx;
		z = det_z;
	}
	
	struct vector3* normal = vector3_new(x, y, z);
	vector3_normalize(normal);
	
	struct plane* plane = plane_new(normal, ref);
	
	vector3_free(normal);
	
	return plane;
}

struct vector3* cloud_normal (struct cloud* cloud, struct vector3* ref)
{
	struct plane* bestfit = cloud_dispersion_plane(cloud, ref);
	struct vector3* normal = vector3_from_vector(bestfit->normal);
	
	plane_free(bestfit);
	
	return normal;
}

struct plane* cloud_plane_fitting (struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	struct plane* plane =  cloud_dispersion_plane(cloud, center);
	
	vector3_free(center);
	
	return plane;
}

struct vector3* cloud_point_faraway_bestfit (struct cloud* cloud)
{
	struct plane* bestfit = cloud_plane_fitting(cloud);
	struct vector3* faraway = cloud_max_distance_from_plane(cloud, bestfit);
	
	plane_free(bestfit);
	
	return faraway;
}

real cloud_curvature (struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	struct vector3* p = NULL;
	int size = cloud_size(cloud);
	real radius = 0.0f;
	real a = center->x;
	real b = center->y;
	real c = center->z;
	
	for (int i = 0; i < size; i++) {
		p = &cloud->points[i];
		
		a -= p->x;
		b -= p->y;
		c -= p->z;
	}
	
	a = center->x + (a / size);
	b = center->y + (b / size);
	c = center->z + (c / size);
	
	for (int i = 0; i < size; i++) {
		p = &cloud->points[i];
		
		radius += sqrt(pow(p->x - a, 2) + pow(p->y - b, 2) + pow(p->z - c, 2));
	}
	
	radius /= size;
	
	vector3_free(center);
	
	return 1.0f / radius;
}

void cloud_dist_sort (struct cloud* cloud)
{
	struct vector3* center = cloud_get_center(cloud);
	struct vector3* p = NULL;
	
	struct plane* bestfit = cloud_plane_fitting(cloud);
	
	for (uint i = 0; i < cloud_size(cloud); i++) {
		p = &cloud->points[i];
		p->alpha = plane_distance2point(bestfit, p);
	}
	
	qsort(cloud->points,
	      cloud->num_pts,
	      sizeof(struct vector3),
	      cloud_polar_compare);
	
	plane_free(bestfit);
	vector3_free(center);
}

struct vector3* cloud_remove_point (struct cloud* cloud, uint idx)
{
	if (idx >= cloud->num_pts || idx < 0) {
		return NULL;
	}

	struct vector3* removed_point;
	removed_point = vector3_from_vector(&cloud->points[idx]);

	if (cloud->num_pts == 1) {	
		cloud_free(cloud);
		cloud = cloud_empty();
	} else {
		cloud->num_pts--;
		for (uint i = idx; i < cloud->num_pts; i++) {
			vector3_copy(&cloud->points[i], &cloud->points[i+1]);
		}
		
		uint new_size = cloud->num_pts * sizeof(struct vector3);
		struct vector3* new_points = realloc(cloud->points, new_size);
		if (!new_points)
			return NULL;
		
		cloud_calc_center(cloud);
	}

	return removed_point;
}

void cloud_debug (struct cloud* cloud, FILE* output)
{
	if (!cloud) {
		fprintf(output, "!!! empty cloud !!!");
		return;
	}
	
    for (uint i = 0; i < cloud->num_pts; i++) {
        fprintf(output, "%le %le %le", cloud->points[i].x,
                        		       cloud->points[i].y,
                               		   cloud->points[i].z);
	}
}

