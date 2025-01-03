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

#ifndef __MATH_POLYGON_2D_H__
#define __MATH_POLYGON_2D_H__

#include "lipsofsuna/system.h"
#include "math-line-2d.h"
#include "math-vertex-2d.h"

typedef struct _LIMatPolygon2d LIMatPolygon2d;
struct _LIMatPolygon2d
{
	const LIMatVtxops2d* ops;
	void* data;
	struct
	{
		int capacity;
		int count;
		void* vertices;
	} vertices;
};

LIAPICALL (LIMatPolygon2d*, limat_polygon2d_new, (
	const LIMatVtxops2d* ops,
	const void*          vertices,
	int                  count));

LIAPICALL (LIMatPolygon2d*, limat_polygon2d_new_from_triangle, (
	const LIMatVtxops2d* ops,
	const void*          v0,
	const void*          v1,
	const void*          v2));

LIAPICALL (void, limat_polygon2d_free, (
	LIMatPolygon2d* self));

LIAPICALL (int, limat_polygon2d_add_vertices, (
	LIMatPolygon2d* self,
	const void*     vertices,
	int             count));

LIAPICALL (int, limat_polygon2d_clip, (
	const LIMatPolygon2d* self,
	const LIMatLine2d*    line,
	LIMatPolygon2d*       front));

LIAPICALL (int, limat_polygon2d_clip_inplace, (
	LIMatPolygon2d*    self,
	const LIMatLine2d* line));

LIAPICALL (int, limat_polygon2d_compare, (
	const LIMatPolygon2d* self,
	const LIMatPolygon2d* polygon));

LIAPICALL (void, limat_polygon2d_print, (
	const LIMatPolygon2d* self));

LIAPICALL (int, limat_polygon2d_split, (
	const LIMatPolygon2d* self,
	const LIMatLine2d*    line,
	LIMatPolygon2d*       front,
	LIMatPolygon2d*       back));

LIAPICALL (int, limat_polygon2d_split_inplace, (
	LIMatPolygon2d*    self,
	const LIMatLine2d* line,
	LIMatPolygon2d*    back));

LIAPICALL (void, limat_polygon2d_remove_duplicates, (
	LIMatPolygon2d* self,
	float           threshold));

LIAPICALL (int, limat_polygon2d_get_degenerate, (
	const LIMatPolygon2d* self));

LIAPICALL (void, limat_polygon2d_get_coord, (
	const LIMatPolygon2d* self,
	int                   index,
	LIMatVector2d*        coord));

LIAPICALL (void*, limat_polygon2d_get_vertex, (
	const LIMatPolygon2d* self,
	int                   index));

#endif
