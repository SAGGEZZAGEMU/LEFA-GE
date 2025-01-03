/*Open LEFA Project : loadEngine.h
2023*/

#ifndef _LOADENGINE_H
#define _LOADENGINE_H

/* main engine loader
*/

#include"core/window_manager.h"


/* Engine Files */

 /* api renderer loader */
 #include"render/api_manager.h" /* Mandatory Inclusion */
 /*#include"lefa/contents/system_choice.h"  /* basic engine defs */
 /*#include"lefa/contents/engineArgs.h"     /* engine Arguments */

#ifdef LOAD_FILE_MNGR
 #include"lefa/contents/LESYS/LEFILESYS/file_manager.h"  /* basic File Manager */
#endif

#ifdef LOAD_CTUTILS
 #include"contents/console_utils.h" /* console Utils */
#endif

#ifdef LOAD_MATH_LIB
 #include"utils/load_math.h" /* Math Library : HYPATIA mathc */
#endif

#ifdef LOAD_LODEPNG /* PNG Decoder and Decoder */
 #include"3rdparty/lodepng/lodepng.h"
#endif

#ifdef LOAD_TPE     /* Physics engine made in C */
 #include"lefa/3rdparty/tpe/tinyphysicsengine.h"
#endif

#ifdef LOAD_OBJLOADER   /* OBJ Loader */
#   define TINYOBJ_LOADER_C_IMPLEMENTATION
 #include"lefa/3rdparty/tol/objloader.h"
#endif

#ifdef LOAD_NKGUI /* NUKLEAR GUI*/
#   define NK_IMPLEMENTATION
#	define NK_INCLUDE_FIXED_TYPES
#	define NK_INCLUDE_STANDARD_IO
//#	define NK_INCLUDE_STANDARD_VARARGS
#	define NK_INCLUDE_DEFAULT_ALLOCATOR
#	define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#	define NK_INCLUDE_FONT_BAKING
#	define NK_INCLUDE_DEFAULT_FONT
#include"3rdparty/nuklear/nuklear.h"
#endif

#endif /* _LOADENGINE_H */

