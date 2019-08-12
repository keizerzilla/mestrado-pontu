/**
 * \file golden.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Moments based in the golden ratio
 */

#ifndef GOLDEN_H
#define GOLDEN_H

#include "cloud.h"
#include "matrix.h"

real golden_phi(real a);

real golden_moment(int p, int q, int r, struct cloud *cloud);

struct matrix *golden_cloud_moments(struct cloud *cloud, int p, int q, int r);

#endif // GOLDEN_H

