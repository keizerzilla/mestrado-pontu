/*
 * @file hu.h
 * @author Artur Rodrigues Rocha Neto
 * @date 2017
 * @brief Arquivo contendo as implementações necessárias para o cálculo dos
 * momentos invariantes de Hu para nuvens de pontos.
 *
 * Os momentos de Hu são 7 valores que descrevem uma nuvem com base em
 * heurísticas invariantes à escala, à rotação e à translação. Para nuvens de
 * pontos, calculamos esses 7 momentos para cada uma dos planos da nuvem (xy,
 * xz e yz). No total, 21 momentos são extráidos de cada nuvem.
 */

#ifndef HU_H
#define HU_H

#include "cloud.h"
#include "matrix.h"

/*
 * @brief A estrutura responsavel por salvar os momentos de Hu para uma dada
 * nuvem de pontos
 *
 * Os momentos de Hu são 7 valores que descrevem uma nuvem com base em
 * heurísticas invariantes à escala, à rotação e à translação. Para nuvens de
 * pontos, calculamos esses 7 momentos para cada uma dos planos da nuvem (xy,
 * xz e yz). No total, 21 momentos são extráidos de cada nuvem.
 */
struct hu {
    real i0;
    real i1;
    real i2;
    real i3;
    real i4;
    real i5;
    real i6;
};

/*
 * @brief hu_regular_moment Calcula o momento regular de Hu
 * @param p A ordem da dimensão x
 * @param q A ordem da dimensão y
 * @param r A ordem da dimensão z
 * @param cloud A nuvem alvo
 * @return O momento regular(p,q,r) da nuvem cloud
 */
real hu_regular_moment(int p, int q, int r, struct cloud* cloud)
{
    real moment = 0.0f;

    struct cloud* aux = cloud;
    while (aux != NULL) {
        moment += pow(aux->point->x, p)
                * pow(aux->point->y, q)
                * pow(aux->point->z, r);

        aux = aux->next;
    }

    return moment;
}

/*
 * @brief hu_central_moment Calcula o momento central de Hu
 * @param p A ordem da dimensão x
 * @param q A ordem da dimensão y
 * @param r A ordem da dimensão z
 * @param cloud A nuvem alvo
 * @return O momento central(p,q,r) da nuvem cloud
 */
real hu_central_moment(int p, int q, int r, struct cloud* cloud)
{
    real moment = 0.0f;
    struct vector3* center = cloud_get_center(cloud);

    struct cloud* aux = cloud;
    while (aux != NULL) {
        moment += pow(aux->point->x - center->x, p)
                * pow(aux->point->y - center->y, q)
                * pow(aux->point->z - center->z, r)
                * vector3_distance(aux->point, center);

        aux = aux->next;
    }

    vector3_free(center);

    return moment;
}

/*
 * @brief hu_normalized_moment Calcula o momento normalizado de Hu
 * @param p A ordem da dimensão x
 * @param q A ordem da dimensão y
 * @param r A ordem da dimensão z
 * @param cloud A nuvem alvo
 * @return O momento normalizado(p,q,r) da nuvem cloud
 */
real hu_normalized_moment(int p, int q, int r, struct cloud* cloud)
{
    real central = hu_central_moment(p, q, r, cloud);
    real zero = hu_central_moment(0, 0, 0, cloud);

    return central / (pow(zero, (p + q + r)/3) + 1);
}

/*
 * @brief hu_cloud_moments Calcula os momentos invariantes de Hu de uma nuvem
 * @param cloud A nuvem alvo
 * @param cut O corte da nuvem
 * @param results A matriz aonde os momentos serão salvos
 */
void hu_cloud_moments(struct cloud* cloud, real cut,
                      struct matrix* results)
{
    real a;
    real b;
    real c;
    real d;
    real e;
    real f;
    real g;

    struct hu hu_xy;

    a = hu_normalized_moment(0, 2, 0, cloud);
    b = hu_normalized_moment(0, 3, 0, cloud);
    c = hu_normalized_moment(1, 1, 0, cloud);
    d = hu_normalized_moment(1, 2, 0, cloud);
    e = hu_normalized_moment(2, 0, 0, cloud);
    f = hu_normalized_moment(2, 1, 0, cloud);
    g = hu_normalized_moment(3, 0, 0, cloud);

    hu_xy.i0 = e + a;

    hu_xy.i1 = pow((e - a), 2) + 4*pow(c, 2);

    hu_xy.i2 = pow((g - 3*d), 2) + pow((3*f - b), 2);

    hu_xy.i3 = pow((g + d), 2) + pow((f + b), 2);

    hu_xy.i4 = (g - 3*d)*(g + d)*(pow((g + d), 2) - 3*pow((f + b), 2)) +
               (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    hu_xy.i5 = (e - a)*(pow((g + d), 2) - pow((f + b), 2)) +
               4*c*(g + d) * (f + b);

    hu_xy.i6 = (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2)) -
               (g - 3*d)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    matrix_set(results, 0, 0, hu_xy.i0);
    matrix_set(results, 0, 1, hu_xy.i1);
    matrix_set(results, 0, 2, hu_xy.i2);
    matrix_set(results, 0, 3, hu_xy.i3);
    matrix_set(results, 0, 4, hu_xy.i4);
    matrix_set(results, 0, 5, hu_xy.i5);
    matrix_set(results, 0, 6, hu_xy.i6);

    struct hu hu_xz;

    a = hu_normalized_moment(0, 0, 2, cloud);
    b = hu_normalized_moment(0, 0, 3, cloud);
    c = hu_normalized_moment(1, 0, 1, cloud);
    d = hu_normalized_moment(1, 0, 2, cloud);
    e = hu_normalized_moment(2, 0, 0, cloud);
    f = hu_normalized_moment(2, 0, 1, cloud);
    g = hu_normalized_moment(3, 0, 0, cloud);

    hu_xz.i0 = e + a;

    hu_xz.i1 = pow((e - a), 2) + 4*pow(c, 2);

    hu_xz.i2 = pow((g - 3*d), 2) + pow((3*f - b), 2);

    hu_xz.i3 = pow((g + d), 2) + pow((f + b), 2);

    hu_xz.i4 = (g - 3*d)*(g + d)*(pow((g + d), 2) - 3*pow((f + b), 2)) +
               (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    hu_xz.i5 = (e - a)*(pow((g + d), 2) - pow((f + b), 2)) +
               4*c*(g + d) * (f + b);

    hu_xz.i6 = (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2)) -
               (g - 3*d)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    matrix_set(results, 0, 7, hu_xz.i0);
    matrix_set(results, 0, 8, hu_xz.i1);
    matrix_set(results, 0, 9, hu_xz.i2);
    matrix_set(results, 0, 10, hu_xz.i3);
    matrix_set(results, 0, 11, hu_xz.i4);
    matrix_set(results, 0, 12, hu_xz.i5);
    matrix_set(results, 0, 13, hu_xz.i6);

    struct hu hu_yz;

    a = hu_normalized_moment(0, 0, 2, cloud);
    b = hu_normalized_moment(0, 0, 3, cloud);
    c = hu_normalized_moment(0, 1, 1, cloud);
    d = hu_normalized_moment(0, 1, 2, cloud);
    e = hu_normalized_moment(0, 2, 0, cloud);
    f = hu_normalized_moment(0, 2, 1, cloud);
    g = hu_normalized_moment(0, 3, 0, cloud);

    hu_yz.i0 = e + a;

    hu_yz.i1 = pow((e - a), 2) + 4*pow(c, 2);

    hu_yz.i2 = pow((g - 3*d), 2) + pow((3*f - b), 2);

    hu_yz.i3 = pow((g + d), 2) + pow((f + b), 2);

    hu_yz.i4 = (g - 3*d)*(g + d)*(pow((g + d), 2) - 3*pow((f + b), 2)) +
               (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    hu_yz.i5 = (e - a)*(pow((g + d), 2) - pow((f + b), 2)) +
               4*c*(g + d) * (f + b);

    hu_yz.i6 = (3*f - b)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2)) -
               (g - 3*d)*(f + b)*(3*pow((g + d), 2) - pow((f + b), 2));

    matrix_set(results, 0, 14, hu_yz.i0);
    matrix_set(results, 0, 15, hu_yz.i1);
    matrix_set(results, 0, 16, hu_yz.i2);
    matrix_set(results, 0, 17, hu_yz.i3);
    matrix_set(results, 0, 18, hu_yz.i4);
    matrix_set(results, 0, 19, hu_yz.i5);
    matrix_set(results, 0, 20, hu_yz.i6);
}

#endif // HU_H

