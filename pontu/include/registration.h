/**
 * \file registration.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Functions for clouds registrations.
 */

#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "./cloud.h"

struct cloud *registration_closest_points_bf(struct cloud *source,
                                             struct cloud *target);

struct cloud *registration_unique_closest_points_bf(struct cloud *source,
                                                    struct cloud *target);

struct cmatrix *registration_align(struct cloud *source, struct cloud *target);

struct cmatrix *registration_icp(struct cloud *source,
                                 struct cloud *target,
                                 struct cloud **aligned,
                                 real t,
                                 uint k);

#endif // REGISTRATION_H
