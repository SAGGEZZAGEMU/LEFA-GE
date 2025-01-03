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
 */

#ifndef __MATH_VERTEX_2D_H__
#define __MATH_VERTEX_2D_H__

#include <stddef.h>
#include "math-vector-2d.h"

/**
 * \addtogroup LIMatVtxops Vtxops
 * @{
 */

typedef struct _LIMatVtxops2d LIMatVtxops2d;
struct _LIMatVtxops2d
{
	size_t size;
	void (*interpolate)(const void* self, const void* other, float frac, void* result);
	void (*getcoord)(const void* self, LIMatVector2d* result);
	void (*setcoord)(void* self, const LIMatVector2d* value);
};

/** @} */

/*****************************************************************************/

/**
 * \addtogroup LIMatVertexV2 VertexV2
 * @{
 */

typedef struct _LIMatVertexV2 LIMatVertexV2;
struct _LIMatVertexV2
{
	LIMatVector2d coord;
};

static inline void __limat_vtxops_v2_getcoord (
	const void*    self,
	LIMatVector2d* result)
{
	const LIMatVertexV2* data = (LIMatVertexV2*) self;

	*result = data->coord;
}

static inline void __limat_vtxops_v2_setcoord (
	void*                self,
	const LIMatVector2d* value)
{
	LIMatVertexV2* data = (LIMatVertexV2*) self;

	data->coord = *value;
}

static inline void __limat_vtxops_v2_interpolate (
	const void* self,
	const void* other,
	float       blend,
	void*       result)
{
}

static const LIMatVtxops2d limat_vtxops_v2 =
{
	sizeof (LIMatVertexV2),
	__limat_vtxops_v2_interpolate,
	__limat_vtxops_v2_getcoord,
	__limat_vtxops_v2_setcoord
};

/** @} */

/** @} */

#endif

/** @} */
