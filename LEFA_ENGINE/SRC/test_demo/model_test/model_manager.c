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
 * \addtogroup LIMdlManager Manager
 * @{
 */

//#include"lipsofsuna/system.h"
#include"model.h"
#include"model_manager.h"

/*****************************************************************************/

LIMdlManager* limdl_manager_new ()
{
	LIMdlManager* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlManager));
	if (self == NULL)
		return NULL;

	/* Allocate the model dictionary. */
	self->models = lialg_u32dic_new ();
	if (self->models == NULL)
	{
		limdl_manager_free (self);
		return NULL;
	}

	/* Initialize random numbers. */
	lialg_random_init (&self->random, lisys_time (NULL));

	return self;
}

void limdl_manager_free(LIMdlManager* self)
{
	if (self->models != NULL)
		lialg_u32dic_free (self->models);
	lisys_free (self);
}


int limdl_manager_add_model (
	LIMdlManager* self,
	LIMdlModel*   model)
{
	lisys_assert (model->id == 0);
	lisys_assert (model->manager == NULL);

	/* Choose a free model ID. */
	while (!model->id)
	{
		model->id = lialg_random_rand (&self->random);
		if (lialg_u32dic_find (self->models, model->id) != NULL)
			model->id = 0;
	}

	/* Add to the dictionary. */
	if (!lialg_u32dic_insert (self->models, model->id, model))
	{
		model->id = 0;
		return 0;
	}
	model->manager = self;

	/* Call the callback. */
	if (self->model_added_callback.callback != NULL)
		self->model_added_callback.callback (self->model_added_callback.userdata, model);

	return 1;
}

void limdl_manager_remove_model (
	LIMdlManager* self,
	LIMdlModel*   model)
{
	lisys_assert (model->id != 0);
	lisys_assert (model->manager == self);
	lisys_assert (lialg_u32dic_find (self->models, model->id) == model);

	/* Call the callback. */
	if (self->model_removed_callback.callback != NULL)
		self->model_removed_callback.callback (self->model_removed_callback.userdata, model);

	/* Remove from the dictionary. */
	lialg_u32dic_remove (self->models, model->id);
	model->id = 0;
	model->manager = NULL;
}

void limdl_manager_free_model (
	LIMdlModel* model)
{
	lisys_assert (model->manager != NULL);
	lisys_assert (model->id != 0);
	lisys_assert (lialg_u32dic_find (model->manager->models, model->id) == model);

	/* Remove and free. */
	limdl_manager_remove_model (model->manager, model);
	limdl_model_free (model);
}

/** @} */
/** @} */
