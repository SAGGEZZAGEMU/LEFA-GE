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

#ifndef __RENDER_MESH_BUILDER_H__
#define __RENDER_MESH_BUILDER_H__

//#include "lipsofsuna/system.h"
//#include "lipsofsuna/model.h"
#include"test_demo/model_test/ANOTHER/allsystem.h"
#include"test_demo/model_test/all_model.h"
#include"r_model_data.h"

//#include <OgreResource.h>
//#include <OgreHardwareBufferManager.h>


typedef struct _LIRenMeshBuilder LIRenMeshBuilder;
struct _LIRenMeshBuilder
{
	bool 			editable;
	LIRenRender*   	render;
	LIRenModelData  data;
	/* Prepared data. */
	int 			step;
	size_t 			buffer_size_0;
	size_t 			buffer_size_1;
	size_t 			buffer_size_2;
	//Ogre::VertexData* vertex_data;
	//Ogre::VertexDeclaration vertex_declaration;
	/* Loaded data */
	//Ogre::VertexBufferBinding* vertex_buffer_binding;
	//Ogre::HardwareVertexBufferSharedPtr vertex_buffer_0;
	//Ogre::HardwareVertexBufferSharedPtr vertex_buffer_1;
	//Ogre::HardwareVertexBufferSharedPtr vertex_buffer_2;
	//Ogre::HardwareIndexBufferSharedPtr index_buffer;
	/* Referenced data. */
	/* To prevent temporary materials from being removed during the
	   period of the mesh being initialized but not used, we need to
	   keep references to the materials. */
	//std::vector<Ogre::MaterialPtr> materials;

};

/*
class LIRenMeshBuilder : public Ogre::ManualResourceLoader
{
public:
	LIRenMeshBuilder (LIRenRender* render, const LIMdlModel* model, bool editable);
	virtual ~LIRenMeshBuilder ();
	LIRenModelData* get_model () { return &data; }
private:
	virtual void loadResource (Ogre::Resource* resource);
	virtual void prepareResource (Ogre::Resource* resource);
public:
	bool is_idle () const;
	void step_1_bg (Ogre::Mesh* mesh);
	void step_2_fg (Ogre::Mesh* mesh);
	void step_3_fg (Ogre::Mesh* mesh);
	void step_4_fg (Ogre::Mesh* mesh);
private:
	bool create_skeleton (Ogre::Mesh* mesh);
	void update_entities ();
private:
	/* Static data. *
	bool editable;
	LIRenRender* render;
	LIRenModelData data;
	/* Prepared data. *
	int step;
	size_t buffer_size_0;
	size_t buffer_size_1;
	size_t buffer_size_2;
	Ogre::VertexData* vertex_data;
	Ogre::VertexDeclaration vertex_declaration;
	/* Loaded data *
	Ogre::VertexBufferBinding* vertex_buffer_binding;
	Ogre::HardwareVertexBufferSharedPtr vertex_buffer_0;
	Ogre::HardwareVertexBufferSharedPtr vertex_buffer_1;
	Ogre::HardwareVertexBufferSharedPtr vertex_buffer_2;
	Ogre::HardwareIndexBufferSharedPtr index_buffer;
	/* Referenced data. *
	/* To prevent temporary materials from being removed during the
	   period of the mesh being initialized but not used, we need to
	   keep references to the materials. *
	std::vector<Ogre::MaterialPtr> materials;
};
*/

#endif
