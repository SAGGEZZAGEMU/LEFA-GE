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

#ifndef __MATH_POLYGON_H__
#define __MATH_POLYGON_H__

#include "lipsofsuna/system.h"
#include "math-aabb.h"
#include "math-plane.h"
#include "math-types.h"
#include "math-vertex.h"

typedef struct _LIMatPolygon LIMatPolygon;
struct _LIMatPolygon
{
	const LIMatVtxops* ops;
	void* data;
	LIMatVector normal;
	struct
	{
		int capacity;
		int count;
		void* vertices;
	} vertices;
};

LIAPICALL (LIMatPolygon*, limat_polygon_new, (
	const LIMatVtxops* ops,
	const LIMatVector* normal,
	const void*        vertices,
	int                count));

LIAPICALL (LIMatPolygon*, limat_polygon_new_from_triangle, (
	const LIMatVtxops* ops,
	const void*        v0,
	const void*        v1,
	const void*        v2));

LIAPICALL (void, limat_polygon_free, (
	LIMatPolygon* self));

LIAPICALL (int, limat_polygon_add_vertices, (
	LIMatPolygon* self,
	const void*   vertices,
	int           count));

LIAPICALL (int, limat_polygon_clip, (
	const LIMatPolygon* self,
	const LIMatPlane*   plane,
	LIMatPolygon*       front));

LIAPICALL (int, limat_polygon_clip_with_planes, (
	const LIMatPolygon* self,
	const LIMatPlane*   planes,
	int                 count,
	LIMatPolygon*       front));

LIAPICALL (int, limat_polygon_clip_with_aabb, (
	const LIMatPolygon* self,
	const LIMatAabb*    bounds,
	LIMatPolygon*       front));

LIAPICALL (int, limat_polygon_split, (
	const LIMatPolygon* self,
	const LIMatPlane*   plane,
	LIMatPolygon*       front,
	LIMatPolygon*       back));

LIAPICALL (int, limat_polygon_get_degenerate, (
	const LIMatPolygon* self));

LIAPICALL (void, limat_polygon_get_coord, (
	const LIMatPolygon* self,
	int                 index,
	LIMatVector*        coord));

LIAPICALL (void*, limat_polygon_get_vertex, (
	const LIMatPolygon* self,
	int                 index));

#endif
