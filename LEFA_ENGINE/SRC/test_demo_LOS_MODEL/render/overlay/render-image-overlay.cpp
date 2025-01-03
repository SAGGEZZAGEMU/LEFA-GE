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
 * \addtogroup LIRenOverlay Overlay
 * @{
 * \addtogroup LIRenImageOverlay ImageOverlay
 * @{
 */

#include "lipsofsuna/math.h"
#include "render-image-overlay.hpp"
#include "render-tile-packer.hpp"
#include <OgreHardwareBufferManager.h>
#include <OgreRoot.h>

Ogre::String LIRenImageOverlay::type_name ("LIRenImageOverlay");

/*****************************************************************************/

LIRenImageOverlay::LIRenImageOverlay (const Ogre::String& name) : Ogre::OverlayElement (name)
{
}

LIRenImageOverlay::~LIRenImageOverlay ()
{
	delete render_op.vertexData;
}

void LIRenImageOverlay::set_alpha (float alpha)
{
	LIRenBaseOverlay::set_alpha (alpha);
	if (this->mInitialised)
		updatePositionGeometry ();
}

void LIRenImageOverlay::set_color (const float* color)
{
	memcpy (this->color, color, 4 * sizeof (float));
}

void LIRenImageOverlay::set_clipping (const int* rect)
{
	if (rect != NULL)
	{
		memcpy (this->dst_clip, rect, 4 * sizeof (int));
		clipping = true;
	}
	else
		clipping = false;
}

void LIRenImageOverlay::set_rotation (
	float angle,
	float center_x,
	float center_y)
{
	rotation_angle = angle;
	rotation_center[0] = center_x;
	rotation_center[1] = center_y;
}

void LIRenImageOverlay::set_tiling (
	const int* source_position,
	const int* source_size,
	const int* source_tiling)
{
	memcpy (this->src_pos, source_position, 2 * sizeof (int));
	memcpy (this->src_size, source_size, 2 * sizeof (int));
	memcpy (this->src_tiling, source_tiling, 6 * sizeof (int));
	mGeomPositionsOutOfDate = true;
}

void LIRenImageOverlay::initialise ()
{
	if (mInitialised)
		return;
	mInitialised = true;

	/* Initialize the render operation. */
	render_op.vertexData = new Ogre::VertexData ();
	render_op.vertexData->vertexStart = 0;
	render_op.vertexData->vertexCount = 0;

	/* Setup the vertex format. */
	size_t offset = 0;
	Ogre::VertexDeclaration* format = render_op.vertexData->vertexDeclaration;
	format->addElement (0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	format->addElement (0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	offset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT2);
	format->addElement (0, offset, Ogre::VET_COLOUR_ABGR, Ogre::VES_DIFFUSE);
	offset += Ogre::VertexElement::getTypeSize (Ogre::VET_COLOUR_ABGR);
	lisys_assert (offset == VERTEX_SIZE);

	/* Create the vertex buffer. */
	vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer (
		offset, VERTEX_COUNT_INIT, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	/* Bind the vertex buffer. */
	render_op.vertexData->vertexBufferBinding->setBinding (0, vbuf);
	render_op.useIndexes = false;
	render_op.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
}

const Ogre::String& LIRenImageOverlay::getTypeName () const
{
	return type_name;
}

void LIRenImageOverlay::getRenderOperation (Ogre::RenderOperation& op)
{
	op = render_op;
}

void LIRenImageOverlay::updatePositionGeometry ()
{
	// Prevent divide by zero.
	if (!mPixelWidth || !mPixelHeight)
	{
		render_op.vertexData->vertexCount = 0;
		return;
	}

	// Calculate offset and dimensions.
	float left = _getDerivedLeft() * 2.0f - 1.0f;
	float top = 1.0f - _getDerivedTop() * 2.0f;
	float xscale = 2.0f * mWidth / mPixelWidth;
	float yscale = 2.0f * mHeight / mPixelHeight;
	float xtile = xscale * src_tiling[1];
	float ytile = yscale * src_tiling[4];

	// Ogre seems to divide by zero during startup when the viewport
	// hasn't been initialized. Refuse to do anything if that happened.
	if (!limat_number_is_finite (left) || !limat_number_is_finite (top))
	{
		render_op.vertexData->vertexCount = 0;
		return;
	}

	// Calculate vertex coordinates.
	float x[4] =
	{
		left,
		left + xscale * src_tiling[0],
		left + xscale * (mPixelWidth - src_tiling[2]),
		left + xscale * mPixelWidth
	};
	float y[4] =
	{
		top,
		top - yscale * src_tiling[3],
		top - yscale * (mPixelHeight - src_tiling[5]),
		top - yscale * mPixelHeight
	};

	/* Calculate texture coordinates. */
	float uscale = 1.0f / src_size[0];
	float vscale = 1.0f / src_size[1];
	float u[4] =
	{
		uscale * (src_pos[0]),
		uscale * (src_pos[0] + src_tiling[0]),
		uscale * (src_pos[0] + src_tiling[0] + src_tiling[1]),
		uscale * (src_pos[0] + src_tiling[0] + src_tiling[1] + src_tiling[2])
	};
	float v[4] =
	{
		vscale * (src_pos[1]),
		vscale * (src_pos[1] + src_tiling[3]),
		vscale * (src_pos[1] + src_tiling[3] + src_tiling[4]),
		vscale * (src_pos[1] + src_tiling[3] + src_tiling[4] + src_tiling[5])
	};

	/* Setup vertex packing. */
	float z = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue ();
	LIRenTilePacker packer (z);
	float color_[4] = { color[0], color[1], color[2], color[3] * alpha };
	packer.set_color (color_);
	if (rotation_angle != 0.0f)
	{
		float cx = left + xscale * rotation_center[0];
		float cy = top - yscale * rotation_center[1];
		packer.set_rotation (rotation_angle, cx, cy, xscale / yscale);
	}
	float clip[4];
	if (clipping)
	{
		clip[0] = left + xscale * dst_clip[0];
		clip[1] = top - yscale * (dst_clip[1] + dst_clip[3]);
		clip[2] = xscale * dst_clip[2];
		clip[3] = yscale * dst_clip[3];
		packer.set_clipping (clip);
	}

	/* Add corners. */
	packer.add_quad (x[0], y[0], u[0], v[0], x[1], y[1], u[1], v[1]);
	packer.add_quad (x[2], y[0], u[2], v[0], x[3], y[1], u[3], v[1]);
	packer.add_quad (x[2], y[2], u[2], v[2], x[3], y[3], u[3], v[3]);
	packer.add_quad (x[0], y[2], u[0], v[2], x[1], y[3], u[1], v[3]);

	/* Add top and bottom borders. */
	for (float i = x[1] ; i < x[2] ; i += xtile)
	{
		float wtile = LIMAT_MIN (x[2] - i, xtile);
		float xe = i + wtile;
		float ue = u[1] + (u[2] - u[1]) * (wtile / xtile);
		packer.add_quad (i, y[0], u[1], v[0], xe, y[1], ue, v[1]);
		packer.add_quad (i, y[2], u[1], v[2], xe, y[3], ue, v[3]);
	}

	/* Add left and right borders. */
	for (float j = y[1] ; j > y[2] ; j -= ytile)
	{
		float htile = LIMAT_MIN (j - y[2], ytile);
		float ye = j - htile;
		float ve = v[1] + (v[2] - v[1]) * (htile / ytile);
		packer.add_quad (x[0], j, u[0], v[1], x[1], ye, u[1], ve);
		packer.add_quad (x[2], j, u[2], v[1], x[3], ye, u[3], ve);
	}

	/* Add the filling. */
	for (float i = x[1] ; i < x[2] ; i += xtile)
	{
		float wtile = LIMAT_MIN (x[2] - i, xtile);
		float xe = i + wtile;
		float ue = u[1] + (u[2] - u[1]) * (wtile / xtile);
		for (float j = y[1] ; j > y[2] ; j -= ytile)
		{
			float htile = LIMAT_MIN (j - y[2], ytile);
			float ye = j - htile;
			float ve = v[1] + (v[2] - v[1]) * (htile / ytile);
			packer.add_quad (i, j, u[1], v[1], xe, ye, ue, ve);
		}
	}

	/* Update vertex buffer. */
	lisys_assert (packer.pos <= 6 * VERTEX_COUNT_MAX);
	if (vbuf->getNumVertices () < (size_t)(packer.pos / 6))
	{
		vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer (
			VERTEX_SIZE, packer.pos / 6, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		render_op.vertexData->vertexBufferBinding->setBinding (0, vbuf);
	}
	vbuf->writeData (0, vbuf->getSizeInBytes (), packer.verts);

	/* Bind the vertex buffer. */
	render_op.useIndexes = false;
	render_op.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	render_op.vertexData->vertexCount = packer.pos / 6;
}

void LIRenImageOverlay::updateTextureGeometry ()
{
}

/** @} */
/** @} */
/** @} */
