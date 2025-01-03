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

/**
 * \addtogroup LIMdl Model
 * @{
 * \addtogroup LIMdlPartition Partition
 * @{
 */

#include"model_partition.h"

int limdl_partition_init_copy (
	LIMdlPartition*       self,
	const LIMdlPartition* partition)
{
	if (partition->name != NULL)
	{
		self->name = lisys_string_dup (partition->name);
		if (self->name == NULL)
			return 0;
	}
	if (partition->vertices.count)
	{
		self->vertices.array = lisys_calloc (partition->vertices.count, sizeof (LIMdlPartitionVertex));
		if (self->vertices.array == NULL)
		{
			lisys_free (self->name);
			return 0;
		}
		self->vertices.count = partition->vertices.count;
		memcpy (self->vertices.array, partition->vertices.array,
			self->vertices.count * sizeof (LIMdlPartitionVertex));
	}

	return 1;
}

void limdl_partition_clear (
	LIMdlPartition*  self)
{
	lisys_free (self->vertices.array);
	lisys_free (self->name);
}

int limdl_partition_read (
	LIMdlPartition* self,
	LIArcReader*    reader)
{
	int i;
	uint32_t tmp;
	LIMdlPartitionVertex* vertex;

	/* Read header. */
	if (!liarc_reader_get_text (reader, "", &self->name) ||
	    !liarc_reader_get_uint32 (reader, &tmp))
		return 0;

	/* Allocate vertices. */
	if (tmp)
	{
		self->vertices.array = lisys_calloc (tmp, sizeof (LIMdlPartitionVertex));
		if (self->vertices.array == NULL)
			return 0;
		self->vertices.count = tmp;
	}

	/* Read vertices. */
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		vertex = self->vertices.array + i;
		if (!liarc_reader_get_uint32 (reader, &tmp) ||
			!liarc_reader_get_float (reader, &vertex->weight))
			return 0;
		vertex->index = tmp;
	}

	return 1;
}

int limdl_partition_write (
	const LIMdlPartition* self,
	LIArcWriter*          writer)
{
	int i;
	LIMdlPartitionVertex* v;

	/* Write the partition header. */
	if (!liarc_writer_append_string (writer, self->name) ||
	    !liarc_writer_append_nul (writer) ||
	    !liarc_writer_append_uint32 (writer, self->vertices.count))
		return 0;

	/* Write the vertices. */
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		v = self->vertices.array + i;
		if (!liarc_writer_append_uint32 (writer, v->index) ||
			!liarc_writer_append_float (writer, v->weight))
			return 0;
	}

	return 1;
}

/** @} */
/** @} */
