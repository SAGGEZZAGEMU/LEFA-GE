/* LEFA ENGINE : api_manager.h
2023-2024 Sild Team 
*/

#ifndef _API_MANAGER_H_
#define _API_MANAGER_H_

typedef struct RenderInterface
{
	void(*init)(LE_Window *window);
	void(*update)(LE_Window *window);
	void(*setVSync)(int interval);
	void(*initTimer)();
	void(*limitFrames)(int targetFPS);
	void(*cleanup)(LE_Window *window);
}RenderInterface;

/* RENDER API LOADER */

#if defined _OPENGL /* OpenGL API */
#	define APIMSG " OpenGL "
#	define RMGRMSG
#	define GL_ONLY
#	include"api_manager/gl_manager.h"
/*=====================================================*/
#elif defined _OPENGLES /* OpenGL ES API */
#   define APIMSG " OpenGL ES " 
#   define RMGRMSG
#   define GLES_ONLY
#   include"api_manager/gl_manager.h"
/*=====================================================*/
#elif defined _VULKAN /* Vulkan API */
#   define APIMSG " Vulkan " 
#   define RMGRMSG
#   define VK_ONLY
#   include"api_manager/vk_manager.h"
/*=====================================================*/
#elif defined _BM_RENDER /* BOTH MODE OpenGL + Vulkan */
#   define APIMSG " OpenGL and Vulkan " 
#   define BM_API
#   define RMGRMSG
#   define GL_ONLY
#   define VK_ONLY
#   include"api_manager/gl_manager.h"
#   include"api_manager/vk_manager.h"

	typedef enum 
	{
    	RENDERER_OPENGL,
    	RENDERER_VULKAN
	} RendererType;


/*=====================================================*/
#else
#   pragma  message ("#====================================================================# ");
#   pragma  message ("# ALERT! No Render API Selected                                      # ");
#   pragma  message ("# ALERT! set OpenGL , OpenGL ES or Vulkan                            # ");
#   pragma  message ("# Example: ( #define _OPENGL )   for OpenGL API                      # ");
#   pragma  message ("# Example: ( #define _OPENGLES ) for OpenGL ES API                   # ");
#   pragma  message ("# Example: ( #define _VULKAN )   for Vulkan API                      # ");
#   pragma  message ("# or select dual mode to load opengl and vulkan elements together    # ");
#   pragma  message ("# Example: ( #define _BM_RENDER ) for to load both  APIS             # ");
#   pragma  message ("#====================================================================# ");
#endif 
/**/
#ifdef RMGRMSG
#	pragma message (" Render API selected : " APIMSG );
#endif

#ifdef GL_ONLY
	RenderInterface openglInterface = 
	{
    	.init =  GL_Init,
    	.update = GL_Update,
    	.setVSync = GL_SetVSync,
    	.cleanup = GL_Cleanup
	};
#endif 

#ifdef VK_ONLY
	RenderInterface vulkanInterface = 
	{
    	.init = VK_Init,
    	.update = VK_Update,
    	.cleanup = VK_Cleanup
	};
#endif















#endif /* _API_MANAGER_H_ */
