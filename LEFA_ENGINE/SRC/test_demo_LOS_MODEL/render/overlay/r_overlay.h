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

#ifndef __RENDER_OVERLAY_OVERLAY_HPP__
#define __RENDER_OVERLAY_OVERLAY_HPP__

//#include "lipsofsuna/math.h"
//#include "lipsofsuna/render/internal/render.hpp"
#include"bloody_termitez/model_test/ANOTHER/math_only.h"
#include"bloody_termitez/render_model/internal/internal_render.h"
#include"render_container.h"
//#include"r_overlay_manager.h"

#include"render_container_factory.h"
#include"render_image_overlay_factory.h"
#include"render_scaled_overlay_factory.h"
#include"render_text_overlay_factory.h"
#include"r_o_m.h"



/*
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
#include <Overlay/OgreOverlaySystem.h>
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayElementFactory.h>
#include <Overlay/OgrePanelOverlayElement.h>
#else
#include <OgreOverlayManager.h>
#include <OgreOverlayElementFactory.h>
#include <OgrePanelOverlayElement.h>
#endif
*/
//class LIRenContainer;
//class LIRenOverlayManager;
//class LIRenRender;

/*
class LIRenOverlay
{
public:
	int id;
	int depth;
	int visible;
	float alpha;
	LIMatVector position;
	LIRenRender* render;
	LIRenOverlay* parent;
	LIRenOverlayManager* manager;
	LIRenContainer* container;
	Ogre::Overlay* overlay;
	struct
	{
		int count;
		LIRenOverlay** array;
	} overlays;
};
*/


typedef struct _LIRenOverlay LIRenOverlay;
struct _LIRenOverlay 
{
	int id;
	int depth;
	int visible;
	float alpha;
	vec3 position;
	LIRenRender* render;
	LIRenOverlay* parent;
	LIRenOverlayManager* manager;
	LIRenContainer* container;
	//Ogre::Overlay* overlay;
	struct
	{
		int count;
		LIRenOverlay** array;
	} overlays;
};

LIAPICALL (LIRenOverlay*, liren_overlay_new, (LIRenRender* render));

LIAPICALL (void, liren_overlay_free, (LIRenOverlay* self));

LIAPICALL (void, liren_overlay_clear, (
	LIRenOverlay* self));

LIAPICALL (void, liren_overlay_add_text, (
	LIRenOverlay* self,
	const char*   shader,
	const char*   font,
	const char*   text,
	const float*  color,
	const int*    scissor,
	const int*    pos,
	const int*    size,
	const float*  align));

LIAPICALL (void, liren_overlay_add_scaled, (
	LIRenOverlay* self,
	const char*   material_name,
	const int*    dest_position,
	const int*    dest_size,
	const int*    source_position,
	const int*    source_tiling,
	const float*  color));

LIAPICALL (void, liren_overlay_add_tiled, (
	LIRenOverlay* self,
	const char*   material_name,
	const int*    dest_clip,
	const int*    dest_position,
	const int*    dest_size,
	const int*    source_position,
	const int*    source_tiling,
	float         rotation_angle,
	const float*  rotation_center,
	const float*  color));

LIAPICALL (void, liren_overlay_add_overlay, (
	LIRenOverlay* self,
	LIRenOverlay* overlay,
	int           layer));

LIAPICALL (void, liren_overlay_remove_overlay, (
	LIRenOverlay* self,
	LIRenOverlay* overlay));

LIAPICALL (void, liren_overlay_set_alpha, (
	LIRenOverlay* self,
	float         value));

LIAPICALL (void, liren_overlay_set_depth, (
	LIRenOverlay* self,
	int           value));

LIAPICALL (void, liren_overlay_set_floating, (
	LIRenOverlay* self,
	int           value));

LIAPICALL (int, liren_overlay_get_id, (
	LIRenOverlay* self));

LIAPICALL (void, liren_overlay_set_position, (
	LIRenOverlay*      self,
	const vec3* value));

LIAPICALL (void, liren_overlay_set_visible, (
	LIRenOverlay* self,
	int           value));

#endif
