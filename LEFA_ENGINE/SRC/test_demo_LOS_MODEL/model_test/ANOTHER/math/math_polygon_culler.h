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

#ifndef __MATH_POLYGON_CULLER_H__
#define __MATH_POLYGON_CULLER_H__

#include "lipsofsuna/system.h"
#include "math-polygon-2d.h"

typedef struct _LIMatPolygonCuller LIMatPolygonCuller;
struct _LIMatPolygonCuller
{
	LIMatPolygon2d* remainder;
	struct
	{
		int count;
		LIMatPolygon2d** array;
	} pieces;
};

LIAPICALL (LIMatPolygonCuller*, limat_polygon_culler_new, (
	const LIMatVtxops2d* ops,
	const void*          vertices,
	int                  count));

LIAPICALL (void, limat_polygon_culler_free, (
	LIMatPolygonCuller* self));

LIAPICALL (int, limat_polygon_culler_subtract_quad, (
	LIMatPolygonCuller*  self,
	const LIMatVector2d* bot0,
	const LIMatVector2d* bot1,
	const LIMatVector2d* top0,
	const LIMatVector2d* top1));

#endif
