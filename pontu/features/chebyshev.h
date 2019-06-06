#ifndef CHEBYSHEV_H
#define CHEBYSHEV_H

#ifndef CHEBYSHEV_ORDER_X
#define CHEBYSHEV_ORDER_X 2
#endif

#ifndef CHEBYSHEV_ORDER_Y
#define CHEBYSHEV_ORDER_Y 2
#endif

#ifndef CHEBYSHEV_ORDER_Z
#define CHEBYSHEV_ORDER_Z 2
#endif

#include "../core.h"
#include "matrix.h"

/**
 * \brief Calculates chebyshev polynomial
 * \param p Polynomial order
 * \param n Number of points of the cloud
 * \param x Polynomial argument
 * \return The chebyshev polynomial of order(p)
 */
real chebyshev_poly (int p, int n, real x);

/**
 * \brief Calculates a chebyshev moment
 * \param p Order of dimension x
 * \param q Order of dimension y
 * \param r Order of dimension z
 * \param cloud Target cloud
 * \return Chebyshev moment of order p+q+r
 */
real chebyshev_moment (int p, int q, int r, struct cloud* cloud);

/**
 * \brief Calculates all chebyshev moments
 * \param cloud Target cloud
 * \return Matrix with moments of the cloud
 */
struct matrix* chebyshev_cloud_moments (struct cloud* cloud);

#endif // CHEBYSHEV_H

