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

#ifndef __RENDER_INTERNAL_CONTAINER_FACTORY_H__
#define __RENDER_INTERNAL_CONTAINER_FACTORY_H__

/*
#include "lipsofsuna/system.h"
#include "render-overlay.hpp"
#include "render-container.hpp"
*/

typedef struct _LIRenContainerFactory LIRenContainerFactory;
struct _LIRenContainerFactory 
{


	char name[256];
	//Ogre::String name;
};

/*
class LIRenContainerFactory : public Ogre::OverlayElementFactory
{
public:
	LIRenContainerFactory () : name ("LIRenContainer")
	{
	}
	virtual ~LIRenContainerFactory ()
	{
	}
	virtual Ogre::OverlayElement* createOverlayElement (const Ogre::String& name)
	{
		return new LIRenContainer (name);
	}
	virtual void destroyOverlayElement (Ogre::OverlayElement* element)
	{
		delete element;
	}
	virtual const Ogre::String& getTypeName () const
	{
		return name;
	}
private:
	Ogre::String name;
};
*/

#endif
