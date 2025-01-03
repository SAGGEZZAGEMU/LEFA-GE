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

/**
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenAnimationUpdater AttachmentEntity
 * @{
 */

#include "lipsofsuna/system.h"
#include "render.hpp"
#include "render-animation-updater.hpp"
#include "render-object.hpp"

static void private_thread_main (LISysThread* thread, void* data);

/*****************************************************************************/

LIRenAnimationUpdater::LIRenAnimationUpdater (LIRenRender* render, float secs) : pos(0), secs(secs), render(render)
{
	// Copy the object list for thread-safety.
	LIAlgU32dicIter iter;
	LIALG_U32DIC_FOREACH (iter, render->objects)
		objects.push_back ((LIRenObject*) iter.value);

	// Create the mutex.
	mutex = lisys_mutex_new ();

	// Create the worker thread.
	thread = lisys_thread_new (private_thread_main, this);
}

LIRenAnimationUpdater::~LIRenAnimationUpdater ()
{
	if (thread != NULL)
		lisys_thread_free (thread);
	if (mutex != NULL)
		lisys_mutex_free (mutex);
}

void LIRenAnimationUpdater::join ()
{
	lisys_thread_join (thread);
}

void LIRenAnimationUpdater::main ()
{
	while (true)
	{
		{
			LISysScopedLock lock (mutex);
			if ((size_t) pos >= objects.size ())
				break;
			objects[pos]->update_animations (secs);
		}
		{
			LISysScopedLock lock (mutex);
			pos++;
		}
	}
}

void LIRenAnimationUpdater::object_added (LIRenObject* object)
{
	LISysScopedLock lock (mutex);

	objects.push_back (object);
}

void LIRenAnimationUpdater::object_removed (LIRenObject* object)
{
	LISysScopedLock lock (mutex);

	for (size_t i = 0 ; i < objects.size () ; i++)
	{
		if (objects[i] == object)
		{
			if (i == (size_t) pos)
				pos -= 1;
			objects.erase (objects.begin () + i);
			break;
		}
	}
}

/*****************************************************************************/

static void private_thread_main (LISysThread* thread, void* data)
{
	((LIRenAnimationUpdater*) data)->main ();
}

/** @} */
/** @} */
/** @} */
