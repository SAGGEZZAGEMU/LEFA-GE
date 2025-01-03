#ifndef RENDER_MODEL_H
#define RENDER_MODEL_H

#include<stdbool.h>

#include"model_test/model.h"

typedef struct Mesh {
    void* pRep;

    int is_loaded; // 1 se o mesh estiver carregado, 0 caso contrário
    // Adicione outros campos relevantes, se necessário
} Mesh;

typedef struct
{
	int id;
	bool editable;
	LIRenRender* render;

	Mesh mesh;

}LIRenModel;


Mesh* mesh_create(const LIMdlModel* model) 
{
    
    
    Mesh* new_mesh = (Mesh*)malloc(sizeof(Mesh));
    if(new_mesh) 
    {
        new_mesh->is_loaded = 1; // Supondo que o mesh é carregado com sucesso
    }
    return new_mesh;
}

void mesh_free(Mesh* mesh) 
{
    if (mesh) 
    {
        free(mesh);
    }
}

int Mesh_isNull(const Mesh* mesh) 
{
    return mesh == NULL || mesh->pRep == NULL;
}


/**
 * \brief Creates a new model from a loaded model buffer.
 *
 * The previous owner of the model buffer retains the ownership and must
 * ensure that the buffer is not freed before the created renderer model.
 *
 * \param render Renderer.
 * \param model Model description.
 * \param id Unique model ID.
 * \param editable True to keep buffer data. False otherwise.
 */
int LIRenModel_init(LIRenModel* self, 
                    LIRenRender* render, 
                    const LIMdlModel* model, 
                    int id, 
                    int editable) 
{
    if (!self || !render) 
    {
        return 0; // Falha devido a parâmetros inválidos
    }

    // Inicializa os campos da estrutura
    self->id = id;
    self->render = render;
    self->editable = editable;
    //self->mesh = NULL;

    // Escolhe um ID único, se necessário
    while(!self->id) 
    {
        self->id = lialg_random_range(&render->random, 0x00000000, 0x7FFFFFFF);
        if (lialg_u32dic_find(render->objects, self->id)) 
        {
            self->id = 0; // Redefine para tentar novamente
        }
    }

    // Carrega o modelo, se fornecido
    if(model) 
    {
        if(!mesh_create(model))
        {
            return 0; // Falha ao criar a malha
        }
    }

    // Adiciona o modelo ao dicionário de modelos
    if(!lialg_u32dic_insert(render->models, self->id, self)) 
    {
        return 0; // Falha ao inserir no dicionário
    }

    return 1; // Sucesso
}
/*
void LIRenModel_free(LIRenModel* self) 
{
    LIAlgU32dicIter iter;
    LIRenObject* object;

    if (!self || !self->render) 
    {
        return; // Evita operar em ponteiros nulos
    }

    /* Remove o modelo de todos os objetos associados *
    LIALG_U32DIC_FOREACH(iter, self->render->objects) 
    {
        object = (LIRenObject*)iter.value;
        if(object) 
        {
            object->remove_model(self);
        }
    }

    /* Remove o modelo do dicionário de modelos *
    lialg_u32dic_remove(self->render->models, self->id);

    /* Libera recursos adicionais se necessário *
    if (self->mesh) 
    {
        free(self->mesh); // Libera a malha associada, se existir
        self->mesh = NULL;
    }

    /* Limpa a memória do próprio modelo *
    free(self);
}
*/
/*
void LIRenModel::replace_buffer_vtx_nml(const void* data)
{
	if (mesh.isNull ())
		return;
	if (!get_loaded ())
		return;
    
	Ogre::VertexBufferBinding* binding = mesh->sharedVertexData->vertexBufferBinding;
	if (binding == NULL)
		return;
	const Ogre::HardwareVertexBufferSharedPtr& buffer = binding->getBuffer (0);
	if (buffer.isNull())
		return;

	buffer->writeData (0, buffer->getSizeInBytes (), data, true);
}*/

/**
 * \brief Replaces a texture.
 * \param name Name of the replaced texture.
 * \param width Width of the new texture.
 * \param height Height of the new texture.
 * \param pixels Pixels in the RGBA format.
 */
/*
void LIRenModel::replace_texture(const char* name,
								 int         width,
								 int         height,
								 const void* pixels)
{
	Ogre::TexturePtr texture;

	if (mesh.isNull ())
		return;
	if (!get_loaded ())
		return;

	for(int submesh_idx = 0; submesh_idx < mesh->getNumSubMeshes(); ++submesh_idx)
	{
		// Get the material of the submesh.
		Ogre::SubMesh* submesh = mesh->getSubMesh (submesh_idx);
		const Ogre::String& submeshmatname = submesh->getMaterialName ();
		Ogre::MaterialPtr submeshmat = render->material_manager->getByName (submeshmatname);
		if (submeshmat.isNull ())
			continue;

		// Check if there are replaceable textures.
		if (!render->material_utils->has_overridable_texture (submeshmat, name))
			continue;

		// Create the replacement texture.
		// FIXME: Why does the Ogre::PF_R8G8B8A8 format not work?
		if (texture.isNull ())
		{
			Ogre::Image img;
			img.loadDynamicImage ((Ogre::uchar*) pixels, width, height, 1, Ogre::PF_A8B8G8R8);
			Ogre::String unique_name = render->id.next ();
			texture = render->texture_manager->loadImage (unique_name, LIREN_RESOURCES_TEMPORARY, img);
		}

		// Create the texture aliases.
		Ogre::String tmp(name);
		submesh->addTextureAlias(tmp + ".png", texture->getName());
		submesh->addTextureAlias(tmp + ".dds", texture->getName());
		submesh->updateMaterialUsingTextureAliases (); // FIXME: Needed?
	}
}*/

/**
 * \brief Gets the editable data of the model.
 * \return Editable data. NULL if not editable.
 */
/*
LIRenModelData* LIRenModel::get_editable() const
{
	LIRenMeshBuilder* builder;

	if(!editable)
	{
		return NULL;
	}
	
	builder = (LIRenMeshBuilder*) lialg_strdic_find(render->mesh_builders, mesh->getName ().c_str ());
	if(!builder)
	{
		return NULL;
	}
	
	return builder->get_model();
}*/

/**
 * \brief Gets the ID of the model.
 * \param model Pointer to the model.
 * \return ID of the model.
 */
int LIRenModel_get_id(const LIRenModel* model) 
{
    if (!model) 
    {
        return -1; // Retorna um valor inválido para indicar erro.
    }
    return model->id;
}


int LIRenModel_get_loaded(const LIRenModel* self) 
{
    if(self == NULL || Mesh_isNull(&self->mesh)) 
    {
        return 0; // Retorna 0 se o modelo ou o mesh não foram inicializados
    }

    return self->mesh.is_loaded; // Verifica o estado "carregado"
}

int LIRenModel_set_model(LIRenModel* self, const LIMdlModel* model) 
{
    if(!self || !model) 
    {
        return 0; // Retorna 0 se os parâmetros forem inválidos
    }

    // Cria ou atualiza a malha a partir do modelo
    if(!mesh_create(model)) 
    {
        return 0; // Retorna 0 se a criação da malha falhar
    }

    return 1; // Sucesso
}







#endif /* RENDER_MODEL_H */
