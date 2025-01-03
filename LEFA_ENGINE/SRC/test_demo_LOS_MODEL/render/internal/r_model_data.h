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

#ifndef __RENDER_MODEL_DATA_H__
#define __RENDER_MODEL_DATA_H__

//#include "lipsofsuna/system.h"
//#include "lipsofsuna/model.h"
#include"test_demo/model_test/ANOTHER/allsystem.h"
#include"test_demo/model_test/all_model.h"
//#include <OgreString.h>
//#include <OgreResource.h>


typedef struct _LIRenModelData LIRenModelData;
struct _LIRenModelData
{
	bool   				clear_buffers;
	size_t 				vertex_count;
	size_t 				index_count;
	float* 				buffer_data_0;
	float* 				buffer_data_1;
	float*  			buffer_data_2;
	uint16_t* 			index_data;
	LIMatAabb 			bounds;
	LIMdlPoseBuffer* 	rest_pose_buffer;
	LIMdlPoseSkeleton* 	rest_pose_skeleton;
	//std::vector<LIRenModelDataMaterial> materials;
};

typedef struct _LIRenModelDataMaterial LIRenModelDataMaterial;
struct _LIRenModelDataMaterial
{
	int 		  start;
	int 		  count;
	LIMdlMaterial material;
};


/*
class LIRenModelDataMaterial
{
public:
	LIRenModelDataMaterial () : start(0), count(0)
	{
		limdl_material_init (&material);
	}
	LIRenModelDataMaterial (const LIRenModelDataMaterial& m) : start(m.start), count(m.count)
	{
		limdl_material_init_copy (&material, &m.material);
	}
	~LIRenModelDataMaterial ()
	{
		limdl_material_free (&material);
	}
public:
	int start;
	int count;
	LIMdlMaterial material;
};

class LIRenModelData
{
public:
	LIRenModelData (const LIMdlModel* model);
	~LIRenModelData ();
	void clear_buffer_data ();
public:
	bool clear_buffers;
	size_t vertex_count;
	size_t index_count;
	float* buffer_data_0;
	float* buffer_data_1;
	float* buffer_data_2;
	uint16_t* index_data;
	LIMatAabb bounds;
	LIMdlPoseBuffer* rest_pose_buffer;
	LIMdlPoseSkeleton* rest_pose_skeleton;
	std::vector<LIRenModelDataMaterial> materials;
};
*/
#endif
