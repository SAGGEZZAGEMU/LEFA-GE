/* Lips of Suna
 * Copyright© 2007-2011 Lips of Suna development team.
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

#ifndef __RENDER_OVERLAY_H__
#define __RENDER_OVERLAY_H__

/*#include "lipsofsuna/math.h"
/*#include "lipsofsuna/system.h"
*/


#include"bloody_termitez/model_test/ANOTHER/math_only.h"
#include"bloody_termitez/model_test/ANOTHER/allsystem.h"
#include"render.h"
#include"render_types.h"

LIAPICALL(int, liren_render_overlay_new, (LIRenRender* render));

LIAPICALL(void, liren_render_overlay_free, (LIRenRender* self,
		  	 								int          id));

LIAPICALL(void, liren_render_overlay_clear, (LIRenRender* self,
				 							 int          id));

LIAPICALL(void, liren_render_overlay_add_text, (LIRenRender* self,
												int          id,
												const char*  shader,
	const char*  font,
	const char*  text,
	const float* color,
	const int*   scissor,
	const int*   pos,
	const int*   size,
	const float* align));

LIAPICALL (void, liren_render_overlay_add_scaled, (
	LIRenRender* self,
	int          id,
	const char*  material_name,
	const int*   dest_position,
	const int*   dest_size,
	const int*   source_position,
	const int*   source_tiling,
	const float* color));

LIAPICALL (void, liren_render_overlay_add_tiled, (
	LIRenRender* self,
	int          id,
	const char*  material_name,
	const int*   dest_clip,
	const int*   dest_position,
	const int*   dest_size,
	const int*   source_position,
	const int*   source_tiling,
	float        rotation_angle,
	const float* rotation_center,
	const float* color));

LIAPICALL (void, liren_render_overlay_add_overlay, (
	LIRenRender* self,
	int          id,
	int          overlay,
	int          layer));

LIAPICALL (void, liren_render_overlay_remove_overlay, (
	LIRenRender* self,
	int          id,
	int          overlay));

LIAPICALL (void, liren_render_overlay_set_alpha, (
	LIRenRender* self,
	int          id,
	float        value));

LIAPICALL (void, liren_render_overlay_set_depth, (
	LIRenRender* self,
	int          id,
	int          value));

LIAPICALL (void, liren_render_overlay_set_floating, (
	LIRenRender* self,
	int          id,
	int          value));

LIAPICALL (void, liren_render_overlay_set_position, (
	LIRenRender*       self,
	int                id,
	const vec3* value));

LIAPICALL (void, liren_render_overlay_set_visible, (
	LIRenRender* self,
	int          id,
	int          value));

#endif
