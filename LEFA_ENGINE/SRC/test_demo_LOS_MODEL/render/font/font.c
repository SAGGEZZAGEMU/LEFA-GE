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
 * \addtogroup LIFnt Font
 * @{
 * \addtogroup LIFntFont Font
 * @{
 */

#include "lipsofsuna/system.h"
#include"../internal/render.h"
#include"font.h"

/*
#include <OgrePass.h>
#include <OgreStringConverter.h>
#include <OgreTechnique.h>
#include <OgreTextureUnitState.h>
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
#include <Overlay/OgreFont.h>
#include <Overlay/OgreFontManager.h>
#else
#include <OgreFont.h>
#include <OgreFontManager.h>
#endif
*/

struct _LIFntFont
{
	int size;
	int height;
	char* name;
	Ogre::FontPtr* font;
};

/*****************************************************************************/

LIFntFont* lifnt_font_new (
	const char* name,
	const char* file,
	int         size)
{
	LIFntFont* self;

	/* Allocate self. */
	self = (LIFntFont*) lisys_calloc (1, sizeof (LIFntFont));
	if (self == NULL)
		return NULL;
	self->size = size;
	self->name = lisys_string_dup (name);
	if (self->name == NULL)
	{
		lisys_free (self);
		return NULL;
	}

	try
	{
		/* Load the font. */
		Ogre::FontManager& manager = Ogre::FontManager::getSingleton ();
		Ogre::FontPtr resource = manager.create (name, LIREN_RESOURCES_TEMPORARY);
		resource->setParameter ("type", "truetype");
		resource->setParameter ("source", Ogre::String (file) + ".ttf");
		resource->setParameter ("size", Ogre::StringConverter::toString (size * 72 / 96));
		resource->setParameter ("resolution", "96");
		resource->load ();
		self->font = new Ogre::FontPtr (resource);

		/* Calculate the height in pixels. */
		Ogre::Font::GlyphInfo glyph = resource->getGlyphInfo (0x0030);
		Ogre::TextureUnitState* unit = resource->getMaterial ()->getTechnique (0)->getPass (0)->getTextureUnitState (0);
		self->height = unit->getTextureDimensions ().second * (glyph.uvRect.bottom - glyph.uvRect.top);
	}
	catch (...)
	{
		lisys_error_set (EINVAL, "Failed to load font `%s'", name);
		lifnt_font_free (self);
		return NULL;
	}

	return self;
}

void lifnt_font_free (
	LIFntFont* self)
{
	if (self->font != NULL)
	{
		delete self->font;
		Ogre::FontManager& manager = Ogre::FontManager::getSingleton ();
		manager.remove (self->name);
	}
	lisys_free (self->name);
	lisys_free (self);
}

/**
 * \brief Reloads the font.
 *
 * This function is called when the video mode changes in Windows. It
 * recreates the glyph texture.
 *
 * \param self Font.
 * \param pass Reload pass.
 */
void lifnt_font_reload (
	LIFntFont* self,
	int        pass)
{
}

/**
 * \brief Gets the horizontal advance of the glyph.
 * \param self A font.
 * \param glyph A wide character.
 * \return The advance in pixels.
 */
int lifnt_font_get_advance (
	LIFntFont* self,
	wchar_t    glyph)
{
	if (glyph == 0x0020)
		return self->size * (*self->font)->getGlyphAspectRatio (0x0030) + 1;
	else
		return self->size * (*self->font)->getGlyphAspectRatio (glyph) + 1;
}

int lifnt_font_get_ascent (
	LIFntFont* self)
{
	/* FIXME */
	return self->size * (*self->font)->getGlyphAspectRatio (0x0030);
}

int lifnt_font_get_height (
	const LIFntFont* self)
{
	return self->height;
}

/** @} */
/** @} */
/** @} */
