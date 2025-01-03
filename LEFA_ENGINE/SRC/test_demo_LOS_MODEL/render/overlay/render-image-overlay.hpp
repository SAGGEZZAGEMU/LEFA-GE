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

#ifndef __RENDER_INTERNAL_IMAGE_OVERLAY_HPP__
#define __RENDER_INTERNAL_IMAGE_OVERLAY_HPP__

#include "lipsofsuna/system.h"
#include "render-base-overlay.hpp"
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
#include <Overlay/OgreOverlayElement.h>
#else
#include <OgreOverlayElement.h>
#endif

class LIRenImageOverlay : public LIRenBaseOverlay, public Ogre::OverlayElement
{
public:
	LIRenImageOverlay (const Ogre::String& name);
	virtual ~LIRenImageOverlay ();
	virtual void set_alpha (float alpha);
	void set_color (const float* color);
	void set_clipping (const int* rect);
	void set_rotation (float angle, float center_x, float center_y);
	void set_tiling (const int* source_position, const int* source_size, const int* source_tiling);
	virtual Ogre::OverlayElement* get_ogre () { return (Ogre::OverlayElement*) this; }
	virtual void initialise ();
	virtual const Ogre::String& getTypeName () const;
	virtual void getRenderOperation (Ogre::RenderOperation& op);
	virtual void updatePositionGeometry ();
	virtual void updateTextureGeometry ();
protected:
	bool clipping;
	int dst_clip[4];
	int src_pos[2];
	int src_size[2];
	int src_tiling[6];
	float color[4];
	float rotation_angle;
	float rotation_center[2];
	Ogre::RenderOperation render_op;
	Ogre::HardwareVertexBufferSharedPtr vbuf;
	static Ogre::String type_name;
};

#endif
