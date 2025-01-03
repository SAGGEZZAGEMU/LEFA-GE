#ifndef __RENDER_INTERNAL_MODEL_H__
#define __RENDER_INTERNAL_MODEL_H__

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

#include<stdbool.h>


//#include"lipsofsuna/model.h"
#include"test_demo/model_test/all_model.h"

//#include"lipsofsuna/system.h"
#include"test_demo/model_test/ANOTHER/allsystem.h"

//#include <OgreMesh.h>

//class LIRenModelData;


typedef struct _LIRenModel LIRenModel;
struct _LIRenModel 
{

	int id;
	bool editable;
	LIRenRender* render;

	//Ogre::MeshPtr mesh;

};

#include"internal_render.h"

/*
LIRenModel(LIRenRender*      render,
		   const LIMdlModel* model,
		   int               id,
		   bool              editable = false);

~LIRenModel();
*/

LIRenModel* new_LIRenModel(LIRenRender* render, 
							const void* model, 
							int 		id, 
							int 		editable);

void destroy_LIRenModel(LIRenModel*  self, 
						LIRenRender* render);

void replace_buffer_vtx_nml(const void* data);

void replace_texture(const char* name,
					 int         width,
					 int         height,
					 const void* pixels);

//LIRenModelData* get_editable();

int get_id(LIRenModel* model);

int get_loaded();

//int set_model(const LIMdlModel* model);

//void create_mesh(const LIMdlModel* model);

void LIRenModel_create_mesh(LIRenModel* 	  self, 
							const LIMdlModel* model);



#endif /* __RENDER_INTERNAL_MODEL_H__ */
