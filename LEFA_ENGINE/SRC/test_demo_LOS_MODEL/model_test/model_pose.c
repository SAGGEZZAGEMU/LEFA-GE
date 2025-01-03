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
 * \addtogroup LIMdl Model
 * @{
 * \addtogroup LIMdlPose Pose
 * @{
 */

//#include "lipsofsuna/system.h"
#include"model_pose.h"

static void private_clear_pose (
	LIMdlPose* self);

static LIMdlPoseChannel* private_create_channel (
	LIMdlPose* self,
	int        channel);

static LIMdlPoseChannel* private_find_channel (
	const LIMdlPose* self,
	int              channel);

static LIMdlAnimation private_empty_anim =
{
	0, "empty", 0.0f, 0.0f, { 0, NULL }, { 0, NULL }
};

/*****************************************************************************/

/**
 * \brief Creates a new model pose.
 * \return New model pose or NULL.
 */
LIMdlPose* limdl_pose_new ()
{
	LIMdlPose* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlPose));
	if (self == NULL)
		return NULL;
	lialg_random_init (&self->random, 1);

	/* Allocate channel tree. */
	self->channels = lialg_u32dic_new ();
	if (self->channels == NULL)
	{
		limdl_pose_free (self);
		return NULL;
	}

	return self;
}

/**
 * \brief Creates a copy of the pose.
 *
 * The copied pose only inherits the channels. The model of the pose is set as
 * NULL so all animations are considered as missing. They can be reinstantiated
 * by assigning a model to the pose.
 *
 * \param pose Model pose.
 * \return Copied model pose.
 */
LIMdlPose* limdl_pose_new_copy (
	LIMdlPose* pose)
{
	LIAlgU32dicIter iter;
	LIMdlPose* self;
	LIMdlPoseChannel* chan;
	LIMdlPoseChannel* chan1;

	/* Allocate the copy. */
	self = limdl_pose_new ();
	if (self == NULL)
		return NULL;

	/* Copy channels. */
	LIALG_U32DIC_FOREACH (iter, pose->channels)
	{
		chan = iter.value;
		chan1 = limdl_pose_channel_new_copy (chan);
		if (chan1)
		{
			if (!lialg_u32dic_insert (self->channels, iter.key, chan1))
				limdl_pose_channel_free (chan1);
		}
	}

	/* TODO: Copy fades. */

	return self;
}

/**
 * \brief Frees the model pose.
 * \param self Model pose.
 */
void limdl_pose_free (
	LIMdlPose* self)
{
	private_clear_pose (self);
	if (self->channels != NULL)
		lialg_u32dic_free (self->channels);
	lisys_free (self);
}

/**
 * \brief Calculates the local transformation of a node.
 * \param self Pose.
 * \param node Node name.
 * \param result_transform Return location for the transformation.
 * \param result_scale Return location for the scale.
 */
void limdl_pose_calculate_node_tranformation (
	const LIMdlPose* self,
	const char*      node,
	LIMatTransform*  result_transform,
	float*           result_scale)
{
	int i;
	int tmp1;
	int tmp2;
	float node_scale;
	float weight;
	float weight1;
	LIAlgU32dicIter iter;
	LIMatTransform node_transform;
	LIMdlPoseChannel* chan;
	LIMdlPoseChannel* channels_s;
	LIMdlPoseChannel* channels_t;

	/* Start from the identity transformation. */
	*result_scale = 1.0f;
	result_transform->position = limat_vector_init (0.0f, 0.0f, 0.0f);
	result_transform->rotation = limat_quaternion_init (0.0f, 0.0f, 0.0f, 1.0f);

	/* Return immediately if no channels exist. */
	if (!self->channels->size)
		return;

	/* Allocate space for the channels. */
	channels_s = lisys_calloc (self->channels->size * 2, sizeof (LIMdlPoseChannel));
	if (channels_s == NULL)
		return;
	channels_t = channels_s + self->channels->size;

	/* Create shallow copies of the channels. */
	i = 0;
	LIALG_U32DIC_FOREACH (iter, self->channels)
	{
		chan = iter.value;
		limdl_pose_channel_get_node_priority (chan, node, &tmp1, &tmp2);
		channels_s[i] = *chan;
		channels_s[i].priority_scale = tmp1;
		channels_s[i].priority_transform = tmp2;
		channels_t[i] = *chan;
		channels_t[i].priority_scale = tmp1;
		channels_t[i].priority_transform = tmp2;
		i++;
	}

	/* Sort the channels by priority. */
	qsort (channels_s, self->channels->size, sizeof (LIMdlPoseChannel),
		(int(*)(const void*, const void*)) limdl_pose_channel_compare_scale);
	qsort (channels_t, self->channels->size, sizeof (LIMdlPoseChannel),
		(int(*)(const void*, const void*)) limdl_pose_channel_compare_transform);

	/* Apply the transformation influences. */
	for (i = 0 ; i < self->channels->size ; i++)
	{
		chan = channels_t + i;
		if (limdl_animation_get_transform (chan->animation, node, chan->time, &node_scale, &node_transform))
		{
			limdl_pose_channel_get_weight (chan, node, &weight1, &weight);
			switch (chan->blend_mode)
			{
				case LIMDL_POSE_BLEND_ADD:
					node_transform.position = limat_vector_multiply (node_transform.position, weight);
					node_transform.rotation = limat_quaternion_nlerp (node_transform.rotation, limat_quaternion_identity (), weight);
					result_transform->rotation = limat_quaternion_multiply (
						result_transform->rotation, node_transform.rotation);
					result_transform->position = limat_vector_add (
						result_transform->position, node_transform.position);
					break;
				case LIMDL_POSE_BLEND_MIX:
					result_transform->rotation = limat_quaternion_nlerp (
						node_transform.rotation, result_transform->rotation, weight);
					result_transform->position = limat_vector_lerp (
						node_transform.position, result_transform->position, weight);
					break;
				case LIMDL_POSE_BLEND_REPLACE:
					result_transform->rotation = node_transform.rotation;
					result_transform->position = node_transform.position;
					break;
			}
		}
	}

	/* Apply the scaling influences. */
	for (i = 0 ; i < self->channels->size ; i++)
	{
		chan = channels_s + i;
		if (limdl_animation_get_transform (chan->animation, node, chan->time, &node_scale, &node_transform))
		{
			limdl_pose_channel_get_weight (chan, node, &weight, &weight1);
			switch (chan->blend_mode)
			{
				case LIMDL_POSE_BLEND_ADD:
					*result_scale += (node_scale - 1.0f);
					break;
				case LIMDL_POSE_BLEND_MIX:
					*result_scale = (*result_scale) * (1.0 - weight) + node_scale * weight;
					break;
				case LIMDL_POSE_BLEND_REPLACE:
					*result_scale = node_scale;
					break;
			}
		}
	}

	/* Free the shallow copies. */
	lisys_free (channels_s);
}

void limdl_pose_clear_channel_node_priorities (
	LIMdlPose*  self,
	int         channel)
{
	LIAlgStrdicIter iter;
	LIMdlPoseChannel* chan;

	chan = lialg_u32dic_find (self->channels, channel);
	if (chan == NULL || chan->priorities == NULL)
		return;
	LIALG_STRDIC_FOREACH (iter, chan->priorities)
		lisys_free (iter.value);
	lialg_strdic_free (chan->priorities);
	chan->priorities = NULL;
}

void limdl_pose_clear_channel_node_weights (
	LIMdlPose*  self,
	int         channel)
{
	LIAlgStrdicIter iter;
	LIMdlPoseChannel* chan;

	chan = lialg_u32dic_find (self->channels, channel);
	if (chan == NULL || chan->weights == NULL)
		return;
	LIALG_STRDIC_FOREACH (iter, chan->weights)
		lisys_free (iter.value);
	lialg_strdic_free (chan->weights);
	chan->weights = NULL;
}

/**
 * \brief Destroys all channels and fades.
 * \param self Model pose.
 */
void limdl_pose_destroy_all (
	LIMdlPose* self)
{
	private_clear_pose (self);
}

/**
 * \brief Destroys an animation channel.
 *
 * Channels are destroyed automatically with the pose but it's also possible to
 * destroy a channel explicitly with this function.
 *
 * \param self Model pose.
 * \param channel Channel number.
 */
void limdl_pose_destroy_channel (
	LIMdlPose* self,
	int        channel)
{
	LIMdlPoseChannel* chan;

	chan = lialg_u32dic_find (self->channels, channel);
	if (chan == NULL)
		return;
	lialg_u32dic_remove (self->channels, channel);
	limdl_pose_channel_free (chan);
}

/**
 * \brief Fades out the channel.
 *
 * If an animation is playing or paused on the channel, a new fade sequence
 * is created and played seamlessly in the background. The channel cleared
 * and immediately ready to accept a new animation.
 *
 * If the channel is empty, no fading takes place, so it is safe to call this
 * function every time you set a new animation and want it to blend.
 *
 * \param self Model pose.
 * \param channel Channel number.
 * \param secs Fade out time in seconds or LIMDL_POSE_FADE_AUTOMATIC.
 */
void limdl_pose_fade_channel (
	LIMdlPose* self,
	int        channel,
	float      secs)
{
	uint32_t id;
	LIMdlPoseChannel* chan;

	/* Remove the channel from the dictionary. */
	chan = lialg_u32dic_find (self->channels, channel);
	if (chan == NULL)
		return;
	lialg_u32dic_remove (self->channels, channel);

	/* Handle auto rate. */
	if (secs == LIMDL_POSE_FADE_AUTOMATIC)
		secs = chan->fade_out.duration;

	/* Destroy immediately if the fade time is zero. */
	if (secs < LIMAT_EPSILON || chan->fade_out.mode == LIMDL_POSE_FADE_OUT_INSTANT)
	{
		limdl_pose_destroy_channel (self, channel);
		return;
	}

	/* Set fade information. */
	chan->fade_in.active = 0;
	chan->fade_out.active = 1;
	chan->fade_out.duration = secs;
	chan->fade_timer = 0.0f;

	/* Choose a random channel number. */
	id = lialg_random_range (&self->random, 0xFFFF, 0x1FFFFFFF);
	if (lialg_u32dic_find (self->channels, id) != NULL)
		id = lialg_random_range (&self->random, 0xFFFF, 0x1FFFFFFF);

	/* Create a new fade channel. */
	if (!lialg_u32dic_insert (self->channels, id, chan))
		limdl_pose_destroy_channel (self, channel);
}

/**
 * \brief Merges or adds a pose channel.
 * \param self Model pose.
 * \param channel Channel number, or -1 for automatic.
 * \param keep Nonzero to keep appropriate data from old animations, zero to overwrite all data.
 * \param info Pose channel data.
 */
void limdl_pose_merge_channel (
	LIMdlPose*              self,
	int                     channel,
	int                     keep,
	const LIMdlPoseChannel* info)
{
	const char* name1;
	LIAlgStrdicIter iter;

	lisys_assert (info != NULL);
	lisys_assert (info->animation != NULL);

	/* Avoid restarts in simple cases. */
	/* The position is kept if the animation is repeating and being replaced with
	   the same one but parameters such as fading and weights still need to be reset. */
	if (info->repeats > 0 && channel != -1)
	{
		if (keep)
		{
			keep = 0;
			if (limdl_pose_get_channel_state (self, channel) == LIMDL_POSE_CHANNEL_STATE_PLAYING &&
				limdl_pose_get_channel_repeats (self, channel) == -1)
			{
				name1 = limdl_pose_get_channel_name (self, channel);
				if (!strcmp (info->animation->name, name1))
					keep = 1;
			}
		}
	}
	else
		keep = 0;

	/* Automatic channel assignment. */
	if (channel == -1)
	{
		for (channel = 254 ; channel > 0 ; channel--)
		{
			if (limdl_pose_get_channel_state (self, channel) == LIMDL_POSE_CHANNEL_STATE_INVALID)
				break;
		}
	}

	/* Update or initialize the channel. */
	if (!keep)
	{
		limdl_pose_fade_channel (self, channel, LIMDL_POSE_FADE_AUTOMATIC);
		limdl_pose_set_channel_animation (self, channel, info->animation);
		limdl_pose_set_channel_repeats (self, channel, info->repeats);
		limdl_pose_set_channel_position (self, channel, info->time);
		limdl_pose_set_channel_state (self, channel, LIMDL_POSE_CHANNEL_STATE_PLAYING);
	}
	limdl_pose_set_channel_blend_mode (self, channel, info->blend_mode);
	limdl_pose_set_channel_repeat_end (self, channel, info->repeat_end);
	limdl_pose_set_channel_repeat_start (self, channel, info->repeat_start);
	limdl_pose_set_channel_priority_scale (self, channel, info->priority_scale);
	limdl_pose_set_channel_priority_transform (self, channel, info->priority_transform);
	limdl_pose_set_channel_weight_scale (self, channel, info->weight_scale);
	limdl_pose_set_channel_weight_transform (self, channel, info->weight_transform);
	limdl_pose_set_channel_time_scale (self, channel, info->time_scale);
	limdl_pose_set_channel_fade_in (self, channel, info->fade_in.mode, info->fade_in.duration);
	limdl_pose_set_channel_fade_out (self, channel, info->fade_out.mode, info->fade_out.duration);

	/* Handle optional per-node priorities. */
	if (info->priorities != NULL)
	{
		limdl_pose_clear_channel_node_priorities (self, channel);
		LIALG_STRDIC_FOREACH (iter, info->priorities)
		{
			limdl_pose_set_channel_priority_node (self, channel,
				iter.key, *((int*) iter.value));
		}
	}

	/* Handle optional per-node weights. */
	if (info->weights != NULL)
	{
		limdl_pose_clear_channel_node_weights (self, channel);
		LIALG_STRDIC_FOREACH (iter, info->weights)
		{
			limdl_pose_set_channel_weight_node (self, channel,
				iter.key, *((float*) iter.value));
		}
	}
}

/**
 * \brief Progresses the animation.
 *
 * \param self Model pose.
 * \param secs Number of seconds to progress.
 */
void limdl_pose_update (
	LIMdlPose* self,
	float      secs)
{
	LIAlgU32dicIter iter;
	LIMdlPoseChannel* chan;

	/* Update channels. */
	LIALG_U32DIC_FOREACH (iter, self->channels)
	{
		chan = iter.value;
		switch (chan->state)
		{
			case LIMDL_POSE_CHANNEL_STATE_PAUSED:
				break;
			case LIMDL_POSE_CHANNEL_STATE_PLAYING:
				if (!limdl_pose_channel_play (chan, secs))
				{
					lialg_u32dic_remove (self->channels, iter.key);
					limdl_pose_channel_free (chan);
				}
				break;
		}
	}
}

int limdl_pose_get_channel_blend_mode (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0;
	return chan->blend_mode;
}

void limdl_pose_set_channel_blend_mode (
	LIMdlPose* self,
	int        channel,
	int        value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->blend_mode = value;
}

LIMdlAnimation* limdl_pose_get_channel_animation (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return NULL;
	return chan->animation;
}

void limdl_pose_set_channel_animation (
	LIMdlPose*      self,
	int             channel,
	LIMdlAnimation* animation)
{
	LIMdlAnimation* anim;
	LIMdlPoseChannel* chan;

	/* Copy the animation. */
	lisys_assert (animation != NULL);
	anim = limdl_animation_new_copy (animation);
	if (anim == NULL)
		return;

	/* Create the channel. */
	chan = private_create_channel (self, channel);
	if (chan == NULL)
	{
		limdl_animation_free (anim);
		return;
	}
	chan->time = 0.0f;
	chan->fade_in.duration = 0.0f;
	chan->fade_out.duration = 0.0f;
	limdl_animation_free (chan->animation);
	chan->animation = anim;
}

/**
 * \brief Gets the repeat range end offset, in seconds.
 * \param self Model pose.
 * \param channel Channel number.
 * \return Repeat range end, in seconds.
 */
float limdl_pose_get_channel_repeat_end (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->repeat_end;
}

/**
 * \brief Sets the repeat range end offset, in seconds.
 * \param self Model pose.
 * \param channel Channel number.
 * \param value Repeat range end, in seconds.
 */
void limdl_pose_set_channel_repeat_end (
	LIMdlPose* self,
	int        channel,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->repeat_end = value;
}

/**
 * \brief Gets the repeat range start offset, in seconds.
 * \param self Model pose.
 * \param channel Channel number.
 * \return Repeat range start, in seconds.
 */
float limdl_pose_get_channel_repeat_start (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->repeat_start;
}

/**
 * \brief Sets the repeat range start offset, in seconds.
 * \param self Model pose.
 * \param channel Channel number.
 * \param value Repeat range start, in seconds.
 */
void limdl_pose_set_channel_repeat_start (
	LIMdlPose* self,
	int        channel,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->repeat_start = value;
}

float limdl_pose_get_channel_fade_in (
	LIMdlPose* self,
	int        channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->fade_in.duration;
}

void limdl_pose_set_channel_fade_in (
	LIMdlPose* self,
	int        channel,
	int        mode,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan != NULL)
	{
		chan->fade_in.mode = mode;
		chan->fade_in.duration = value;
	}
}

float limdl_pose_get_channel_fade_out (
	LIMdlPose* self,
	int        channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->fade_out.duration;
}

void limdl_pose_set_channel_fade_out (
	LIMdlPose* self,
	int        channel,
	int        mode,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan != NULL)
	{
		chan->fade_out.mode = mode;
		chan->fade_out.duration = value;
	}
}

const char* limdl_pose_get_channel_name (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return "";
	return chan->animation->name;
}

float limdl_pose_get_channel_position (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->time;
}

void limdl_pose_set_channel_position (
	LIMdlPose* self,
	int        channel,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->time = value;
	if (chan->time > limdl_animation_get_duration (chan->animation))
		chan->time = limdl_animation_get_duration (chan->animation);
}

int* limdl_pose_get_channel_priority_node (
	const LIMdlPose* self,
	int              channel,
	const char*      node)
{
	int* ptr;
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL || chan->priorities == NULL)
		return NULL;
	ptr = lialg_strdic_find (chan->priorities, node);

	return ptr;
}

int limdl_pose_set_channel_priority_node (
	LIMdlPose*  self,
	int         channel,
	const char* node,
	int         value)
{
	LIMdlPoseChannel* chan;

	/* Find the channel. */
	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0;

	/* Replace or add a node priority. */
	return limdl_pose_channel_set_node_priority (chan, node, value);
}

LIAlgStrdic* limdl_pose_get_channel_priority_nodes (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return NULL;

	return chan->priorities;
}

int limdl_pose_get_channel_priority_scale (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->priority_scale;
}

void limdl_pose_set_channel_priority_scale (
	LIMdlPose* self,
	int        channel,
	int      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->priority_scale = value;
}

int limdl_pose_get_channel_priority_transform (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->priority_transform;
}

void limdl_pose_set_channel_priority_transform (
	LIMdlPose* self,
	int        channel,
	int      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->priority_transform = value;
}

float* limdl_pose_get_channel_weight_node (
	const LIMdlPose* self,
	int              channel,
	const char*      node)
{
	float* ptr;
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL || chan->weights == NULL)
		return NULL;
	ptr = lialg_strdic_find (chan->weights, node);

	return ptr;
}

int limdl_pose_set_channel_weight_node (
	LIMdlPose*  self,
	int         channel,
	const char* node,
	float       value)
{
	LIMdlPoseChannel* chan;

	/* Find the channel. */
	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0;

	/* Replace or add a node weight. */
	return limdl_pose_channel_set_node_weight (chan, node, value);
}

LIAlgStrdic* limdl_pose_get_channel_weight_nodes (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return NULL;

	return chan->weights;
}

float limdl_pose_get_channel_weight_scale (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->weight_scale;
}

void limdl_pose_set_channel_weight_scale (
	LIMdlPose* self,
	int        channel,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->weight_scale = value;
}

float limdl_pose_get_channel_weight_transform (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0.0f;
	return chan->weight_transform;
}

void limdl_pose_set_channel_weight_transform (
	LIMdlPose* self,
	int        channel,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->weight_transform = value;
}

int limdl_pose_get_channel_repeats (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0;
	return chan->repeats;
}

void limdl_pose_set_channel_repeats (
	LIMdlPose* self,
	int        channel,
	int        value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->repeats = value;
}

/**
 * \brief Gets the state of a channel.
 * \param self Model pose.
 * \param channel Channel number.
 * \return Current state.
 */
LIMdlPoseChannelState limdl_pose_get_channel_state (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return LIMDL_POSE_CHANNEL_STATE_INVALID;
	return chan->state;
}

/**
 * \brief Sets the state of a channel.
 *
 * \param self Model pose.
 * \param channel Channel number.
 * \param value New state.
 */
void limdl_pose_set_channel_state (
	LIMdlPose*            self,
	int                   channel,
	LIMdlPoseChannelState value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL || chan->state == value)
		return;
	switch (value)
	{
		case LIMDL_POSE_CHANNEL_STATE_PLAYING:
			chan->state = value;
			break;
		case LIMDL_POSE_CHANNEL_STATE_PAUSED:
			chan->state = value;
			break;
		default:
			lisys_assert (0);
			break;
	}
}

float limdl_pose_get_channel_time_scale (
	const LIMdlPose* self,
	int              channel)
{
	LIMdlPoseChannel* chan;

	chan = private_find_channel (self, channel);
	if (chan == NULL)
		return 0;
	return chan->time_scale;
}

void limdl_pose_set_channel_time_scale (
	LIMdlPose* self,
	int        channel,
	float      value)
{
	LIMdlPoseChannel* chan;

	chan = private_create_channel (self, channel);
	if (chan == NULL)
		return;
	chan->time_scale = value;
}

/*****************************************************************************/

static void private_clear_pose (
	LIMdlPose* self)
{
	LIAlgU32dicIter iter;

	/* Clear the channel tree. */
	if (self->channels != NULL)
	{
		LIALG_U32DIC_FOREACH (iter, self->channels)
			limdl_pose_channel_free (iter.value);
		lialg_u32dic_clear (self->channels);
	}
}

static LIMdlPoseChannel* private_create_channel (
	LIMdlPose* self,
	int        channel)
{
	LIMdlAnimation* anim;
	LIMdlPoseChannel* chan;

	/* Check for an existing channel. */
	chan = private_find_channel (self, channel);
	if (chan != NULL)
		return chan;

	/* Create an empty animation. */
	anim = limdl_animation_new_copy (&private_empty_anim);
	if (anim == NULL)
		return 0;

	/* Create a new channel. */
	chan = lisys_calloc (1, sizeof (LIMdlPoseChannel));
	if (chan == NULL)
	{
		limdl_animation_free (anim);
		return 0;
	}
	chan->state = LIMDL_POSE_CHANNEL_STATE_PLAYING;
	chan->animation = anim;
	chan->priority_scale = 0;
	chan->priority_transform = 0;
	chan->weight_scale = 1.0f;
	chan->weight_transform = 1.0f;
	chan->time_scale = 1.0f;

	/* Register the channel. */
	if (!lialg_u32dic_insert (self->channels, channel, chan))
	{
		limdl_animation_free (chan->animation);
		lisys_free (chan);
		return 0;
	}

	return chan;
}

static LIMdlPoseChannel* private_find_channel (
	const LIMdlPose* self,
	int              channel)
{
	return lialg_u32dic_find (self->channels, channel);
}

/** @} */
/** @} */
