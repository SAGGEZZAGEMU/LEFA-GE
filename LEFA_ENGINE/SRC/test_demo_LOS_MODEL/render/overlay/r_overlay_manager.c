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
 * \addtogroup LIRenOverlayManager OverlayManager
 * @{
 */

#include "render-container-factory.hpp"
#include "render-image-overlay-factory.hpp"
#include "render-overlay.hpp"
#include "render-overlay-manager.hpp"
#include "render-scaled-overlay-factory.hpp"
#include "render-text-overlay-factory.hpp"

LIRenOverlayManager::LIRenOverlayManager(LIRenRender* render)
{
	/* Allocate the overlay dictionary. */
	overlays = lialg_u32dic_new ();
	if (overlays == NULL)
		throw;

#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 9
	overlay_system = new Ogre::OverlaySystem ();
	render->scene_manager->addRenderQueueListener (overlay_system);
#endif

	/* Add the overlay factories. */
	overlay_manager = &(Ogre::OverlayManager::getSingleton ());
	container_factory = new LIRenContainerFactory;
	overlay_manager->addOverlayElementFactory (container_factory);
	image_factory = new LIRenImageOverlayFactory;
	overlay_manager->addOverlayElementFactory (image_factory);
	scaled_factory = new LIRenScaledOverlayFactory;
	overlay_manager->addOverlayElementFactory (scaled_factory);
	text_factory = new LIRenTextOverlayFactory;
	overlay_manager->addOverlayElementFactory (text_factory);
}

LIRenOverlayManager::~LIRenOverlayManager ()
{
	LIAlgU32dicIter iter;

	/* Free overlays. */
	if (overlays != NULL)
	{
		LIALG_U32DIC_FOREACH (iter, overlays)
			liren_overlay_free ((LIRenOverlay*) iter.value);
		lialg_u32dic_free (overlays);
	}

	delete container_factory;
	delete image_factory;
	delete scaled_factory;
	delete text_factory;
}

/** @} */
/** @} */
/** @} */
