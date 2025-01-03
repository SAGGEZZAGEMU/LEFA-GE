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

#ifndef __RENDER_INTERNAL_CONTAINER_H__
#define __RENDER_INTERNAL_CONTAINER_H__
/*
#include "lipsofsuna/system.h"
#include "render-overlay.hpp"
#include "render-base-overlay.hpp"
*/

typedef struct _LIRenContainer LIRenContainer;
struct _LIRenContainer 
{

	int layer;
	char type_name[256];
	//Ogre::String name;
};

/*
class LIRenContainer : public Ogre::PanelOverlayElement
{
public:
	LIRenContainer (const Ogre::String& name);
	virtual ~LIRenContainer ();
	virtual void initialise ();
	virtual const Ogre::String& getTypeName () const;
	virtual void getRenderOperation (Ogre::RenderOperation& op);
	void add_container (LIRenContainer* cont, int layer);
	void add_element (LIRenBaseOverlay* elem);
	void remove_container (int index);
	void remove_element (int index);
	void remove_all_elements ();
	void set_alpha (float value);
protected:
	virtual Ogre::ushort _notifyZOrder (Ogre::ushort z);
	virtual Ogre::ushort _notifyZOrderNonrecursive (Ogre::ushort z);
	virtual void _updateRenderQueue (Ogre::RenderQueue* queue);
	void get_children (std::vector<LIRenContainer*>& children);
private:
	int layer;
	Ogre::RenderOperation render_op;
	static Ogre::String type_name;
public:
	std::vector<LIRenContainer*> containers;
	std::vector<LIRenBaseOverlay*> elements;
};
*/
#endif
