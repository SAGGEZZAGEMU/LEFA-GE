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
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenObject Object
 * @{
 */

#include "lipsofsuna/model.h"
#include "lipsofsuna/system.h"
#include "render.hpp"
#include "render-attachment-entity.hpp"
#include "render-attachment-particle.hpp"
#include "render-model.hpp"
#include "render-object.hpp"

/**
 * \brief Creates a new render object and adds it to the scene.
 * \param render Renderer.
 * \param id Unique identifier.
 * \return Object.
 */
LIRenObject::LIRenObject (
	LIRenRender* render,
	int          id)
{
	/* Allocate self. */
	this->id = id;
	visible = 0;
	render_queue = Ogre::RENDER_QUEUE_MAIN;
	shadow_casting = 0;
	skeleton_rebuild_needed = 0;
	render_distance = -1.0f;
	transform = limat_transform_identity ();
	pose = NULL;
	pose_skeleton = NULL;
	this->render = render;
	node = NULL;

	/* Choose a unique ID. */
	while (!this->id)
	{
		this->id = lialg_random_range (&render->random, 0x00000000, 0x7FFFFFFF);
		if (lialg_u32dic_find (render->objects, this->id))
			this->id = 0;
	}

	/* Create the scene node. */
	node = render->scene_root->createChildSceneNode ();
	node->setVisible (false);

	/* Create the mutex. */
	mutex_pose_attachment = lisys_mutex_new ();

	/* Add self to the object dictionary. */
	render->add_object (this);
}

/**
 * \brief Frees the render object.
 */
LIRenObject::~LIRenObject ()
{
	/* Remove from the object dictionary. */
	render->remove_object (this);

	/* Free models and particles. */
	clear_models ();

	/* Free the scene node. */
	if (node != NULL)
		render->scene_root->removeAndDestroyChild (node->getName ());

	/* Free the pose. */
	if (pose_skeleton != NULL)
		limdl_pose_skeleton_free (pose_skeleton);
	if (pose != NULL)
		limdl_pose_free (pose);

	/* Free the mutex. */
	if (mutex_pose_attachment != NULL)
		lisys_mutex_free (mutex_pose_attachment);
}

/**
 * \brief Adds a model to the object.
 * \param model Model.
 */
void LIRenObject::add_model (
	LIRenModel*  model)
{
	LISysScopedLock lock (mutex_pose_attachment);

	/* Create the new entity. */
	LIRenAttachmentEntity* attachment = OGRE_NEW LIRenAttachmentEntity (this, model);
	attachments.push_back (attachment);
	skeleton_rebuild_needed = 1;

	/* Apply the texture aliases. */
	apply_texture_aliases (attachment);
}

/**
 * \brief Adds a permanent texture alias.
 * \param name Name of the replaced texture.
 * \param width Width of the new texture.
 * \param height Height of the new texture.
 * \param pixels Pixels in the RGBA format.
 */
void LIRenObject::add_texture_alias (
	const char* name,
	int         width,
	int         height,
	const void* pixels)
{
	LISysScopedLock lock (mutex_pose_attachment);

	Ogre::TexturePtr texture = create_texture (width, height, pixels);
	replace_texture (name, texture);
	texture_aliases[name] = texture;
}

/**
 * \brief Adds an animation to an animation channel.
 * \param channel Channel number.
 * \param keep Non-zero to try to avoid replacing existing animations.
 * \param info Pose channel.
 * \return One on success. Zero otherwise.
 */
int LIRenObject::channel_animate (
	int                     channel,
	int                     keep,
	const LIMdlPoseChannel* info)
{
	LISysScopedLock lock (mutex_pose_attachment);
	LIMdlPose* pose1;

	/* Update the reference pose. */
	pose1 = channel_animate (pose, channel, keep, info);
	if (pose1 != NULL)
	{
		pose = pose1;
		if (pose_skeleton == NULL)
		{
			pose_skeleton = limdl_pose_skeleton_new (NULL, 0);
			rebuild_skeleton ();
		}
	}

	return 1;
}

void LIRenObject::channel_fade (
	int   channel,
	float time)
{
	LISysScopedLock lock (mutex_pose_attachment);

	channel_fade (pose, channel, time);
}

LIMdlPoseChannel* LIRenObject::channel_get_state (
	int channel) const
{
	LISysScopedLock lock (mutex_pose_attachment);
	LIMdlPoseChannel* chan;

	if (pose == NULL)
		return NULL;
	chan = (LIMdlPoseChannel*) lialg_u32dic_find (pose->channels, channel);
	if (chan == NULL)
		return NULL;
	chan = limdl_pose_channel_new_copy (chan);

	return chan;
}

/**
 * \brief Clears all animations from the object.
 */
void LIRenObject::clear_animations ()
{
	LISysScopedLock lock (mutex_pose_attachment);

	if (pose == NULL)
		return;
	limdl_pose_destroy_all (pose);
}

/**
 * \brief Clears all models from the object.
 */
void LIRenObject::clear_models ()
{
	LISysScopedLock lock (mutex_pose_attachment);

	/* Detach everything from the scene node. */
	node->detachAllObjects ();

	/* Remove entities. */
	for (size_t i = 0 ; i < attachments.size () ; i++)
		OGRE_DELETE attachments[i];
	attachments.clear ();

	/* Remove the skeleton. */
	if (pose == NULL && pose_skeleton != NULL)
	{
		limdl_pose_skeleton_free (pose_skeleton);
		pose_skeleton = NULL;
	}
}

int LIRenObject::find_node (
	const char*     name,
	int             world,
	LIMatTransform* result) const
{
	float scale;
	LIMatTransform transform;
	LIMdlNode* node = NULL;
	LISysScopedLock lock (mutex_pose_attachment);

	/* Search from the skeleton. */
	if (pose_skeleton != NULL) {
		node = limdl_pose_skeleton_find_node (pose_skeleton, name);
		if(node != NULL) {
			/* Get the transformation. */
			limdl_node_get_world_transform (node, &scale, &transform);
		}
	} else {
		/*Our skeleton isn't animated, so we need to search the unposed bones.*/
		for (size_t i = 0 ; i < attachments.size () ; i++) {
			if (attachments[i]->get_replacer () == NULL) {
				LIRenModelData* m = attachments[i]->get_model ();
				if (m != NULL) {
					node = limdl_nodes_find_node(&(m->rest_pose_skeleton->nodes), name);
					if(node != NULL) {
						break;
					}
				}
			}
		}
		if(node != NULL) {
			/* Get the transformation. */
			limdl_node_get_full_rest_transform(node, &scale, &transform);
		}
	}
	if (node == NULL) {
		return 0;
	}

	if (world)
		transform = limat_transform_multiply (this->transform, transform);
	*result = transform;

	return 1;
}

/**
 * \brief Sets the particle animation state of the object.
 * \param start Animation offset in seconds.
 * \param loop Nonzero to enable looping.
 */
void LIRenObject::particle_animation (
	float start,
	int   loop)
{
	/* TODO */
}

/**
 * \brief Marks a model for rebuild.
 *
 * This function is called for all objects when a model is changed. Hence, the
 * model does not necessarily exist in the object.
 *
 * The old mesh is not removed immediately because it takes time for the new
 * entity to finish building. To avoid the object disappearing during the
 * build period, we keep the old model around until the build has finished.
 *
 * \param model Model.
 */
void LIRenObject::model_changed (
	LIRenModel* model)
{
	LISysScopedLock lock (mutex_pose_attachment);

	for (size_t i = 0 ; i < attachments.size () ; i++)
	{
		if (attachments[i]->has_model (model))
		{
			/* Create a new attachment. */
			LIRenAttachmentEntity* attachment = OGRE_NEW LIRenAttachmentEntity (this, model);
			attachments[i]->set_replacer (attachment);
			attachments.insert (attachments.begin () + i, attachment);
			i++;

			/* Apply the texture aliases. */
			apply_texture_aliases (attachment);
		}
	}
}

/**
 * \brief Removes a model from the object.
 * \param model Model.
 */
void LIRenObject::remove_model (
	LIRenModel* model)
{
	LISysScopedLock lock (mutex_pose_attachment);

	for (size_t i = 0 ; i < attachments.size () ; i++)
	{
		if (attachments[i]->has_model (model))
		{
			remove_entity (i);
			skeleton_rebuild_needed = 1;
			break;
		}
	}
}

/**
 * \brief Replace a model with another.
 *
 * This performs a delayed replacement in such a way that the old model
 * won't be removed until the new model has been background loaded.
 *
 * \param model_old Model to be removed.
 * \param model_new Model to be added.
 */
void LIRenObject::replace_model (
	LIRenModel* model_old,
	LIRenModel* model_new)
{
	LISysScopedLock lock (mutex_pose_attachment);

	/* Try to replace. */
	for (size_t i = 0 ; i < attachments.size () ; i++)
	{
		if (attachments[i]->has_model (model_old))
		{
			/* Create a replacement attachment. */
			LIRenAttachmentEntity* attachment = OGRE_NEW LIRenAttachmentEntity (this, model_new);
			attachments[i]->set_replacer (attachment);
			attachments.insert (attachments.begin () + i, attachment);
			skeleton_rebuild_needed = 1;

			/* Apply the texture aliases. */
			apply_texture_aliases (attachment);
			return;
		}
	}

	/* Just add if there was nothing to replace. */
	add_model (model_new);
}

/**
 * \brief Replaces a texture.
 * \param name Name of the replaced texture.
 * \param width Width of the new texture.
 * \param height Height of the new texture.
 * \param pixels Pixels in the RGBA format.
 */
void LIRenObject::replace_texture (
	const char* name,
	int         width,
	int         height,
	const void* pixels)
{
	LISysScopedLock lock (mutex_pose_attachment);

	Ogre::TexturePtr texture = create_texture (width, height, pixels);
	replace_texture (name, texture);
}

/**
 * \brief Updates the object.
 * \param secs Seconds since the last update.
 */
void LIRenObject::update (
	float secs)
{
	LISysScopedLock lock (mutex_pose_attachment);

	/* Update attachments. */
	/* This needs to be done before replacing attachments since an
	   attachment may finish loading here. If replacing were done first,
	   both the replaced attachment and the replacement would be shown
	   during the same frame. */
	for (size_t i = 0 ; i < attachments.size () ; i++)
		attachments[i]->update (secs);

	/* Replace old attachments with built ones. */
	/* Removing an attachment is a potentially recursive operation so there is no
	   guarantee of the list index being in any given position after the removal.
	   Because of that, the operation needs to be restarted from scratch after
	   each removal. */
	while (true)
	{
		bool found = false;
		for (size_t i = 0 ; i < attachments.size () ; i++)
		{
			LIRenAttachment* repl = attachments[i]->get_replacer ();
			if (repl != NULL && repl->is_loaded ())
			{
				skeleton_rebuild_needed = 1;
				remove_entity (i);
				found = true;
				break;
			}
		}
		if (!found)
			break;
	}

	/* Hide objects too far away. */
	/* Frustum culling cannot necessarily eliminate all desirable objects if the
	   view distance is very low. Because of that, we allow the user to specify
	   the render distance for the object. */
	if (render_distance > 0 && visible)
	{
		bool vis = false;
		for(size_t i = 0; i < render->viewports.size(); i++) {
			float dist2 = node->getSquaredViewDepth (render->viewports[i].camera);
			if (dist2 <= render_distance * render_distance) {
				vis = true;
				break;
			}
		}
		node->setVisible (vis);
	}
	else
		node->setVisible (visible);
}

/**
 * \brief Updates the animations of the object.
 * \param secs Seconds since the last update.
 */
void LIRenObject::update_animations (
	float secs)
{
	LISysScopedLock lock (mutex_pose_attachment);

	if (pose_skeleton != NULL)
	{
		limdl_pose_update (pose, secs);
		if (skeleton_rebuild_needed)
			rebuild_skeleton ();
		limdl_pose_skeleton_update (pose_skeleton, pose);
		for (size_t i = 0 ; i < attachments.size () ; i++)
			attachments[i]->update_pose (pose_skeleton);
	}
}

/**
 * \brief Sets a custom shader parameter.
 * \param index Parameter index.
 * \param r Parameter value.
 * \param g Parameter value.
 * \param b Parameter value.
 * \param a Parameter value.
 */
void LIRenObject::set_custom_param (
	int   index,
	float r,
	float g,
	float b,
	float a)
{
	LISysScopedLock lock (mutex_pose_attachment);

	custom_params[index] = Ogre::Vector4 (r, g, b, a);
	update_entity_settings ();
}

/**
 * \brief Sets the effect layer of the object.
 * \param shader Shader name or NULL to disable the effect.
 * \param params Effect parameters or NULL.
 * \return Nonzero on success.
 */
int LIRenObject::set_effect (
	const char*  shader,
	const float* params)
{
	/* TODO */

	return 1;
}

/**
 * \brief Gets the ID of the object.
 * \return ID.
 */
int LIRenObject::get_id () const
{
	return id;
}

/**
 * \brief Returns non-zero if the object has finished background loading.
 * \return Non-zero if finished background loading.
 */
int LIRenObject::get_loaded () const
{
	LISysScopedLock lock (mutex_pose_attachment);

	for (size_t i = 0 ; i < attachments.size () ; i++)
	{
		if (!attachments[i]->is_loaded ())
			return 0;
		if (attachments[i]->get_replacer () != NULL)
			return 0;
	}
	return 1;
}

/**
 * \brief Sets the model of the object.
 * \param model Model.
 * \return Nonzero on success.
 */
int LIRenObject::set_model (
	LIRenModel* model)
{
	LISysScopedLock lock (mutex_pose_attachment);

	/* Simply clear everything if setting to NULL. */
	if (model == NULL)
	{
		clear_models ();
		return 1;
	}

	/* Add the new model. */
	LIRenAttachmentEntity* attachment = OGRE_NEW LIRenAttachmentEntity (this, model);
	skeleton_rebuild_needed = 1;

	/* Mark all the old entities for replacement. */
	for (size_t i = 0 ; i < attachments.size () ; i++)
		attachments[i]->set_replacer (attachment);
	attachments.push_back (attachment);

	/* Apply the texture aliases. */
	apply_texture_aliases (attachment);

	return 1;
}

/**
 * \brief Gets the transformation of a pose skeleton node.
 * \param name Node name.
 * \param result Return location for the transformation.
 * \return True if the node was found. False otherwise.
 */
bool LIRenObject::get_node_transform (
	const char*     name,
	LIMatTransform& result)
{
	if (!pose_skeleton)
		return false;
	LIMdlNode* node = limdl_pose_skeleton_find_node (pose_skeleton, name);
	if (!node)
		return false;
	result = node->rest_transform.global;
	return true;
}

/**
 * \brief Sets the particle effect of the object.
 * \param name Particle effect name.
 * \return Nonzero on success.
 */
int LIRenObject::set_particle (
	const char* name)
{
	LISysScopedLock lock (mutex_pose_attachment);

	/* Remove the existing model or particle system. */
	clear_models ();

	/* Add the new attachment. */
	attachments.push_back (OGRE_NEW LIRenAttachmentParticle (this, name));

	return 1;
}

/**
 * \brief Enables or disables particle emission.
 * \param value Nonzero to enable, zero to disable.
 */
void LIRenObject::set_particle_emitting (
	int value)
{
	LISysScopedLock lock (mutex_pose_attachment);

	for (size_t i = 0 ; i < attachments.size () ; i++)
		attachments[i]->set_emitting (value);
}

/**
 * \brief Sets the realized flag of the object.
 * \param value Flag value.
 * \return Nonzero if succeeded.
 */
int LIRenObject::set_visible (
	int value)
{
	visible = value;
	node->setVisible (value);
	return 1;
}

/**
 * \brief Sets the render distance of the object.
 * \param value Render distance.
 */
void LIRenObject::set_render_distance (
	float value)
{
	LISysScopedLock lock (mutex_pose_attachment);

	render_distance = value;
	update_entity_settings ();
}

/**
 * \brief Sets the render queue of the object.
 * \param value Render queue name.
 */
void LIRenObject::set_render_queue (
	const char* value)
{
	LISysScopedLock lock (mutex_pose_attachment);

	if (!strcmp (value, "background"))
		render_queue = Ogre::RENDER_QUEUE_BACKGROUND;
	else if (!strcmp (value, "skies_early"))
		render_queue = Ogre::RENDER_QUEUE_SKIES_EARLY;
	else if (!strcmp (value, "1"))
		render_queue = Ogre::RENDER_QUEUE_1;
	else if (!strcmp (value, "2"))
		render_queue = Ogre::RENDER_QUEUE_2;
	else if (!strcmp (value, "3"))
		render_queue = Ogre::RENDER_QUEUE_3;
	else if (!strcmp (value, "4"))
		render_queue = Ogre::RENDER_QUEUE_4;
	else if (!strcmp (value, "main"))
		render_queue = Ogre::RENDER_QUEUE_MAIN;
	else if (!strcmp (value, "6"))
		render_queue = Ogre::RENDER_QUEUE_6;
	else if (!strcmp (value, "7"))
		render_queue = Ogre::RENDER_QUEUE_7;
	else if (!strcmp (value, "8"))
		render_queue = Ogre::RENDER_QUEUE_8;
	else if (!strcmp (value, "9"))
		render_queue = Ogre::RENDER_QUEUE_9;
	else if (!strcmp (value, "skies_late"))
		render_queue = Ogre::RENDER_QUEUE_SKIES_LATE;
	else if (!strcmp (value, "overlay"))
		render_queue = Ogre::RENDER_QUEUE_OVERLAY;
	update_entity_settings ();
}

/**
 * \brief Sets the shadow casting mode of the object.
 * \param value Nonzero to allow shadow casting, zero to disable.
 */
void LIRenObject::set_shadow_casting (
	int value)
{
	LISysScopedLock lock (mutex_pose_attachment);

	shadow_casting = value;
	update_entity_settings ();
}

/**
 * \brief Sets the transformation of the object.
 * \param value Transformation.
 */
void LIRenObject::set_transform (
	const LIMatTransform* value)
{
	transform = *value;
	node->setPosition (value->position.x, value->position.y, value->position.z);
	node->setOrientation (value->rotation.w, value->rotation.x, value->rotation.y, value->rotation.z);
}

/*****************************************************************************/

void LIRenObject::apply_texture_aliases (
	LIRenAttachment* attachment)
{
	std::map<Ogre::String, Ogre::TexturePtr>::iterator iter;
	for (iter = texture_aliases.begin () ; iter != texture_aliases.end () ; ++iter)
		attachment->replace_texture (iter->first.c_str (), iter->second);
}

LIMdlPose* LIRenObject::channel_animate (
	LIMdlPose*              pose,
	int                     channel,
	int                     keep,
	const LIMdlPoseChannel* info)
{
	/* Create the pose if it doesn't exist. */
	if (pose == NULL)
	{
		pose = limdl_pose_new ();
		if (pose == NULL)
			return NULL;
	}

	/* Clear the channel if NULL was given as the animation. */
	if (info == NULL)
	{
		if (keep)
			limdl_pose_fade_channel (pose, channel, LIMDL_POSE_FADE_AUTOMATIC);
		else
			limdl_pose_destroy_channel (pose, channel);
		return pose;
	}

	/* Merge the channel. */
	limdl_pose_merge_channel (pose, channel, keep, info);

	return pose;
}

void LIRenObject::channel_fade (
	LIMdlPose* pose,
	int        channel,
	float      time)
{
	if (pose != NULL)
		limdl_pose_fade_channel (pose, channel, time);
}

Ogre::TexturePtr LIRenObject::create_texture (int width, int height, const void* pixels) const
{
	// FIXME: Why does the Ogre::PF_R8G8B8A8 format not work?
	Ogre::Image img;
	img.loadDynamicImage ((Ogre::uchar*) pixels, width, height, 1, Ogre::PF_A8B8G8R8);
	Ogre::String unique_name = render->id.next ();
	return render->texture_manager->loadImage (unique_name, LIREN_RESOURCES_TEMPORARY, img);
}

void LIRenObject::rebuild_skeleton ()
{
	int count;
	const LIMdlPoseSkeleton** skeletons;

	/* Check if a skeleton exists. */
	skeleton_rebuild_needed = 0;
	if (pose_skeleton == NULL)
		return;

	/* Check for attachments. */
	if (!attachments.size ())
	{
		limdl_pose_skeleton_rebuild_from_skeletons (pose_skeleton, NULL, 0);
		return;
	}

	/* Allocate space for models. */
	count = 0;
	skeletons = (const LIMdlPoseSkeleton**) lisys_calloc (attachments.size (), sizeof (LIMdlPoseSkeleton*));
	if (skeletons == NULL)
		return;

	/* Add each model to the list. */
	for (size_t i = 0 ; i < attachments.size () ; i++)
	{
		if (attachments[i]->get_replacer () == NULL)
		{
			LIRenModelData* m = attachments[i]->get_model ();
			if (m != NULL && m->rest_pose_skeleton->nodes.count)
			{
				skeletons[count] = m->rest_pose_skeleton;
				count++;
			}
		}
	}

	/* Rebuild the skeleton. */
	limdl_pose_skeleton_rebuild_from_skeletons (pose_skeleton, skeletons, count);
	lisys_free (skeletons);
}

void LIRenObject::remove_entity (
	int index)
{
	/* Remove from the list. */
	/* This must be done first to avoid potential double deletion. */
	LIRenAttachment* attachment = attachments[index];
	attachments.erase(attachments.begin () + index);

	/* Free attachments waiting for the removal of this one. */
	/* This is a potentially recursive operation so the list indices may change
	   wildly. To avoid out of bounds errors for sure, we restart the loop from
	   scratch immediately after removing something. */
	while (true)
	{
		bool found = false;
		for (size_t i = 0 ; i < attachments.size () ; i++)
		{
			if (attachment == attachments[i]->get_replacer ())
			{
				remove_entity (i);
				found = true;
				break;
			}
		}
		if (!found)
			break;
	}

	/* Free the attachment. */
	OGRE_DELETE attachment;
}

void LIRenObject::replace_texture (const char* name, Ogre::TexturePtr& texture)
{
	// Replace in all non-deprecated entities.
	for (size_t i = 0 ; i < attachments.size () ; i++)
	{
		if (!attachments[i]->get_replacer ())
			attachments[i]->replace_texture (name, texture);
	}
}

void LIRenObject::update_entity_settings ()
{
	for (size_t i = 0 ; i < attachments.size () ; i++)
		attachments[i]->update_settings ();
}

/** @} */
/** @} */
/** @} */
