#include "../include/registration.h"

struct cloud *registration_closest_points_bf(struct cloud *source,
                                             struct cloud *target)
{
    struct cloud *closest_points = cloud_new(source->numpts);

    if (closest_points == NULL)
        return NULL;

    uint idx = 0;

    for (uint i = 0; i < source->numpts; i++) {
        idx = cloud_closest_point_idx(target, &source->points[i]);

        closest_points->points[i].x = target->points[idx].x;
        closest_points->points[i].y = target->points[idx].y;
        closest_points->points[i].z = target->points[idx].z;
    }

    return closest_points;
}

struct matrix *registration_align(struct cloud *source, struct cloud *target)
{
    cloud_calc_center(source);
    cloud_calc_center(target);

    struct matrix *source_aux = matrix_new(3, 1);
    struct matrix *target_aux = matrix_new(1, 3);

    matrix_set(source_aux, 0, 0, source->centroid->x);
    matrix_set(source_aux, 1, 0, source->centroid->y);
    matrix_set(source_aux, 2, 0, source->centroid->z);

    matrix_set(target_aux, 0, 0, target->centroid->x);
    matrix_set(target_aux, 0, 1, target->centroid->y);
    matrix_set(target_aux, 0, 2, target->centroid->z);

    struct matrix *centroid_prod = algebra_mat_prod(source_aux, target_aux);
    struct matrix *s = matrix_new(3, 3);

    for (uint i = 0; i < source->numpts; i++) {

        matrix_set(s, 0, 0,
                    matrix_get(s, 0, 0) +
                    source->points[i].x * target->points[i].x - 
                    matrix_get(centroid_prod, 0, 0));

        matrix_set(s, 0, 1,
                    matrix_get(s, 0, 1) +
                    source->points[i].x * target->points[i].y - 
                    matrix_get(centroid_prod, 0, 1));

        matrix_set(s, 0, 2,
                    matrix_get(s, 0, 2) +
                    source->points[i].x * target->points[i].z - 
                    matrix_get(centroid_prod, 0, 2));

        matrix_set(s, 1, 0,
                    matrix_get(s, 1, 0) +
                    source->points[i].y * target->points[i].x - 
                    matrix_get(centroid_prod, 1, 0));

        matrix_set(s, 1, 1,
                    matrix_get(s, 1, 1) +
                    source->points[i].y * target->points[i].y - 
                    matrix_get(centroid_prod, 1, 1));

        matrix_set(s, 1, 2,
                    matrix_get(s, 1, 2) +
                    source->points[i].y * target->points[i].z - 
                    matrix_get(centroid_prod, 1, 2));

        matrix_set(s, 2, 0,
                    matrix_get(s, 2, 0) +
                    source->points[i].z * target->points[i].x - 
                    matrix_get(centroid_prod, 2, 0));

        matrix_set(s, 2, 1,
                    matrix_get(s, 2, 1) +
                    source->points[i].z * target->points[i].y - 
                    matrix_get(centroid_prod, 2, 1));

        matrix_set(s, 2, 2,
                    matrix_get(s, 2, 2) +
                    source->points[i].z * target->points[i].z - 
                    matrix_get(centroid_prod, 2, 2));
    }

    s = algebra_mat_vs_scalar(s, 1.0f/((double)source->numpts));
    
    struct matrix *a = algebra_mat_sub(s, algebra_mat_transpose(s));
    struct matrix *q = matrix_new(4, 4);

    real tr_s = algebra_mat_trace(s);

    matrix_set(q, 0, 0, tr_s);
    matrix_set(q, 0, 1, matrix_get(a, 1, 2));
    matrix_set(q, 0, 2, matrix_get(a, 2, 0));
    matrix_set(q, 0, 3, matrix_get(a, 0, 1));
    matrix_set(q, 1, 0, matrix_get(a, 1, 2));
    matrix_set(q, 2, 0, matrix_get(a, 2, 0));
    matrix_set(q, 3, 0, matrix_get(a, 0, 1));

    matrix_set(q, 1, 1, matrix_get(s, 0, 0) * 2.0f - tr_s);
    matrix_set(q, 2, 2, matrix_get(s, 1, 1) * 2.0f - tr_s);
    matrix_set(q, 3, 3, matrix_get(s, 2, 2) * 2.0f - tr_s);

    matrix_set(q, 1, 2, matrix_get(s, 0, 1) + matrix_get(s, 1, 0));
    matrix_set(q, 1, 3, matrix_get(s, 0, 2) + matrix_get(s, 2, 0));
    matrix_set(q, 2, 3, matrix_get(s, 1, 2) + matrix_get(s, 2, 1));

    matrix_set(q, 2, 1, matrix_get(q, 1, 2));
    matrix_set(q, 3, 1, matrix_get(q, 1, 3));
    matrix_set(q, 3, 2, matrix_get(q, 2, 3));

    struct matrix *eig_val = algebra_mat_eigen_val(q);
    struct matrix *eig_vec = algebra_mat_eigen_vec(q, eig_val);

    uint max_eig_idx = 0;
    real eig_aux = creal(matrix_get(eig_val, 0, 0));
    for (uint i = 0; i < eig_val->rows; i++) {
        if (eig_aux < creal(matrix_get(eig_val, i, 0))) {
            eig_aux = creal(matrix_get(eig_val, i, 0));
            max_eig_idx = i;
        }
    }

    struct matrix *r = matrix_new(3, 3);

    real q0 = creal(matrix_get(eig_vec, 0, max_eig_idx));
    real q1 = creal(matrix_get(eig_vec, 1, max_eig_idx));
    real q2 = creal(matrix_get(eig_vec, 2, max_eig_idx));
    real q3 = creal(matrix_get(eig_vec, 3, max_eig_idx));

    real mag = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);

    q0 /= mag;
    q1 /= mag;
    q2 /= mag;
    q3 /= mag;

    matrix_set(r, 0, 0, q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);
    matrix_set(r, 0, 1, 2.0f * (q1 * q2 - q0 * q3));
    matrix_set(r, 0, 2, 2.0f * (q1 * q3 + q0 * q2));
    matrix_set(r, 1, 0, 2.0f * (q1 * q2 + q0 * q3));
    matrix_set(r, 1, 1, q0 * q0 + q2 * q2 - q1 * q1 - q3 * q3);
    matrix_set(r, 1, 2, 2.0f * (q2 * q3 - q0 * q1));
    matrix_set(r, 2, 0, 2.0f * (q1 * q3 - q0 * q2));
    matrix_set(r, 2, 1, 2.0f * (q2 * q3 + q0 * q1));
    matrix_set(r, 2, 2, q0 * q0 + q3 * q3 - q1 * q1 - q2 * q2);

    matrix_free(&target_aux);
    target_aux = matrix_new(3, 1);
    matrix_set(target_aux, 0, 0, target->centroid->x);
    matrix_set(target_aux, 1, 0, target->centroid->y);
    matrix_set(target_aux, 2, 0, target->centroid->z);

    struct matrix *t = algebra_mat_sub(target_aux,
                                        algebra_mat_prod(r, source_aux));
    struct matrix *rt = matrix_new(4, 4);

    matrix_set(rt, 0, 0, creal(matrix_get(r, 0, 0)));
    matrix_set(rt, 0, 1, creal(matrix_get(r, 0, 1)));
    matrix_set(rt, 0, 2, creal(matrix_get(r, 0, 2)));
    matrix_set(rt, 1, 0, creal(matrix_get(r, 1, 0)));
    matrix_set(rt, 1, 1, creal(matrix_get(r, 1, 1)));
    matrix_set(rt, 1, 2, creal(matrix_get(r, 1, 2)));
    matrix_set(rt, 2, 0, creal(matrix_get(r, 2, 0)));
    matrix_set(rt, 2, 1, creal(matrix_get(r, 2, 1)));
    matrix_set(rt, 2, 2, creal(matrix_get(r, 2, 2)));

    matrix_set(rt, 0, 3, creal(matrix_get(t, 0, 0)));
    matrix_set(rt, 1, 3, creal(matrix_get(t, 1, 0)));
    matrix_set(rt, 2, 3, creal(matrix_get(t, 2, 0)));

    matrix_set(rt, 3, 0, 0.0f);
    matrix_set(rt, 3, 1, 0.0f);
    matrix_set(rt, 3, 2, 0.0f);
    matrix_set(rt, 3, 3, 1.0f);


    matrix_free(&t);
    matrix_free(&target_aux);
    matrix_free(&source_aux);
    matrix_free(&r);
    matrix_free(&eig_val);
    matrix_free(&eig_vec);
    matrix_free(&q);
    matrix_free(&a);
    matrix_free(&s);
    matrix_free(&centroid_prod);

    return rt;
}

struct matrix *registration_icp(struct cloud *source,
                                 struct cloud *target,
                                 struct cloud **aligned,
                                 real t,
                                 uint k)
{
    struct cloud *eq_points = registration_closest_points_bf(source, target);
    if (eq_points == NULL)
        return NULL;

    cloud_free(aligned);
    *aligned = NULL;

    *aligned = cloud_copy(source);

    if (*aligned == NULL) {
        cloud_free(&eq_points);
        return NULL;
    }

    struct matrix *rt = registration_align(source, eq_points);
    if (rt == NULL) {
        cloud_free(&eq_points);
        cloud_free(aligned);
        return NULL;
    }

    struct matrix *rt_final = matrix_copy(rt);
    if (rt_final == NULL) {
        cloud_free(&eq_points);
        cloud_free(aligned);
        matrix_free(&rt);
        return NULL;
    }

    real err, dif_err;

    cloud_transform(*aligned, rt);

    if (*aligned == NULL) {
        cloud_free(&eq_points);
        matrix_free(&rt);
        matrix_free(&rt_final);
        return NULL;
    }

    dif_err = cloud_rmse(source, eq_points);
    if (dif_err == -1) {
        cloud_free(&eq_points);
        cloud_free(aligned);
        matrix_free(&rt);
        matrix_free(&rt_final);
        return NULL;
    }

    cloud_free(&eq_points);
    eq_points = NULL;

    eq_points = registration_closest_points_bf(*aligned, target);
    if (eq_points == NULL) {
        cloud_free(aligned);
        matrix_free(&rt);
        matrix_free(&rt_final);
        return NULL;
    }

    err = cloud_rmse(*aligned, eq_points);
    if (err == -1) {
        cloud_free(&eq_points);
        cloud_free(aligned);
        matrix_free(&rt);
        matrix_free(&rt_final);
        return NULL;
    }

    dif_err -= err;

    struct matrix* aux;

    for (uint i = 0; i < k; i++) {
        if (fabs(dif_err) < t) {
            break;
        }

        rt = registration_align(*aligned, eq_points);

        if (rt == NULL) {
            cloud_free(&eq_points);
            cloud_free(aligned);
            matrix_free(&rt_final);
            return NULL;
        }

        aux = algebra_mat_prod(rt, rt_final);
        if (rt_final == NULL) {
            cloud_free(&eq_points);
            cloud_free(aligned);
            matrix_free(&rt);
            return NULL;
        }

        matrix_free(&rt_final);
        rt_final = aux;

        cloud_transform(*aligned, rt);

        if (*aligned == NULL) {
            cloud_free(&eq_points);
            matrix_free(&rt);
            matrix_free(&rt_final);
            return NULL;
        }

        dif_err = err;

        cloud_free(&eq_points);
        eq_points = NULL;

        eq_points = registration_closest_points_bf(*aligned, target);
        if (eq_points == NULL) {
            cloud_free(aligned);
            matrix_free(&rt);
            matrix_free(&rt_final);
            return NULL;
        }

        err = cloud_rmse(*aligned, eq_points);
        if (err == -1) {
            cloud_free(&eq_points);
            cloud_free(aligned);
            matrix_free(&rt);
            matrix_free(&rt_final);
            return NULL;
        }

        dif_err -= err;
    }

    cloud_free(&eq_points);
    matrix_free(&rt);

    return rt_final;

}

