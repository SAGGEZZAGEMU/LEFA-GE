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

#ifndef __RENDER_TYPES_COMMON_H__
#define __RENDER_TYPES_COMMON_H__

#include"test_demo/model_test/all_model.h"
#include"test_demo/model_test/ANOTHER/math_only.h"

enum _LIRenFlags
{
	LIREN_FLAG_FIXED     = 0x01, /* Force fixed function. */
	LIREN_FLAG_SHADOW0   = 0x02, /* Sun casts shadows. */
	LIREN_FLAG_SHADOW1   = 0x04, /* Lamps cast shadows. */
	LIREN_FLAG_LIGHTING  = 0x08, /* Render with lighting. */
	LIREN_FLAG_TEXTURING = 0x10, /* Render with texturing. */
};

enum
{
	LIREN_MATERIAL_FLAG_BILLBOARD = 0x010000,
	LIREN_MATERIAL_FLAG_CULLFACE = 0x020000,
	LIREN_MATERIAL_FLAG_TRANSPARENCY = 0x040000,
	LIREN_MATERIAL_FLAG_SORTFACES = 0x080000
};

/*
#ifdef __cplusplus
	class LIRenRender;
#else
	typedef void* LIRenRender;
#endif
*/

typedef enum _LIRenFlags LIRenFlags;
typedef struct _LIRenVideomode LIRenVideomode;

struct _LIRenVideomode
{
	int width;
	int height;
	int fullscreen;
	int sync;
	int multisamples;
};

typedef struct _LIRenStats LIRenStats;
struct _LIRenStats
{
	int attachment_count;
	int batch_count;
	int entity_count;
	int face_count;
	int material_count;
	int material_count_loaded;
	int mesh_count;
	int mesh_memory;
	int skeleton_count;
	int texture_count;
	int texture_count_loaded;
	int texture_memory;
};

#endif
