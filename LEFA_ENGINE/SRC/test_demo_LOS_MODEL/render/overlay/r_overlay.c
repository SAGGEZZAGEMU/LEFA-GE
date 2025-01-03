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
 * \addtogroup LIRenOverlay_ Overlay
 * @{
 */

#include "render-container.hpp"
#include "render-image-overlay.hpp"
#include "render-overlay.hpp"
#include "render-overlay-manager.hpp"
#include "render-text-overlay.hpp"
#include "../font/font.h"
#include "../font/font-layout.h"
#include "../internal/render.hpp"

static bool private_create_material (
	LIRenOverlay*      self,
	const char*        material_name,
	Ogre::MaterialPtr* material_result,
	Ogre::TexturePtr*  texture_result);

static void private_remove_overlay (
	LIRenOverlay* self,
	LIRenOverlay* child);

static void private_update_position (
	LIRenOverlay* self);

/*****************************************************************************/

/**
 * \brief Creates a new overlay.
 * \param render Renderer.
 * \return Overlay or NULL on failure.
 */
LIRenOverlay* liren_overlay_new (
	LIRenRender* render)
{
	LIRenOverlay* self;

	/* Allocate self. */
	self = new LIRenOverlay ();
	if (self == NULL)
		return 0;
	self->render = render;
	self->manager = render->overlay_mgr;
	self->alpha = 1.0f;

	/* Choose a unique ID. */
	while (!self->id)
	{
		self->id = lialg_random_range (&render->random, 0x00000000, 0x0FFFFFFF);
		if (lialg_u32dic_find (render->overlay_mgr->overlays, self->id))
			self->id = 0;
	}

	/* Add to the dictionary. */
	if (!lialg_u32dic_insert (self->manager->overlays, self->id, self))
	{
		liren_overlay_free (self);
		return 0;
	}

	/* Create the overlay element. */
	self->container = (LIRenContainer*) self->manager->overlay_manager->createOverlayElement ("LIRenContainer", self->render->id.next ());
	self->container->setMetricsMode (Ogre::GMM_PIXELS);

	return self;
}

/**
 * \brief Frees the overlay.
 * \param self Overlay.
 */
void liren_overlay_free (
	LIRenOverlay* self)
{
	/* Detach all our children. */
	while (self->overlays.count)
		private_remove_overlay (self, self->overlays.array[0]);
	lisys_free (self->overlays.array);

	/* Remove ourselves from our parent. */
	if (self->parent != NULL)
		private_remove_overlay (self->parent, self);
	lialg_u32dic_remove (self->render->overlay_mgr->overlays, self->id);

	/* Free the overlay element. */
	liren_overlay_clear (self);
	if (self->overlay != NULL)
	{
		self->overlay->remove2D (self->container);
		self->render->overlay_mgr->overlay_manager->destroy (self->overlay->getName ());
	}
	if (self->container != NULL)
		self->render->overlay_mgr->overlay_manager->destroyOverlayElement (self->container);

	delete self;
}

/**
 * \brief Clears the elements of the overlay.
 * \param self Overlay.
 */
void liren_overlay_clear (
	LIRenOverlay* self)
{
	self->container->remove_all_elements ();
}

/**
 * \brief Adds text to the overlay.
 * \param self Overlay.
 * \param shader Shader name.
 * \param font Font name.
 * \param text Text of the overlay.
 * \param color Diffuse color.
 * \param scissor Screen space scissor rectangle.
 * \param pos Text offset relative to the overlay origin.
 * \param size Clip rectangle size.
 * \param align Text alignment.
 */
void liren_overlay_add_text (
	LIRenOverlay* self,
	const char*   shader,
	const char*   font,
	const char*   text,
	const float*  color,
	const int*    scissor,
	const int*    pos,
	const int*    size,
	const float*  align)
{
	int i;
	int j;
	int h;
	int w;
	int x;
	int y;
	LIFntFont* font_;
	LIFntLayout* layout;

	/* Get the font. */
	font_ = (LIFntFont*) lialg_strdic_find (self->render->fonts, font);
	if (font_ == NULL)
		return;
	h = lifnt_font_get_height (font_);

	/* Layout the text. */
	layout = lifnt_layout_new ();
	if (layout == NULL)
		return;
	lifnt_layout_set_width_limit (layout, size[0]);
	lifnt_layout_append_string (layout, font_, text);
	lifnt_layout_update (layout);

	/* Vertical alignment. */
	y = pos[1];
	y += (size[1] - lifnt_layout_get_height (layout)) * align[1];

	/* Allocate the temporary character buffer. */
	/* The extra fields are used to silence valgrind errors caused by SIMD.
	   The actual length required would be "layout->n_glyphs + 1". */
	wchar_t* chars = (wchar_t*) lisys_calloc (layout->n_glyphs + 9, sizeof (wchar_t));

	/* Handle each line */
	for (i = 0 ; i < layout->n_glyphs ; i += j)
	{
		/* Get the characters of the line. */
		for (w = 0, j = 0 ; i + j < layout->n_glyphs ; j++)
		{
			if (j > 0 && layout->glyphs[i + j].wrapped)
				break;
			chars[j] = layout->glyphs[i + j].glyph;
			w = layout->glyphs[i + j].x + layout->glyphs[i + j].advance;
		}
		chars[j] = L'\0';

		/* Horizontal alignment. */
		x = pos[0];
		x += (size[0] - w) * align[0];

		/* Create the element. */
		Ogre::String id = self->render->id.next ();
		LIRenTextOverlay* elem = (LIRenTextOverlay*) self->manager->overlay_manager->createOverlayElement ("LIRenTextOverlay", id);
		elem->setMetricsMode (Ogre::GMM_PIXELS);
		elem->setPosition (x, y);
		elem->setDimensions (size[0], size[1]);
		elem->setCharHeight (h);
		elem->setFontName (font);
		elem->setCaption (chars);
		elem->set_color (color);
		elem->set_alpha (self->alpha);
		elem->show ();

		/* Add to the container. */
		self->container->add_element (elem);

		/* Start the next line. */
		y += h;
	}

	lifnt_layout_free (layout);
	lisys_free (chars);
}

/**
 * \brief Adds a tiled image to the overlay.
 * \param self Overlay.
 * \param material_name Material name.
 * \param dest_clip Destination clip rectangle in pixels, or NULL for no clipping.
 * \param dest_position Position in pixels relative to the parent.
 * \param dest_size Size in pixels.
 * \param source_position Position in pixels in the source image.
 * \param source_tiling Tiling pattern in pixels.
 * \param rotation_angle Rotation angle in radians.
 * \param rotation_center Rotation center position in pixels.
 * \param color Diffuse color.
 */
void liren_overlay_add_tiled (
	LIRenOverlay* self,
	const char*   material_name,
	const int*    dest_clip,
	const int*    dest_position,
	const int*    dest_size,
	const int*    source_position,
	const int*    source_tiling,
	float         rotation_angle,
	const float*  rotation_center,
	const float*  color)
{
	/* Get the texture size. */
	Ogre::MaterialPtr material;
	Ogre::TexturePtr texture;
	if (!private_create_material (self, material_name, &material, &texture))
		return;
	int source_size[2] = { static_cast<int>(texture->getWidth ()), static_cast<int>(texture->getHeight ()) };

	/* Create a new image overlay. */
	Ogre::String id = self->render->id.next ();
	LIRenImageOverlay* elem = (LIRenImageOverlay*) self->manager->overlay_manager->createOverlayElement ("LIRenImageOverlay", id); 
	elem->setMetricsMode (Ogre::GMM_PIXELS);
	elem->setPosition (dest_position[0], dest_position[1]);
	elem->setDimensions (dest_size[0], dest_size[1]);
	elem->setMaterialName (material->getName ());
	elem->set_clipping (dest_clip);
	elem->set_tiling (source_position, source_size, source_tiling);
	elem->set_rotation (rotation_angle, rotation_center[0], rotation_center[1]);
	elem->set_color (color);
	elem->set_alpha (self->alpha);
	elem->show ();

	/* Add to the container. */
	self->container->add_element (elem);
}

/**
 * \brief Adds a tiled image to the overlay.
 * \param self Overlay.
 * \param material_name Material name.
 * \param dest_position Position in pixels relative to the parent.
 * \param dest_size Size in pixels.
 * \param source_position Position in pixels in the source image.
 * \param source_tiling Tiling pattern in pixels.
 * \param color Diffuse color.
 */
void liren_overlay_add_scaled (
	LIRenOverlay* self,
	const char*   material_name,
	const int*    dest_position,
	const int*    dest_size,
	const int*    source_position,
	const int*    source_tiling,
	const float*  color)
{
	/* Get the texture size. */
	Ogre::MaterialPtr material;
	Ogre::TexturePtr texture;
	if (!private_create_material (self, material_name, &material, &texture))
		return;
	int source_size[2] = { static_cast<int>(texture->getWidth ()), static_cast<int>(texture->getHeight ()) };
	int source_tiling1[6] = {0, source_tiling[1], 0, 0, source_tiling[4], 0};
	if (source_tiling1[1] < 0)
		source_tiling1[1] = source_size[0];
	if (source_tiling1[4] < 0)
		source_tiling1[4] = source_size[1];

	/* Create a new scaled overlay. */
	Ogre::String id = self->render->id.next ();
	LIRenImageOverlay* elem = (LIRenImageOverlay*) self->manager->overlay_manager->createOverlayElement ("LIRenScaledOverlay", id); 
	elem->setMetricsMode (Ogre::GMM_PIXELS);
	elem->setPosition (dest_position[0], dest_position[1]);
	elem->setDimensions (dest_size[0], dest_size[1]);
	elem->setMaterialName (material->getName ());
	elem->set_clipping (NULL);
	elem->set_rotation (0.0f, 0, 0);
	elem->set_tiling (source_position, source_size, source_tiling1);
	elem->set_color (color);
	elem->set_alpha (self->alpha);
	elem->show ();

	/* Add to the container. */
	self->container->add_element (elem);
}

/**
 * \brief Adds a child overlay to the overlay.
 * \param self Overlay.
 * \param overlay Overlay ID.
 * \param layer Zero for bottom, one for top.
 */
void liren_overlay_add_overlay (
	LIRenOverlay* self,
	LIRenOverlay* overlay,
	int           layer)
{
	LIRenOverlay** tmp;

	/* Detach the overlay. */
	if (overlay->parent == self)
		return;
	if (overlay->overlay != NULL)
	{
		overlay->overlay->remove2D (overlay->container);
		overlay->manager->overlay_manager->destroy (overlay->overlay->getName ());
		overlay->overlay = NULL;
	}
	if (overlay->parent != NULL)
		private_remove_overlay (overlay->parent, overlay);

	/* Attach the overlay. */
	tmp = (LIRenOverlay**) lisys_realloc (self->overlays.array, (self->overlays.count + 1) * sizeof (LIRenOverlay*));
	if (tmp == NULL)
		return;
	tmp[self->overlays.count] = overlay;
	self->overlays.array = tmp;
	self->overlays.count++;
	overlay->parent = self;

	/* Add to container. */
	self->container->add_container (overlay->container, layer);
	private_update_position (overlay);
}

/**
 * \brief Removes a child overlay from the overlay.
 * \param self Overlay.
 * \param overlay Overlay ID.
 */
void liren_overlay_remove_overlay (
	LIRenOverlay* self,
	LIRenOverlay* overlay)
{
	/* Detach the overlay. */
	if (overlay->parent == self)
		private_remove_overlay (self, overlay);
}

/** Sets the multiplicative alpha of the overlay.
 * \param self Overlay.
 * \param value Alpha in the range [0,1].
 */
void liren_overlay_set_alpha (
	LIRenOverlay* self,
	float         value)
{
	float a = LIMAT_CLAMP (value, 0.0f, 1.0f);

	if (self->alpha == a)
		return;
	self->alpha = a;
	self->container->set_alpha (a);
}

/**
 * \brief Sets the depth sort priority of the overlay.
 * \param self Overlay.
 * \param value Depth sort priority.
 */
void liren_overlay_set_depth (
	LIRenOverlay* self,
	int           value)
{
	if (self->depth == value)
		return;
	self->depth = value;
	if (self->overlay != NULL)
		self->overlay->setZOrder (self->depth);
}

void liren_overlay_set_floating (
	LIRenOverlay* self,
	int           value)
{
	Ogre::String id (self->render->id.next ());
	if (value && self->overlay == NULL)
	{
		if (self->parent != NULL)
			private_remove_overlay (self->parent, self);
		self->overlay = self->manager->overlay_manager->create (id);
		self->overlay->add2D (self->container);
		self->overlay->setZOrder (self->depth);
		if (self->visible)
			self->overlay->show ();
		private_update_position (self);
	}
	else if (!value && self->overlay != NULL)
	{
		self->overlay->remove2D (self->container);
		self->manager->overlay_manager->destroy (self->overlay->getName ());
		self->overlay = NULL;
	}
}

/**
 * \brief Gets the ID of the overlay.
 * \param self Overlay.
 * \return ID.
 */
int liren_overlay_get_id (
	LIRenOverlay* self)
{
	return self->id;
}

/**
 * \brief Sets the screen position of the overlay.
 * \param self Overlay.
 * \param value Position vector.
 */
void liren_overlay_set_position (
	LIRenOverlay*      self,
	const LIMatVector* value)
{
	self->position = *value;
	private_update_position (self);
}

/**
 * \brief Sets the visibility of the overlay.
 * \param self Overlay.
 * \param value Nonzero for visible.
 */
void liren_overlay_set_visible (
	LIRenOverlay* self,
	int           value)
{
	self->visible = value;
	if (value)
	{
		self->container->show ();
		private_update_position (self);
	}
	else
		self->container->hide ();
	if (self->overlay != NULL)
	{
		if (value)
			self->overlay->show ();
		else
			self->overlay->hide ();
	}
}

/*****************************************************************************/

static bool private_create_material (
	LIRenOverlay*      self,
	const char*        material_name,
	Ogre::MaterialPtr* material_result,
	Ogre::TexturePtr*  texture_result)
{
	/* Check for an existing material. */
	Ogre::String matname = Ogre::String ("LOS_GUI_") + material_name;
	Ogre::MaterialPtr material = self->render->material_manager->getByName (matname);
	if (!material.isNull ())
	{
		/* Get the texture unit. */
		lisys_assert (material->getNumTechniques () == 1);
		Ogre::Technique* technique = material->getTechnique (0);
		lisys_assert (technique->getNumPasses () == 1);
		Ogre::Pass* pass = technique->getPass (0);
		lisys_assert (pass->getNumTextureUnitStates () == 1);
		Ogre::TextureUnitState* unit = pass->getTextureUnitState (0);

		/* Get the texture. */
		const Ogre::String& texname = unit->getTextureName ();
		Ogre::TexturePtr texture = self->render->texture_manager->getByName (texname);
		lisys_assert (!texture.isNull ());

		/* Return the results. */
		*material_result = material;
		*texture_result = texture;

		return true;
	}

	/* Load the texture. */
	Ogre::String group = LIREN_RESOURCES_TEMPORARY;
	Ogre::String texname = Ogre::String (material_name) + ".png";
	Ogre::TexturePtr texture;
	try
	{
		texture = self->render->texture_manager->load (texname, group);
	}
	catch (...)
	{
		texname = Ogre::String (material_name) + ".dds";
		try
		{
			texture = self->render->texture_manager->load (texname, group);
		}
		catch (...)
		{
			return false;
		}
	}

	/* Create a new material. */
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
	self->render->material_manager->load (matname, group);
	material = self->render->material_manager->getByName (matname);
#else
	material = self->render->material_manager->load (matname, group);
#endif
	lisys_assert (!material.isNull ());
	material->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);

	/* Create the texture unit. */
	lisys_assert (material->getNumTechniques () == 1);
	Ogre::Technique* technique = material->getTechnique (0);
	lisys_assert (technique->getNumPasses () == 1);
	Ogre::Pass* pass = technique->getPass (0);
	lisys_assert (pass->getNumTextureUnitStates () == 0);
	pass->createTextureUnitState (texname);

	/* Return the results. */
	*material_result = material;
	*texture_result = texture;

	return true;
}

static void private_remove_overlay (
	LIRenOverlay* self,
	LIRenOverlay* child)
{
	int i;
	int j;

	/* Floating overlays can't have a parent so such an overlay getting
	   here is an error. We check for it here just in case. */
	lisys_assert (child->parent == self);
	lisys_assert (child->overlay == NULL);

	/* Find the child from the list and detach it. */
	for (i = 0 ; i < self->overlays.count ; i++)
	{
		if (self->overlays.array[i] == child)
		{
			/* Remove from the array. */
			if (self->overlays.count > 1)
			{
				for (j = i ; j < self->overlays.count - 1 ; j++)
					self->overlays.array[j] = self->overlays.array[j + 1];
				self->overlays.count--;
			}
			else
			{
				lisys_free (self->overlays.array);
				self->overlays.array = NULL;
				self->overlays.count = 0;
			}

			/* Detach the child. */
			child->parent = NULL;
			self->container->remove_container (i);
			break;
		}
	}
}

static void private_update_position (
	LIRenOverlay* self)
{
	float x = self->position.x;
	float y = self->position.y;

	if (self->parent != NULL)
	{
		x -= self->parent->position.x;
		y -= self->parent->position.y;
	}
	self->container->setPosition (x, y);
}

/** @} */
/** @} */
/** @} */
