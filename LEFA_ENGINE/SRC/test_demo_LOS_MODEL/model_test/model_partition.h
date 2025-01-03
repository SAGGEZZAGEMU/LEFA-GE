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

#ifndef __MODEL_PARTITION_H__
#define __MODEL_PARTITION_H__

//#include "lipsofsuna/archive.h"
//#include "lipsofsuna/system.h"
/*#include"ANOTHER/allsystem.h"
*/

#include"ANOTHER/system/system_string.h"
#include"ANOTHER/system/system_async_call.h"

#include"ANOTHER/archive/archive_reader.h"
#include"ANOTHER/archive/archive_writer.h"
#include"model_types.h"

struct _LIMdlPartitionVertex
{
	int index;
	float weight;
};

struct _LIMdlPartition
{
	char* name;
	struct
	{
		int count;
		LIMdlPartitionVertex* array;
	} vertices;
};

LIAPICALL (int, limdl_partition_init_copy, (
	LIMdlPartition*       self,
	const LIMdlPartition* partition));

LIAPICALL (void, limdl_partition_clear, (
	LIMdlPartition* self));

LIAPICALL (int, limdl_partition_read, (
	LIMdlPartition* self,
	LIArcReader*   reader));

LIAPICALL (int, limdl_partition_write, (
	const LIMdlPartition* self,
	LIArcWriter*          writer));

#endif
