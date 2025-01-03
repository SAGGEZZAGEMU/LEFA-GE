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

#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__

/*
#include "lipsofsuna/math.h"
#include "lipsofsuna/model.h"
#include "lipsofsuna/system.h"
*/

#include"bloody_termitez/model_test/ANOTHER/allsystem.h"
#include"bloody_termitez/model_test/ANOTHER/math_only.h"
#include"bloody_termitez\model_test/all_model.h"

#include"render.h"
#include"internal/internal_render.h"
#include"internal/render2.h"

LIAPICALL(int, liren_render_object_new, (LIRenRender* render));

LIAPICALL (void, liren_render_object_free, (LIRenRender* self,
											int          id));

LIAPICALL (void, liren_render_object_add_model, (LIRenRender* self,
												 int          id,
												 int          model));

LIAPICALL (void, liren_render_object_add_texture_alias, (LIRenRender* render,
														  int          id,
	const char*  name,
	int          width,
	int          height,
	const void*  pixels));

LIAPICALL (void, liren_render_object_channel_animate, (
	LIRenRender*            self,
	int                     id,
	int                     channel,
	int                     keep,
	const LIMdlPoseChannel* info));

LIAPICALL (void, liren_render_object_channel_fade, (
	LIRenRender* self,
	int          id,
	int          channel,
	float        time));

LIAPICALL (LIMdlPoseChannel*, liren_render_object_channel_get_state, (
	LIRenRender* self,
	int          id,
	int          channel));

LIAPICALL (void, liren_render_object_clear_animations, (
	LIRenRender* self,
	int          id));

LIAPICALL (int, liren_render_object_find_node, (
	LIRenRender*    self,
	int             id,
	const char*     name,
	int             world,
	LIMatTransform* result));

LIAPICALL (void, liren_render_object_particle_animation, (
	LIRenRender* self,
	int          id,
	float        start,
	int          loop));

LIAPICALL (void, liren_render_object_remove_model, (
	LIRenRender* self,
	int          id,
	int          model));

LIAPICALL (void, liren_render_object_replace_model, (
	LIRenRender* self,
	int          id,
	int          model_old,
	int          model_new));

LIAPICALL (void, liren_render_object_replace_texture, (
	LIRenRender* render,
	int          id,
	const char*  name,
	int          width,
	int          height,
	const void*  pixels));

LIAPICALL (void, liren_render_object_set_custom_param, (
	LIRenRender* self,
	int          id,
	int          index,
	float        r,
	float        g,
	float        b,
	float        a));

LIAPICALL (void, liren_render_object_set_effect, (
	LIRenRender* self,
	int          id,
	const char*  shader,
	const float* params));

LIAPICALL (int, liren_render_object_get_loaded, (
	LIRenRender* render,
	int          id));

LIAPICALL (void, liren_render_object_set_model, (
	LIRenRender* self,
	int          id,
	int          model));

LIAPICALL (void, liren_render_object_set_particle, (
	LIRenRender* self,
	int          id,
	const char*  name));

LIAPICALL (void, liren_render_object_set_particle_emitting, (
	LIRenRender* self,
	int          id,
	int          value));

LIAPICALL (void, liren_render_object_set_realized, (
	LIRenRender* self,
	int          id,
	int          value));

LIAPICALL (void, liren_render_object_set_render_distance, (
	LIRenRender* self,
	int          id,
	float        value));

LIAPICALL (void, liren_render_object_set_render_queue, (
	LIRenRender* self,
	int          id,
	const char*  value));

LIAPICALL (void, liren_render_object_set_shadow, (
	LIRenRender* self,
	int          id,
	int          value));

LIAPICALL (void, liren_render_object_set_transform, (
	LIRenRender*          self,
	int                   id,
	const LIMatTransform* value));

#endif
