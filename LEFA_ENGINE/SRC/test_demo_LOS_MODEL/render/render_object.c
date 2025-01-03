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

/**
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenObject Object
 * @{
 */


/*#include"lipsofsuna/system.h"
*/
#include"../model_test/ANOTHER/allsystem.h"
#include"render_object.h"
#include"internal/internal_render.h"
#include"internal/r_object.h"

/**
 * \brief Creates a new render object and adds it to the scene.
 * \param render Renderer.
 * \return Object ID, or zero on failure.
 */

/*
int liren_render_object_new (
	LIRenRender* render)
{
	LIRenObject* self;

	self = new LIRenObject (render, 0);
	if (self == NULL)
		return 0;

	return self->get_id ();
}

/**
 * \brief Frees the render object.
 * \param self Renderer.
 * \param id Object ID.
 *
void liren_render_object_free (
	LIRenRender* self,
	int          id)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		delete object;
}

/**
 * \brief Adds a model to the object.
 * \param self Object.
 * \param self Renderer.
 * \param id Object ID.
 * \param model Model ID.
 *
void liren_render_object_add_model (
	LIRenRender* self,
	int          id,
	int          model)
{
	LIRenObject* object;
	LIRenModel* model_;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
	{
		model_ = (LIRenModel*) lialg_u32dic_find (self->models, model);
		if (model_ != NULL)
			object->add_model (model_);
	}
}

/**
 * \brief Adds a permanent texture alias.
 * \param self Renderer.
 * \param id Object ID.
 * \brief Adds a permanent texture alias.
 * \param name Name of the replaced texture.
 * \param width Width of the new texture.
 * \param height Height of the new texture.
 * \param pixels Pixels in the RGBA format.
 *
void liren_render_object_add_texture_alias (
	LIRenRender* self,
	int          id,
	const char*  name,
	int          width,
	int          height,
	const void*  pixels)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->add_texture_alias (name, width, height, pixels);
}

/**
 * \brief Adds an animation to an animation channel.
 * \param self Renderer.
 * \param id Object ID.
 * \param channel Channel number.
 * \param keep Non-zero to try to avoid replacing existing animations.
 * \param info Pose channel.
 *
void liren_render_object_channel_animate (
	LIRenRender*            self,
	int                     id,
	int                     channel,
	int                     keep,
	const LIMdlPoseChannel* info)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object == NULL)
		return;

	object->channel_animate (channel, keep, info);
}

void liren_render_object_channel_fade (
	LIRenRender* self,
	int          id,
	int          channel,
	float        time)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object == NULL)
		return;

	object->channel_fade (channel, time);
}

LIMdlPoseChannel* liren_render_object_channel_get_state (
	LIRenRender* self,
	int          id,
	int          channel)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object == NULL)
		return NULL;

	return object->channel_get_state (channel);
}

void liren_render_object_clear_animations (
	LIRenRender* self,
	int          id)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object == NULL)
		return;

	object->clear_animations ();
}

int liren_render_object_find_node (
	LIRenRender*    self,
	int             id,
	const char*     name,
	int             world,
	LIMatTransform* result)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object == NULL)
		return 0;

	return object->find_node (name, world, result);
}

/**
 * \brief Sets the particle animation state of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param start Animation offset in seconds.
 * \param loop Nonzero to enable looping.
 *
void liren_render_object_particle_animation (
	LIRenRender* self,
	int          id,
	float        start,
	int          loop)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->particle_animation (start, loop);
}

/**
 * \brief Replaces a model with another.
 *
 * This function performs a delayed removal for the old model. The new
 * model is loaded in the background and shown after loading. At the same
 * time of showing the new model, the old model is removed.
 *
 * \param self Object.
 * \param self Renderer.
 * \param id Object ID.
 * \param model_old Model ID to remove.
 * \param model_new Model ID to add.
 *
void liren_render_object_replace_model (
	LIRenRender* self,
	int          id,
	int          model_old,
	int          model_new)
{
	LIRenObject* object;
	LIRenModel* model_old_;
	LIRenModel* model_new_;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
	{
		model_old_ = (LIRenModel*) lialg_u32dic_find (self->models, model_old);
		model_new_ = (LIRenModel*) lialg_u32dic_find (self->models, model_new);
		if (model_new_ != NULL)
			object->replace_model (model_old_, model_new_);
	}
}

/**
 * \brief Replaces a texture.
 * \param self Renderer.
 * \param id Object ID.
 * \param name Name of the replaced texture.
 * \param width Width of the new texture.
 * \param height Height of the new texture.
 * \param pixels Pixels in the RGBA format.
 *
void liren_render_object_replace_texture (
	LIRenRender* self,
	int          id,
	const char*  name,
	int          width,
	int          height,
	const void*  pixels)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->replace_texture (name, width, height, pixels);
}

/**
 * \brief Removes a model from the object.
 * \param self Object.
 * \param self Renderer.
 * \param id Object ID.
 * \param model Model ID.
 *
void liren_render_object_remove_model (
	LIRenRender* self,
	int          id,
	int          model)
{
	LIRenObject* object;
	LIRenModel* model_;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
	{
		model_ = (LIRenModel*) lialg_u32dic_find (self->models, model);
		if (model_ != NULL)
			object->remove_model (model_);
	}
}

/**
 * \brief Sets a custom shader parameter.
 * \param self Renderer.
 * \param id Object ID.
 * \param index Parameter index.
 * \param r Parameter value.
 * \param g Parameter value.
 * \param b Parameter value.
 * \param a Parameter value.
 *
void liren_render_object_set_custom_param (
	LIRenRender* self,
	int          id,
	int          index,
	float        r,
	float        g,
	float        b,
	float        a)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_custom_param (index, r, g, b, a);
}

/**
 * \brief Sets the effect layer of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param shader Shader name or NULL to disable the effect.
 * \param params Effect parameters or NULL.
 *
void liren_render_object_set_effect (
	LIRenRender* self,
	int          id,
	const char*  shader,
	const float* params)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_effect (shader, params);
}

/**
 * \brief Returns nonzero if the object has been fully background loaded.
 * \param self Renderer.
 * \param id Object ID.
 * \return Nonzero if background loaded, zero if not.
 *
int liren_render_object_get_loaded (
	LIRenRender* self,
	int          id)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		return object->get_loaded ();
	else
		return 1;
}

/**
 * \brief Sets the model of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param model Model.
 *
void liren_render_object_set_model (
	LIRenRender* self,
	int          id,
	int          model)
{
	LIRenObject* object;
	LIRenModel* model_;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
	{
		model_ = (LIRenModel*) lialg_u32dic_find (self->models, model);
		object->set_model (model_);
	}
}

/**
 * \brief Sets the particle system of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param name Particle system name.
 *
void liren_render_object_set_particle (
	LIRenRender* self,
	int          id,
	const char*  name)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_particle (name);
}

/**
 * \brief Enables or disables particle emission.
 * \param self Renderer.
 * \param id Object ID.
 * \param value Nonzero to enable, zero to disable.
 *
void liren_render_object_set_particle_emitting (
	LIRenRender* self,
	int          id,
	int          value)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_particle_emitting (value);
}

/**
 * \brief Sets the realized flag of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param value Flag value.
 *
void liren_render_object_set_realized (
	LIRenRender* self,
	int          id,
	int          value)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_visible (value);
}

/**
 * \brief Sets the render distance of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param value Render distance.
 *
void liren_render_object_set_render_distance (
	LIRenRender* self,
	int          id,
	float        value)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_render_distance (value);
}

/**
 * \brief Sets the render queue of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param value Queue name.
 *
void liren_render_object_set_render_queue (
	LIRenRender* self,
	int          id,
	const char*  value)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_render_queue (value);
}

/**
 * \brief Sets the shadow casting mode of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param value Nonzero to allow shadow casting, zero to disable.
 *
void liren_render_object_set_shadow (
	LIRenRender* self,
	int          id,
	int          value)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_shadow_casting (value);
}

/**
 * \brief Sets the transformation of the object.
 * \param self Renderer.
 * \param id Object ID.
 * \param value Transformation.
 *
void liren_render_object_set_transform (
	LIRenRender*          self,
	int                   id,
	const LIMatTransform* value)
{
	LIRenObject* object;

	object = (LIRenObject*) lialg_u32dic_find (self->objects, id);
	if (object != NULL)
		object->set_transform (value);
}

/** @} */
/** @} */
