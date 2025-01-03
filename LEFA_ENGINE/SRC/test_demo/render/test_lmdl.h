#ifndef TEST_LMDL_H
#define TEST_LMDL_H

#include<stdbool.h>


#include"model_test/model.h"

#include"test_lmdl/LIRenRender.h"
#include"test_lmdl/render_model.h"




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

	//self = new LIRenModel (render, model, 0);
	if(self == NULL)
	{
		return 0;
	}

	//return self->get_id();
	return LIRenModel_get_id(self);
}

/**
 * \brief Frees the model.
 * \param self Renderer.
 * \param id Model ID.
 */
void liren_render_model_free (LIRenRender* self,
							  int          id)
{
	LIRenModel* model;

	model = (LIRenModel*)lialg_u32dic_find(self->models, id);
    if (model != NULL) 
    {
        // Remove o modelo do dicionário.
        lialg_u32dic_remove(self->models, id);

        // Libera a memória associada ao modelo.
        free(model);
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
/*
void liren_render_model_replace_texture (LIRenRender* self,
										 int          id,
										 const char*  name,
										 int          width,
										 int          height,
										 const void*  pixels)
{
	LIRenModel* model;

	model = (LIRenModel*) lialg_u32dic_find (self->models, id);
	if (model != NULL)
		model->replace_texture (name, width, height, pixels);
}*/

/**
 * \brief Returns nonzero if the model has been fully background loaded.
 * \param self Renderer.
 * \param id Model ID.
 * \return Nonzero if background loaded, zero if not.
 */
int liren_render_model_get_loaded (LIRenRender* self,
								   int          id)
{
	LIRenModel* model;

	model = (LIRenModel*)lialg_u32dic_find(self->models, id);
	if(model != NULL)
	{
		//return model->get_loaded();
		return LIRenModel_get_loaded(model);
	}
	else
	{
		return 1;
	}
}
















#endif /* TEST_LMDL_H */
