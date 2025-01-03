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
 * \addtogroup LIRenRender Render
 * @{
 */

#include"bloody_termitez/model_test/ANOTHER/allsystem.h"
#include"render.h"
#include"render_overlay.h"

#include"font/font.h"
#include"internal/internal_render.h"
#include"internal/r_model.h"


LIRenRender* liren_render_new(LIPthPaths*     paths,
							  LIRenVideomode* mode)
{
	LIRenRender* self;

	/* Allocate self. */
	self = new LIRenRender();
	if (self == NULL)
		return NULL;
	self->paths = paths;
	lialg_random_init (&self->random, lisys_time (NULL));

	/* Initialize the font dictionary. */
	self->fonts = lialg_strdic_new ();
	if (self->fonts == NULL)
	{
		liren_render_free (self);
		return NULL;
	}

	/* Allocate the light dictionary. */
	self->lights = lialg_u32dic_new ();
	if (self->lights == NULL)
	{
		liren_render_free(self);
		return NULL;
	}

	/* Allocate the model dictionary. */
	self->models = lialg_u32dic_new();
	if (self->models == NULL)
	{
		liren_render_free(self);
		return NULL;
	}

	/* Allocate the object dictionary. */
	self->objects = lialg_u32dic_new ();
	if (self->objects == NULL)
	{
		liren_render_free (self);
		return NULL;
	}

	/* Initialize the backend. */
	if (!self->init (mode))
	{
		liren_render_free (self);
		return NULL;
	}

	/* Initialize the videomode. */
	if (!self->set_videomode (mode))
	{
		liren_render_free (self);
		return NULL;
	}

	return self;
}

void liren_render_free(LIRenRender* self)
{
	LIAlgStrdicIter iter1;
	LIAlgU32dicIter iter2;

	/* Free lights. */
	if (self->lights != NULL)
		lialg_u32dic_free (self->lights);

	/* Free objects. */
	if (self->objects != NULL)
		lialg_u32dic_free (self->objects);

	/* Free models. */
	if (self->models != NULL)
	{
		LIALG_U32DIC_FOREACH (iter2, self->models)
			delete (LIRenModel*) iter2.value;
		lialg_u32dic_free (self->models);
	}

	/* Free fonts. */
	if (self->fonts != NULL)
	{
		LIALG_STRDIC_FOREACH (iter1, self->fonts)
			lifnt_font_free ((LIFntFont*) iter1.value);
		lialg_strdic_free (self->fonts);
	}

	/* Free the backend. */
	self->deinit ();

	delete self;
}

/**
 * \brief Enables a compositor script.
 * \param self Renderer.
 * \param name Compositor script name.
 */
void liren_render_add_compositor(LIRenRender* self,
								 const char*  name)
{
	self->add_compositor (name);
}

/**
 * \brief Disables a compositor script.
 * \param self Renderer.
 * \param name Compositor script name.
 */
void liren_render_remove_compositor(LIRenRender* self,
									const char*  name)
{
	self->remove_compositor (name);
}

int liren_render_load_font(LIRenRender* self,
						   const char*  name,
						   const char*  file,
						   int          size)
{
	return self->load_font (name, file, size);
}

void liren_render_load_resources(LIRenRender* self)
{
	self->load_resources ();
}

int liren_render_measure_text(LIRenRender* self,
							  const char*  font,
							  const char*  text,
							  int          width_limit,
							  int*         result_width,
							  int*         result_height)
{
	return self->measure_text (font, text, width_limit, result_width, result_height);
}

/**
 * \brief Projects a point in the world space to the screen space.
 * \param self Renderer.
 * \param world Point in the world space.
 * \param screen Return location for the point in the screen space.
 */
void liren_render_project(LIRenRender*       self,
						  const vec3* 		 world,
						  vec3*       		 screen)
{
	self->project(world, screen);
}

/**
 * \brief Renders the overlays.
 * \param self Renderer.
 */
void liren_render_render(LIRenRender* self)
{
	try
	{
		self->render();
	}
	catch (Ogre::Exception e)
	{
		printf("ERROR: %s\n", e.getFullDescription().c_str());
	}
}

int liren_render_screenshot(LIRenRender* self,
							const char*  path)
{
	return self->screenshot (path);
}

/**
 * \brief Updates the renderer state.
 * \param self Renderer.
 * \param secs Number of seconds since the last update.
 * \return Nonzero when running, zero if the window was closed.
 */
int liren_render_update(LIRenRender* self,
						float        secs)
{
	return self->update (secs);
}

/**
 * \brief Updates the animations of the scene.
 * \param self Renderer.
 * \param secs Number of seconds since the last update.
 */
void liren_render_update_animations(LIRenRender* self,
								    float        secs)
{
	return self->update_animations (secs);
}

int liren_render_layout_text(LIRenRender* self,
							 const char*  font,
							 const char*  text,
							 int          width_limit,
							 int**        result_glyphs,
							 int*         result_glyphs_num)
{
	return self->layout_text(font, text, width_limit, result_glyphs, result_glyphs_num);
}

int liren_render_get_anisotropy(const LIRenRender* self)
{
	return self->get_anisotropy ();
}

void liren_render_set_anisotropy(LIRenRender* self,
								 int          value)
{
	self->set_anisotropy(value);
}

/**
 * \brief Sets the far plane distance of the camera.
 * \param self Renderer.
 * \param value Distance.
 */
void liren_render_set_camera_far(LIRenRender* self,
								 float        value)
{
	self->set_camera_far (value);
}

/**
 * \brief Sets the near plane distance of the camera.
 * \param self Renderer.
 * \param value Distance.
 */
void liren_render_set_camera_near(LIRenRender* self,
								  float        value)
{
	self->set_camera_near (value);
}

/**
 * \brief Sets the position and orientation of the camera.
 * \param self Renderer.
 * \param value Transformation.
 */
void liren_render_set_camera_transform(LIRenRender*          self,
									   const LIMatTransform* value)
{
	self->set_camera_transform (value);
}

/**
 * \brief Sets the name of the preferred Ogre material scheme.
 * \param self Renderer.
 * \param value Material scheme name.
 */
void liren_render_set_material_scheme(LIRenRender* self,
									  const char*  value)
{
	self->set_material_scheme(value);
}

float liren_render_get_opengl_version(LIRenRender* self)
{
	/* TODO */
	return 3.2f;
}

void liren_render_set_scene_ambient(LIRenRender* self,
									const float* value)
{
	self->set_scene_ambient (value);
}

/**
 * \brief Sets the skybox material.
 * \param self Renderer.
 * \param value Material name.
 */
void liren_render_set_skybox(LIRenRender* self,
							 const char*  value)
{
	self->set_skybox (value);
}

void liren_render_get_stats(LIRenRender* self,
							LIRenStats*  result)
{
	self->get_stats(result);
}

void liren_render_update_texture(LIRenRender* self,
								 const char*  name,
								 int 		  width,
								 int 		  height,
								 const void*  pixels)
{
	self->update_texture(name, 
						 width, 
						 height, 
						 pixels);
}

void liren_render_set_title(LIRenRender* self,
							const char*  value)
{
	self->set_title (value);
}

void liren_render_get_videomode(LIRenRender*    self,
								LIRenVideomode* mode)
{
	*mode = self->mode;
}

int liren_render_set_videomode(LIRenRender*    self,
							   LIRenVideomode* mode)
{
	return self->set_videomode(mode);
}

int liren_render_get_videomodes(LIRenRender*     self,
								LIRenVideomode** modes,
								int*             modes_num)
{
	return self->get_videomodes(modes, modes_num);
} 

/** @} */
/** @} */
