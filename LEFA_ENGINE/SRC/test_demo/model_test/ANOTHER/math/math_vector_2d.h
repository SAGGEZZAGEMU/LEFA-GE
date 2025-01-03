/* Lips of Suna
 * Copyright© 2007-2013 Lips of Suna development team.
 *
 * Lips of Suna is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Lips of Suna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Lips of Suna. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \addtogroup LIMat Math
 * @{
 * \addtogroup LIMatVector2d Vector2d
 * @{
 */

#ifndef __MATH_VECTOR_2D_H__
#define __MATH_VECTOR_2D_H__

#include <math.h>
#include "math-generic.h"

/**
 * \brief A three-dimensional vector type.
 */
typedef struct _LIMatVector2d LIMatVector2d;
struct _LIMatVector2d
{
	float x;
	float y;
};

/**
 * \brief Returns a vector with the given components.
 * \param x Float.
 * \param y Float.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_init (
	float x,
	float y)
{
	LIMatVector2d result = { x, y };
	return result;
}

/**
 * \brief Compares two vertices using a threshold.
 * \param self Vector.
 * \param vector Vector.
 * \param threshold Threshold for each coordinate.
 * \return Vector.
 */
static inline int limat_vector2d_compare (
	LIMatVector2d self,
	LIMatVector2d vector,
	float         threshold)
{
	LIMatVector2d tmp =
	{
		self.x - vector.x,
		self.y - vector.y
	};
	if (-threshold <= tmp.x && tmp.x < threshold &&
	    -threshold <= tmp.y && tmp.y < threshold)
		return 1;
	return 0;
}

/**
 * \brief Gets the opposite vector.
 * \param self Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_invert (
	LIMatVector2d self)
{
	LIMatVector2d result = { -self.x, -self.y };
	return result;
}

/**
 * \brief Gets the length of the vector.
 * \param self Vector.
 * \return Float.
 */
static inline float limat_vector2d_get_length (
	LIMatVector2d self)
{
	return sqrt (self.x * self.x + self.y * self.y);
}

/**
 * \brief Adds a vector to another.
 * \param self Vector.
 * \param vector Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_add (
	LIMatVector2d self,
	LIMatVector2d vector)
{
	LIMatVector2d result;

	result.x = self.x + vector.x;
	result.y = self.y + vector.y;
	return result;
}

/**
 * \brief Subtracts a vector from another.
 * \param self Vector.
 * \param vector Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_subtract (
	LIMatVector2d self,
	LIMatVector2d vector)
{
	LIMatVector2d result;

	result.x = self.x - vector.x;
	result.y = self.y - vector.y;
	return result;
}

/**
 * \brief Multiplies the vector by a scalar.
 * \param self Vector.
 * \param scalar Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_multiply (
	LIMatVector2d self,
	float         scalar)
{
	LIMatVector2d result;

	result.x = self.x * scalar;
	result.y = self.y * scalar;
	return result;
}

/**
 * \brief Normalizes the vector.
 * \param self Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_normalize (
	LIMatVector2d self)
{
	LIMatVector2d result;
	float len = limat_vector2d_get_length (self);

	if (len < LIMAT_EPSILON)
	{
		result.x = 0.0f;
		result.y = 0.0f;
	}
	else
	{
		result.x = self.x / len;
		result.y = self.y / len;
	}
	return result;
}

/**
 * \brief Removes all non-finite values from the vector.
 * \param self Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_validate (
	LIMatVector2d self)
{
	LIMatVector2d result =
	{
		limat_number_validate (self.x),
		limat_number_validate (self.y)
	};
	return result;
}

/**
 * \brief Calculates the scalar product of two vectors.
 * \param self Vector.
 * \param vector Vector.
 * \return Scalar.
 */
static inline float limat_vector2d_dot (
	LIMatVector2d self,
	LIMatVector2d vector)
{
	return self.x * vector.x + self.y * vector.y;
}

/**
 * \brief Gets the normal of the vector.
 * \param self Vector.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_get_normal (
	LIMatVector2d self)
{
	LIMatVector2d result;

	result.x = self.y;
	result.y = -self.x;
	result = limat_vector2d_normalize (result);

	return result;
}

/**
 * \brief Performs linear vector interpolation.
 * \param self First interpolated vector.
 * \param vector Second interpolated vector.
 * \param weight Interpolating scalar.
 * \return Vector.
 */
static inline LIMatVector2d limat_vector2d_lerp (
	LIMatVector2d self,
	LIMatVector2d vector,
	float         weight)
{
	float a = weight;
	float b = 1.0f - weight;
	LIMatVector2d result;

	result.x = a * self.x + b * vector.x;
	result.y = a * self.y + b * vector.y;

	return result;
}

#endif

/** @} */
/** @} */
