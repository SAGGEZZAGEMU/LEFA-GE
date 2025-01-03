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
 * \addtogroup LIMat Math
 * @{
 * \addtogroup LIMatPolygon2d Polygon2d
 * @{
 */

#include "math-polygon-2d.h"

#define DEBUGPRINT(...)
//#define DEBUGPRINT printf

/**
 * \brief Creates a new polygon.
 * \param ops Vertex access operations.
 * \param vertices Array of vertices.
 * \param count Number of vertices in the array.
 * \return New polygon or NULL.
 */
LIMatPolygon2d* limat_polygon2d_new (
	const LIMatVtxops2d* ops,
	const void*          vertices,
	int                  count)
{
	LIMatPolygon2d* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMatPolygon2d));
	if (self == NULL)
		return NULL;
	self->ops = ops;
	if (count == 0)
		return self;

	/* Allocate vertices. */
	self->vertices.capacity = count;
	self->vertices.vertices = lisys_malloc (count * ops->size);
	if (self->vertices.vertices == NULL)
	{
		lisys_free (self);
		return NULL;
	}

	/* Copy vertices. */
	memcpy (self->vertices.vertices, vertices, count * ops->size);
	self->vertices.count = count;

	return self;
}

/**
 * \brief Creates a new polygon with three vertices.
 * \param ops Vertex access operations.
 * \param v0 Vertex.
 * \param v1 Vertex.
 * \param v2 Vertex.
 * \return New polygon or NULL.
 */
LIMatPolygon2d* limat_polygon2d_new_from_triangle (
	const LIMatVtxops2d* ops,
	const void*          v0,
	const void*          v1,
	const void*          v2)
{
	LIMatPolygon2d* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIMatPolygon2d));
	if (self == NULL)
		return NULL;
	self->ops = ops;

	/* Allocate vertices. */
	self->vertices.capacity = 3;
	self->vertices.count = 3;
	self->vertices.vertices = lisys_malloc (3 * ops->size);
	if (self->vertices.vertices == NULL)
	{
		lisys_free (self);
		return NULL;
	}
	memcpy (self->vertices.vertices + 0 * ops->size, v0, ops->size);
	memcpy (self->vertices.vertices + 1 * ops->size, v1, ops->size);
	memcpy (self->vertices.vertices + 2 * ops->size, v2, ops->size);

	return self;
}

/**
 * \brief Frees the polygon.
 * \param self Polygon.
 */
void limat_polygon2d_free (
	LIMatPolygon2d* self)
{
	lisys_free (self->vertices.vertices);
	lisys_free (self);
}

/**
 * \brief Appends vertices to the polygon.
 * \param self Polygon.
 * \param vertices Array of vertices.
 * \param count Number of vertices in the array.
 * \return Nonzero on success.
 */
int limat_polygon2d_add_vertices (
	LIMatPolygon2d* self,
	const void*     vertices,
	int             count)
{
	int num;
	LIMatVector2d* tmp;
	const LIMatVtxops2d* ops = self->ops;

	num = self->vertices.count + count;
	if (num > self->vertices.capacity)
	{
		tmp = lisys_realloc (self->vertices.vertices, num * ops->size);
		if (tmp == NULL)
			return 0;
		self->vertices.vertices = tmp;
		self->vertices.capacity = num;
	}
	memcpy (self->vertices.vertices + self->vertices.count * ops->size, vertices, count * ops->size);
	self->vertices.count += count;

	return 1;
}

/**
 * \brief Returns the part of the polygon in front of the line.
 *
 * If the algorithm runs out of memory, zero is returned and the returned
 * polygon misses one or more vertices.
 *
 * Uses the Sutherland-Hodgman algorithm.
 *
 * \param self Polygon.
 * \param line Line.
 * \param front Return location for the polygon on the front side.
 * \return Nonzero on success.
 */
int limat_polygon2d_clip (
	const LIMatPolygon2d* self,
	const LIMatLine2d*    line,
	LIMatPolygon2d*       front)
{
	int i;
	int type;
	int ret = 1;
	int curr_in;
	int curr_edge;
	int prev_in;
	int prev_edge;
	float dist;
	float frac;
	void* curr_vtx;
	void* prev_vtx;
	void* tmp_vtx;
	LIMatVector2d curr_coord;
	LIMatVector2d prev_coord;
	LIMatVector2d tmp_coord;
	const LIMatVtxops2d* ops = self->ops;

	/* Initialize. */
	front->data = self->data;
	front->vertices.count = 0;
	if (!self->vertices.count)
		return 1;
	tmp_vtx = lisys_malloc (ops->size);
	if (tmp_vtx == NULL)
		return 0;
	DEBUGPRINT ("BEGIN CLIP %f,%f,%f\n", line->x, line->y, line->w);

	/* Solve the side of the previous vertex. */
	prev_vtx = self->vertices.vertices + (self->vertices.count - 1) * ops->size;
	ops->getcoord (prev_vtx, &prev_coord);
	dist = limat_line2d_signed_distance_to_point (line, &prev_coord);
	prev_in = (dist >= LIMAT_EPSILON);
	prev_edge = (LIMAT_ABS(dist) < LIMAT_EPSILON);

	/* Calculate the vertices of the new polygons. */
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		/* Get the vertex. */
		curr_vtx = self->vertices.vertices + i * ops->size;
		ops->getcoord (curr_vtx, &curr_coord);

		/* Solve the side of the vertex. */
		dist = limat_line2d_signed_distance_to_point (line, &curr_coord);
		curr_in = (dist >= LIMAT_EPSILON);
		curr_edge = (LIMAT_ABS(dist) < LIMAT_EPSILON);
		DEBUGPRINT ("  VERTEX vtx=%f,%f prev_in=%d prev_edge=%d curr_in=%d curr_edge=%d\n",
			curr_coord.x, curr_coord.y, prev_in, prev_edge, curr_in, curr_edge);

		/* Emit vertices. */
		if (curr_edge)
		{
			/* Front/back/edge to edge. */
			DEBUGPRINT ("    EDGE %f,%f\n", curr_coord.x, curr_coord.y);
			ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
		}
		else if (prev_edge)
		{
			/* Edge to front/back. */
			if (curr_in)
			{
				DEBUGPRINT ("    EDGE_IN %f,%f\n", curr_coord.x, curr_coord.y);
				ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
			}
		}
		else if (curr_in)
		{
			if (!prev_in)
			{
				type = limat_line2d_intersects_line (line, &prev_coord, &curr_coord, &tmp_coord);
				lisys_assert (type == LIMAT_LINE2D_INTERSECT);
				frac = limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, tmp_coord)) /
				       limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, curr_coord));
				ops->setcoord (tmp_vtx, &tmp_coord);
				ops->interpolate (curr_vtx, prev_vtx, frac, tmp_vtx);
				DEBUGPRINT ("    CUT1 %f,%f\n", tmp_coord.x, tmp_coord.y);
				ret &= limat_polygon2d_add_vertices (front, tmp_vtx, 1);
			}
			DEBUGPRINT ("    KEEP %f,%f\n", curr_coord.x, curr_coord.y);
			ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
		}
		else
		{
			if (prev_in)
			{
				type = limat_line2d_intersects_line (line, &prev_coord, &curr_coord, &tmp_coord);
				lisys_assert (type == LIMAT_LINE2D_INTERSECT);
				frac = limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, tmp_coord)) /
				       limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, curr_coord));
				ops->setcoord (tmp_vtx, &tmp_coord);
				ops->interpolate (curr_vtx, prev_vtx, frac, tmp_vtx);
				DEBUGPRINT ("    CUT2 %f,%f\n", tmp_coord.x, tmp_coord.y);
				ret &= limat_polygon2d_add_vertices (front, tmp_vtx, 1);
			}
		}

		/* Move to the next vertex. */
		prev_coord = curr_coord;
		prev_vtx = curr_vtx;
		prev_in = curr_in;
		prev_edge = curr_edge;
	}

	lisys_free (tmp_vtx);

	return ret;
}

/**
 * \brief Returns the part of the polygon in front of the line.
 *
 * If the algorithm runs out of memory, zero is returned and the returned
 * polygon misses one or more vertices.
 *
 * Uses the Sutherland-Hodgman algorithm.
 *
 * \param self Polygon.
 * \param line Line.
 * \return Nonzero on success.
 */
int limat_polygon2d_clip_inplace (
	LIMatPolygon2d*    self,
	const LIMatLine2d* line)
{
	int ret;
	LIMatPolygon2d front;

	/* Create temporary storage for the front vertices. */
	memset (&front, 0, sizeof (LIMatPolygon2d));
	front.ops = self->ops;

	/* Split into the temporary storage. */
	ret = limat_polygon2d_clip (self, line, &front);

	/* Replace the current vertices with the new ones. */
	lisys_free (self->vertices.vertices);
	self->vertices = front.vertices;

	return ret;
}

/**
 * \brief Compares two polygons.
 * \param self Polygon.
 * \param polygon Polygon.
 * \return One if the polygons are equal. Zero otherwise.
 */
int limat_polygon2d_compare (
	const LIMatPolygon2d* self,
	const LIMatPolygon2d* polygon)
{
	int i;
	LIMatVector2d v0;
	LIMatVector2d v1;

	if (self->vertices.count != polygon->vertices.count)
		return 0;
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		limat_polygon2d_get_coord (self, i, &v0);
		limat_polygon2d_get_coord (polygon, i, &v1);
		if (!limat_vector2d_compare (v0, v1, 0.00001f))
			return 0;
	}
	return 1;
}

/**
 * \brief Debug prints the polygon.
 * \param self Polygon.
 */
void limat_polygon2d_print (
	const LIMatPolygon2d* self)
{
	int i;
	LIMatVector2d v;

	printf ("{");
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		limat_polygon2d_get_coord (self, i, &v);
		printf ("{%f,%f}", v.x, v.y);
		if (i < self->vertices.count - 1)
			printf (", ");
	}
	printf ("}\n");
}

/**
 * \brief Splits the polygon in two parts by the line.
 *
 * If the algorithm runs out of memory, zero is returned and at least one
 * of the resulting polygons misses one or more vertices.
 *
 * Uses the Sutherland-Hodgman algorithm.
 *
 * \param self Polygon.
 * \param line Line.
 * \param front Return location for the polygon on the front side.
 * \param back Return location for the polygon on the back side.
 * \return Nonzero on success.
 */
int limat_polygon2d_split (
	const LIMatPolygon2d* self,
	const LIMatLine2d*    line,
	LIMatPolygon2d*       front,
	LIMatPolygon2d*       back)
{
	int i;
	int type;
	int ret = 1;
	int curr_in;
	int curr_edge;
	int prev_in;
	int prev_edge;
	float dist;
	float frac;
	void* curr_vtx;
	void* prev_vtx;
	void* tmp_vtx;
	LIMatVector2d curr_coord;
	LIMatVector2d prev_coord;
	LIMatVector2d tmp_coord = { 0.0f, 0.0f };
	const LIMatVtxops2d* ops = self->ops;

	/* Initialize. */
	front->data = self->data;
	back->data = self->data;
	front->vertices.count = 0;
	back->vertices.count = 0;
	if (!self->vertices.count)
		return 1;
	tmp_vtx = lisys_malloc (ops->size);
	if (tmp_vtx == NULL)
		return 0;
	DEBUGPRINT ("BEGIN CULL %f,%f,%f\n", line->x, line->y, line->w);

	/* Solve the side of the previous vertex. */
	prev_vtx = self->vertices.vertices + (self->vertices.count - 1) * ops->size;
	ops->getcoord (prev_vtx, &prev_coord);
	dist = limat_line2d_signed_distance_to_point (line, &prev_coord);
	prev_in = (dist >= LIMAT_EPSILON);
	prev_edge = (LIMAT_ABS(dist) < LIMAT_EPSILON);

	/* Calculate the vertices of the new polygons. */
	for (i = 0 ; i < self->vertices.count ; i++)
	{
		/* Get the vertex. */
		curr_vtx = self->vertices.vertices + i * ops->size;
		ops->getcoord (curr_vtx, &curr_coord);

		/* Solve the side of the vertex. */
		dist = limat_line2d_signed_distance_to_point (line, &curr_coord);
		curr_in = (dist >= LIMAT_EPSILON);
		curr_edge = (LIMAT_ABS(dist) < LIMAT_EPSILON);
		DEBUGPRINT ("  VERTEX vtx=%f,%f prev_in=%d prev_edge=%d curr_in=%d curr_edge=%d\n",
			curr_coord.x, curr_coord.y, prev_in, prev_edge, curr_in, curr_edge);

		/* Emit vertices. */
		if (curr_edge)
		{
			/* Front/back/edge to edge. */
			ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
			ret &= limat_polygon2d_add_vertices (back, curr_vtx, 1);
		}
		else if (prev_edge)
		{
			/* Edge to front/back. */
			if (curr_in)
				ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
			else
				ret &= limat_polygon2d_add_vertices (back, curr_vtx, 1);
		}
		else if (curr_in && !prev_in)
		{
			/* Back to front. */
			type = limat_line2d_intersects_line (line, &prev_coord, &curr_coord, &tmp_coord);
			lisys_assert (type == LIMAT_LINE2D_INTERSECT);
			frac = limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, tmp_coord)) /
			       limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, curr_coord));
			ops->setcoord (tmp_vtx, &tmp_coord);
			ops->interpolate (curr_vtx, prev_vtx, frac, tmp_vtx);
			ret &= limat_polygon2d_add_vertices (back, tmp_vtx, 1);
			ret &= limat_polygon2d_add_vertices (front, tmp_vtx, 1);
			ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
		}
		else if (!curr_in && prev_in)
		{
			/* Front to back. */
			type = limat_line2d_intersects_line (line, &prev_coord, &curr_coord, &tmp_coord);
			lisys_assert (type == LIMAT_LINE2D_INTERSECT);
			frac = limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, tmp_coord)) /
			       limat_vector2d_get_length (limat_vector2d_subtract (prev_coord, curr_coord));
			ops->setcoord (tmp_vtx, &tmp_coord);
			ops->interpolate (curr_vtx, prev_vtx, frac, tmp_vtx);
			ret &= limat_polygon2d_add_vertices (front, tmp_vtx, 1);
			ret &= limat_polygon2d_add_vertices (back, tmp_vtx, 1);
			ret &= limat_polygon2d_add_vertices (back, curr_vtx, 1);
		}
		else if (curr_in)
		{
			/* Stay in front. */
			ret &= limat_polygon2d_add_vertices (front, curr_vtx, 1);
		}
		else
		{
			/* Stay behind. */
			ret &= limat_polygon2d_add_vertices (back, curr_vtx, 1);
		}

		/* Move to the next vertex. */
		prev_coord = curr_coord;
		prev_vtx = curr_vtx;
		prev_in = curr_in;
		prev_edge = curr_edge;
	}

	lisys_free (tmp_vtx);

	return ret;
}

/**
 * \brief Splits the polygon in two parts in-place.
 *
 * If the algorithm runs out of memory, zero is returned and at least one
 * of the resulting polygons misses one or more vertices.
 *
 * Uses the Sutherland-Hodgman algorithm.
 *
 * \param self Polygon.
 * \param line Line.
 * \param back Return location for the polygon on the back side.
 * \return Nonzero on success.
 */
int limat_polygon2d_split_inplace (
	LIMatPolygon2d*    self,
	const LIMatLine2d* line,
	LIMatPolygon2d*    back)
{
	int ret;
	LIMatPolygon2d front;

	/* Create temporary storage for the front vertices. */
	memset (&front, 0, sizeof (LIMatPolygon2d));
	front.ops = self->ops;

	/* Split into the temporary storage. */
	ret = limat_polygon2d_split (self, line, &front, back);

	/* Replace the current vertices with the new ones. */
	lisys_free (self->vertices.vertices);
	self->vertices = front.vertices;

	return ret;
}

/**
 * \brief Removes duplicate vertices.
 * \param self Polygon.
 * \param threshold Maximum distance between removed vertices.
 * \return Nonzero if the polygon is degenerate.
 */
void limat_polygon2d_remove_duplicates (
	LIMatPolygon2d* self,
	float           threshold)
{
	int i;
	int j;
	LIMatVector2d prev;
	LIMatVector2d curr;
	const LIMatVtxops2d* ops = self->ops;

	if (!self->vertices.count)
		return;
	ops->getcoord (self->vertices.vertices + (self->vertices.count - 1) * ops->size, &prev);
	for (i = 0 ; i < self->vertices.count ; )
	{
		ops->getcoord (self->vertices.vertices + i * ops->size, &curr);
		if (limat_vector2d_compare (prev, curr, threshold))
		{
			for (j = i ; j < self->vertices.count - 1 ; j++)
			{
				memcpy (self->vertices.vertices + j * ops->size,
					self->vertices.vertices + (j + 1) * ops->size, ops->size);
			}
			self->vertices.count--;
		}
		else
		{
			prev = curr;
			i++;
		}
	}
}

/**
 * \brief Checks if the polygon is completely degenerate.
 *
 * If this function returns nonzero, any attempts to triangulate the polygon
 * will yield zero non-degenerate triangles.
 *
 * \param self Polygon.
 * \return Nonzero if the polygon is degenerate.
 */
int limat_polygon2d_get_degenerate (
	const LIMatPolygon2d* self)
{
	if (self->vertices.count < 3)
		return 1;

	return 0;
}

/**
 * \brief Gets the coordinates of a vertex from the polygon.
 * \param self Polygon.
 * \param index Vertex index.
 * \param coord Return location for a vector.
 */
void limat_polygon2d_get_coord (
	const LIMatPolygon2d* self,
	int                 index,
	LIMatVector2d*      coord)
{
	const LIMatVtxops2d* ops = self->ops;

	self->ops->getcoord (self->vertices.vertices + index * ops->size, coord);
}

/**
 * \brief Gets a vertex from the polygon.
 * \param self Polygon.
 * \param index Vertex index.
 * \return Pointer to vertex.
 */
void* limat_polygon2d_get_vertex (
	const LIMatPolygon2d* self,
	int                   index)
{
	const LIMatVtxops2d* ops = self->ops;

	return self->vertices.vertices + index * ops->size;
}

/** @} */
/** @} */
