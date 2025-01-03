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
 * \addtogroup LIRenOverlay Overlay
 * @{
 * \addtogroup LIRenTilePacker TilePacker
 * @{
 */

#include "lipsofsuna/math.h"
#include "render-tile-packer.hpp"

LIRenTilePacker::LIRenTilePacker (float zv)
{
	pos = 0;
	z = zv;
	clip = NULL;
	rotation_angle = 0.0f;
	rotation_center[0] = 0.0f;
	rotation_center[1] = 0.0f;
}

void LIRenTilePacker::set_color (const float* color)
{
	memcpy (this->color, color, 4 * sizeof (float));
}

void LIRenTilePacker::set_clipping (float* rect)
{
	clip = rect;
}

void LIRenTilePacker::set_rotation (float angle, float x, float y, float aspect)
{
	rotation_angle = angle;
	rotation_aspect = aspect;
	rotation_center[0] = x;
	rotation_center[1] = y;
}

void LIRenTilePacker::add_quad (float x0, float y0, float u0, float v0, float x1, float y1, float u1, float v1)
{
	lisys_assert (x0 <= x1);
	lisys_assert (y1 <= y0);
	if (clip != NULL)
	{
		float uscale = (u1 - u0) / (x1 - x0);
		float vscale = (v1 - v0) / (y1 - y0);
		if (x1 < clip[0] || x0 >= clip[0] + clip[2] ||
			y0 < clip[1] || y1 >= clip[1] + clip[3])
			return;
		clip_coord (x0, u0, uscale, clip[0], clip[0] + clip[2]);
		clip_coord (y0, v0, vscale, clip[1], clip[1] + clip[3]);
		clip_coord (x1, u1, uscale, clip[0], clip[0] + clip[2]);
		clip_coord (y1, v1, vscale, clip[1], clip[1] + clip[3]);
	}
	add_vertex (x0, y0, u0, v0);
	add_vertex (x1, y1, u1, v1);
	add_vertex (x1, y0, u1, v0);
	add_vertex (x0, y0, u0, v0);
	add_vertex (x0, y1, u0, v1);
	add_vertex (x1, y1, u1, v1);
} 

void LIRenTilePacker::add_vertex (float x, float y, float u, float v)
{
	if (pos >= 6 * VERTEX_COUNT_MAX)
		return;
	if (rotation_angle != 0.0f)
	{
		float x1 = x - rotation_center[0];
		float y1 = (y - rotation_center[1]) * rotation_aspect;
		float x2 = x1 * cosf (rotation_angle) - y1 * sinf (rotation_angle);
		float y2 = x1 * sinf (rotation_angle) + y1 * cosf (rotation_angle);
		x = x2 + rotation_center[0];
		y = y2 / rotation_aspect + rotation_center[1];
	}
	verts[pos++] = x;
	verts[pos++] = y;
	verts[pos++] = z;
	verts[pos++] = u;
	verts[pos++] = v;
	uint8_t* c = (uint8_t*)(verts + pos++);
	c[0] = (uint8_t) 255 * color[0];
	c[1] = (uint8_t) 255 * color[1];
	c[2] = (uint8_t) 255 * color[2];
	c[3] = (uint8_t) 255 * color[3];
}

void LIRenTilePacker::clip_coord (float& coord, float& tex, float tex_scale, float min, float max)
{
	float clip0 = min - coord;
	float clip1 = coord - max;
	if (clip0 > 0)
	{
		tex += tex_scale * clip0;
		coord += clip0;
	}
	if (clip1 > 0)
	{
		tex -= tex_scale * clip1;
		coord -= clip1;
	}
}

/** @} */
/** @} */
/** @} */
