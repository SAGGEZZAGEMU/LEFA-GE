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

#ifndef __RENDER_INTERNAL_IMAGE_TEXT_HPP__
#define __RENDER_INTERNAL_IMAGE_TEXT_HPP__

#include "lipsofsuna/system.h"
#include "render-base-overlay.hpp"
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
#include <Overlay/OgreTextAreaOverlayElement.h>
#else
#include <OgreTextAreaOverlayElement.h>
#endif

class LIRenTextOverlay : public LIRenBaseOverlay, public Ogre::TextAreaOverlayElement
{
public:
	LIRenTextOverlay (const Ogre::String& name);
	virtual ~LIRenTextOverlay ();
	virtual void set_alpha (float alpha);
	virtual Ogre::OverlayElement* get_ogre () { return (Ogre::OverlayElement*) this; }
	void set_color (const float* color);
protected:
	float color[4];
	static Ogre::String type_name;
};

#endif
