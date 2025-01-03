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

#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

//#include "lipsofsuna/algorithm.h"
//#include "lipsofsuna/system.h"
#include"ANOTHER/algorithm.h"
//#include"model.h"

/*#include"ANOTHER/allsystem.h"
*/

typedef struct _LIMdlManager LIMdlManager;
typedef struct _LIMdlManagerCallback LIMdlManagerCallback;

struct _LIMdlManagerCallback
{
	void (*callback)(void*, LIMdlModel*);
	void* userdata;
};

struct _LIMdlManager
{
	LIAlgRandom random;
	LIAlgU32dic* models;
	LIMdlManagerCallback model_added_callback;
	LIMdlManagerCallback model_removed_callback;
};

LIAPICALL (LIMdlManager*, limdl_manager_new, ());

LIAPICALL (void, limdl_manager_free, (LIMdlManager* self));

LIAPICALL (int, limdl_manager_add_model, (LIMdlManager* self, LIMdlModel* model));

LIAPICALL (void, limdl_manager_remove_model, (LIMdlManager* self, LIMdlModel* model));

LIAPICALL (void, limdl_manager_free_model, (LIMdlModel* model));

#endif
