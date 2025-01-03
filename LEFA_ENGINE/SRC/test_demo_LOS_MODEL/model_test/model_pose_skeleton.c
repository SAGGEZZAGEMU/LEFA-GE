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
 * \addtogroup LIMdlPoseSkeleton PoseSkeleton
 * @{
 */

//#include "lipsofsuna/system.h"
#include"model.h"
#include"model_pose.h"
#include"model_pose_skeleton.h"

static void private_copy_nodes (
	LIMdlPoseSkeleton* self,
	const LIMdlModel** models,
	int                models_num);

static void private_rebuild_lookup (
	LIMdlPoseSkeleton* self);

static void private_rebuild_lookup_node (
	LIMdlPoseSkeleton* self,
	LIMdlNode*         node);

static void private_transform_node (
	LIMdlPoseSkeleton* self,
	LIMdlPose*         pose,
	LIMdlNode*         node);

/*****************************************************************************/

LIMdlPoseSkeleton* limdl_pose_skeleton_new (
	const LIMdlModel** models,
	int                count)
{
	LIMdlPoseSkeleton* self;

	lisys_assert (models != NULL || !count);

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlPoseSkeleton));
	if (self == NULL)
		return NULL;

	/* Copy nodes. */
	if (count)
	{
		private_copy_nodes (self, models, count);
		private_rebuild_lookup (self);
	}

	return self;
}

void limdl_pose_skeleton_free (
	LIMdlPoseSkeleton* self)
{
	if (self->lookup != NULL)
		lialg_strdic_free (self->lookup);
	limdl_nodes_deinit (&self->nodes);
	lisys_free (self);
}

/**
 * \brief Finds a node by name.
 * \param self Pose skeleton.
 * \param name Name of the node to find.
 * \return Node or NULL.
 */
LIMdlNode* limdl_pose_skeleton_find_node (
	const LIMdlPoseSkeleton* self,
	const char*              name)
{
	if (self->lookup == NULL)
		return NULL;
	return lialg_strdic_find (self->lookup, name);
}

/**
 * \brief Rebuilds the skeleton from multiple models.
 *
 * Rebuilds the node hierarchy of the skeleton from the array of models.
 * The array should contain all models whose bones are needed by the
 * pose buffer, including the model of the pose buffer itself. In the case
 * of duplicate bones, the bone of the first model in the list is used.
 *
 * \param self Pose skeleton.
 * \param models Array of models.
 * \param count Number of models in the array.
 */
void limdl_pose_skeleton_rebuild_from_models (
	LIMdlPoseSkeleton* self,
	const LIMdlModel** models,
	int                count)
{
	private_copy_nodes (self, models, count);
	private_rebuild_lookup (self);
}

/**
 * \brief Rebuilds the skeleton from multiple skeletons.
 * \param self Pose skeleton.
 * \param skeletons Array of pose skeletons.
 * \param count Number of skeletons in the array.
 */
void limdl_pose_skeleton_rebuild_from_skeletons (
	LIMdlPoseSkeleton*        self,
	const LIMdlPoseSkeleton** skeletons,
	int                       count)
{
	int i;
	int j;

	limdl_nodes_clear (&self->nodes);
	for (j = 0 ; j < count ; j++)
	{
		for (i = 0 ; i < skeletons[j]->nodes.count ; i++)
			limdl_nodes_merge (&self->nodes, &skeletons[j]->nodes);
	}
	private_rebuild_lookup (self);
}

void limdl_pose_skeleton_update (
	LIMdlPoseSkeleton* self,
	LIMdlPose*         pose)
{
	int i;

	/* Transform each node. */
	for (i = 0 ; i < self->nodes.count ; i++)
		private_transform_node (self, pose, self->nodes.array[i]);
}

/*****************************************************************************/

static void private_copy_nodes (
	LIMdlPoseSkeleton* self,
	const LIMdlModel** models,
	int                models_num)
{
	int i;
	int j;

	/* Free the old nodes. */
	limdl_nodes_clear (&self->nodes);

	/* Copy nodes from the models. */
	/* This does skeleton merging by adding new nodes under an existing node
	   if the name of the parent exists in the skeleton. If no parent exists,
	   the node is added to the root of the skeleton. */
	for (j = 0 ; j < models_num ; j++)
	{
		for (i = 0 ; i < models[j]->nodes.count ; i++)
			limdl_nodes_merge (&self->nodes, &models[j]->nodes);
	}
}

static void private_rebuild_lookup (
	LIMdlPoseSkeleton* self)
{
	int i;

	/* Clear the lookup. */
	if (self->lookup == NULL)
	{
		self->lookup = lialg_strdic_new ();
		if (self->lookup == NULL)
			return;
	}
	else
		lialg_strdic_clear (self->lookup);

	/* Add the nodes recursively. */
	for (i = 0 ; i < self->nodes.count ; i++)
		private_rebuild_lookup_node (self, self->nodes.array[i]);
}

static void private_rebuild_lookup_node (
	LIMdlPoseSkeleton* self,
	LIMdlNode*         node)
{
	int i;

	/* Add the node. */
	if (node->name != NULL)
	{
		if (lialg_strdic_find (self->lookup, node->name) == NULL)
			lialg_strdic_insert (self->lookup, node->name, node);
	}

	/* Add the child nodes recursively. */
	for (i = 0 ; i < node->nodes.count ; i++)
		private_rebuild_lookup_node (self, node->nodes.array[i]);
}

static void private_transform_node (
	LIMdlPoseSkeleton* self,
	LIMdlPose*         pose,
	LIMdlNode*         node)
{
	int i;
	float scale;
	LIMatTransform transform;

	/* Calculate the transformation. */
	limdl_pose_calculate_node_tranformation (pose, node->name, &transform, &scale);

	/* Update the node transformation. */
	limdl_node_set_local_transform (node, scale, &transform);
	limdl_node_rebuild_pose (node, 0);

	/* Update the child transformations recursively. */
	for (i = 0 ; i < node->nodes.count ; i++)
		private_transform_node (self, pose, node->nodes.array[i]);
}

/** @} */
/** @} */
