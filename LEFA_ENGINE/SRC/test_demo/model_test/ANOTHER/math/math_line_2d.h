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
 * \addtogroup LIMatLine2d Line2d
 * @{
 */

#ifndef __MATH_LINE_2D_H__
#define __MATH_LINE_2D_H__

#include "math-generic.h"
#include "math-vector-2d.h"

enum
{
	LIMAT_LINE2D_MISS,
	LIMAT_LINE2D_INTERSECT,
	LIMAT_LINE2D_PARALLEL,
	LIMAT_LINE2D_COINCIDE
};

/**
 * \brief A 2D line defined by a point and a normal.
 */
typedef struct _LIMatLine2d LIMatLine2d;
struct _LIMatLine2d
{
	float x;
	float y;
	float w;
};

/**
 * \brief Initializes the line variables from a line equation.
 * \param self Line.
 * \param a X term of the line normal.
 * \param b Y term of the line normal.
 * \param c Signed distance from the line to the origin.
 */
static inline void limat_line2d_init (
	LIMatLine2d* self,
	float        a,
	float        b,
	float        c)
{
	self->x = a;
	self->y = b;
	self->w = c;
}

/**
 * \brief Initializes the line variables from a point and normal.
 * \param self Line.
 * \param point Point on the plane.
 * \param normal Normal of the plane.
 */
static inline void limat_line2d_init_from_point (
	LIMatLine2d*         self,
	const LIMatVector2d* point,
	const LIMatVector2d* normal)
{
	self->x = normal->x;
	self->y = normal->y;
	self->w = point->x * normal->x + point->y * normal->y;
}

/**
 * \brief Initializes the plane variables from a triangle.
 * \param self Line.
 * \param vertex0 Vertex.
 * \param vertex1 Vertex.
 * \param vertex2 Vertex.
 */
static inline void limat_line2d_init_from_points (
	LIMatLine2d*         self,
	const LIMatVector2d* vertex0,
	const LIMatVector2d* vertex1)
{
	LIMatVector2d normal;

	normal = limat_vector2d_get_normal (limat_vector2d_subtract (*vertex1, *vertex0));
	self->x = normal.x;
	self->y = normal.y;
	self->w = limat_vector2d_dot (normal, *vertex0);
}

/**
 * \brief Gets the normal vector of the line.
 * \param self Line.
 * \param vector Return location for the normal.
 */
static inline void limat_line2d_get_normal (
	const LIMatLine2d* self,
	LIMatVector2d*     vector)
{
	vector->x = self->x;
	vector->y = self->y;
}

/**
 * \brief Gets the support vector of the line.
 * \param self Line.
 * \param vector Return location for the normal.
 */
static inline void limat_line2d_get_point (
	const LIMatLine2d* self,
	LIMatVector2d*     vector)
{
	vector->x = self->x * self->w;
	vector->y = self->y * self->w;
}

/**
 * \brief Gets the distance to the point.
 * \param self Line.
 * \param point Point.
 * \return Distance to the point.
 */
static inline float limat_line2d_distance_to_point (
	const LIMatLine2d*   self,
	const LIMatVector2d* point)
{
	return LIMAT_ABS (self->x * point->x + self->y * point->y - self->w);
}

/**
 * \brief Gets the signed distance to the point.
 * \param self Line.
 * \param point Point.
 * \return Signed distance to the point.
 */
static inline float limat_line2d_signed_distance_to_point (
	const LIMatLine2d*   self,
	const LIMatVector2d* point)
{
	return self->x * point->x + self->y * point->y - self->w;
}

/**
 * \brief Gets the intersection point of two lines.
 *
 * Lines coinciding with the plane are not considered to intersect.
 *
 * \param self Line.
 * \param point0 First point of the line.
 * \param point1 Second point of the line.
 * \param point Return location for the intersection point.
 * \return Intersection type.
 */
static inline int limat_line2d_intersects_line (
	const LIMatLine2d*   self,
	const LIMatVector2d* point0,
	const LIMatVector2d* point1,
	LIMatVector2d*       point)
{
	float t;
	LIMatVector2d tmp;
	LIMatVector2d line;
	LIMatVector2d offs;

	/* Line vector and plane offset. */
	limat_line2d_get_point (self, &tmp);
	line = limat_vector2d_subtract (*point1, *point0);
	offs = limat_vector2d_subtract (tmp, *point0);

	/* Check if parallel. */
	t = self->x * line.x + self->y * line.y;
	if (LIMAT_ABS (t) < LIMAT_EPSILON)
	{
		if (limat_line2d_distance_to_point (self, point0) < LIMAT_EPSILON)
			return LIMAT_LINE2D_COINCIDE;
		else
			return LIMAT_LINE2D_PARALLEL;
	}

	/* Find the intersection. */
	t = (self->x * offs.x + self->y * offs.y) / t;
	*point = limat_vector2d_add (*point0, limat_vector2d_multiply (line, t));
	return LIMAT_LINE2D_INTERSECT;
}

/**
 * \brief Gets the intersection point of a plane and a line segment.
 *
 * Line segments coinciding with the plane are not considered to intersect.
 *
 * \param self Line.
 * \param point0 First point of the line segment.
 * \param point1 First point of the line segment.
 * \param point Return location for the intersection point.
 * \return Intersection type.
 */
static inline int limat_line2d_intersects_segment (
	const LIMatLine2d*   self,
	const LIMatVector2d* point0,
	const LIMatVector2d* point1,
	LIMatVector2d*       point)
{
	float t;
	LIMatVector2d tmp;
	LIMatVector2d line;
	LIMatVector2d offs;

	/* Line vector and plane offset. */
	limat_line2d_get_point (self, &tmp);
	line = limat_vector2d_subtract (*point1, *point0);
	offs = limat_vector2d_subtract (tmp, *point0);

	/* Check if parallel. */
	t = self->x * line.x + self->y * line.y;
	if (LIMAT_ABS (t) <= LIMAT_EPSILON)
	{
		if (limat_line2d_distance_to_point (self, point0) < LIMAT_EPSILON)
			return LIMAT_LINE2D_COINCIDE;
		else
			return LIMAT_LINE2D_PARALLEL;
	}

	/* Check if intersects. */
	t = (self->x * offs.x + self->y * offs.y) / t;
	if (t < 0.0 || t > 1.0)
		return LIMAT_LINE2D_MISS;

	/* Find the intersection. */
	*point = limat_vector2d_add (*point0, limat_vector2d_multiply (line, t));
	return LIMAT_LINE2D_INTERSECT;
}

#endif

/** @} */
/** @} */
