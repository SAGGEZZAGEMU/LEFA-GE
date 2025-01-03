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

#include "math-polygon-culler.h"

/**
 * \addtogroup LIMat Math
 * @{
 * \addtogroup LIMatPolygonCuller PolygonCuller
 * @{
 */

/**
 * \brief Creates a new polygon culler.
 * \param ops Vertex operations.
 * \param vertices Vertex array.
 * \param count Vertex count.
 * \return Polygon culler.
 */
LIMatPolygonCuller* limat_polygon_culler_new (
	const LIMatVtxops2d* ops,
	const void*          vertices,
	int                  count)
{
	LIMatPolygonCuller* self;

	self = lisys_calloc (1, sizeof (LIMatPolygonCuller));
	if (self == NULL)
		return 0;
	self->remainder = limat_polygon2d_new (ops, vertices, count);
	if (self->remainder == NULL)
	{
		lisys_free (self);
		return 0;
	}

	return self;
}

/**
 * \brief Frees the polygon culler.
 * \param self Polygon culler.
 */
void limat_polygon_culler_free (
	LIMatPolygonCuller* self)
{
	int i;

	if (self->remainder != NULL)
		limat_polygon2d_free (self->remainder);
	for (i = 0 ; i < self->pieces.count ; ++i)
		limat_polygon2d_free (self->pieces.array[i]);
	lisys_free (self->pieces.array);
	lisys_free (self);
}

/**
 * \brief Culls the polygon using a quad.
 * 
 * Only the remainder of the polygon is culler. Pieces already split out
 * of the polygon are not evaluated so it's critical that culling is done
 * strictly the strict bottom to top order.
 * 
 * \param self Polygon culler.
 * \param bot0 Bottom left vertex.
 * \param bot1 Bottom right vertex.
 * \param top0 Top left vertex.
 * \param top1 Top right vertex.
 */
int limat_polygon_culler_subtract_quad (
	LIMatPolygonCuller* self,
	const LIMatVector2d* bot0,
	const LIMatVector2d* bot1,
	const LIMatVector2d* top0,
	const LIMatVector2d* top1)
{
	LIMatLine2d l1;
	LIMatLine2d l2;
	LIMatPolygon2d* piece;
	LIMatPolygon2d** tmp;

	/* Do nothing if the whole polygon is consumed. */
	if (!self->remainder->vertices.count)
		return 1;

	/* Use the lower line to split a piece out of the polygon. */
	/* The piece is added to the output list since it doesn't
	 * intersect the quads above it given the ordering requirement.
	 */
	if (bot1->x > bot0->x)
		limat_line2d_init_from_points (&l1, bot1, bot0);
	else
		limat_line2d_init_from_points (&l1, bot0, bot1);
	piece = limat_polygon2d_new (self->remainder->ops, NULL, 0);
	if (piece == NULL)
		return 0;
	limat_polygon2d_split_inplace (self->remainder, &l1, piece);

	limat_polygon2d_remove_duplicates (piece, 0.001f);
	if (!limat_polygon2d_get_degenerate (piece))
	{
		/* Allocate space for the polygon. */
		tmp = lisys_realloc (self->pieces.array, (self->pieces.count + 1) * sizeof(LIMatPolygon2d*));
		if (tmp == NULL)
		{
			limat_polygon2d_free (piece);
			return 0;
		}

		/* Add the polygon into the array. */
		self->pieces.array = tmp;
		self->pieces.array[self->pieces.count++] = piece;
	}
	else
		limat_polygon2d_free (piece);

	/* Remove degenerate vertices. */
	/* If the culling line is parallel to one of the edges, the polygon
	 * may become degenerate instead of all the vertices being removed. */
	limat_polygon2d_remove_duplicates (self->remainder, 0.001f);
	if (limat_polygon2d_get_degenerate (self->remainder))
	{
		lisys_free (self->remainder->vertices.vertices);
		self->remainder->vertices.vertices = NULL;
		self->remainder->vertices.count = 0;
		return 1;
	}

	/* Use the upper line to split a piece out of the polygon. */
	/* The piece is discarded since it represents the portion that
	 * did intersect with the subtracted quad.
	 */
	if (top1->x > top0->x)
		limat_line2d_init_from_points (&l2, top1, top0);
	else
		limat_line2d_init_from_points (&l2, top0, top1);
	limat_polygon2d_clip_inplace (self->remainder, &l2);

	/* Remove degenerate vertices. */
	/* If the culling line is parallel to one of the edges, the polygon
	 * may become degenerate instead of all the vertices being removed. */
	limat_polygon2d_remove_duplicates (self->remainder, 0.001f);
	if (limat_polygon2d_get_degenerate (self->remainder))
	{
		lisys_free (self->remainder->vertices.vertices);
		self->remainder->vertices.vertices = NULL;
		self->remainder->vertices.count = 0;
	}

	return 1;
}

/** @} */
/** @} */
