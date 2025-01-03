/* Lips of Suna
 * Copyright© 2007-2010 Lips of Suna development team.
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
 * \addtogroup LIMdlNode Node
 * @{
 */

//#include "lipsofsuna/system.h"
#include"model_node.h"

static void private_calculate_world_transform (
	LIMdlNode*          self,
	LIMdlNodeTransform* ts,
	LIMdlNodeTransform* tp);

/*****************************************************************************/

LIMdlNode* limdl_node_new ()
{
	LIMdlNode* self;

	self = lisys_calloc (1, sizeof (LIMdlNode));
	if (self == NULL)
		return NULL;
	self->rest_transform.rest = limat_transform_identity ();
	self->rest_transform.local = limat_transform_identity ();
	self->rest_transform.local_scale = 1.0f;
	self->rest_transform.global = limat_transform_identity ();
	self->rest_transform.global_scale = 1.0f;
	self->pose_transform = self->rest_transform;

	return self;
}

LIMdlNode* limdl_node_copy (
	const LIMdlNode* node,
	int              recursive)
{
	int i;
	LIMdlNode* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlNode));
	if (self == NULL)
		return NULL;
	self->type = node->type;
	self->rest_transform.rest = node->rest_transform.rest;
	self->rest_transform.local = node->rest_transform.local;
	self->rest_transform.local_scale = node->rest_transform.local_scale;
	self->rest_transform.global = node->rest_transform.global;
	self->rest_transform.global_scale = node->rest_transform.global_scale;
	self->pose_transform.rest = node->pose_transform.rest;
	self->pose_transform.local = node->pose_transform.local;
	self->pose_transform.local_scale = node->pose_transform.local_scale;
	self->pose_transform.global = node->pose_transform.global;
	self->pose_transform.global_scale = node->pose_transform.global_scale;

	/* Copy the name. */
	if (node->name != NULL)
	{
		self->name = lisys_string_dup (node->name);
		if (self->name == NULL)
		{
			limdl_node_free (self);
			return NULL;
		}
	}

	/* Copy type specific data. */
	switch (node->type)
	{
		case LIMDL_NODE_BONE:
			self->bone = node->bone;
			break;
		case LIMDL_NODE_EMPTY:
			break;
		case LIMDL_NODE_LIGHT:
			self->light = node->light;
			break;
		default:
			lisys_assert (0);
			break;
	}

	/* Copy child nodes. */
	if (recursive && node->nodes.count)
	{
		self->nodes.count = node->nodes.count;
		self->nodes.array = lisys_calloc (self->nodes.count, sizeof (LIMdlNode*));
		if (self->nodes.array == NULL)
		{
			limdl_node_free (self);
			return NULL;
		}
		for (i = 0 ; i < self->nodes.count ; i++)
		{
			self->nodes.array[i] = limdl_node_copy (node->nodes.array[i], 1);
			if (self->nodes.array[i] == NULL)
			{
				limdl_node_free (self);
				return NULL;
			}
			self->nodes.array[i]->parent = self;
		}
	}

	return self;
}

void limdl_node_free (
	LIMdlNode* self)
{
	int i;

	/* Free nodes. */
	if (self->nodes.array != NULL)
	{
		for (i = 0 ; i < self->nodes.count ; i++)
		{
			if (self->nodes.array[i] != NULL)
				limdl_node_free (self->nodes.array[i]);
		}
		lisys_free (self->nodes.array);
	}

	lisys_free (self->name);
	lisys_free (self);
}

/**
 * \brief Adds a child node to the node.
 * \param self Node.
 * \param node Node.
 * \return Nonzero on success.
 */
int limdl_node_add_child (
	LIMdlNode* self,
	LIMdlNode* node)
{
	LIMdlNode** tmp;

	tmp = lisys_realloc (self->nodes.array, (self->nodes.count + 1) * sizeof (LIMdlNode*));
	if (tmp == NULL)
		return 0;
	self->nodes.array = tmp;
	self->nodes.array[self->nodes.count] = node;
	self->nodes.count++;

	return 1;
}	

LIMdlNode* limdl_node_find_node (
	const LIMdlNode* self,
	const char*      name)
{
	int i;
	LIMdlNode* tmp;

	/* Test self. */
	if (self->name != NULL && !strcmp (self->name, name))
		return (LIMdlNode*) self;

	/* Test children. */
	for (i = 0 ; i < self->nodes.count ; i++)
	{
		if (self->nodes.array[i] != NULL)
		{
			tmp = limdl_node_find_node (self->nodes.array[i], name);
			if (tmp != NULL)
				return tmp;
		}
	}

	return NULL;
}

int limdl_node_read (
	LIMdlNode*   self,
	LIArcReader* reader)
{
	int i;
	uint32_t count;
	uint32_t type;
	vec3 position;
	quat rotation;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &type) ||
	    !liarc_reader_get_uint32 (reader, &count) ||
	    !liarc_reader_get_text (reader, "", &self->name) ||
	    !liarc_reader_get_float (reader, &position.x) ||
	    !liarc_reader_get_float (reader, &position.y) ||
	    !liarc_reader_get_float (reader, &position.z) ||
	    !liarc_reader_get_float (reader, &rotation.x) ||
	    !liarc_reader_get_float (reader, &rotation.y) ||
	    !liarc_reader_get_float (reader, &rotation.z) ||
	    !liarc_reader_get_float (reader, &rotation.w))
		return 0;
	self->type = type;
	self->nodes.count = count;
	self->rest_transform.rest = limat_transform_init (position, rotation);
	self->rest_transform.local = limat_transform_identity ();
	self->rest_transform.local_scale = 1.0f;
	self->rest_transform.global = limat_transform_init (position, rotation);
	self->rest_transform.global_scale = 1.0f;
	self->pose_transform = self->rest_transform;

	/* Read type sepecific data. */
	switch (type)
	{
		case LIMDL_NODE_BONE:
			if (!limdl_bone_read (self, reader))
				return 0;
			break;
		case LIMDL_NODE_EMPTY:
			break;
		case LIMDL_NODE_LIGHT:
			if (!limdl_light_read (self, reader))
				return 0;
			break;
		default:
			lisys_error_set (EINVAL, "invalid node type %d", type);
			return 0;
	}

	/* Read child nodes. */
	if (self->nodes.count)
	{
		self->nodes.array = lisys_calloc (self->nodes.count, sizeof (LIMdlNode*));
		if (self->nodes.array == NULL)
			return 0;
		for (i = 0 ; i < self->nodes.count ; i++)
		{
			self->nodes.array[i] = limdl_node_new ();
			if (self->nodes.array[i] == NULL)
				return 0;
			self->nodes.array[i]->parent = self;
			if (!limdl_node_read (self->nodes.array[i], reader))
				return 0;
		}
	}

	return 1;
}

/**
 * \brief Updates the world space rest transformation of the node and, optionally, its children.
 * \param self Node.
 * \param recursive Nonzero if children should be rebuilt.
 */
void limdl_node_rebuild (
	LIMdlNode* self,
	int        recursive)
{
	int i;
	LIMdlNodeTransform* ts;
	LIMdlNodeTransform* tp;

	/* Calculate the world transformation. */
	ts = &self->rest_transform;
	if (self->parent != NULL)
		tp = &self->parent->rest_transform;
	else
		tp = NULL;
	private_calculate_world_transform (self, ts, tp);
	self->pose_transform = *ts;

	/* Rebuild children. */
	if (recursive)
	{
		for (i = 0 ; i < self->nodes.count ; i++)
			limdl_node_rebuild (self->nodes.array[i], 1);
	}
}

/**
 * \brief Updates the world space pose transformation of the node and, optionally, its children.
 * \param self Node.
 * \param recursive Nonzero if children should be rebuilt.
 */
void limdl_node_rebuild_pose (
	LIMdlNode* self,
	int        recursive)
{
	int i;
	LIMdlNodeTransform* ts;
	LIMdlNodeTransform* tp;

	/* Reset the rest pose. */
	self->pose_transform.rest = self->rest_transform.rest;

	/* Calculate the world transformation. */
	ts = &self->pose_transform;
	if (self->parent != NULL)
		tp = &self->parent->pose_transform;
	else
		tp = NULL;
	private_calculate_world_transform (self, ts, tp);

	/* Rebuild children. */
	if (recursive)
	{
		for (i = 0 ; i < self->nodes.count ; i++)
			limdl_node_rebuild (self->nodes.array[i], 1);
	}
}

int limdl_node_write (
	const LIMdlNode* self,
	LIArcWriter*     writer)
{
	int i;
	int count = 0;

	/* Count the real number of nodes. */
	for (i = 0 ; i < self->nodes.count ; i++)
	{
		if (self->nodes.array[i] != NULL)
			count++;
	}

	/* Write header. */
	liarc_writer_append_uint32 (writer, self->type);
	liarc_writer_append_uint32 (writer, count);
	if (self->name != NULL)
		liarc_writer_append_string (writer, self->name);
	liarc_writer_append_nul (writer);
	liarc_writer_append_float (writer, self->rest_transform.rest.position.x);
	liarc_writer_append_float (writer, self->rest_transform.rest.position.y);
	liarc_writer_append_float (writer, self->rest_transform.rest.position.z);
	liarc_writer_append_float (writer, self->rest_transform.rest.rotation.x);
	liarc_writer_append_float (writer, self->rest_transform.rest.rotation.y);
	liarc_writer_append_float (writer, self->rest_transform.rest.rotation.z);
	liarc_writer_append_float (writer, self->rest_transform.rest.rotation.w);

	/* Write type sepecific data. */
	switch (self->type)
	{
		case LIMDL_NODE_BONE:
			if (!limdl_bone_write (self, writer))
				return 0;
			break;
		case LIMDL_NODE_EMPTY:
			break;
		case LIMDL_NODE_LIGHT:
			if (!limdl_light_write (self, writer))
				return 0;
			break;
		default:
			lisys_assert (0 && "invalid node type");
			break;
	}

	/* Write child nodes. */
	for (i = 0 ; i < self->nodes.count ; i++)
	{
		if (!limdl_node_write (self->nodes.array[i], writer))
			return 0;
	}

	return !writer->error;
}

LIMdlNode* limdl_node_get_child (
	const LIMdlNode* self,
	int              index)
{
	return self->nodes.array[index];
}

/**
 * \brief Gets the number of direct children.
 * \param self Node.
 * \return Child count.
 */
int limdl_node_get_child_count (
	const LIMdlNode* self)
{
	return self->nodes.count;
}

/**
 * \brief Gets the recursive child count of the node.
 * \param self Node.
 * \return Recursive child count.
 */
int limdl_node_get_child_total (
	const LIMdlNode* self)
{
	int i;
	int n;
	const LIMdlNode* node;

	n = self->nodes.count;
	for (i = 0 ; i < self->nodes.count ; i++)
	{
		node = self->nodes.array[i];
		n += limdl_node_get_child_total (node);
	}

	return n;
}

/**
 * \brief Gets the name of the node.
 * \param self Node.
 * \return Name.
 */
const char* limdl_node_get_name (
	const LIMdlNode* self)
{
	if (self->name != NULL)
		return self->name;
	return "";
}

/**
 * \brief Gets the axes of the node in global coordinates.
 * \param self Node.
 * \param x Return location for the X axis.
 * \param y Return location for the Y axis.
 * \param z Return location for the Z axis.
 */
void limdl_node_get_pose_axes (
	const LIMdlNode* self,
	vec3*     x,
	vec3*     y,
	vec3*     z)
{
	*x = limat_quaternion_transform (self->rest_transform.global.rotation, limat_vector_init (1.0f, 0.0f, 0.0f));
	*y = limat_quaternion_transform (self->rest_transform.global.rotation, limat_vector_init (0.0f, 1.0f, 0.0f));
	*z = limat_quaternion_transform (self->rest_transform.global.rotation, limat_vector_init (0.0f, 0.0f, 1.0f));
}

void limdl_node_get_rest_transform (
	const LIMdlNode* self,
	LIMatTransform*  value)
{
	*value = self->rest_transform.rest;
}

/**
 * \brief Gets the full rest transformation.
 * \param self Node.
 * \param scale Return location for the scale factor.
 * \param value Return location for the transformation.
 */
void limdl_node_get_full_rest_transform (
	const LIMdlNode* self,
	float*           scale,
	LIMatTransform*  value)
{
	*scale = self->rest_transform.global_scale;
	*value = self->rest_transform.global;
}

/**
 * \brief Gets the world space pose transformation.
 * \param self Node.
 * \param scale Return location for the scale factor.
 * \param value Return location for the transformation.
 */
void limdl_node_get_world_transform (
	const LIMdlNode* self,
	float*           scale,
	LIMatTransform*  value)
{
	*scale = self->pose_transform.global_scale;
	*value = self->pose_transform.global;
}

/**
 * \brief Sets the pose transformation of the node.
 *
 * Call #limdl_node_rebuild to apply the transformation.
 *
 * \param self Node.
 * \param scale Scale factor.
 * \param value Local transformation of the node.
 */
void limdl_node_set_local_transform (
	LIMdlNode*            self,
	float                 scale,
	const LIMatTransform* value)
{
	self->pose_transform.local_scale = scale;
	self->pose_transform.local = *value;
}

LIMdlNodeType limdl_node_get_type (
	const LIMdlNode* self)
{
	return self->type;
}

/*****************************************************************************/

static void private_calculate_world_transform (
	LIMdlNode*          self,
	LIMdlNodeTransform* ts,
	LIMdlNodeTransform* tp)
{
	LIMatTransform p;
	LIMatTransform t;
	vec3 tmp;

	if (self->parent != NULL)
	{
		/* Get the tail transformation of the parent. */
		p = tp->global;
		if (self->parent->type == LIMDL_NODE_BONE)
			p.position = tp->tail;

		/* Calculate the global scale factor. */
		/* This is the product of the local scale factors of all the ancestors. */
		ts->global_scale = tp->global_scale * ts->local_scale;

		/* Calculate the global transformation. */
		/* This is the local transformation concatenated with the parent tail transformation. */
		t = ts->rest;
		t.position = limat_vector_multiply (t.position, tp->global_scale);
		t = limat_transform_multiply (p, t);
		t = limat_transform_multiply (t, ts->local);
		ts->global = t;

		/* Calculate the tail position. */
		if (self->type == LIMDL_NODE_BONE)
		{
			tmp = limat_vector_multiply (self->bone.length, ts->global_scale);
			ts->tail = limat_transform_transform (t, tmp);
		}
		else
			ts->tail = ts->global.position;
	}
	else
	{
		/* Calculate global scale factor. */
		/* There are no parents so this is simply the local scale factor. */
		ts->global_scale = ts->local_scale;

		/* Calculate the global transformation. */
		/* There are no parents so this is simply the local transformation. */
		t = limat_transform_multiply (ts->rest, ts->local);
		ts->global = t;

		/* Calculate the tail position. */
		if (self->type == LIMDL_NODE_BONE)
		{
			tmp = limat_vector_multiply (self->bone.length, ts->global_scale);
			ts->tail = limat_transform_transform (t, tmp);
		}
		else
			ts->tail = ts->global.position;
	}
}

/** @} */
/** @} */
