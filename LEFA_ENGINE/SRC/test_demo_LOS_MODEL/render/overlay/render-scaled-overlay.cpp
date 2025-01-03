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
 * \addtogroup LIRenOverlay Overlay
 * @{
 * \addtogroup LIRenImageOverlay ImageOverlay
 * @{
 */

#include "lipsofsuna/math.h"
#include "render-scaled-overlay.hpp"
#include "render-tile-packer.hpp"
#include <OgreHardwareBufferManager.h>
#include <OgreRoot.h>

Ogre::String LIRenScaledOverlay::type_name ("LIRenScaledOverlay");

/*****************************************************************************/

LIRenScaledOverlay::LIRenScaledOverlay (const Ogre::String& name) : LIRenImageOverlay (name)
{
}

LIRenScaledOverlay::~LIRenScaledOverlay ()
{
}

const Ogre::String& LIRenScaledOverlay::getTypeName () const
{
	return type_name;
}

void LIRenScaledOverlay::updatePositionGeometry ()
{
	/* Prevent divide by zero. */
	if (!mPixelWidth || !mPixelHeight)
	{
		render_op.vertexData->vertexCount = 0;
		return;
	}

	/* Calculate vertex coordinates. */
	float left = _getDerivedLeft() * 2.0f - 1.0f;
	float top = 1.0f - _getDerivedTop() * 2.0f;
	float xscale = 2.0f * mWidth / mPixelWidth;
	float yscale = 2.0f * mHeight / mPixelHeight;
	float x[2] =
	{
		left,
		left + xscale * mPixelWidth
	};
	float y[2] =
	{
		top,
		top - yscale * mPixelHeight
	};

	// Ogre seems to divide by zero during startup when the viewport
	// hasn't been initialized. Refuse to do anything if that happened.
	if (!limat_number_is_finite (left) || !limat_number_is_finite (top))
	{
		render_op.vertexData->vertexCount = 0;
		return;
	}

	/* Calculate texture coordinates. */
	float uscale = 1.0f / src_size[0];
	float vscale = 1.0f / src_size[1];
	float u[2] =
	{
		uscale * (src_pos[0]),
		uscale * (src_pos[0] + src_tiling[1])
	};
	float v[2] =
	{
		vscale * (src_pos[1]),
		vscale * (src_pos[1] + src_tiling[4])
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

	/* Add the quad. */
	packer.add_quad (x[0], y[0], u[0], v[0], x[1], y[1], u[1], v[1]);

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

/** @} */
/** @} */
/** @} */
