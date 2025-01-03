/* LEFA Engine : gl_manager.h
2023 SILD Team
*/

#ifndef _GL_MANAGER_H_
#define _GL_MANAGER_H_

/*
 ! NOTE ! this is just an opengl command limiter, 
    in case you want to use only version-specific commands.

This will not change the version of OpenGL your system will use
*/

/* OpenGL short */
#ifdef GL_VERSION_LIMIT
    /* OpenGL version */
    #if defined GL2_0
        #   define SHADER_110
        #   define GL_V "2.0"
        #   define GL_VERSION_2_1
        #   define GL_VERSION_3_0
        #   define GL_VERSION_3_1
        #   define GL_VERSION_3_2
        #   define GL_VERSION_3_3
        #   define GL_VERSION_4_0
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL2_1
        #   define SHADER_120
        #   define GL_V "2.1"
        #   define GL_VERSION_3_0
        #   define GL_VERSION_3_1
        #   define GL_VERSION_3_2
        #   define GL_VERSION_3_3
        #   define GL_VERSION_4_0
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL3_0
        #   define SHADER_130
        #   define GL_V "3.0"
        #   define GL_VERSION_3_1
        #   define GL_VERSION_3_2
        #   define GL_VERSION_3_3
        #   define GL_VERSION_4_0
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL3_1
        #   define SHADER_140
        #   define GL_V "3.1"
        #   define GL_VERSION_3_2
        #   define GL_VERSION_3_3
        #   define GL_VERSION_4_0
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL3_2
        #   define SHADER_150
        #   define GL_V "3.2"
        #   define GL_VERSION_3_3
        #   define GL_VERSION_4_0
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL3_3
        #   define SHADER_330
        #   define GL_V "3.3"
        #   define GL_VERSION_4_0
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL4_0
        #   define shader_400
        #   define GL_V "4.0"
        #   define GL_VERSION_4_1
        #   define GL_VERSION_4_2
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL4_1
        #   define SHADER_410
        #   define GL_V "4.1"
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GL4_2
        #   define SHADER_420
        #   define GL_V "4.2"
        #   define GL_VERSION_4_3
        #   define GL_VERSION_4_4
        #   define GL_VERSION_4_5
        #   define GL_VERSION_4_6
        #   define LOAD_GLADGL
        #   define GLVMSG
    /**/
    #elif defined GLES2_0
        #   define SHADER_ES100
        #   define GL_V "ES 2.0"
        #   define LOAD_GLADGLES
        #   define GLVMSG
    /**/
    #elif defined GLES3_0
        #   define SHADER_ES300
        #   define GL_V "ES 3.0"
        #   define LOAD_GLADGLES
        #   define GLVMSG
    /**/
    #elif defined GLES3_1
        #   define SHADER_ES310
        #   define GL_V "ES 3.1"
        #   define LOAD_GLADGLES
        #   define GLVMSG
    /**/
    #elif defined GLES3_2
        #   define SHADER_ES320
        #   define GL_V "ES 3.2"
        #   define LOAD_GLADGLES
        #   define GLVMSG
    /**/
    #elif defined GLNONE
    /*  GLNONE
        This option will load all of GLAD GL at once, 
        without limiting it to one version of OpenGL, 
        that is, it will use up to the latest version of OpenGL. 
    */
    #   pragma  message (" All OpenGL versions loaded ");
    #   define LOAD_GLADGL
    /**/
    #else
        #define GL_V "NOT DEFINED!"
        #   pragma  message ("#================================================# ");
        #   pragma  message ("# ALERT! No OpenGL Version Selected              # ");
        #   pragma  message ("# ALERT! set an opengl version to load           # ");
        #   pragma  message ("# Example: ( #define GL2_1 ) for Opengl 2.1      # ");
        #   pragma  message ("# Example: ( #define GL3_0 ) for Opengl 3.0      # ");
        #   pragma  message ("#================================================# ");
    #endif

#endif /*GL_VERSION_LIMIT */

#ifdef GLVMSG
    #pragma message ( " OpenGL Version : " GL_V );
#endif

/* GLU_INCLUDE
    activating this causes several errors, 
    creates confusion with the glad library  
*/
#ifdef GLU_INCLUDE 
    #include<gl/glu.h>
    #undef GL_ZERO
    #undef GL_ONE
    #undef GL_NONE
    #undef GL_NO_ERROR
    #undef GL_ALL_ATTRIB_BITS
    #undef GL_LOGIC_OP
    #undef GL_TEXTURE_COMPONENTS
#endif /* GLU_INCLUDE */

#ifdef LOAD_GLADGL
#   include"3rdparty/glad/OpenGL/glad_gl.h"
#endif
#ifdef LOAD_GLADGLES
#   include"3rdparty/glad/OpenGL_ES/glad_gles.h"
#endif

/*
#include"lefa/render/Shader/shaderv.h"
#include"lefa/render/Shader/shaderLoader.h"
*/

#include<core/window_manager.h>

void GL_Init(LE_Window *window);
void GL_Update(LE_Window *window);
void GL_SetVSync(int interval);
void GL_Cleanup(LE_Window *window);






#endif /* _GL_MANAGER_H_ */


