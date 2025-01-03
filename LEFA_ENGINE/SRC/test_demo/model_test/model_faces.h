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

#ifndef __MODEL_FACES_H__
#define __MODEL_FACES_H__

//#include "lipsofsuna/archive.h"
//#include "lipsofsuna/system.h"
#include"model_types.h"

/*#include"ANOTHER/allsystem.h"
*/

#include"ANOTHER/system/system_compiler.h"

typedef struct _LIMdlFaces LIMdlFaces;
struct _LIMdlFaces
{
	int start;
	int count;
};

LIAPICALL(int, limdl_faces_init_copy, (LIMdlFaces* self, LIMdlFaces* faces));

LIAPICALL (void, limdl_faces_free, (
	LIMdlFaces* self));

#endif