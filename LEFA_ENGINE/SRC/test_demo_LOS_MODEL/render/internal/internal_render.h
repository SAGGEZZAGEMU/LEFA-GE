#ifndef __RENDER_INTERNAL_RENDER_H__
#define __RENDER_INTERNAL_RENDER_H__

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


#define LIREN_RESOURCES_PERMANENT "permanent"
#define LIREN_RESOURCES_TEMPORARY "temporary"

#include<stdbool.h>

//#include"lipsofsuna/paths.h"
#include"test_demo/model_test/ANOTHER/paths.h"
#include"../render_types.h"
//#include"../overlay/r_overlay_manager.h"


/*
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreMaterialManager.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <OgreLogManager.h>
*/


/*
class LIRenAnimationUpdater;
class LIRenMaterialUtils;
class LIRenModel;
class LIRenObject;
class LIRenOverlayManager;
class LIRenResourceLoadingListener;
*/

typedef struct _LIRenRender  LIRenRender;
struct _LIRenRender  
{
	int 		 					anisotropy;
	LIAlgRandom  					random;
	LIAlgStrdic* 					fonts;
	LIAlgU32dic* 					lights;
	LIAlgU32dic* 					models;
	LIAlgU32dic* 					objects;
	LIPthPaths*  					paths;
	LIRenVideomode 					mode;

	float 							unload_timer;
	//std::vector<LIRenViewport> 		viewports; //<Array of viewports and associated names and cameras.
/*
	LIRenOverlayManager* 			overlay_mgr;
	LIRenMaterialUtils* 			material_utils;
	LIRenResourceLoadingListener* 	resource_loading_listener; */
	LIRenUniqueId 					id;
	LIAlgStrdic* 					mesh_builders;
/*
	LIRenAnimationUpdater* 			animation_updater;

/*
	Ogre::RenderWindow* 			render_window;
	Ogre::RenderSystem* 			render_system;
	Ogre::SceneManager* 			scene_manager;
	Ogre::SceneNode* 				scene_root;
	Ogre::TextureManager* 			texture_manager;
	Ogre::LogManager* 				log;
	Ogre::MaterialManager* 			material_manager;
	Ogre::Root* 					root;
*/
};


typedef struct _LIRenViewport LIRenViewport;
struct _LIRenViewport 
{
	char name[256]; 			/* <The name of the view port. */
	//Ogre::Camera* camera; 	/* <The camera used for this viewport. **/
	vec3 position; 				/* <The position of the camera, relative to the unrotated viewer. */
	//Ogre::Viewport* viewport; /* <The viewport itself. */
	int zorder;
};


/* include this after declare structs */
#include"r_object.h" 
#include"r_model.h"
#include"r_unique_id.h"

/*
int init(LIRenVideomode* mode);

void deinit();

void add_compositor(const char* name);

void remove_compositor(const char* name);

void add_viewport(const char* name,
				  float view_rect[4],
				  float camera_pos[3]);

void remove_viewport(const char* name);

void remove_all_viewports();

void add_object(LIRenObject* object);

void remove_object(LIRenObject* object);

LIRenModel* find_model(int id);

int layout_text(const char* font,
				const char* text,
				int         width_limit,
				int**       result_glyphs,
				int*        result_glyphs_num);

int load_font(const char* name,
			  const char* file,
			  int         size);

void load_resources();

int measure_text(const char* font,
				 const char* text,
				 int         width_limit,
				 int*        result_width,
				 int*        result_height);

void project(const vec3* world,
			 vec3*       screen);

void reload(int pass);

void render();

int screenshot(const char* path);

int update(float secs);

void update_animations(float secs);

int get_anisotropy();

void set_anisotropy(int value);

void set_camera_far(float value);

void set_camera_near(float value);

void set_camera_transform(const LIMatTransform* value);

float get_opengl_version();

void set_material_scheme(const char* value);

void set_scene_ambient(const float* value);

void set_skybox(const char* value);

void get_stats(LIRenStats* result);

void update_texture(const char* name,
					int 		width,
					int 		height,
					const void* pixels);

void set_title(const char* value);

int set_videomode(LIRenVideomode* mode);

int get_videomodes(LIRenVideomode** modes,
				   int*             modes_num);
/*
bool init_window(LIRenVideomode*                  mode,
			     Ogre::RenderSystemCapabilities*& retry);
*
void deinit_window();

//int count_resources(const Ogre::ResourceManager& manager);

//int count_resources_loaded(const Ogre::ResourceManager& manager);

void load_plugin(const char* name);

int check_plugin(const char* name);

void garbage_collect_builders();

//void unload_unused_resources(Ogre::ResourceManager& manager);

void update_mode();
*/

#endif  /* __RENDER_INTERNAL_RENDER_H__ */
