/* Lips of Suna
 * Copyright© 2007-2014 Lips of Suna development team.
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

#ifndef __RENDER_ANIMATION_UPDATER_HPP__
#define __RENDER_ANIMATION_UPDATER_HPP__

#include "lipsofsuna/system.h"
#include <vector>

class LIRenObject;
class LIRenRender;

class LIRenAnimationUpdater
{
public:
	LIRenAnimationUpdater (LIRenRender* render, float secs);
	virtual ~LIRenAnimationUpdater ();
	void join ();
	void main ();
	void object_added (LIRenObject* object);
	void object_removed (LIRenObject* object);
private:
	int pos;
	float secs;
	LIRenRender* render;
	LISysThread* thread;
	LISysMutex* mutex;
	std::vector<LIRenObject*> objects;
};

#endif
