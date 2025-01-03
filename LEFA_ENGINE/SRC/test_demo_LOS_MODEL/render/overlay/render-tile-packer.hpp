/* Lips of Suna
 * Copyright© 2007-2012 Lips of Suna development team.
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
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenTilePacker TilePacker
 * @{
 */

#ifndef __RENDER_INTERNAL_TILE_PACKER_HPP__
#define __RENDER_INTERNAL_TILE_PACKER_HPP__

#include "lipsofsuna/system.h"

#define VERTEX_SIZE 24
#define VERTEX_COUNT_INIT 0
#define VERTEX_COUNT_MAX 10240

class LIRenTilePacker
{
public:
	LIRenTilePacker (float zv);
	void set_color (const float* color);
	void set_clipping (float* rect);
	void set_rotation (float angle, float x, float y, float aspect);
	void add_quad (float x0, float y0, float u0, float v0, float x1, float y1, float u1, float v1);
protected:
	void add_vertex (float x, float y, float u, float v);
	void clip_coord (float& coord, float& tex, float tex_scale, float min, float max);
public:
	int pos;
	float z;
	float color[4];
	float* clip;
	float verts[VERTEX_COUNT_MAX * 6];
	float rotation_angle;
	float rotation_aspect;
	float rotation_center[2];
};

#endif

/** @} */
/** @} */
/** @} */
