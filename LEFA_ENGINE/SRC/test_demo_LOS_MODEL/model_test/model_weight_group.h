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

#ifndef __MODEL_WEIGHT_GROUP_H__
#define __MODEL_WEIGHT_GROUP_H__

//#include"lipsofsuna/system.h"
/*#include"ANOTHER/allsystem.h"
*/

#include"model_types.h"
#include"ANOTHER/system/system_compiler.h"
#include"ANOTHER/system/system_async_call.h"

struct _LIMdlWeightGroup
{
	char* name;
	char* bone;
};

LIAPICALL(int, limdl_weight_group_init_copy, 
		 (LIMdlWeightGroup* self,
		 const LIMdlWeightGroup* group));

#endif