/* Lips of Suna
 * Copyright© 2007-2010 Lips of Suna development team.
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

#ifndef __MODEL_BONE_H__
#define __MODEL_BONE_H__

//#include "lipsofsuna/archive.h"
//#include "lipsofsuna/math.h"
//#include "lipsofsuna/system.h"
#include"model_types.h"
#include"lefa/include/utils/system_choice.h"

#include"ANOTHER/math_only.h"


/*#include"ANOTHER/allsystem.h"
*/

#include"ANOTHER/archive.h"
#include"ANOTHER/archive/archive_reader.h"
#include"ANOTHER/archive/archive_writer.h"


struct _LIMdlBone
{
	vec3 length;
};

LE_APICALL (int, limdl_bone_read, (LIMdlNode*   self, LIArcReader* reader));

LE_APICALL (int, limdl_bone_write, (const LIMdlNode* self, LIArcWriter*     writer));

LE_APICALL (void, limdl_bone_get_head, (const LIMdlNode* self, vec3*     head));

LE_APICALL (void, limdl_bone_get_tail, (const LIMdlNode* self, vec3*     tail));

#endif
