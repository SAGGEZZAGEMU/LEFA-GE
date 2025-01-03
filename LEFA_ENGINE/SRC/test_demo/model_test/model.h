#ifndef __MODEL_H__
#define __MODEL_H__

//#include"lipsofsuna/math.h"
//#include"lipsofsuna/system.h"
/*
*/

#include"ANOTHER/math_only.h"

/*#include"ANOTHER/allsystem.h"
*/

//#include"all_model.h"


#include"model_animation.h"
#include"model_bone.h"
#include"model_faces.h"
#include"model_hair.h"
#include"model_light.h"
#include"model_lod.h"
#include"model_manager.h"
#include"model_material.h"
#include"model_nodes.h"
#include"model_particle.h"
#include"model_partition.h"
#include"model_node.h"
#include"model_shape.h"
#include"model_shape_key.h"
#include"model_types.h"
#include"model_vertex.h"
#include"model_weight_group.h"


#define LIMDL_FORMAT_VERSION 0xFFFFFFF3

typedef struct _LIMdlModel LIMdlModel;

typedef int LIMdlModelFlags;

struct _LIMdlModel
{
	int id;
	int flags;
	LIMatAabb bounds;
	LIMdlNodes nodes;
	LIMdlManager* manager;

	struct 
	{ 
		int count; 
		LIMdlHairs* array; 
	} hairs;

	struct 
	{ 
		int count; 
		LIMdlLod* array; 
	} lod;

	struct 
	{ 
		int count; 
		LIMdlMaterial* array; 
	} materials;

	struct 
	{ 
		int count; 
		LIMdlParticleSystem* array;
	} particle_systems;
	
	struct 
	{ 
		int count; 
		LIMdlPartition* array; 
	} partitions;
	
	struct 
	{ 
		int count; 
		LIMdlShape* array; 
	} shapes;
	
	struct 
	{ 
		int count; 
		LIMdlShapeKey* array; 
	} shape_keys;
	
	struct 
	{ 
		int count; 
		LIMdlVertex* array; 
	} vertices;
	
	struct 
	{ 
		int count; 
		LIMdlWeightGroup* array; 
	} weight_groups;
};



LIMdlModel* limdl_model_new();

LIMdlModel* limdl_model_new_copy(const LIMdlModel* model, int shape_keys);

LIMdlModel* limdl_model_new_from_data(LIArcReader* reader, int mesh);

LIMdlModel* limdl_model_new_from_file(const char* path, int mesh);

void limdl_model_free(LIMdlModel* self);

int limdl_model_apply_partitions(LIMdlModel* self, 
								 const char** partition_array,
								 int partition_count);

void limdl_model_calculate_bounds (LIMdlModel* self);

void limdl_model_calculate_tangents (LIMdlModel* self);

void limdl_model_clear (LIMdlModel* self);

void limdl_model_clear_vertices (LIMdlModel* self);

int limdl_model_find_material (const LIMdlModel* self,
							   const LIMdlMaterial* material);

LIMdlMaterial* limdl_model_find_material_by_texture(LIMdlModel* self,
													const char* shader,
													const char* texture);

LIMdlNode* limdl_model_find_node(const LIMdlModel* self,
								 const char*       name);

LIMdlPartition* limdl_model_find_partition (const LIMdlModel* self,
											const char*       name);


LIMdlShapeKey* limdl_model_find_shape_key (LIMdlModel* self,
											  const char* name);


int limdl_model_find_vertex (LIMdlModel*        self,
								const LIMdlVertex* vertex);

 
int limdl_model_find_weightgroup(LIMdlModel* self,
					 			const char* name, 
					 			const char* bone);

int limdl_model_merge (LIMdlModel* self, const LIMdlModel* model);

int limdl_model_morph(LIMdlModel* self, 
					  LIMdlModel* key_model,
 					  LIMdlModel* ref_model, 
 					  const char* shape, 
 					  float value);

int limdl_model_replace(LIMdlModel* self, const LIMdlModel* model);

void limdl_model_replace_material_by_string(LIMdlModel*  self, 
											const char*  match_material, 
 											const float* set_diffuse, 
 											const float* set_specular, 
 											const char*  set_material, 
 											const char** set_textures, 
 											int set_textures_count);

void limdl_model_scale (LIMdlModel* self, float factor);

int limdl_model_write(const LIMdlModel* self, LIArcWriter* writer);

int limdl_model_write_file(const LIMdlModel* self, const char* path);

float* limdl_model_get_buffer_bon_wgt (const LIMdlModel* self);

uint16_t* limdl_model_get_buffer_idx (const LIMdlModel* self, int level);

float* limdl_model_get_buffer_tan_tex_col (const LIMdlModel* self);

float* limdl_model_get_buffer_vtx_nml (const LIMdlModel* self);

int limdl_model_get_memory (const LIMdlModel* self);

#endif /*__MODEL_H__ */

