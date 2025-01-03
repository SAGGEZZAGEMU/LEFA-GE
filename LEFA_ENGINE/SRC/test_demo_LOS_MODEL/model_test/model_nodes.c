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
 * \addtogroup LIMdlNodes Nodes
 * @{
 */

//#include "lipsofsuna/system.h"
#include"model_nodes.h"

static LIMdlNode* private_find_node(const LIMdlNodes* self,
									const char*       name);

static int private_copy_node(LIMdlNodes*  self,
							 LIMdlNode*   node,
							 LIMdlNode*   parent,
							 LIMdlNode*** root_array,
							 int*         root_count);

/*****************************************************************************/

void limdl_nodes_init(LIMdlNodes* self)
{
	self->count = 0;
	self->array = NULL;
}

void limdl_nodes_deinit(LIMdlNodes* self)
{
	int i;

	if (self->array != NULL)
	{
		for (i = 0 ; i < self->count ; i++)
		{
			if (self->array[i] != NULL)
				limdl_node_free (self->array[i]);
		}
		lisys_free (self->array);
	}
}

void limdl_nodes_clear(LIMdlNodes* self)
{
	limdl_nodes_deinit (self);
	limdl_nodes_init (self);
}

/**
 * \brief Finds a node by name.
 * \param self Nodes.
 * \param name Name of the node to find.
 * \return Node or NULL.
 */
LIMdlNode* limdl_nodes_find_node(const LIMdlNodes* self,
								 const char*       name)
{
	return private_find_node (self, name);
}

/**
 * \brief Merges two node hierarchies.
 *
 * This does skeleton merging by parenting newly created nodes to existing
 * ones whenever possible. Nodes that already exist are skipped. Nodes whose
 * parent has the same name as one of the existing nodes are merged to the
 * existing node. If no parent exists, the node is added to the root of the
 * skeleton.
 *
 * \param self Nodes.
 * \param nodes Nodes to copy.
 */
void limdl_nodes_merge(LIMdlNodes*       self,
					   const LIMdlNodes* nodes)
{
	int i;

	for (i = 0 ; i < nodes->count ; i++)
		private_copy_node (self, nodes->array[i], NULL, &self->array, &self->count);
}

/*****************************************************************************/

static LIMdlNode* private_find_node(const LIMdlNodes* self,
									const char*       name)
{
	int i;
	LIMdlNode* node;

	for (i = 0 ; i < self->count ; i++)
	{
		node = self->array[i];
		node = limdl_node_find_node (node, name);
		if (node != NULL)
			return node;
	}

	return NULL;
}

static int private_copy_node (
	LIMdlNodes*  self,
	LIMdlNode*   node,
	LIMdlNode*   parent,
	LIMdlNode*** root_array,
	int*         root_count)
{
	int i;
	LIMdlNode* copy;
	LIMdlNode** tmp;

	/* Create a new node if there was no existing one. */
	/* If no existing node with the same name existed, we need to create a new
	   node. The new node is added to the parent node, if any, or to the list
	   of root nodes. */
	copy = private_find_node (self, node->name);
	if (copy == NULL)
	{
		/* Copy the node. */
		copy = limdl_node_copy (node, 0);
		if (copy == NULL)
			return 0;

		/* Store the copy. */
		if (parent != NULL)
		{
			if (!limdl_node_add_child (parent, copy))
			{
				limdl_node_free (copy);
				return 0;
			}
			copy->parent = parent;
		}
		else
		{
			tmp = lisys_realloc (*root_array, ((*root_count) + 1) * sizeof (LIMdlNode*));
			if (tmp == NULL)
			{
				limdl_node_free (copy);
				return 0;
			}
			*root_array = tmp;
			(*root_array)[*root_count] = copy;
			*root_count = (*root_count) + 1;
		}
	}

	/* Copy the child nodes recursively. */
	for (i = 0 ; i < node->nodes.count ; i++)
	{
		if (!private_copy_node (self, node->nodes.array[i], copy, root_array, root_count))
			return 0;
	}

	return 1;
}

/** @} */
/** @} */
