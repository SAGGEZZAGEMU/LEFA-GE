/* Lips of Suna
 * Copyright© 2007-2011 Lips of Suna development team.
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
 * \addtogroup LIRenOverlay Overlay
 * @{
 * \addtogroup LIRenContainer Container
 * @{
 */

#include "render-container.hpp"
#include <OgreHardwareBufferManager.h>

#define MAX_VERTEX_COUNT 1024

Ogre::String LIRenContainer::type_name ("LIRenContainer");

/*****************************************************************************/

LIRenContainer::LIRenContainer (const Ogre::String& name) : Ogre::PanelOverlayElement (name)
{
	layer = 0;
}

LIRenContainer::~LIRenContainer ()
{
	delete render_op.vertexData;
}

void LIRenContainer::initialise ()
{
	bool init = !mInitialised;
	Ogre::PanelOverlayElement::initialise();
	if (init)
	{
		/* Initialize the render operation. */
		render_op.vertexData = new Ogre::VertexData ();
		render_op.vertexData->vertexStart = 0;
		render_op.vertexData->vertexCount = 0;

		/* Setup the vertex format. */
		size_t offset = 0;
		Ogre::VertexDeclaration* format = render_op.vertexData->vertexDeclaration;
		format->addElement (0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
		offset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT2);
		format->addElement (0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);

		/* Create the vertex buffer. */
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer (
			offset, MAX_VERTEX_COUNT, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		/* Bind the vertex buffer. */
		render_op.vertexData->vertexBufferBinding->setBinding (0, vbuf);
		render_op.useIndexes = false;
		render_op.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mInitialised = true;
	}
}

const Ogre::String& LIRenContainer::getTypeName () const
{
	return type_name;
}

void LIRenContainer::getRenderOperation (Ogre::RenderOperation& op)
{
	op = render_op;
}

void LIRenContainer::add_container (LIRenContainer* cont, int layer)
{
	cont->layer = layer;
	containers.push_back (cont);
	addChild (cont);
	_notifyZOrder (mZOrder);
}

void LIRenContainer::add_element (LIRenBaseOverlay* elem)
{
	elements.push_back (elem);
	addChild (elem->get_ogre ());
	_notifyZOrder (mZOrder);
}

void LIRenContainer::remove_container (int index)
{
	LIRenContainer* child = containers[index];
	containers.erase (containers.begin () + index);
	Ogre::String name (child->getName ());
	removeChild (name);
	_notifyZOrder (mZOrder);
}

void LIRenContainer::remove_element (int index)
{
	Ogre::OverlayElement* child = elements[index]->get_ogre ();
	elements.erase (elements.begin () + index);
	Ogre::String name (child->getName ());
	removeChild (name);
	_notifyZOrder (mZOrder);
}

void LIRenContainer::remove_all_elements ()
{
	Ogre::OverlayElement* elem;

	for (size_t i = 0 ; i < elements.size () ; i++)
	{
		elem = elements[i]->get_ogre ();
		Ogre::String name (elem->getName ());
		removeChild (name);
		Ogre::OverlayManager::getSingleton ().destroyOverlayElement (name);
	}
	elements.clear ();
	_notifyZOrder (mZOrder);
}

void LIRenContainer::set_alpha (float value)
{
	for (size_t i = 0 ; i < containers.size () ; i++)
		containers[i]->set_alpha (value);
	for (size_t i = 0 ; i < elements.size () ; i++)
	{
		LIRenBaseOverlay* overlay = elements[i];
		overlay->set_alpha (value);
	}
}

Ogre::ushort LIRenContainer::_notifyZOrder (Ogre::ushort z)
{
	/* Set our own Z order. */
	mZOrder = z;

	/* Get the first set of children. */
	std::vector<LIRenContainer*> children;
	children.push_back (this);

	/* Sort children using the breadth first search algorithm. */
	for (size_t count = children.size () ; count ; count = children.size ())
	{
		/* Sort the layers separately. */
		for (int layer = 0 ; layer < 2 ; layer++)
		{
			for (size_t i = 0 ; i < count ; i++)
			{
				LIRenContainer* child = children[i];
				if (child->layer == layer)
				{
					z = child->_notifyZOrderNonrecursive (z);
					child->get_children (children);
				}
			}
		}

		/* Remove the previous hierarchy level. */
		children.erase (children.begin (), children.begin () + count);
	}

	return z + 1;
}

Ogre::ushort LIRenContainer::_notifyZOrderNonrecursive (Ogre::ushort z)
{
	z = OverlayElement::_notifyZOrder (z);

	for (size_t i = 0 ; i < elements.size () ; i++)
		z = elements[i]->get_ogre ()->_notifyZOrder (z);

	return z + 1;
}

void LIRenContainer::_updateRenderQueue (Ogre::RenderQueue* queue)
{
	if (mVisible)
	{
		OverlayElement::_updateRenderQueue (queue);
		for (size_t i = 0 ; i < elements.size () ; i++)
			elements[i]->get_ogre ()->_updateRenderQueue (queue);
		for (size_t i = 0 ; i < containers.size () ; i++)
			containers[i]->_updateRenderQueue (queue);
	}
}

void LIRenContainer::get_children (std::vector<LIRenContainer*>& children)
{
	for (size_t i = 0 ; i < containers.size () ; i++)
		children.push_back (containers[i]);
}

/** @} */
/** @} */
/** @} */
