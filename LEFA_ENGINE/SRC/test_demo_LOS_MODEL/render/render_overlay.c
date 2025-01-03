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

/**
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenOverlay Overlay
 * @{
 */

#include"render_overlay.h"
#include"internal/internal_render.h"
#include"overlay/r_overlay.h"
#include"overlay/r_overlay_manager.h"

/**
 * \brief Creates a new overlay.
 * \param render Renderer.
 * \return Overlay ID or zero on failure.
 */

/*
int liren_render_overlay_new (
	LIRenRender* render)
{
	LIRenOverlay* self;

	self = liren_overlay_new (render);
	if (self == NULL)
		return 0;

	return liren_overlay_get_id (self);
}

/**
 * \brief Frees the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 *
void liren_render_overlay_free (
	LIRenRender* self,
	int          id)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_free (overlay);
}

/**
 * \brief Clears the elements of the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 *
void liren_render_overlay_clear (
	LIRenRender* self,
	int          id)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_clear (overlay);
}

/**
 * \brief Adds text to the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param shader Shader name.
 * \param font Font name.
 * \param text Text of the overlay.
 * \param color Diffuse color.
 * \param scissor Screen space scissor rectangle.
 * \param pos Text offset relative to the overlay origin.
 * \param size Clip rectangle size.
 * \param align Text alignment.
 *
void liren_render_overlay_add_text (
	LIRenRender* self,
	int          id,
	const char*  shader,
	const char*  font,
	const char*  text,
	const float* color,
	const int*   scissor,
	const int*   pos,
	const int*   size,
	const float* align)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_add_text (overlay, shader, font, text, color, scissor, pos, size, align);
}

void liren_render_overlay_add_scaled (
	LIRenRender* self,
	int          id,
	const char*  material_name,
	const int*   dest_position,
	const int*   dest_size,
	const int*   source_position,
	const int*   source_tiling,
	const float* color)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_add_scaled (overlay, material_name, dest_position, dest_size, source_position, source_tiling, color);
}

void liren_render_overlay_add_tiled (
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
	const float* color)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_add_tiled (overlay, material_name, dest_clip, dest_position, dest_size, source_position, source_tiling, rotation_angle, rotation_center, color);
}

/**
 * \brief Adds a child overlay to the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param overlay Overlay ID.
 * \param layer Zero for bottom, one for top.
 *
void liren_render_overlay_add_overlay (
	LIRenRender* self,
	int          id,
	int          overlay,
	int          layer)
{
	LIRenOverlay* overlay1;
	LIRenOverlay* overlay2;

	overlay1 = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	overlay2 = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, overlay);
	if (overlay1 != NULL && overlay2 != NULL)
		liren_overlay_add_overlay (overlay1, overlay2, layer);
}

/**
 * \brief Removes a child overlay from the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param overlay Overlay ID.
 *
void liren_render_overlay_remove_overlay (
	LIRenRender* self,
	int          id,
	int          overlay)
{
	LIRenOverlay* overlay1;
	LIRenOverlay* overlay2;

	overlay1 = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	overlay2 = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, overlay);
	if (overlay1 != NULL && overlay2 != NULL)
		liren_overlay_remove_overlay (overlay1, overlay2);
}

/**
 * \brief Sets the multiplicative alpha of the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param value Alpha in the range [0,1].
 *
void liren_render_overlay_set_alpha (
	LIRenRender* self,
	int          id,
	float        value)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_set_alpha (overlay, value);
}

/**
 * \brief Sets the Z offset of the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param value Positive integer.
 *
void liren_render_overlay_set_depth (
	LIRenRender* self,
	int          id,
	int          value)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_set_depth (overlay, value);
}

/**
 * \brief Sets the floating status of the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param value Nonzero for floating.
 *
void liren_render_overlay_set_floating (
	LIRenRender* self,
	int          id,
	int          value)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_set_floating (overlay, value);
}

/**
 * \brief Sets the screen position of the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param value Position vector.
 *
void liren_render_overlay_set_position (
	LIRenRender*       self,
	int                id,
	const LIMatVector* value)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_set_position (overlay, value);
}

/**
 * \brief Sets the visibility of the overlay.
 * \param self Renderer.
 * \param id Overlay ID.
 * \param value Nonzero for visible.
 *
void liren_render_overlay_set_visible (
	LIRenRender* self,
	int          id,
	int          value)
{
	LIRenOverlay* overlay;

	overlay = (LIRenOverlay*) lialg_u32dic_find (self->overlay_mgr->overlays, id);
	if (overlay != NULL)
		liren_overlay_set_visible (overlay, value);
}

/** @} */
/** @} */
