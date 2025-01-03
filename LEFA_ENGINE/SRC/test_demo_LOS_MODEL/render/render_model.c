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
 * \addtogroup LIRenModel Model
 * @{
 */

/*#include"lipsofsuna/system.h"
*/

#include"internal/internal_render.h"

#include"render_model.h"

#include"internal/r_model.h"

#include"test_demo/model_test/ANOTHER/allsystem.h"

/*#include"internal/render.h"
*/

/**
 * \brief Creates a new model from a loaded model buffer.
 *
 * The previous owner of the model buffer retains the ownership and must
 * ensure that the buffer is not freed before the created renderer model.
 *
 * \param render Renderer.
 * \param model Model description.
 * \return Model ID, or zero on failure.
 */


int liren_render_model_new(LIRenRender* render,
						   LIMdlModel*  model)
{
	LIRenModel* self;

	self = new_LIRenModel(render, model, 0, 1);
	if(self == NULL)
	{
		return 0;
	}

	return get_id(self);
}

/**
 * \brief Frees the model.
 * \param self Renderer.
 * \param id Model ID.
 */
void liren_render_model_free(LIRenRender* self,
							 int          id)
{
	LIRenModel* model;

	model = (LIRenModel*) lialg_u32dic_find(self->models, id);
	if (model != NULL)
	{
		//delete model;
		destroy_LIRenModel(model, self);
	}
}

/**
 * \brief Replaces a texture.
 * \param self Renderer.
 * \param id Model ID.
 * \param name Name of the replaced texture.
 * \param width Width of the new texture.
 * \param height Height of the new texture.
 * \param pixels Pixels in the RGBA format.
 */
void liren_render_model_replace_texture(LIRenRender* self,
										int          id,
										const char*  name,
										int          width,
										int          height,
										const void*  pixels)
{
	LIRenModel* model;

	model = (LIRenModel*) lialg_u32dic_find (self->models, id);
	if (model != NULL)
		model->replace_texture(name, width, height, pixels);
}

/**
 * \brief Returns nonzero if the model has been fully background loaded.
 * \param self Renderer.
 * \param id Model ID.
 * \return Nonzero if background loaded, zero if not.
 */
int liren_render_model_get_loaded(LIRenRender* self,
								  int          id)
{
	LIRenModel* model;

	model = (LIRenModel*) lialg_u32dic_find (self->models, id);
	if(model != NULL)
		return model->get_loaded();
	else
		return 1;
}

/**
 * \brief Replaces the contents of the model.
 * \param self Renderer.
 * \param id Model ID.
 * \param model Model.
 */
void liren_render_model_set_model(LIRenRender* self,
								  int          id,
								  LIMdlModel*  model)
{
	LIRenModel* model_;

	model_ = (LIRenModel*) lialg_u32dic_find(self->models, id);
	if (model_ != NULL)
		model_->set_model (model);
}

/** @} */
/** @} */
