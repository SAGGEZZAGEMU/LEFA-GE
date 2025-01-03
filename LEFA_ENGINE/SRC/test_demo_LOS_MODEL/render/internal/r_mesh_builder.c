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
 * \addtogroup LIRenMeshBuilder MeshBuilder
 * @{
 */

#include "lipsofsuna/system.h"
#include "render.hpp"
#include "render-material-utils.hpp"
#include "render-mesh-builder.hpp"
#include "render-object.hpp"
#include <OgreSubMesh.h>
#include <OgreSkeletonManager.h>

LIRenMeshBuilder::LIRenMeshBuilder (LIRenRender* render, const LIMdlModel* model, bool editable) :
	editable(editable), render(render), data(model)
{
	step = 0;
	buffer_size_0 = 0;
	buffer_size_1 = 0;
	buffer_size_2 = 0;
	vertex_data = NULL;
	vertex_buffer_binding = NULL;
}

LIRenMeshBuilder::~LIRenMeshBuilder ()
{
}

void LIRenMeshBuilder::prepareResource (Ogre::Resource* resource)
{
	materials.clear ();
	if (step < 1)
	{
		step_1_bg ((Ogre::Mesh*) resource);
		step = 1;
	}
}

void LIRenMeshBuilder::loadResource (Ogre::Resource* resource)
{
	materials.clear ();
	if (step < 1)
	{
		step_1_bg ((Ogre::Mesh*) resource);
		step = 1;
	}
	if (step < 2)
	{
		step_2_fg ((Ogre::Mesh*) resource);
		step = 2;
	}
	if (step < 3)
	{
		step_3_fg ((Ogre::Mesh*) resource);
		step = 3;
	}
	if (step < 4)
	{
		step_4_fg ((Ogre::Mesh*) resource);
		step = 4;
	}
	step = 0;
	if (!editable)
		data.clear_buffer_data ();
	vertex_data = NULL;
	vertex_buffer_binding = NULL;
}

bool LIRenMeshBuilder::is_idle () const
{
	return step == 0;
}

/**
 * \brief Prepares the mesh.
 *
 * This is called from the background thread. It basically does everything
 * except material initialization, which was done before, and vertex buffer
 * initialization, which must be done in load_mesh() due to thread safe issues.
 *
 * Materials are also loaded here. They are loaded in yet another background
 * loader. The function waits for the background loading of the materials to
 * finish, but since this is done in a background thread, the main thread will
 * never stall.
 *
 * \param mesh Ogre mesh to prepare.
 */
void LIRenMeshBuilder::step_1_bg (Ogre::Mesh* mesh)
{
	if (!data.vertex_count)
		return;

	/* Allocate the vertex data. */
	vertex_data = OGRE_NEW Ogre::VertexData ();
	vertex_data->vertexCount = data.vertex_count;
	mesh->sharedVertexData = vertex_data;

	/* Initialize the vertex format. */
	/* Positions and normals need to be in their own vertex buffer for software
	   skinning to work. Let's be nice and support the software fallback too. */
	Ogre::VertexDeclaration* format = vertex_data->vertexDeclaration;
	format->addElement (0, buffer_size_0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	buffer_size_0 += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	format->addElement (0, buffer_size_0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	buffer_size_0 += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	format->addElement (1, buffer_size_1, Ogre::VET_FLOAT3, Ogre::VES_TANGENT);
	buffer_size_1 += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	format->addElement (1, buffer_size_1, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	buffer_size_1 += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT2);
	format->addElement (1, buffer_size_1, Ogre::VET_COLOUR_ABGR, Ogre::VES_DIFFUSE);
	buffer_size_1 += Ogre::VertexElement::getTypeSize (Ogre::VET_COLOUR_ABGR);
	format->addElement (2, buffer_size_2, Ogre::VET_UBYTE4, Ogre::VES_BLEND_INDICES);
	buffer_size_2 += Ogre::VertexElement::getTypeSize (Ogre::VET_UBYTE4);
	format->addElement (2, buffer_size_2, Ogre::VET_FLOAT4, Ogre::VES_BLEND_WEIGHTS);
	buffer_size_2 += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT4);
	lisys_assert (buffer_size_0 == 6 * 4);
	lisys_assert (buffer_size_1 == 6 * 4);
	lisys_assert (buffer_size_2 == 5 * 4);

	/* Create the bone index to blend index mapping. */
	/* The indices are always the same but Ogre needs the map regardless. */
	mesh->sharedBlendIndexToBoneIndexMap.resize (data.rest_pose_buffer->bones.count);
	for (int i = 0 ; i < data.rest_pose_buffer->bones.count ; i++)
		mesh->sharedBlendIndexToBoneIndexMap[i] = i;

	/* Create submeshes. */
	/* The index buffer cannot be prepared yet since it doesn't exist. */
	for (size_t i = 0 ; i < data.materials.size () ; i++)
	{
		const LIRenModelDataMaterial& mat = data.materials[i];
		Ogre::SubMesh* submesh = mesh->createSubMesh ();
		submesh->useSharedVertices = true;
		submesh->indexData->indexStart = mat.start;
		submesh->indexData->indexCount = mat.count;
	}

	/* Set the bounding box. */
	LIMatAabb bounds = data.bounds;
	mesh->_setBounds (Ogre::AxisAlignedBox (
		bounds.min.x, bounds.min.y, bounds.min.z,
		bounds.max.x, bounds.max.y, bounds.max.z));
	mesh->_setBoundingSphereRadius (
		(bounds.max.x - bounds.min.x) +
		(bounds.max.y - bounds.min.y) +
		(bounds.max.z - bounds.min.z));
}

/**
 * \brief Creates the materials.
 */
void LIRenMeshBuilder::step_2_fg (Ogre::Mesh* mesh)
{
	if (!data.vertex_count)
		return;

	for (size_t i = 0 ; i < data.materials.size () ; i++)
	{
		/* Create the material. */
		const LIRenModelDataMaterial& mat = data.materials[i];
		Ogre::MaterialPtr material = render->material_utils->create_material (&mat.material);

		/* Set the material name of the submesh. */
		Ogre::SubMesh* submesh = mesh->getSubMesh (i);
		submesh->setMaterialName (material->getName ());

		/* Reference the material. */
		/* Ogre meshes don't reference the material, instead only storing
		   its name. Since the mesh can have temporary materials that would
		   be subject to garbage collection without a reference, we need to
		   add keep one here. */
		materials.push_back (material);
	}
}

/**
 * \brief Creates the mesh.
 *
 * This is called from the main thread. It creates the vertex buffers.
 *
 * \param mesh Ogre mesh to finish.
 */
void LIRenMeshBuilder::step_3_fg (Ogre::Mesh* mesh)
{
	if (!data.vertex_count)
		return;

	/* Create the first vertex buffer. */
	/* Contains positions and normals only. */
	vertex_buffer_0 = Ogre::HardwareBufferManager::getSingleton ().createVertexBuffer (
		buffer_size_0, data.vertex_count, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vertex_buffer_0->writeData (0, vertex_buffer_0->getSizeInBytes (), data.buffer_data_0, true);

	/* Create the second vertex buffer. */
	/* Contains the rest apart from blending information. */
	vertex_buffer_1 = Ogre::HardwareBufferManager::getSingleton ().createVertexBuffer (
		buffer_size_1, data.vertex_count, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vertex_buffer_1->writeData (0, vertex_buffer_1->getSizeInBytes (), data.buffer_data_1, true);

	/* Create the third vertex buffer. */
	/* Contains blending indices and weights. */
	vertex_buffer_2 = Ogre::HardwareBufferManager::getSingleton ().createVertexBuffer (
		buffer_size_2, data.vertex_count, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, true);
	vertex_buffer_2->writeData (0, vertex_buffer_2->getSizeInBytes (), data.buffer_data_2, true);

	/* Bind the vertex buffers. */
	vertex_buffer_binding = vertex_data->vertexBufferBinding;
	vertex_buffer_binding->setBinding (0, vertex_buffer_0);
	vertex_buffer_binding->setBinding (1, vertex_buffer_1);
	vertex_buffer_binding->setBinding (2, vertex_buffer_2);

	/* Create the index buffer. */
	index_buffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer (
		Ogre::HardwareIndexBuffer::IT_16BIT, data.index_count, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	index_buffer->writeData (0, index_buffer->getSizeInBytes (), data.index_data, true);

	/* Complete submeshes. */
	/* The index buffer needs to be setup here since prepare couldn't do it. */
	for (size_t i = 0 ; i < data.materials.size () ; i++)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh (i);
		submesh->indexData->indexBuffer = index_buffer;
	}
}

/**
 * \brief Creates the skeleton.
 *
 * This is called from the main thread. It initializes the skeleton
 * when needed.
 *
 * \param mesh Ogre mesh to finish.
 */
void LIRenMeshBuilder::step_4_fg (Ogre::Mesh* mesh)
{
	/* Create a skeleton if needed. */
	if (!data.vertex_count || data.rest_pose_buffer->bones.count <= 1)
		return;
	bool ok = create_skeleton (mesh);

	/* If creating the skeleton failed due to too many bones, disable
	   the mesh completely to avoid exploding vertices. */
	if (!ok)
	{
		for (size_t i = 0 ; i < data.materials.size () ; i++)
			mesh->destroySubMesh (i);
	}
}

bool LIRenMeshBuilder::create_skeleton (Ogre::Mesh* mesh)
{
	Ogre::Bone* bone;

	/* Make sure that there aren't too many bones. */
	/* Ogre seems to occasionally fail an assertion when it tries to
	   upload bone matrices to shaders. This check seems to have eliminated
	   the issue so it probably can't handle too many matrices. */
	if (data.rest_pose_buffer->bones.count > 32)
	{
		printf ("WARNING: too many weighted bones: %d/%d!\n", data.rest_pose_buffer->bones.count, 32);
		return false;
	}

	/* Create the skeleton. */
	Ogre::String name (render->id.next ());
	Ogre::ResourcePtr resource = Ogre::SkeletonManager::getSingleton ().create (name, LIREN_RESOURCES_TEMPORARY, true);
	Ogre::Skeleton* skeleton = (Ogre::Skeleton*) resource.getPointer ();

	/* Create the bones. */
	/* We disregard the hierarchy and just create a bone for each weight group.
	   This ensures that the minimal number of bones is uploaded to the GPU. */
	for (int i = 0 ; i < data.rest_pose_buffer->bones.count ; i++)
	{
		bone = skeleton->createBone (i);
		if (i)
		{
			const LIMatTransform& t = data.rest_pose_buffer->bones.array[i].transform;
			bone->setPosition (t.position.x, t.position.y, t.position.z);
			bone->setOrientation (t.rotation.w, t.rotation.x, t.rotation.y, t.rotation.z);
		}
		else
			bone->setManuallyControlled (true);
	}

	/* Set the binding pose. */
	skeleton->setBindingPose ();
	skeleton->load ();

	/* Assign the skeleton to the mesh. */
	mesh->setSkeletonName (skeleton->getName ());

	return true;
}

/** @} */
/** @} */
/** @} */
