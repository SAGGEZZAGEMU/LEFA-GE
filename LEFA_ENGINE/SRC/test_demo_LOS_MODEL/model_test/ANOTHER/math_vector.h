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
 * \addtogroup LIMatVector Vector
 * @{
 */

#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include <math.h>
#include"math_generic.h"
//#include"math_only.h"



#define LIMAT_VECTOR_EPSILON 0.0001f


/*
 * \brief Returns a vector with the given components.
 * \param x Float.
 * \param y Float.
 * \param z Float.
 * \return Vector.
 */
static inline vec3 limat_vector_init(float x, float y, float z)
{
	vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
	return result;
}

/**
 * \brief Compares two vertices using a threshold.
 * \param self Vector.
 * \param vector Vector.
 * \param threshold Threshold for each coordinate.
 * \return Vector.
 */
static inline int limat_vector_compare(vec3 self,
									   vec3 vector,
									   float threshold)
{
	vec3 tmp;
    tmp.x = self.x - vector.x;
    tmp.y = self.y - vector.y;
    tmp.z = self.z - vector.z;

	if (-threshold <= tmp.x && tmp.x < threshold &&
	    -threshold <= tmp.y && tmp.y < threshold &&
	    -threshold <= tmp.z && tmp.z < threshold)
		return 1;
	return 0;
}

/**
 * \brief Gets the opposite vector.
 * \param self Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_invert (
	vec3 self)
{
    vec3 result;
    result.x = -self.x;
    result.y = -self.y;
    result.z = -self.z;

	return result;
}

/**
 * \brief Gets the length of the vector.
 * \param self Vector.
 * \return Float.
 */
static inline float limat_vector_get_length (
	vec3 self)
{
	return sqrt (self.x * self.x + self.y * self.y + self.z * self.z);
}

/**
 * \brief Adds a vector to another.
 * \param self Vector.
 * \param vector Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_add (
	vec3 self,
	vec3 vector)
{
	vec3 result;

	result.x = self.x + vector.x;
	result.y = self.y + vector.y;
	result.z = self.z + vector.z;
	return result;
}

/**
 * \brief Subtracts a vector from another.
 * \param self Vector.
 * \param vector Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_subtract (
	vec3 self,
	vec3 vector)
{
	vec3 result;

	result.x = self.x - vector.x;
	result.y = self.y - vector.y;
	result.z = self.z - vector.z;
	return result;
}

/**
 * \brief Multiplies the vector by a scalar.
 * \param self Vector.
 * \param scalar Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_multiply (
	vec3 self,
	float       scalar)
{
	vec3 result;

	result.x = self.x * scalar;
	result.y = self.y * scalar;
	result.z = self.z * scalar;
	return result;
}

/**
 * \brief Normalizes the vector.
 * \param self Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_normalize (
	vec3 self)
{
	vec3 result;
	float len = limat_vector_get_length (self);

	if (len < LIMAT_VECTOR_EPSILON)
	{
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
	}
	else
	{
		result.x = self.x / len;
		result.y = self.y / len;
		result.z = self.z / len;
	}
	return result;
}

/**
 * \brief Removes all non-finite values from the vector.
 * \param self Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_validate(vec3 self)
{
	vec3 result;
	result.x = limat_number_validate(self.x);
	result.y = limat_number_validate(self.y);
	result.z = limat_number_validate(self.z);
	return result;
}

/**
 * \brief Calculates the scalar product of two vectors.
 * \param self Vector.
 * \param vector Vector.
 * \return Scalar.
 */
static inline float limat_vector_dot (
	vec3 self,
	vec3 vector)
{
	return self.x * vector.x + self.y * vector.y + self.z * vector.z;
}

/**
 * \brief Calculates the cross product of two vectors.
 * \param self Vector.
 * \param vector Vector.
 * \return Vector.
 */
static inline vec3 limat_vector_cross (
	vec3 self,
	vec3 vector)
{
	vec3 result;

	result.x =  (self.y * vector.z - self.z * vector.y);
	result.y = -(self.x * vector.z - self.z * vector.x);
	result.z =  (self.x * vector.y - self.y * vector.x);
	return result;
}

/**
 * \brief Performs linear vector interpolation.
 * \param self First interpolated vector.
 * \param vector Second interpolated vector.
 * \param weight Interpolating scalar.
 * \return Vector.
 */
static inline vec3 limat_vector_lerp(vec3 self,
									 vec3 vector,
									 float weight)
{
	float a = weight;
	float b = 1.0f - weight;
	vec3 result;
	
	result.x = a * self.x + b * vector.x;
	result.y = a * self.y + b * vector.y;
	result.z = a * self.z + b * vector.z;

	return result;
}

#endif

/** @} */
/** @} */
