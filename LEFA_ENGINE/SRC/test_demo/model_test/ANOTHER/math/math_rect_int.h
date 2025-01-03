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
 * \addtogroup LIMatRectInt Recti
 * @{
 */

#ifndef __MATH_RECT_INT_H__
#define __MATH_RECT_INT_H__

#include "math-generic.h"

/**
 * \brief A class respresenting a rectangle.
 */
typedef struct _LIMatRectInt LIMatRectInt;
struct _LIMatRectInt
{
	int x;
	int y;
	int width;
	int height;
};

/**
 * \brief Sets the rectangle from integer values.
 * \param self Rectangle.
 * \param x Integer.
 * \param y An integer.
 * \param width An integer.
 * \param height An integer.
 */
static inline void limat_rect_int_set (
	LIMatRectInt* self,
	int         x,
	int         y,
	int         width,
	int         height)
{
	self->x = x;
	self->y = y;
	self->width = width;
	self->height = height;
}

/**
 * \brief Calculates the union of the two rectangles and stores it in self.
 * \param self Rectangle.
 * \param rect0 Rectangle.
 * \param rect1 Rectangle.
 * \return Nonzero if the rectangle has an area greater than zero.
 */
static inline int limat_rect_int_union (
	LIMatRectInt*       self,
	const LIMatRectInt* rect0,
	const LIMatRectInt* rect1)
{
	int x0;
	int y0;
	int x1;
	int y1;

	x0 = LIMAT_MIN (rect0->x, rect1->x);
	y0 = LIMAT_MIN (rect0->y, rect1->y);
	x1 = LIMAT_MAX (rect0->x + rect0->width, rect1->x + rect1->width);
	y1 = LIMAT_MAX (rect0->y + rect0->height, rect1->y + rect1->height);
	self->x = x0;
	self->y = y0;
	self->width = x1 - x0;
	self->height = y1 - y0;

	return self->width > 0 && self->height > 0;
}

/**
 * \brief Calculates the intersection of the two rectangles and stores it in self.
 * \param self Rectangle.
 * \param rect0 Rectangle.
 * \param rect1 Rectangle.
 * \return Nonzero if the rectangle has an area greater than zero.
 */
static inline int limat_rect_int_intersection (
	LIMatRectInt*       self,
	const LIMatRectInt* rect0,
	const LIMatRectInt* rect1)
{
	int x0;
	int y0;
	int x1;
	int y1;

	x0 = LIMAT_MAX (rect0->x, rect1->x);
	y0 = LIMAT_MAX (rect0->y, rect1->y);
	x1 = LIMAT_MIN (rect0->x + rect0->width, rect1->x + rect1->width);
	y1 = LIMAT_MIN (rect0->y + rect0->height, rect1->y + rect1->height);
	self->x = x0;
	self->y = y0;
	self->width = x1 - x0;
	self->height = y1 - y0;

	return self->width > 0 && self->height > 0;
}

#endif

/** @} */
/** @} */
