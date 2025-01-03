
#include"model.h"
#include"model_builder.h"

#include<stdint.h>
#include<stdbool.h>


typedef int (*LIMdlWriteFunc)(const LIMdlModel*, LIArcWriter*);

void private_build(LIMdlModel* self);

void private_build_tangents(LIMdlModel* self);

void private_clear(LIMdlModel* self);

int private_read(LIMdlModel*  self, LIArcReader* reader, int mesh);

static int private_read_bounds(LIMdlModel*  self, LIArcReader* reader);

static int private_read_faces(LIMdlModel*  self, LIArcReader* reader);

static int private_read_hairs(LIMdlModel*  self, LIArcReader* reader);

static int private_read_lod(LIMdlModel*  self,LIArcReader* reader);

static int private_read_materials(LIMdlModel*  self,LIArcReader* reader);

static int private_read_nodes(LIMdlModel*  self, LIArcReader* reader);

static int private_read_particles(LIMdlModel*  self, LIArcReader* reader);

static int private_read_partitions(LIMdlModel*  self, LIArcReader* reader);

static int private_read_shapes(LIMdlModel*  self, LIArcReader* reader);

static int private_read_shape_keys(LIMdlModel*  self, LIArcReader* reader);

static int private_read_vertices(LIMdlModel*  self, LIArcReader* reader);

static int private_read_weights(LIMdlModel*  self, LIArcReader* reader);

static int private_read_vertex_weights(LIMdlModel*  self, LIMdlVertex* vertex, LIArcReader* reader);

static int private_write(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_block(const LIMdlModel* self,
							   const char*       name,
							   LIMdlWriteFunc    func,
							   LIArcWriter*      writer);

static int private_write_bounds(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_hairs(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_header(const LIMdlModel* self,LIArcWriter* writer);

static int private_write_lod(const LIMdlModel* self,LIArcWriter* writer);

static int private_write_materials(const LIMdlModel* self, LIArcWriter*  writer);

static int private_write_nodes(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_particles(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_partitions(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_shapes(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_shape_keys(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_vertices(const LIMdlModel* self, LIArcWriter* writer);

static int private_write_weights(const LIMdlModel* self, LIArcWriter* writer);

/*****************************************************************************/

LIMdlModel* limdl_model_new()
{
	LIMdlModel* self;

	/* Allocate self. */
	self = lisys_calloc(1, sizeof (LIMdlModel));

	if (self == NULL)
		return NULL;

	/* Allocate the mandatory LOD. */
	self->lod.count = 1;
	self->lod.array = lisys_calloc(1, sizeof (LIMdlLod));

	if (self->lod.array == NULL)
	{
		lisys_free (self);
		return NULL;
	}

	return self;
}

LIMdlModel* limdl_model_new_copy(const LIMdlModel* model, int shape_keys)
{
	int i;
	LIMdlModel* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlModel));
	if (self == NULL)
		return NULL;
	self->flags = model->flags;
	self->bounds = model->bounds;

	/* Copy everything. */
	/* FIXME: Not all are supported yet.  */
#if 0
	if (model->hairs.count)
	{
		self->hairs.array = lisys_calloc (model->hairs.count, sizeof (LIMdlHairs));
		self->hairs.count = model->hairs.count;
		for (i = 0 ; i < model->hairs.count ; i++)
			limdl_hairs_init_copy (self->hairs.array + i, model->hairs.array + i);
	}
#endif
	if (model->lod.count)
	{
		self->lod.array = lisys_calloc(model->lod.count, sizeof (LIMdlLod));
		self->lod.count = model->lod.count;
		for (i = 0 ; i < model->lod.count ; i++)
			limdl_lod_init_copy (self->lod.array + i, model->lod.array + i);
	}
	if (model->materials.count)
	{
		self->materials.array = lisys_calloc (model->materials.count, sizeof (LIMdlMaterial));
		self->materials.count = model->materials.count;
		for (i = 0 ; i < model->materials.count ; i++)
			limdl_material_init_copy (self->materials.array + i, model->materials.array + i);
	}
	if (model->nodes.count)
	{
		self->nodes.array = lisys_calloc (model->nodes.count, sizeof (LIMdlNode*));
		self->nodes.count = model->nodes.count;
		for (i = 0 ; i < model->nodes.count ; i++)
			self->nodes.array[i] = limdl_node_copy (model->nodes.array[i], 1);
	}
	if (model->particle_systems.count)
	{
		self->particle_systems.array = lisys_calloc (model->particle_systems.count, sizeof (LIMdlParticleSystem));
		self->particle_systems.count = model->particle_systems.count;
		for (i = 0 ; i < model->particle_systems.count ; i++)
			limdl_particle_system_init_copy (self->particle_systems.array + i, model->particle_systems.array + i);
	}
	if (model->partitions.count)
	{
		self->partitions.array = lisys_calloc (model->partitions.count, sizeof (LIMdlPartition));
		self->partitions.count = model->partitions.count;
		for (i = 0 ; i < model->partitions.count ; i++)
			limdl_partition_init_copy (self->partitions.array + i, model->partitions.array + i);
	}
	if (model->shapes.count)
	{
		self->shapes.array = lisys_calloc (model->shapes.count, sizeof (LIMdlShape));
		self->shapes.count = model->shapes.count;
		for (i = 0 ; i < model->shapes.count ; i++)
			limdl_shape_init_copy (self->shapes.array + i, model->shapes.array + i);
	}
	if (shape_keys && model->shape_keys.count)
	{
		self->shape_keys.array = lisys_calloc (model->shape_keys.count, sizeof (LIMdlShapeKey));
		self->shape_keys.count = model->shape_keys.count;
		for (i = 0 ; i < model->shape_keys.count ; i++)
			limdl_shape_key_init_copy (self->shape_keys.array + i, model->shape_keys.array + i);
	}
	if (model->vertices.count)
	{
		self->vertices.array = lisys_calloc (model->vertices.count, sizeof (LIMdlVertex));
		self->vertices.count = model->vertices.count;
		for (i = 0 ; i < model->vertices.count ; i++)
			limdl_vertex_init_copy (self->vertices.array + i, model->vertices.array + i);
	}
	if (model->weight_groups.count)
	{
		self->weight_groups.array = lisys_calloc (model->weight_groups.count, sizeof (LIMdlWeightGroup));
		self->weight_groups.count = model->weight_groups.count;
		for (i = 0 ; i < model->weight_groups.count ; i++)
			limdl_weight_group_init_copy (self->weight_groups.array + i, model->weight_groups.array + i);
	}

	return self;
}

/**
 * \brief Loads a model from uncompressed data.
 * \param reader A reader.
 * \param mesh Zero to only load collision shapes.
 * \return A new model or NULL.
 */
LIMdlModel* limdl_model_new_from_data(LIArcReader* reader, int mesh)
{
	LIMdlModel* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlModel));
	if (self == NULL)
	{
		lisys_error_append ("cannot load model");
		return NULL;
	}

	/* Read from stream. */
	if (!private_read (self, reader, mesh))
	{
		lisys_error_append ("cannot load model");
		limdl_model_free (self);
		return NULL;
	}

	/* Construct the rest pose. */
	private_build (self);
	return self;
}

/**
 * \brief Loads a model from a file.
 * \param path The path to the file.
 * \param mesh Zero to only load collision shapes.
 * \return A new model or NULL.
 */
LIMdlModel* limdl_model_new_from_file(const char* path, int mesh)
{
	LIArcReader* reader;
	LIMdlModel* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMdlModel));
	if (self == NULL)
	{
		lisys_error_append ("cannot load model `%s'", path);
		return NULL;
	}

	/* Open the file. */
	reader = liarc_reader_new_from_file (path);
	if (reader == NULL)
		goto error;

	/* Read from stream. */
	if (!private_read (self, reader, mesh))
		goto error;
	liarc_reader_free (reader);

	/* Construct the rest pose. */
	private_build (self);

	return self;

error:
	lisys_error_append ("cannot load model `%s'", path);
	limdl_model_free (self);
	if (reader != NULL)
		liarc_reader_free (reader);
	return NULL;
}

/**
 * \brief Frees the model.
 * \param self Model.
 */
void limdl_model_free (LIMdlModel* self)
{
	lisys_assert (self->manager == NULL);

	private_clear (self);
	lisys_free (self);
}

/**
 * \brief Removes faces that don't belong to the given partitions.
 * \param self Model.
 * \param partition_array Array of partition names.
 * \param partition_count Number of partitions.
 * \return One on success. Zero otherwise.
 */
int limdl_model_apply_partitions (
	LIMdlModel*  self,
	const char** partition_array,
	int          partition_count)
{
	int i;
	int j;
	int k;
	int src;
	int dst;
	int* mask;
	LIMdlFaces* faces;
	LIMdlLod* lod;
	LIMdlPartition* partition;

	/* Allocate the vertex mask. */
	mask = lisys_calloc (self->vertices.count, sizeof (int));
	if (mask == NULL)
		return 0;

	/* Find the vertices belonging to the partitions. */
	for (i = 0 ; i < partition_count ; i++)
	{
		for (j = 0 ; j < self->partitions.count ; j++)
		{
			partition = self->partitions.array + j;
			if (!strcmp (partition_array[i], partition->name))
			{
				for (k = 0 ; k < partition->vertices.count ; k++)
					mask[partition->vertices.array[k].index] = 1;
			}
		}
	}

	/* Remove faces with vertices that are not in the partitions. */
	for (i = 0 ; i < self->lod.count ; i++)
	{
		lod = self->lod.array + i;
		for (j = lod->face_groups.count - 1 ; j >= 0 ; j--)
		{
			faces = lod->face_groups.array + j;
			for (src = dst = faces->start ; src < faces->start + faces->count ; src += 3)
			{
				if (mask[lod->indices.array[src]] && mask[lod->indices.array[src + 1]] && mask[lod->indices.array[src + 2]])
				{
					lod->indices.array[dst] = lod->indices.array[src];
					lod->indices.array[dst + 1] = lod->indices.array[src + 1];
					lod->indices.array[dst + 2] = lod->indices.array[src + 2];
					dst += 3;
				}
			}
			if (src != dst)
			{
				memmove (lod->indices.array + dst, lod->indices.array + src, lod->indices.count - src);
				faces->count = dst;
				lod->indices.count -= src - dst;
			}
		}
	}

	/* FIXME: Partitions now contain invalid indices. */

	lisys_free (mask);

	return 1;
}

/**
 * \brief Recalculates the bounding box of the model.
 *
 * Loops through all vertices of the model and calculates the minimum and
 * maximum axis values used.
 *
 * \param self Model.
 */
void limdl_model_calculate_bounds (
	LIMdlModel* self)
{
	int j;
	vec3 v;

	self->bounds.min = limat_vector_init (2.0E10, 2.0E10, 2.0E10);
	self->bounds.max = limat_vector_init (-2.0E10, -2.0E10, -2.0E10);
	for (j = 0 ; j < self->vertices.count ; j++)
	{
		v = self->vertices.array[j].coord;
		if (self->bounds.min.x > v.x)
			self->bounds.min.x = v.x;
		if (self->bounds.min.y > v.y)
			self->bounds.min.y = v.y;
		if (self->bounds.min.z > v.z)
			self->bounds.min.z = v.z;
		if (self->bounds.max.x < v.x)
			self->bounds.max.x = v.x;
		if (self->bounds.max.y < v.y)
			self->bounds.max.y = v.y;
		if (self->bounds.max.z < v.z)
			self->bounds.max.z = v.z;
	}
	if (self->bounds.min.x > self->bounds.max.x)
	{
		self->bounds.min = limat_vector_init (0.0f, 0.0f, 0.0f);
		self->bounds.max = limat_vector_init (0.0f, 0.0f, 0.0f);
	}
}

/**
 * \brief Calculates the vertex tangents.
 * \param self Model.
 */
void limdl_model_calculate_tangents (LIMdlModel* self)
{
	private_build_tangents (self);
}

/**
 * \brief Clears all data of the model.
 * \param self Model.
 */
void limdl_model_clear (LIMdlModel* self)
{
	int id;
	LIMdlManager* manager;

	/* Clear all data. */
	private_clear (self);
	id = self->id;
	manager = self->manager;
	memset (self, 0, sizeof (LIMdlModel));
	self->id = id;
	self->manager = manager;

	/* Allocate the mandatory LOD. */
	self->lod.count = 1;
	self->lod.array = lisys_calloc (1, sizeof (LIMdlLod));
}

/**
 * \brief Clears the vertex and face data of the model.
 * \param self Model.
 */
void limdl_model_clear_vertices (LIMdlModel* self)
{
	int i;
	int j;
	LIMdlLod* lod;
	LIMdlFaces* group;

	/* Free vertices. */
	lisys_free (self->vertices.array);
	self->vertices.array = NULL;
	self->vertices.count = 0;

	/* Free levels of detail. */
	for (j = 0 ; j < self->lod.count ; j++)
	{
		lod = self->lod.array + j;
		for (i = 0 ; i < lod->face_groups.count ; i++)
		{
			group = lod->face_groups.array + i;
			group->start = 0;
			group->count = 0;
		}
		if (j)
			lisys_free (lod->face_groups.array);
		lisys_free (lod->indices.array);
		lod->indices.array = NULL;
		lod->indices.count = 0;
	}
	self->lod.count = 1;
}

/**
 * \brief Finds the material index of the material.
 *
 * Finds the first material that matches the passed material.
 * The materials are considered to match if they have the same shader,
 * shininess, diffuse color, specular color, and texture images.
 *
 * \param self Model.
 * \param material Material.
 * \return Material index or -1.
 */
int limdl_model_find_material (const LIMdlModel*    self,
							   const LIMdlMaterial* material)
{
	int i;

	for (i = 0 ; i < self->materials.count ; i++)
	{
		if (limdl_material_compare (self->materials.array + i, material))
			return i;
	}

	return -1;
}

/**
 * \brief Finds a material by shader and/or texture.
 * \param self Model.
 * \param shader Shader name or NULL.
 * \param texture Texture name or NULL.
 * \return Material or NULL.
 */
LIMdlMaterial* limdl_model_find_material_by_texture (
	LIMdlModel* self,
	const char* shader,
	const char* texture)
{
	int i;
	LIMdlMaterial* material;

	for (i = 0 ; i < self->materials.count ; i++)
	{
		material = self->materials.array + i;
		if (limdl_material_compare_shader_and_texture (material, NULL, shader, texture))
			return material;
	}

	return NULL;
}

/**
 * \brief Finds a node by name.
 * \param self Model.
 * \param name Name of the node to find.
 * \return Node or NULL.
 */
LIMdlNode* limdl_model_find_node (
	const LIMdlModel* self,
	const char*       name)
{
	return limdl_nodes_find_node (&self->nodes, name);
}

/**
 * \brief Finds a partition by name.
 * \param self Model.
 * \param name Name of the partition to find.
 * \return Partition or NULL.
 */
LIMdlPartition* limdl_model_find_partition (
	const LIMdlModel* self,
	const char*       name)
{
	int i;

	for (i = 0 ; i < self->partitions.count ; i++)
	{
		if (!strcmp (self->partitions.array[i].name, name))
			return self->partitions.array + i;
	}

	return NULL;
}

/**
 * \brief Finds a shape key by name.
 * \param self Model.
 * \param name Shape key name.
 * \return Shape key or NULL.
 */
LIMdlShapeKey* limdl_model_find_shape_key (
	LIMdlModel* self,
	const char* name)
{
	int i;

	for (i = 0 ; i < self->shape_keys.count ; i++)
	{
		if (!strcmp (self->shape_keys.array[i].name, name))
			return self->shape_keys.array + i;
	}

	return NULL;
}

/**
 * \brief Finds the index of a matching vertex.
 * \param self Model.
 * \param vertex Vertex.
 * \return Index in vertex array or -1 if not found.
 */
int limdl_model_find_vertex (
	LIMdlModel*        self,
	const LIMdlVertex* vertex)
{
	int i;

	for (i = 0 ; i < self->vertices.count ; i++)
	{
		if (limdl_vertex_compare (self->vertices.array + i, vertex) == 0)
			return i;
	}

	return -1;
}

/**
 * \brief Finds a matching weight group.
 * \param self Model.
 * \param name Group name.
 * \param bone Bone name.
 * \return Index in weight group array or -1 if not found.
 */
int limdl_model_find_weightgroup (
	LIMdlModel* self,
	const char* name,
	const char* bone)
{
	int i;
	LIMdlWeightGroup* group;

	for (i = 0 ; i < self->weight_groups.count ; i++)
	{
		group = self->weight_groups.array + i;
		if (!strcmp (group->name, name) &&
		    !strcmp (group->bone, bone))
			return i;
	}

	return -1;
}

/**
 * \brief Merges the model to this one.
 * \param self Model.
 * \param model Model.
 * \return Nonzero on success.
 */
int limdl_model_merge (
	LIMdlModel*       self,
	const LIMdlModel* model)
{
	LIMdlBuilder* builder;

	/* Create a model builder. */
	builder = limdl_builder_new (self);
	if (builder == NULL)
		return 0;

	/* Merge the models using the builder. */
	if (!limdl_builder_merge_model (builder, model, NULL))
	{
		limdl_builder_free (builder);
		return 0;
	}

	/* FIXME: Recalculates the bounding box from scratch even though could just calculate the intersection. */
	limdl_builder_finish (builder, 0);
	limdl_builder_free (builder);

	return 1;
}

/**
 * \brief Morphs the vertices of the model and one of its shape keys.
 * \param self Model.
 * \param key_model Model containing the shape keys.
 * \param ref_model Reference model for relative morphing.
 * \param shape Shape key name.
 * \param value Shape key influence.
 * \return Nonzero on success.
 */
int limdl_model_morph (
	LIMdlModel* self,
	LIMdlModel* key_model,
	LIMdlModel* ref_model,
	const char* shape,
	float       value)
{
	int i;
	quat q0;
	quat q1;
	LIMdlShapeKey* key;
	LIMdlShapeKeyVertex* k;
	LIMdlVertex* r;
	LIMdlVertex* v;

	lisys_assert (self->vertices.count == key_model->vertices.count);

	/* Find the shape key. */
	key = limdl_model_find_shape_key (key_model, shape);
	if (key == NULL)
		return 0;

	if (ref_model != NULL && ref_model->vertices.count == self->vertices.count)
	{
		/* Relative morphing. */
		for (i = 0 ; i < key->vertices.count && i < self->vertices.count ; i++)
		{
			k = key->vertices.array + i;
			r = ref_model->vertices.array + i;
			v = self->vertices.array + i;
			v->coord = limat_vector_add (v->coord, limat_vector_multiply (
				limat_vector_subtract (k->coord, r->coord), value));

			/* Normals need spherical interpolation because a linear sum gives
			   ugly results in detailed areas such as character faces. */
			q0 = limat_quaternion_identity ();
			q1 = limat_quaternion_init_vectors (v->normal, k->normal);
			q1 = limat_quaternion_nlerp (q1, q0, value);
			v->normal = limat_quaternion_transform (q1, v->normal);
		}
	}
	else
	{
		/* Absolute morphing. */
		for (i = 0 ; i < key->vertices.count && i < self->vertices.count ; i++)
		{
			k = key->vertices.array + i;
			v = self->vertices.array + i;
			v->coord = limat_vector_lerp (k->coord, v->coord, value);
			v->normal = limat_vector_normalize (limat_vector_lerp (
				k->normal, v->normal, value));
		}
	}

	return 1;
}

/**
 * \brief Replaces the contents of the model with another.
 *
 * The ID and the model manager pointer are not replaced, but all the
 * model data is. This will invalidate any pointers to the model data.
 *
 * \param self Model.
 * \param model Model.
 * \return Nonzero on success.
 */
int limdl_model_replace (
	LIMdlModel*       self,
	const LIMdlModel* model)
{
	int id;
	LIMdlManager* manager;
	LIMdlModel* copy;

	/* Create a copy of the passed model. */
	copy = limdl_model_new_copy (model, 1);
	if (copy == NULL)
		return 0;

	/* Hijack the data of the copy and free the old data. */
	id = self->id;
	manager = self->manager;
	*self = *copy;
	self->id = id;
	self->manager = manager;

	/* Free only the copy pointer but not its data. */
	lisys_free (copy);

	return 1;
}

/**
 * \brief Replaces contents of all materials whose material string matches the search.
 * \param self Model.
 * \param match_material Material string to match.
 * \param set_diffuse Diffuse color, or NULL to not alter.
 * \param set_specular Specular color, or NULL to not alter.
 * \param set_material Material string, or NULL to not alter.
 * \param set_textures Texture list, or NULL to not alter.
 * \param set_textures_count Texture count in the list.
 */
void limdl_model_replace_material_by_string (
	LIMdlModel*  self,
	const char*  match_material,
	const float* set_diffuse,
	const float* set_specular,
	const char*  set_material,
	const char** set_textures,
	int          set_textures_count)
{
	int i;
	int j;
	LIMdlMaterial* material;

	/* Edit each matching material. */
	for (i = 0 ; i < self->materials.count ; i++)
	{
		material = self->materials.array + i;
		if (material->material == NULL)
			continue;
		if (strcmp (material->material, match_material))
			continue;
		if (set_diffuse != NULL)
			limdl_material_set_diffuse (material, set_diffuse);
		if (set_specular != NULL)
			limdl_material_set_specular (material, set_specular);
		if (set_material != NULL)
			limdl_material_set_material (material, set_material);
		if (set_textures != NULL)
		{
			if (material->textures.count != set_textures_count)
			{
				if (!limdl_material_realloc_textures (material, set_textures_count))
					continue;
			}
			for (j = 0 ; j < set_textures_count ; j++)
			{
				limdl_material_set_texture (material, j, LIMDL_TEXTURE_TYPE_IMAGE,
					LIMDL_TEXTURE_FLAG_BILINEAR | LIMDL_TEXTURE_FLAG_MIPMAP |
					LIMDL_TEXTURE_FLAG_REPEAT, set_textures[j]);
			}
		}
	}
}

/**
 * \brief Scales the model.
 * \param self Model.
 * \param factor Scale factor.
 */
void limdl_model_scale (
	LIMdlModel* self,
	float       factor)
{
	int i;
	int j;
	LIMdlVertex* v;
	LIMdlShapeKey* s;
	LIMdlShapeKeyVertex* vs;

	for (i = 0 ; i < self->vertices.count ; i++)
	{
		v = self->vertices.array + i;
		v->coord.x *= factor;
		v->coord.y *= factor;
		v->coord.z *= factor;
	}
	for (j = 0 ; j < self->shape_keys.count ; j++)
	{
		s = self->shape_keys.array + j;
		for (i = 0 ; i < self->vertices.count ; i++)
		{
			vs = s->vertices.array + i;
			vs->coord.x *= factor;
			vs->coord.y *= factor;
			vs->coord.z *= factor;
		}
	}
}

/**
 * \brief Writes the model into a steam.
 * \param self Model.
 * \param writer Stream writer.
 * \return One on success. Zero otherwise.
 */
int limdl_model_write(const LIMdlModel* self, LIArcWriter* writer)
{
	return private_write (self, writer);
}

/**
 * \brief Writes the model into a file.
 * \param self Model.
 * \param path Path to the file.
 * \return One on success. Zero otherwise.
 */
int limdl_model_write_file(const LIMdlModel* self,
						   const char*       path)
{
	LIArcWriter* writer;

	/* Create a writer. */
	writer = liarc_writer_new_file (path);
	if (writer == NULL)
		return 0;

	/* Pack to the writer. */
	if (!limdl_model_write (self, writer))
	{
		liarc_writer_free (writer);
		return 0;
	}

	liarc_writer_free (writer);

	return 1;
}

/**
 * \brief Gets the vertex buffer data for tangents, textures and and colors.
 * \param self Model.
 * \param count Return location for the number of bytes. NULL to not return.
 * \return Vertex buffer data on success. NULL otherwise.
 */
float* limdl_model_get_buffer_bon_wgt(const LIMdlModel* self)
{
	int i;
	int j;
	float* data;
	LIMdlVertex* v;

	data = lisys_calloc (5 * self->vertices.count, sizeof (float));
	if (data == NULL)
		return NULL;
	for (i = j = 0 ; i < self->vertices.count ; i++)
	{
		v = self->vertices.array + i;
		memcpy (data + j, v->bones, LIMDL_VERTEX_WEIGHTS_MAX * sizeof (uint8_t));
		j++;
		memcpy (data + j, v->weights, LIMDL_VERTEX_WEIGHTS_MAX * sizeof (float));
		j += LIMDL_VERTEX_WEIGHTS_MAX;
	}

	return data;
}

/**
 * \brief Gets the index buffer data for the given LOD level.
 * \param self Model.
 * \return Vertex buffer data on success. NULL otherwise.
 */
uint16_t* limdl_model_get_buffer_idx(const LIMdlModel* self,
									 int               level)
{
#ifndef _NDEBUG
	int i;
#endif
	uint16_t* data;
	const LIMdlLod* lod;

	/* Get the LOD. */
	lisys_assert (level < self->lod.count);
	lod = self->lod.array + level;

	/* Validate the indices. */
#ifndef _NDEBUG
	lisys_assert (lod->indices.count % 3 == 0);
	for (i = 0 ; i < lod->indices.count ; i++)
		lisys_assert (lod->indices.array[i] < self->vertices.count);
#endif

	/* Clone the indices. */
	data = lisys_calloc (lod->indices.count, sizeof (uint16_t));
	if (data == NULL)
		return NULL;
	memcpy (data, lod->indices.array, lod->indices.count * sizeof (uint16_t));

	return data;
}

/**
 * \brief Gets the vertex buffer data for tangents, textures and and colors.
 * \param self Model.
 * \return Vertex buffer data on success. NULL otherwise.
 */
float* limdl_model_get_buffer_tan_tex_col (const LIMdlModel* self)
{
	int i;
	int j;
	float* data;
	LIMdlVertex* v;
	uint8_t* color;

	data = lisys_calloc (6 * self->vertices.count, sizeof (float));
	if (data == NULL)
		return NULL;
	for (i = j = 0 ; i < self->vertices.count ; i++)
	{
		v = self->vertices.array + i;
		data[j++] = v->tangent.x;
		data[j++] = v->tangent.y;
		data[j++] = v->tangent.z;
		data[j++] = v->texcoord[0];
		data[j++] = v->texcoord[1];
		color = (uint8_t*)(data + j++);
		color[0] = v->color[0];
		color[1] = v->color[1];
		color[2] = v->color[2];
		color[3] = v->color[3];
	}

	return data;
}

/**
 * \brief Gets the vertex buffer data for coordinates and normals.
 * \param self Model.
 * \return Vertex buffer data on success. NULL otherwise.
 */
float* limdl_model_get_buffer_vtx_nml (
	const LIMdlModel* self)
{
	int i;
	int j;
	float* data;
	LIMdlVertex* v;

	data = lisys_calloc (6 * self->vertices.count, sizeof (float));
	if (data == NULL)
		return NULL;
	for (i = j = 0 ; i < self->vertices.count ; i++)
	{
		v = self->vertices.array + i;
		data[j++] = v->coord.x;
		data[j++] = v->coord.y;
		data[j++] = v->coord.z;
		data[j++] = v->normal.x;
		data[j++] = v->normal.y;
		data[j++] = v->normal.z;
	}

	return data;
}

/**
 * \brief Gets the approximate memory used by the model.
 * \param self Model.
 * \return Memory used, in bytes.
 */
int limdl_model_get_memory (
	const LIMdlModel* self)
{
	int i;
	int total;

	/* TODO: Many of these have memory allocations of their own, */
	total = sizeof (LIMdlModel);
	for (i = 0 ; i < self->hairs.count ; i++)
		total += sizeof (LIMdlHairs);
	for (i = 0 ; i < self->lod.count ; i++)
		total += limdl_lod_get_memory (self->lod.array + i);
	for (i = 0 ; i < self->materials.count ; i++)
		total += sizeof (LIMdlMaterial*) + sizeof (LIMdlMaterial);
	for (i = 0 ; i < self->nodes.count ; i++)
		total += sizeof (LIMdlNode*) + sizeof (LIMdlNode);
	for (i = 0 ; i < self->particle_systems.count ; i++)
		total += sizeof (LIMdlParticleSystem);
	for (i = 0 ; i < self->shapes.count ; i++)
		total += sizeof (LIMdlShape);
	for (i = 0 ; i < self->shape_keys.count ; i++)
	{
		total += sizeof (LIMdlShapeKey);
		total += strlen (self->shape_keys.array[i].name) + 1;
		total += self->shape_keys.array[i].vertices.count * sizeof (LIMdlShapeKeyVertex);
	}
	for (i = 0 ; i < self->vertices.count ; i++)
		total += sizeof (LIMdlVertex);
	for (i = 0 ; i < self->weight_groups.count ; i++)
		total += sizeof (LIMdlWeightGroup);

	return total;
}

/*****************************************************************************/

void private_build(LIMdlModel* self)
{
	int i;
	LIMdlNode* node;

	/* Transform each node. */
	for (i = 0 ; i < self->nodes.count ; i++)
	{
		node = self->nodes.array[i];
		limdl_node_rebuild (node, 1);
	}

	/* Calculate vertex tangents. */
	private_build_tangents (self);
}

void private_build_tangents (LIMdlModel* self)
{
	int i;
	int k;
	float sign;
	float uv0[2];
	float uv1[2];
	LIMdlIndex* idx;
	vec3 tmp;
	vec3 ed0;
	vec3 ed1;
	LIMdlLod* lod;
	LIMdlVertex* vtx[3];

	lisys_assert (self->lod.count);
	lod = self->lod.array;

	for (i = 0 ; i < self->vertices.count ; i++)
		self->vertices.array[i].tangent = limat_vector_init (0.0f, 0.0f, 0.0f);
	for (i = 0, idx = lod->indices.array ; i < lod->indices.count ; i += 3, idx += 3)
	{
		vtx[0] = self->vertices.array + idx[0];
		vtx[1] = self->vertices.array + idx[1];
		vtx[2] = self->vertices.array + idx[2];
		ed0 = limat_vector_subtract (vtx[1]->coord, vtx[0]->coord);
		ed1 = limat_vector_subtract (vtx[2]->coord, vtx[0]->coord);
		uv0[0] = vtx[1]->texcoord[0] - vtx[0]->texcoord[0];
		uv0[1] = vtx[1]->texcoord[1] - vtx[0]->texcoord[1];
		uv1[0] = vtx[2]->texcoord[0] - vtx[0]->texcoord[0];
		uv1[1] = vtx[2]->texcoord[1] - vtx[0]->texcoord[1];
		sign = uv0[0] * uv1[1] - uv1[0] * uv0[1];
		sign = (sign > 0.0f)? 1.0f : -1.0f;
		tmp = limat_vector_subtract (
			limat_vector_multiply (ed1, uv0[1]),
			limat_vector_multiply (ed0, uv1[1]));
		tmp = limat_vector_multiply (tmp, sign);
		tmp = limat_vector_normalize (tmp);
		for (k = 0 ; k < 3 ; k++)
			vtx[k]->tangent = limat_vector_add (vtx[k]->tangent, tmp);
	}
	for (i = 0 ; i < self->vertices.count ; i++)
		self->vertices.array[i].tangent = limat_vector_normalize (self->vertices.array[i].tangent);
}

void private_clear(LIMdlModel* self)
{
	int i;
	LIMdlMaterial* material;

	/* Free materials. */
	if (self->materials.array != NULL)
	{
		for (i = 0 ; i < self->materials.count ; i++)
		{
			material = self->materials.array + i;
			limdl_material_free (material);
		}
		lisys_free (self->materials.array);
	}

	/* Free levels of detail. */
	if (self->lod.array != NULL)
	{
		for (i = 0 ; i < self->lod.count ; i++)
			limdl_lod_free (self->lod.array + i);
		lisys_free (self->lod.array);
	}

	/* Free weight groups. */
	if (self->weight_groups.array != NULL)
	{
		for (i = 0 ; i < self->weight_groups.count ; i++)
		{
			lisys_free (self->weight_groups.array[i].name);
			lisys_free (self->weight_groups.array[i].bone);
		}
		lisys_free (self->weight_groups.array);
	}
	lisys_free (self->vertices.array);

	/* Free nodes. */
	if (self->nodes.array != NULL)
	{
		for (i = 0 ; i < self->nodes.count ; i++)
		{
			if (self->nodes.array[i] != NULL)
				limdl_node_free (self->nodes.array[i]);
		}
		lisys_free (self->nodes.array);
	}

	/* Free particles. */
	if (self->particle_systems.array != NULL)
	{
		for (i = 0 ; i < self->particle_systems.count ; i++)
			limdl_particle_system_clear (self->particle_systems.array + i);
		lisys_free (self->particle_systems.array);
	}
	if (self->hairs.array != NULL)
	{
		for (i = 0 ; i < self->hairs.count ; i++)
			limdl_hairs_free (self->hairs.array + i);
		lisys_free (self->hairs.array);
	}

	/* Free partitions. */
	if (self->partitions.array != NULL)
	{
		for (i = 0 ; i < self->partitions.count ; i++)
			limdl_partition_clear (self->partitions.array + i);
		lisys_free (self->partitions.array);
	}

	/* Free shapes. */
	if (self->shapes.array != NULL)
	{
		for (i = 0 ; i < self->shapes.count ; i++)
			limdl_shape_clear (self->shapes.array + i);
		lisys_free (self->shapes.array);
	}

	/* Free shape keys. */
	if (self->shape_keys.array != NULL)
	{
		for (i = 0 ; i < self->shape_keys.count ; i++)
			limdl_shape_key_clear (self->shape_keys.array + i);
		lisys_free (self->shape_keys.array);
	}
}

int private_read(LIMdlModel* self,LIArcReader* reader,int mesh)
{
	int i;
	int j;
	int ret;
	char* id;
	uint32_t tmp;
	uint32_t size;
	uint32_t version;
	LIMdlLod* lod;
	LIMdlPartition* partition;

	/* Read magic. */
	if (!liarc_reader_check_text (reader, "lips/mdl", ""))
	{
		lisys_error_set (EINVAL, "wrong file format");
		return 0;
	}

	/* Read header chunk size. */
	if (!liarc_reader_get_uint32 (reader, &size))
		return 0;
	if (size != 8)
	{
		lisys_error_set (LISYS_ERROR_VERSION, "invalid header block");
		return 0;
	}

	/* Read version. */
	if (!liarc_reader_get_uint32 (reader, &version))
		return 0;
	if (version != LIMDL_FORMAT_VERSION)
	{
		lisys_error_set (LISYS_ERROR_VERSION, "model version mismatch");
		return 0;
	}

	/* Read flags. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->flags = tmp;

	/* Read chunks. */
	while (!liarc_reader_check_end (reader))
	{
		id = NULL;
		if (!liarc_reader_get_text (reader, "", &id) ||
		    !liarc_reader_get_uint32 (reader, &size))
		{
			lisys_error_append ("reading chunk failed");
			lisys_free (id);
			return 0;
		}
		if (size > reader->length - reader->pos)
		{
			lisys_error_append ("invalid chunk size for `%s'", id);
			lisys_free (id);
			return 0;
		}
		if (!strcmp (id, "bou"))
			ret = private_read_bounds (self, reader);
		else if (mesh && !strcmp (id, "mat"))
			ret = private_read_materials (self, reader);
		else if (mesh && !strcmp (id, "ver"))
			ret = private_read_vertices (self, reader);
		else if (mesh && !strcmp (id, "fac")) /* Deprecated */
			ret = private_read_faces (self, reader);
		else if (mesh && !strcmp (id, "wei"))
			ret = private_read_weights (self, reader);
		else if (mesh && !strcmp (id, "lod"))
			ret = private_read_lod (self, reader);
		else if (mesh && !strcmp (id, "shk"))
			ret = private_read_shape_keys (self, reader);
		else if (mesh && !strcmp (id, "nod"))
			ret = private_read_nodes (self, reader);
		else if (mesh && !strcmp (id, "hai"))
			ret = private_read_hairs (self, reader);
		else if (mesh && !strcmp (id, "par"))
			ret = private_read_particles (self, reader);
		else if (mesh && !strcmp (id, "ptt"))
			ret = private_read_partitions (self, reader);
		else if (!strcmp (id, "sha"))
			ret = private_read_shapes (self, reader);
		else
		{
			if (!liarc_reader_skip_bytes (reader, size))
			{
				lisys_error_append ("failed to skip block `%s'", id);
				lisys_free (id);
				return 0;
			}
			ret = 1;
		}
		if (!ret)
		{
			lisys_error_append ("failed to read block `%s'", id);
			lisys_free (id);
			return 0;
		}
		lisys_free (id);
	}

	/* Ensure that the mandatory first level of detail exists. */
	if (!self->lod.count)
	{
		if (self->materials.count)
		{
			lisys_error_set (EINVAL, "model with materials but no level-of-detail data");
			return 0;
		}
		self->lod.array = lisys_calloc (1, sizeof (LIMdlLod));
		if (self->lod.array == NULL)
			return 0;
		self->lod.count = 1;
	}

	/* Sanity checks. */
	for (i = 0 ; i < self->lod.count ; i++)
	{
		lod = self->lod.array + i;
		if (lod->face_groups.count != self->materials.count)
		{
			lisys_error_set (EINVAL, "material and face group counts don't match");
			return 0;
		}
		for (j = 0 ; j < lod->indices.count ; j++)
		{
			if (lod->indices.array[i] >= self->vertices.count)
				lisys_error_set (EINVAL, "vertex index out of bounds");
		}
	}
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		for (j = 0 ; j < LIMDL_VERTEX_WEIGHTS_MAX ; j++)
		{
			if (self->vertices.array[i].bones[j] > self->weight_groups.count)
			{
				lisys_error_set (EINVAL, "weight group index out of bounds");
				return 0;
			}
		}
	}
	for (i = 0 ; i < self->partitions.count ; i++)
	{
		partition = self->partitions.array + i;
		for (j = 0 ; j < partition->vertices.count ; j++)
		{
			if (partition->vertices.array[i].index > self->vertices.count)
			{
				lisys_error_set (EINVAL, "partition vertex index out of bounds");
				return 0;
			}
		}
	}

	return 1;
}

static int private_read_bounds (LIMdlModel*  self,
								LIArcReader* reader)
{
	vec3 min;
	vec3 max;

	if (!liarc_reader_get_float (reader, &min.x) ||
	    !liarc_reader_get_float (reader, &min.y) ||
	    !liarc_reader_get_float (reader, &min.z) ||
	    !liarc_reader_get_float (reader, &max.x) ||
	    !liarc_reader_get_float (reader, &max.y) ||
	    !liarc_reader_get_float (reader, &max.z))
		return 0;
	limat_aabb_init_from_points (&self->bounds, &min, &max);

	return 1;
}

/* Deprecated by read_lod(). */
static int private_read_faces (LIMdlModel*  self,
							   LIArcReader* reader)
{
	/* Allocate the mandatory LOD. */
	lisys_assert (!self->lod.count);
	self->lod.array = lisys_calloc (1, sizeof (LIMdlLod));
	if (self->lod.array == NULL)
		return 0;
	self->lod.count = 1;

	return limdl_lod_read_old (self->lod.array, reader);
}

static int private_read_hairs (LIMdlModel*  self,
							   LIArcReader* reader)
{
	int i;
	uint32_t count;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &count))
		return 0;
	self->hairs.count = count;

	/* Read hairs. */
	if (self->hairs.count)
	{
		self->hairs.array = lisys_calloc (self->hairs.count, sizeof (LIMdlHairs));
		if (self->hairs.array == NULL)
			return 0;
		for (i = 0 ; i < self->hairs.count ; i++)
		{
			if (!limdl_hairs_read (self->hairs.array + i, reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_lod (LIMdlModel*  self,
							 LIArcReader* reader)
{
	uint32_t i;
	uint32_t count;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &count))
		return 0;
	if (!count)
	{
		lisys_error_set (EINVAL, "empty lod block");
		return 0;
	}

	/* Allocate the detail levels. */
	self->lod.array = lisys_calloc (count, sizeof (LIMdlLod));
	if (self->lod.array == NULL)
		return 0;
	self->lod.count = count;

	/* Read the detail levels. */
	for (i = 0 ; i < count ; i++)
	{
		if (!limdl_lod_read (self->lod.array + i, reader))
			return 0;
	}

	return 1;
}

static int private_read_materials (LIMdlModel*  self,
								   LIArcReader* reader)
{
	int i;
	uint32_t tmp;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->materials.count = tmp;

	/* Read materials. */
	if (self->materials.count)
	{
		self->materials.array = lisys_calloc (self->materials.count, sizeof (LIMdlMaterial));
		if (self->materials.array == NULL)
			return 0;
		for (i = 0 ; i < self->materials.count ; i++)
		{
			if (!limdl_material_read (self->materials.array + i, reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_nodes (LIMdlModel*  self,
							   LIArcReader* reader)
{
	int i;
	uint32_t tmp;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->nodes.count = tmp;

	/* Read nodes. */
	if (self->nodes.count)
	{
		self->nodes.array = lisys_calloc (self->nodes.count, sizeof (LIMdlNode*));
		if (self->nodes.array == NULL)
			return 0;
		for (i = 0 ; i < self->nodes.count ; i++)
		{
			self->nodes.array[i] = limdl_node_new (self);
			if (self->nodes.array[i] == NULL)
				return 0;
			if (!limdl_node_read (self->nodes.array[i], reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_particles (LIMdlModel*  self,
								   LIArcReader* reader)
{
	int i;
	uint32_t tmp[1];

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, tmp + 0))
		return 0;

	/* Allocate particle systems. */
	if (tmp[0])
	{
		self->particle_systems.array = lisys_calloc (tmp[0], sizeof (LIMdlParticleSystem));
		if (self->particle_systems.array == NULL)
			return 0;
		self->particle_systems.count = tmp[0];
	}

	/* Read particle systems. */
	for (i = 0 ; i < self->particle_systems.count ; i++)
	{
		if (!limdl_particle_system_read (self->particle_systems.array + i, reader))
			return 0;
	}

	return 1;
}

static int private_read_partitions (LIMdlModel*  self,
	LIArcReader* reader)
{
	int i;
	uint32_t tmp;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->partitions.count = tmp;

	/* Read shapes. */
	if (self->partitions.count)
	{
		self->partitions.array = lisys_calloc (self->partitions.count, sizeof (LIMdlPartition));
		if (self->partitions.array == NULL)
			return 0;
		for (i = 0 ; i < self->partitions.count ; i++)
		{
			if (!limdl_partition_read (self->partitions.array + i, reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_shapes (LIMdlModel*  self,
								LIArcReader* reader)
{
	int i;
	uint32_t tmp;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->shapes.count = tmp;

	/* Read shapes. */
	if (self->shapes.count)
	{
		self->shapes.array = lisys_calloc (self->shapes.count, sizeof (LIMdlShape));
		if (self->shapes.array == NULL)
			return 0;
		for (i = 0 ; i < self->shapes.count ; i++)
		{
			if (!limdl_shape_read (self->shapes.array + i, reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_shape_keys (LIMdlModel*  self,
									LIArcReader* reader)
{
	int i;
	uint32_t tmp;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->shape_keys.count = tmp;

	/* Read shape keys. */
	if (self->shape_keys.count)
	{
		self->shape_keys.array = lisys_calloc (self->shape_keys.count, sizeof (LIMdlShapeKey));
		if (self->shape_keys.array == NULL)
			return 0;
		for (i = 0 ; i < self->shape_keys.count ; i++)
		{
			if (!limdl_shape_key_read (self->shape_keys.array + i, reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_vertices (LIMdlModel*  self,
								  LIArcReader* reader)
{
	int i;
	uint32_t tmp;
	LIMdlVertex* vertex;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &tmp))
		return 0;
	self->vertices.count = tmp;

	/* Read vertices. */
	if (tmp)
	{
		self->vertices.array = lisys_calloc (tmp, sizeof (LIMdlVertex));
		if (self->vertices.array == NULL)
			return 0;
		self->vertices.count = tmp;
		for (i = 0 ; i < self->vertices.count ; i++)
		{
			vertex = self->vertices.array + i;
			vertex->weights[0] = 1.0f;
			if (!liarc_reader_get_float (reader, vertex->texcoord + 0) ||
				!liarc_reader_get_float (reader, vertex->texcoord + 1) ||
				!liarc_reader_get_float (reader, &vertex->normal.x) ||
				!liarc_reader_get_float (reader, &vertex->normal.y) ||
				!liarc_reader_get_float (reader, &vertex->normal.z) ||
				!liarc_reader_get_float (reader, &vertex->coord.x) ||
				!liarc_reader_get_float (reader, &vertex->coord.y) ||
				!liarc_reader_get_float (reader, &vertex->coord.z))
				return 0;
		}
	}

	return 1;
}

static int private_read_weights (LIMdlModel*  self,
								 LIArcReader* reader)
{
	int i;
	uint32_t tmp[2];
	LIMdlWeightGroup* group;

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, tmp + 0))
		return 0;

	/* Read weight groups. */
	if (tmp[0])
	{
		self->weight_groups.array = lisys_calloc (tmp[0], sizeof (LIMdlWeightGroup));
		if (self->weight_groups.array == NULL)
			return 0;
		self->weight_groups.count = tmp[0];
		for (i = 0 ; i < self->weight_groups.count ; i++)
		{
			group = self->weight_groups.array + i;
			if (!liarc_reader_get_text (reader, "", &group->name) ||
				!liarc_reader_get_text (reader, "", &group->bone))
				return 0;
		}
	}

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, tmp + 1))
		return 0;
	if (tmp[1] != self->vertices.count)
	{
		lisys_error_set (EINVAL, "vertex and weight counts must be equal");
		return 0;
	}

	/* Read vertex weights. */
	if (tmp[1])
	{
		for (i = 0 ; i < self->vertices.count ; i++)
		{
			if (!private_read_vertex_weights (self, self->vertices.array + i, reader))
				return 0;
		}
	}

	return 1;
}

static int private_read_vertex_weights (LIMdlModel*  self,
										LIMdlVertex* vertex,
										LIArcReader* reader)
{
	uint32_t i;
	uint32_t j;
	uint32_t count;
	uint32_t group;
	float weight;
	float weights_tmp[LIMDL_VERTEX_WEIGHTS_MAX + 1];
	LIMdlVertexBone bones_tmp[LIMDL_VERTEX_WEIGHTS_MAX + 1];

	/* Read header. */
	if (!liarc_reader_get_uint32 (reader, &count))
		return 0;

	/* Initialize empty weights. */
	for (i = 0 ; i < LIMDL_VERTEX_WEIGHTS_MAX + 1 ; i++)
	{
		weights_tmp[i] = 0.0f;
		bones_tmp[i] = 0;
	}

	/* Read weights. */
	/* We only save the most significant weights by doing in place insertion
	   sorting. Weights cannot be eliminated here because the bones using them
	   might become available only after a model merge. */
	for (i = 0 ; i < count ; i++)
	{
		if (!liarc_reader_get_uint32 (reader, &group) ||
		    !liarc_reader_get_float (reader, &weight))
			return 0;
		if (group >= self->weight_groups.count)
		{
			lisys_error_set (EINVAL, "weight group index out of bounds");
			return 0;
		}
		if (weight == 0.0f)
			continue;
		for (j = LIMDL_VERTEX_WEIGHTS_MAX ; j > 0 && weight > weights_tmp[j - 1] ; j--)
		{
			weights_tmp[j] = weights_tmp[j - 1];
			bones_tmp[j] = bones_tmp[j - 1];
		}
		bones_tmp[j] = group + 1;
		weights_tmp[j] = weight;
	}

	/* Normalize the weights. */
	weight = 0.0f;
	for (i = 0 ; i < LIMDL_VERTEX_WEIGHTS_MAX ; i++)
		weight += weights_tmp[i];
	if (weight > LIMAT_EPSILON)
	{
		for (i = 0 ; i < LIMDL_VERTEX_WEIGHTS_MAX ; i++)
			weights_tmp[i] /= weight;
	}
	else
	{
		weights_tmp[0] = 1.0f;
		bones_tmp[0] = 0;
	}

	/* Store the most significant weights to the vertex. */
	memcpy (vertex->bones, bones_tmp, LIMDL_VERTEX_WEIGHTS_MAX * sizeof (char));
	memcpy (vertex->weights, weights_tmp, LIMDL_VERTEX_WEIGHTS_MAX * sizeof (float));

	return 1;
}

static int private_write (const LIMdlModel* self,
						  LIArcWriter*      writer)
{
	if (!private_write_block (self, "lips/mdl", private_write_header, writer) ||
	    !private_write_block (self, "bou", private_write_bounds, writer) ||
	    !private_write_block (self, "mat", private_write_materials, writer) ||
	    !private_write_block (self, "ver", private_write_vertices, writer) ||
	    !private_write_block (self, "wei", private_write_weights, writer) ||
	    !private_write_block (self, "lod", private_write_lod, writer) ||
	    !private_write_block (self, "shk", private_write_shape_keys, writer) ||
	    !private_write_block (self, "nod", private_write_nodes, writer) ||
	    !private_write_block (self, "hai", private_write_hairs, writer) ||
	    !private_write_block (self, "par", private_write_particles, writer) ||
	    !private_write_block (self, "ptt", private_write_partitions, writer) ||
	    !private_write_block (self, "sha", private_write_shapes, writer))
		return 0;
	return 1;
}

static int private_write_block (const LIMdlModel* self,
								const char*       name,
								LIMdlWriteFunc    func,
								LIArcWriter*      writer)
{
	int len;
	LIArcWriter* data;

	/* Write the block to a temporary buffer. */
	data = liarc_writer_new ();
	if (data == NULL)
		return 0;
	if (!func (self, data))
	{
		liarc_writer_free (data);
		return 0;
	}
	len = liarc_writer_get_length (data);
	if (!len)
	{
		liarc_writer_free (data);
		return 1;
	}

	/* Write the temporary to the main writer as a block. */
	if (!liarc_writer_append_string (writer, name) ||
	    !liarc_writer_append_nul (writer) ||
	    !liarc_writer_append_uint32 (writer, len) ||
	    !liarc_writer_append_raw (writer, liarc_writer_get_buffer (data), len))
	{
		liarc_writer_free (data);
		return 0;
	}
	liarc_writer_free (data);

	return 1;
}

static int private_write_bounds (const LIMdlModel* self,
								 LIArcWriter*      writer)
{
	if (!liarc_writer_append_float (writer, self->bounds.min.x) ||
	    !liarc_writer_append_float (writer, self->bounds.min.y) ||
	    !liarc_writer_append_float (writer, self->bounds.min.z) ||
	    !liarc_writer_append_float (writer, self->bounds.max.x) ||
	    !liarc_writer_append_float (writer, self->bounds.max.y) ||
	    !liarc_writer_append_float (writer, self->bounds.max.z))
		return 0;
	return 1;
}

static int private_write_hairs (const LIMdlModel* self,
								LIArcWriter*      writer)
{
	int i;
	LIMdlHairs* hairs;

	/* Check if writing is needed. */
	if (!self->hairs.count)
		return 1;

	/* Write animations. */
	if (!liarc_writer_append_uint32 (writer, self->hairs.count))
		return 0;
	for (i = 0 ; i < self->hairs.count ; i++)
	{
		hairs = self->hairs.array + i;
		if (!limdl_hairs_write (hairs, writer))
			return 0;
	}

	return 1;
}

static int private_write_header (const LIMdlModel* self,
								 LIArcWriter*      writer)
{
	if (!liarc_writer_append_uint32 (writer, LIMDL_FORMAT_VERSION) ||
	    !liarc_writer_append_uint32 (writer, self->flags))
		return 0;
	return 1;
}

static int private_write_lod (const LIMdlModel* self,
							  LIArcWriter*      writer)
{
	int i;
	LIMdlLod* lod;

	/* Check if writing is needed. */
	if (self->lod.count == 1 && !self->lod.array[0].face_groups.count)
		return 1;

	if (!liarc_writer_append_uint32 (writer, self->lod.count))
		return 0;
	for (i = 0 ; i < self->lod.count ; i++)
	{
		lod = self->lod.array + i;
		if (!limdl_lod_write (lod, writer))
			return 0;
	}

	return 1;
}

static int private_write_materials (const LIMdlModel* self,
									LIArcWriter*      writer)
{
	int i;
	LIMdlMaterial* material;

	/* Check if writing is needed. */
	if (!self->materials.count)
		return 1;

	if (!liarc_writer_append_uint32 (writer, self->materials.count))
		return 0;
	for (i = 0 ; i < self->materials.count ; i++)
	{
		material = self->materials.array + i;
		if (!limdl_material_write (material, writer))
			return 0;
	}

	return 1;
}

static int private_write_nodes (const LIMdlModel* self,
								LIArcWriter*      writer)
{
	int i;
	LIMdlNode* node;

	/* Check if writing is needed. */
	if (!self->nodes.count)
		return 1;

	/* Write nodes. */
	if (!liarc_writer_append_uint32 (writer, self->nodes.count))
		return 0;
	for (i = 0 ; i < self->nodes.count ; i++)
	{
		node = self->nodes.array[i];
		if (!limdl_node_write (node, writer))
			return 0;
	}

	return 1;
}

static int private_write_particles (const LIMdlModel* self,
									LIArcWriter*      writer)
{
	int i;

	/* Check if writing is needed. */
	if (!self->particle_systems.count)
		return 1;

	/* Write header. */
	if (!liarc_writer_append_uint32 (writer, self->particle_systems.count))
		return 0;

	/* Write particle systems. */
	for (i = 0 ; i < self->particle_systems.count ; i++)
	{
		if (!limdl_particle_system_write (self->particle_systems.array + i, writer))
			return 0;
	}

	return 1;
}

static int private_write_partitions (const LIMdlModel* self,
									 LIArcWriter*      writer)
{
	int i;
	LIMdlPartition* partition;

	/* Check if writing is needed. */
	if (!self->partitions.count)
		return 1;

	/* Write the partitions. */
	if (!liarc_writer_append_uint32 (writer, self->partitions.count))
		return 0;
	for (i = 0 ; i < self->partitions.count ; i++)
	{
		partition = self->partitions.array + i;
		if (!limdl_partition_write (partition, writer))
			return 0;
	}

	return 1;
}

static int private_write_shapes (const LIMdlModel* self,
								 LIArcWriter*      writer)
{
	int i;
	LIMdlShape* shape;

	/* Check if writing is needed. */
	if (!self->shapes.count)
		return 1;

	/* Write nodes. */
	if (!liarc_writer_append_uint32 (writer, self->shapes.count))
		return 0;
	for (i = 0 ; i < self->shapes.count ; i++)
	{
		shape = self->shapes.array + i;
		if (!limdl_shape_write (shape, writer))
			return 0;
	}

	return 1;
}

static int private_write_shape_keys (const LIMdlModel* self,
									 LIArcWriter*      writer)
{
	int i;
	LIMdlShapeKey* shape;

	/* Check if writing is needed. */
	if (!self->shape_keys.count)
		return 1;

	/* Write shape keys. */
	if (!liarc_writer_append_uint32 (writer, self->shape_keys.count))
		return 0;
	for (i = 0 ; i < self->shape_keys.count ; i++)
	{
		shape = self->shape_keys.array + i;
		if (!limdl_shape_key_write (shape, writer))
			return 0;
	}

	return 1;
}

static int private_write_vertices (const LIMdlModel* self,
								   LIArcWriter*      writer)
{
	int i;
	LIMdlVertex* vertex;

	/* Check if writing is needed. */
	if (!self->vertices.count)
		return 1;

	/* Write header. */
	if (!liarc_writer_append_uint32 (writer, self->vertices.count))
		return 0;

	/* Write vertices. */
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		vertex = self->vertices.array + i;
		if (!liarc_writer_append_float (writer, vertex->texcoord[0]) ||
		    !liarc_writer_append_float (writer, vertex->texcoord[1]) ||
		    !liarc_writer_append_float (writer, vertex->normal.x) ||
		    !liarc_writer_append_float (writer, vertex->normal.y) ||
		    !liarc_writer_append_float (writer, vertex->normal.z) ||
		    !liarc_writer_append_float (writer, vertex->coord.x) ||
		    !liarc_writer_append_float (writer, vertex->coord.y) ||
		    !liarc_writer_append_float (writer, vertex->coord.z))
			return 0;
	}

	return 1;
}

static int private_write_weights (const LIMdlModel* self,
								  LIArcWriter*      writer)
{
	int i;
	int j;
	int count;
	LIMdlVertex* vertex;
	LIMdlWeightGroup* group;

	/* Check if writing is needed. */
	if (!self->weight_groups.count)
		return 1;

	/* Write header. */
	if (!liarc_writer_append_uint32 (writer, self->weight_groups.count))
		return 0;

	/* Write weight groups. */
	for (i = 0 ; i < self->weight_groups.count ; i++)
	{
		group = self->weight_groups.array + i;
		liarc_writer_append_string (writer, group->name);
		liarc_writer_append_nul (writer);
		liarc_writer_append_string (writer, group->bone);
		liarc_writer_append_nul (writer);
	}

	/* Write header. */
	if (!liarc_writer_append_uint32 (writer, self->vertices.count))
		return 0;

	/* Write vertex weights. */
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		vertex = self->vertices.array + i;
		for (count = 0 ; count < LIMDL_VERTEX_WEIGHTS_MAX ; count++)
		{
			if (!vertex->bones[count])
				break;
		}
		if (!liarc_writer_append_uint32 (writer, count))
			return 0;
		for (j = 0 ; j < count ; j++)
		{
			if (!liarc_writer_append_uint32 (writer, vertex->bones[j] - 1) ||
			    !liarc_writer_append_float (writer, vertex->weights[j]))
				return 0;
		}
	}

	return !writer->error;
}

