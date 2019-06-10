/**
 * \file vector3.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief 3D vector data structure.
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/**
 * \brief Struct to store 3D points
 */
struct vector3 {
	union {
		struct {
			real x, y, z;
		};
		struct {
			real coord[3];
		};
	};
};

/**
 * \brief Creates a new vector
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 * \return Pointer to the new vector
 */
struct vector3 *vector3_new(real x, real y, real z);

/**
 * \brief Creates a new empty vector
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 * \return Poiter to the new vector
 */
struct vector3 *vector3_zero();

/**
 * \brief Creates a new vector from another vector
 * \param src The source vector
 * \return Pointer to the new vector
 */
struct vector3 *vector3_from_vector(struct vector3 *src);

/**
 * \brief Creates a copy of a vector
 * \param dst The vector which will store the copy
 * \param src The vector to be copied
 */
void vector3_copy(struct vector3 *dst, struct vector3 *src);

/**
 * \brief Frees memory allocated by a vector
 * \param v The vector to be freed
 */
void vector3_free(struct vector3 *v);

/**
 * \brief Sums 2 vectors generating a new one
 * \param a The first vector
 * \param b The second vector
 * \return The vector generated
 */
struct vector3 *vector3_add(struct vector3 *a, struct vector3 *b);

/**
 * \brief Sums 2 vectors and stores in one of them
 * \param src The vector to be modfied
 * \param inc Vector to be summed to the src
 */
void vector3_increase(struct vector3 *src, struct vector3 *inc);
/**
 * \brief Subtracts 2 vectors generating a new one
 * \param a The first vector
 * \param b The second vector
 * \return The vector generated
 */
struct vector3 *vector3_sub(struct vector3 *a, struct vector3 *b);

/**
 * \brief Apply power n to each element of the vector
 * \param p The target vector
 * \param n The expoenent
 */
void vector3_pow(struct vector3 *p, real n);

/**
 * \brief Decreases a vector
 * \param src The vector to be modified
 * \param dec The vector to decrease
 */
void vector3_decrease(struct vector3 *src, struct vector3 *dec);

/**
 * \brief Set values of a vector os valores de um vetor
 * \param v The vector
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 */
void vector3_set(struct vector3 *v, real x, real y, real z);

/**
 * \brief Swaps the vector elements by their opposites
 * \param v The target vector
 */
void vector3_invert(struct vector3 *v);

/**
 * \brief Calculates the euclidian distance between two vectors
 * \param a First vector
 * \param b Second vector
 * \return Euclidian distance between a and b
 */
real vector3_distance(struct vector3 *a, struct vector3 *b);

/**
 * \brief Calculate the manhattan distance between two vectors
 * \param a First vector
 * \param b Second vector
 * \return Manhattan distance between a and b
 */
real vector3_manhattan(struct vector3 *a, struct vector3 *b);

/**
 * \brief Calculates the square length of a vector
 * \param v Target vector
 * \return Square length of the vector
 */
real vector3_squared_length(struct vector3 *v);

/**
 * \brief Calculates the length of a vector
 * \param v Target vector
 * \return Length of the vector
 */
real vector3_length(struct vector3 *v);

/**
 * \brief Calculates the squared length (2D) of a vector
 * \param v Target vector
 * \return Squared length of the vector
 */
real vector3_squared_length2(struct vector3 *v);

/**
 * \brief Calculates the length (2D) of a vector
 * \param v Target vector
 * \return Length of the vector
 */
real vector3_length2(struct vector3 *v);

/**
 * \brief Modifies the vector length keeping its direction
 * \param v Target vector
 * \param l New vector length
 */
void vector3_setlength(struct vector3 *v, real l);

/**
 * \brief Gets the unit vector of a vector
 * \param v Target vector
 * \return Pointer to the new unit vector
 */
struct vector3 *vector3_unit(struct vector3 *v);

/**
 * \brief Normalizes a vector
 * \param v Target vector
 */
void vector3_normalize(struct vector3 *v);

/**
 * \brief Scales a vector by a numerical factor
 * \param v Target vector
 * \param factor The factor
 */
void vector3_scale(struct vector3 *v, real f);

/**
 * \brief Rotates a vector around x
 * \param v Target vector
 * \param d Rotation angle
 */
void vector3_rotate_x(struct vector3 *v, real d);

/**
 * \brief Rotates a vector around y
 * \param v Target vector
 * \param d Rotation angle
 */
void vector3_rotate_y(struct vector3 *v, real d);

/**
 * \brief Rotates a vector around z
 * \param v Target vector
 * \param d Rotation angle
 */
void vector3_rotate_z(struct vector3 *v, real d);

/**
 * \brief Calculates the scalar product of two vectors
 * \param a First vector
 * \param b Second vector
 * \return Scalar product of a and b
 */
real vector3_dot(struct vector3 *a, struct vector3 *b);

/**
 * \brief Calculates the angle between two vectors
 * \param a First vector
 * \param b Second vector
 * \return The angle between a and b (radians)
 */
real vector3_angle(struct vector3 *a, struct vector3 *b);

/**
 * \brief Calculates the vector product between two vectors
 * \param a First vector
 * \param b Second vector
 * \return The vector product between a and b
 */
struct vector3 *vector3_cross(struct vector3 *a, struct vector3 *b);

/**
 * \brief Calculates the 2D vector product of two vectors
 * \param a First vector
 * \param b Second vector
 * \return 2D vector product of a and b
 */
real vector3_cross2(struct vector3 *a, struct vector3 *b);

/**
 * \brief Clears the z-axis from the vector
 * \param v Target vector
 */
void vector3_push2(struct vector3 *v);

/**
 * \brief Calculates the Minkowski distance between two vectors
 * \param v1 First vector
 * \param v2 Second vector
 * \param m Distance order
 * \return The distance of order m between v1 and v2
 */
real vector3_minkowski(struct vector3 *v1, struct vector3 *v2, real m);

/**
 * \brief Calculates the Chebyshev distance between two vectors
 * \param v1 First vector
 * \param v2 Second vector
 * \return Chebyshev distance between v1 and v2
 */
real vector3_chebyshev(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Modifies the sum of the square error (MODSSE) as distance
 * \param v1 First vector
 * \param v2 Second vector
 * \return MODSSE distance
 */
real vector3_modsse(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Calculates the cosino distance between two vectors
 * \param v1 First vector
 * \param v2 Second vector
 * \return Cosino distance between v1 and v2
 */
real vector3_cosdistance(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Calculates the mean square error distance
 * \param v1 First vector
 * \param v2 Second vector
 * \return MSE distance between v1 and v2
 */
real vector3_mse(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Calculates square euclidian distance
 * \param v1 First vector
 * \param v2 Second vector
 * \return SSE distance between v1 and v2
 */
real vector3_sse(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Calculates Chi distance
 * \param v1 First vector
 * \param v2 Second vector
 * \return Chi distance between v1 and v2
 */
real vector3_chi_distance(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Calculates Canberra distance
 * \param v1 First vector
 * \param v2 Second vector
 * \return Canberra distance between v1 and v2
 */
real vector3_canberra(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Calculates modified manhattan distance
 * \param v1 First vector
 * \param v2 Second vector
 * \return Modified manhattan distance between v1 and v2
 */
real vector3_modmanhattan(struct vector3 *v1, struct vector3 *v2);

/**
 * \brief Does linear interpolation of a vector
 * \param s Vector to be interpolated
 * \param t Target vector
 * \param v Interpolation speed
 */
void vector3_lerp(struct vector3 *s, struct vector3 *t, real v);

/**
 * \brief Calculates the volume of a 3D (x, y, z) box
 * \param v Vector holding the box dimensions
 * \return The volume
 */
real vector3_volume(struct vector3 *v);

/**
 * \brief Calculates the projection of a vector over another
 * \param a First vector
 * \param b Second vector
 * \return Projection vector of a over b
 */
struct vector3 *vector3_projection(struct vector3 *a, struct vector3 *b);

/**
 * \brief Gets the reflex of a vector in a plane
 * \param v Vector to be reflected
 * \param n Normal vector of the plane
 * \return Reflection vector of v in n
 */
struct vector3 *vector3_reflection(struct vector3 *v, struct vector3 *n);

/**
 * \brief Finds the surface normal in a triangle
 * \param a First vector
 * \param b Second vector
 * \param c Third vector
 * \return The normal vector
 */
struct vector3 *vector3_normal(struct vector3 *a,
			                   struct vector3 *b,
			                   struct vector3 *c);

/**
 * \brief Finds the area of a triangle surface
 * \param a First vector
 * \param b Second vector
 * \param c Third vector
 * \return Area of the surface
 */
real vector3_area(struct vector3 *a, struct vector3 *b, struct vector3 *c);

/**
 * \brief Calculates the Menger Curvature
 * \param a First vector
 * \param b Second vector
 * \param c Third vector
 * \return The curvature of the generated space (a, b, c)
 */
real vector3_menger_curvature(struct vector3 *a,
			                  struct vector3 *b,
			                  struct vector3 *c);

/**
 * \brief Average point between two vectors
 * \param a First vector
 * \param b Second vector
 * \return Average point between a and b
 */
struct vector3 *vector3_average(struct vector3 *a, struct vector3 *b);

/**
 * \brief Displays coordinates of a vector (debug a vector)
 * \param v Vector to be debugged
 * \param output Output file
 */
void vector3_debug(struct vector3 *v, FILE * output);

#endif // VECTOR3_H

