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
 * \addtogroup LIMdl Model
 * @{
 * \addtogroup LIMdlPoseBuffer PoseBuffer
 * @{
 */

//#include "lipsofsuna/system.h"
#include"model.h"
#include"model_pose.h"
#include"model_pose_buffer.h"

LIMdlPoseBuffer* limdl_pose_buffer_new (
	const LIMdlModel* model)
{
	int i;
	float s;
	LIMdlPoseBuffer* self;
	LIMdlPoseBufferBone* bone;
	const LIMdlNode* node;
	const LIMdlWeightGroup* group;

	lisys_assert (model != NULL);

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlPoseBuffer));
	if (self == NULL)
		return NULL;

	/* Allocate the bones. */
	self->bones.count = model->weight_groups.count + 1;
	self->bones.array = lisys_calloc (self->bones.count, sizeof (LIMdlPoseBufferBone));
	if (self->bones.array == NULL)
	{
		limdl_pose_buffer_free (self);
		return NULL;
	}

	/* Initialize the fallback bone. */
	self->bones.array[0].transform = limat_transform_identity ();
	self->bones.array[0].scale = limat_vector_init (1.0f, 1.0f, 1.0f);

	/* Initialize the real bones. */
	for (i = 0 ; i < model->weight_groups.count ; i++)
	{
		group = model->weight_groups.array + i;
		bone = self->bones.array + i + 1;
		bone->name = lisys_string_dup (group->bone);
		if (bone->name == NULL)
		{
			limdl_pose_buffer_free (self);
			return NULL;
		}
		node = limdl_model_find_node (model, group->bone);
		if (node != NULL)
		{
			s = node->pose_transform.global_scale;
			bone->scale = limat_vector_init (s, s, s);
			bone->transform = node->pose_transform.global;
		}
		else
		{
			bone->scale = limat_vector_init (1.0f, 1.0f, 1.0f);
			bone->transform = limat_transform_identity ();
		}
	}

	return self;
}

LIMdlPoseBuffer* limdl_pose_buffer_new_copy (
	const LIMdlPoseBuffer* buffer)
{
	int i;
	LIMdlPoseBuffer* self;
	LIMdlPoseBufferBone* bone;
	LIMdlPoseBufferBone* src;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlPoseBuffer));
	if (self == NULL)
		return NULL;

	/* Allocate the bones. */
	self->bones.count = buffer->bones.count;
	self->bones.array = lisys_calloc (self->bones.count, sizeof (LIMdlPoseBufferBone));
	if (self->bones.array == NULL)
	{
		limdl_pose_buffer_free (self);
		return NULL;
	}

	/* Copy the bones. */
	for (i = 0 ; i < self->bones.count ; i++)
	{
		bone = self->bones.array + i;
		src = buffer->bones.array + i;
		if (src->name != NULL)
		{
			bone->name = lisys_string_dup (src->name);
			if (bone->name == NULL)
			{
				limdl_pose_buffer_free (self);
				return NULL;
			}
		}
		bone->scale = src->scale;
		bone->transform = src->transform;
	}

	return self;
}

void limdl_pose_buffer_free (
	LIMdlPoseBuffer* self)
{
	int i;

	if (self->bones.array != NULL)
	{
		for (i = 0 ; i < self->bones.count ; i++)
			lisys_free (self->bones.array[i].name);
		lisys_free (self->bones.array);
	}
	lisys_free (self);
}

void limdl_pose_buffer_copy_pose (
	LIMdlPoseBuffer*       self,
	const LIMdlPoseBuffer* src)
{
	int i;
	LIMdlPoseBufferBone* bone;

	lisys_assert (self->bones.count == src->bones.count);
	for (i = 0 ; i < self->bones.count ; i++)
	{
		bone = self->bones.array + i + 1;
		bone->scale = src->bones.array[i].scale;
		bone->transform = src->bones.array[i].transform;
	}
}

void limdl_pose_buffer_update (
	LIMdlPoseBuffer*   self,
	LIMdlPoseSkeleton* skeleton)
{
	int i;
	float s;
	LIMdlNode* node;
	LIMdlPoseBufferBone* bone;

	/* Update the pose buffer. */
	for (i = 0 ; i < self->bones.count - 1 ; i++)
	{
		bone = self->bones.array + i + 1;
		node = limdl_pose_skeleton_find_node (skeleton, bone->name);
		if (node != NULL)
		{
			s = node->pose_transform.global_scale;
			bone->scale = limat_vector_init (s, s, s);
			bone->transform = node->pose_transform.global;
		}
	}
}

/** @} */
/** @} */
