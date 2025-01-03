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
 * \addtogroup LIRenTextOverlay TextOverlay
 * @{
 */

#include "render-text-overlay.hpp"

Ogre::String LIRenTextOverlay::type_name ("LIRenTextOverlay");

/*****************************************************************************/

LIRenTextOverlay::LIRenTextOverlay (const Ogre::String& name) : Ogre::TextAreaOverlayElement (name)
{
}

LIRenTextOverlay::~LIRenTextOverlay ()
{
}

void LIRenTextOverlay::set_alpha (float alpha)
{
	LIRenBaseOverlay::set_alpha (alpha);
	setColour (Ogre::ColourValue (color[0], color[1], color[2], color[3] * alpha));
/*	if (mInitialized)
	{
		updatePositionGeometry ();
		updateTextureGeometry ();
	}*/
}

void LIRenTextOverlay::set_color (const float* color)
{
	memcpy (this->color, color, 4 * sizeof (float));
	setColour (Ogre::ColourValue (color[0], color[1], color[2], color[3] * alpha));
}

/** @} */
/** @} */
/** @} */
