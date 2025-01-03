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
 * \addtogroup LIRenLight Light
 * @{
 */

#include"bloody_termitez/model_test/ANOTHER/allsystem.h"
#include"render_light.h"
#include"internal/internal_render.h"
#include"internal/r_light.h"

/**
 * \brief Creates a new light source.
 * \param render Renderer.
 * \param ambient Ambient color, array of 4 floats.
 * \param diffuse Diffuse color, array of 4 floats.
 * \param specular Specular color, array of 4 floats.
 * \param equation Attenuation equation, array of 3 floats.
 * \param cutoff Spot cutoff in radians.
 * \param exponent Spot expoent.
 * \param shadows Nonzero if the lamp casts shadows.
 * \return Light ID or zero on failure.
 */
int liren_render_light_new(LIRenRender* render,
	const float* ambient,
	const float* diffuse,
	const float* specular,
	const float* equation,
	float        cutoff,
	float        exponent,
	int          shadows)
{
	LIRenLight* self;

	self = liren_light_new (render, ambient, diffuse, specular, equation, cutoff, exponent, shadows);
	if (self == NULL)
		return 0;

	return liren_light_get_id (self);
}

/**
 * \brief Frees a light source.
 * \param self Renderer.
 * \param id Light ID.
 */
void liren_render_light_free(LIRenRender* self,
							 int          id)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_free (light);
}

/**
 * \brief Sets the ambient color of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Array of 4 floats.
 */
void liren_render_light_set_ambient(LIRenRender* self,
									int          id,
									const float* value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_ambient (light, value);
}

/**
 * \brief Sets the diffuse color of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Array of 4 floats.
 */
void liren_render_light_set_diffuse(LIRenRender* self,
									int          id,
									const float* value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_diffuse (light, value);
}

/**
 * \brief Sets the light directionality.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Nonzero to set directional.
 */
void liren_render_light_set_directional(LIRenRender* self,
										int          id,
										int          value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_directional (light, value);
}

/**
 * \brief Enables or disables the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Nonzero to enable.
 */
void liren_render_light_set_enabled(LIRenRender* self,
									int          id,
									int          value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_enabled (light, value);
}

/**
 * \brief Sets the attenuation equation of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Array of 3 floats.
 */
void liren_render_light_set_equation(LIRenRender* self,
									 int          id,
									 const float* value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_equation (light, value);
}

/**
 * \brief Sets the priority of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Priority value, higher means more important.
 */
void liren_render_light_set_priority(LIRenRender* self,
									 int          id,
									 float        value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_priority (light, value);
}

/**
 * \brief Sets the shadow casting mode of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Nonzero to allow shadow casting, zero to disable.
 */
void liren_render_light_set_shadow(LIRenRender* self,
								   int          id,
								   int          value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_shadow (light, value);
}

/**
 * \brief Sets the shadow map far distance of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Distance in world units.
 */
void liren_render_light_set_shadow_far(LIRenRender* self,
									   int          id,
									   float        value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_shadow_far (light, value);
}

/**
 * \brief Sets the shadow map near distance of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Distance in world units.
 */
void liren_render_light_set_shadow_near(LIRenRender* self,
										int          id,
										float        value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_shadow_near (light, value);
}

/**
 * \brief Sets the specular color of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Array of 4 floats.
 */
void liren_render_light_set_specular(LIRenRender* self,
									 int          id,
									 const float* value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_specular (light, value);
}

/**
 * \brief Sets the spot cutoff angle of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Angle in radians.
 */
void liren_render_light_set_spot_cutoff(LIRenRender* self,
										int          id,
										float        value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_spot_cutoff (light, value);
}

/**
 * \brief Sets the spot exponent of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Spot exponent.
 */
void liren_render_light_set_spot_exponent(LIRenRender* self,
										  int          id,
										  float        value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_spot_exponent (light, value);
}

/**
 * \brief Gets the transformation of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Return value for the transformation.
 */
void liren_render_light_get_transform(LIRenRender*    self,
									  int             id,
									  LIMatTransform* value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_get_transform (light, value);
}

/**
 * \brief Sets the transformation of the light.
 * \param self Renderer.
 * \param id Light ID.
 * \param value Transformation.
 */
void liren_render_light_set_transform(LIRenRender*          self,
									  int                   id,
									  const LIMatTransform* value)
{
	LIRenLight* light;

	light = (LIRenLight*) lialg_u32dic_find (self->lights, id);
	if (light != NULL)
		liren_light_set_transform (light, value);
}

/** @} */
/** @} */
