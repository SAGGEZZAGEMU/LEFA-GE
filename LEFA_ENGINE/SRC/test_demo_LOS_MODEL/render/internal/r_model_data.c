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
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenModelData ModelData
 * @{
 */

#include "lipsofsuna/system.h"
#include "render.hpp"
#include "render-model-data.hpp"

LIRenModelData::LIRenModelData (const LIMdlModel* model)
{
	bounds = model->bounds;

	/* Prepare the index buffer data. */
	vertex_count = model->vertices.count;
	if (model->lod.count)
	{
		const LIMdlLod* lod = model->lod.array;
		index_count = lod->indices.count;
		index_data = index_count? limdl_model_get_buffer_idx (model, 0) : NULL;
	}
	else
	{
		index_count = 0;
		index_data = NULL;
	}

	/* Prepare the vertex buffer data. */
	if (vertex_count && index_count)
	{
		buffer_data_0 = limdl_model_get_buffer_vtx_nml (model);
		buffer_data_1 = limdl_model_get_buffer_tan_tex_col (model);
		buffer_data_2 = limdl_model_get_buffer_bon_wgt (model);
	}
	else
	{
		vertex_count = 0;
		buffer_data_0 = NULL;
		buffer_data_1 = NULL;
		buffer_data_2 = NULL;
	}

	/* Copy the material data. */
	if (vertex_count)
	{
		const LIMdlLod* lod = model->lod.array;
		for (int i = 0, j = 0 ; i < model->materials.count ; i++)
		{
			if (lod->face_groups.array[i].count)
			{
				materials.push_back (LIRenModelDataMaterial ());
				LIRenModelDataMaterial& data = materials[j++];
				data.start = lod->face_groups.array[i].start;
				data.count = lod->face_groups.array[i].count;
				limdl_material_init_copy (&data.material, model->materials.array + i);
			}
		}
	}

	/* Initialize the rest pose. */
	rest_pose_buffer = limdl_pose_buffer_new (model);
	rest_pose_skeleton = limdl_pose_skeleton_new (&model, 1);
}

LIRenModelData::~LIRenModelData ()
{
	limdl_pose_buffer_free (rest_pose_buffer);
	limdl_pose_skeleton_free (rest_pose_skeleton);
	lisys_free (buffer_data_0);
	lisys_free (buffer_data_1);
	lisys_free (buffer_data_2);
	lisys_free (index_data);
}

void LIRenModelData::clear_buffer_data ()
{
	lisys_free (buffer_data_0);
	lisys_free (buffer_data_1);
	lisys_free (buffer_data_2);
	lisys_free (index_data);
	buffer_data_0 = NULL;
	buffer_data_1 = NULL;
	buffer_data_2 = NULL;
	index_data = NULL;
}

/** @} */
/** @} */
/** @} */
