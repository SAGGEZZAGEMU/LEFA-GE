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

#ifndef __RENDER_FONT_FONT_H__
#define __RENDER_FONT_FONT_H__

#include <wchar.h>
/*
#include "lipsofsuna/algorithm.h"
#include "lipsofsuna/system.h"
*/

#include"../model_test/ANOTHER/allsystem.h"
#include"../model_test/ANOTHER/algorithm.h"


typedef struct _LIFntFont LIFntFont;

LIAPICALL(LIFntFont*, lifnt_font_new, (const char* name,
									   const char* file,
									   int         size));

LIAPICALL (void, lifnt_font_free, (
	LIFntFont* self));

LIAPICALL (void, lifnt_font_reload, (
	LIFntFont* self,
	int        pass));

LIAPICALL (int, lifnt_font_get_advance, (
	LIFntFont* self,
	wchar_t    glyph));

LIAPICALL (int, lifnt_font_get_ascent, (
	LIFntFont* self));

LIAPICALL (int, lifnt_font_get_height, (
	const LIFntFont* self));

#endif
