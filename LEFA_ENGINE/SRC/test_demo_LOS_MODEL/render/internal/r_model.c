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
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenModel Model
 * @{
 */

#include"internal_render.h"
//#include"r_material_utils.h"
#include"r_mesh_builder.h"
#include"r_model.h"
#include"r_object.h"
//#include <OgreMeshManager.h>
//#include <OgreSubMesh.h>

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

/*
LIRenModel::LIRenModel(LIRenRender*      render,
					   const LIMdlModel* model,
					   int               id,
	bool              editable) : editable(editable)
{
	this->id = id;
	this->render = render;

	/* Choose a unique ID. *
	while (!this->id)
	{
		this->id = lialg_random_range (&render->random, 0x00000000, 0x7FFFFFFF);
		if (lialg_u32dic_find (render->objects, this->id))
			this->id = 0;
	}

	/* Load the model. *
	if (model != NULL)
		create_mesh (model);

	/* Add to the dictionary. *
	lialg_u32dic_insert (render->models, this->id, this);
}*/

LIRenModel* new_LIRenModel(LIRenRender* render, 
							const void* model, 
							int 		id, 
							int 		editable) 
{
    LIRenModel* self = (LIRenModel*)malloc(sizeof(LIRenModel));
    if (!self) 
    {
        return NULL;  // Falha ao alocar memória
    }

    self->id = id;
    self->render = render;
    self->editable = editable;

    /* Choose a unique ID. */
    while(!self->id) 
    {
        self->id = lialg_random_range(&render->random, 0x00000000, 0x7FFFFFFF);
        if (lialg_u32dic_find(render->objects, self->id)) 
        {
            self->id = 0;
        }
    }

    /* Load the model. */
    if (model != NULL) 
    {
        //create_mesh(model);
        LIRenModel_create_mesh(self, model);
    }

    /* Add to the dictionary. */
    lialg_u32dic_insert(render->models, self->id, self);

    return self;  /* return created model */
}


/**
 * \brief Frees the model.
 */

/*
LIRenModel::~LIRenModel ()
{
	/* Remove from objects. *
	/* Keeping the model alive when it's assigned to objects is the job of scripts.
	   If they don't reference the model, we'll remove it even if it's in use. We
	   prevent crashing by removing it from objects in such a case. *
	LIAlgU32dicIter iter;
	LIALG_U32DIC_FOREACH (iter, render->objects)
	{
		LIRenObject* object = (LIRenObject*) iter.value;
		object->remove_model (this);
	}

	/* Remove from the model dictionary. *
	lialg_u32dic_remove (render->models, id);
}*/

void destroy_LIRenModel(LIRenModel*  self, 
						LIRenRender* render) 
{
    if (self == NULL) return;

    /* Remover dos objetos */
    // Aqui você removeria a referência do modelo de todos os objetos que o utilizam
    // Isso pode envolver uma iteração pelos objetos como no código C++ original

    /* Remover do dicionário */
    lialg_u32dic_remove(render->models, self->id);

    /* Liberar a memória do objeto */
    free(self);
}

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

	if (!editable)
		return NULL;
	builder = (LIRenMeshBuilder*) lialg_strdic_find (render->mesh_builders, mesh->getName ().c_str ());
	if (!builder)
		return NULL;
	return builder->get_model ();
}*/

/**
 * \brief Gets the ID of the model.
 * \return ID.
 */
int get_id(LIRenModel* model)
{
	return model->id;;
}

/**
 * \brief Returns nonzero if the model has been fully background loaded.
 * \return Nonzero if background loaded, zero if not.
 */
/*
int LIRenModel::get_loaded() const
{
	if (mesh.isNull ())
		return 0;
	return mesh->isLoaded ();
}*/

/**
 * \brief Sets the current LIMdlModel.
 * \param model Model.
 */
/*
int LIRenModel::set_model(const LIMdlModel* model)
{
	create_mesh(model);

	return 1;
}*/

/*****************************************************************************/

/*
void LIRenModel::create_mesh(const LIMdlModel* model)
{
	/* Create the resource loader. *
	/* Ogre doesn't free the manual resource loader, nor does it allow us to
	   store custom data to meshes. Because of those reasons, we store the
	   loader to a dictionary that is searched by mesh name. The render class
	   will also use the dictionary to garbage collect unused loaders. *
	LIRenMeshBuilder* builder = new LIRenMeshBuilder (render, model, editable);
	Ogre::String name = render->id.next ();
	lialg_strdic_insert (render->mesh_builders, name.c_str (), builder);

	/* Create the manual mesh. *
	Ogre::String group = LIREN_RESOURCES_TEMPORARY;
	mesh = Ogre::MeshManager::getSingleton ().createManual (name, group, builder);

	/* Prevent edge list building. *
	/* Building edge lists is hideously slow. Since we don't use shadow
	   volumes, it's also completely useless for us. *
	mesh->setAutoBuildEdgeLists (false);

	/* Tell objects using the old mesh to rebuild. *
	LIAlgU32dicIter iter;
	LIALG_U32DIC_FOREACH (iter, render->objects)
	{
		LIRenObject* object = (LIRenObject*) iter.value;
		object->model_changed (this);
	}
}*/

void LIRenModel_create_mesh(LIRenModel* 	  self, 
							const LIMdlModel* model) //  implement this
{
	LIAlgU32dicIter iter;
	
    // Criação de um "builder" para construir a malha
    LIRenMeshBuilder* builder = LIRenMeshBuilder_create(self->render, model, self->editable);
    
    // Gerar um nome único para a malha (pode ser um ID ou hash gerado)
    const char* mesh_name = lialg_generate_unique_name(self->render);
    lialg_strdic_insert(self->render->mesh_builders, mesh_name, builder);
    
    // Criar buffers de vértices e índices a partir dos dados do modelo
    Mesh* mesh = mesh_create_from_model(model, self->editable);

    // Aqui criamos os buffers de vértices e índices no OpenGL ou Vulkan
    if(mesh != NULL) 
    {
        // Carregar os dados da malha (vértices, índices) no buffer da API gráfica
        mesh_load_data(mesh);
        
        // Guardar a malha no LIRenModel
        self->mesh = mesh;
    } 
    else 
    {
        printf("Erro ao criar a malha do modelo\n");
    }

    // Notificar objetos para reconstruir a malha
    LIALG_U32DIC_FOREACH(iter, self->render->objects) 
    {
        LIRenObject* object = (LIRenObject*) iter.value;
        object->model_changed(self);
    }
}

/** @} */
/** @} */
/** @} */
