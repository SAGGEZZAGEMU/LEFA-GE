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
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenRender Render
 * @{
 */

#include "render.h"
#include "render-animation-updater.hpp"
#include "render-material-utils.hpp"
#include "render-mesh-builder.hpp"
#include "render-model.hpp"
#include "render-object.hpp"
#include "render-resource-loading-listener.hpp"
#include "../render-types.h"
#include "../font/font.h"
#include "../font/font-layout.h"
#include "../overlay/render-overlay-manager.hpp"

#include <OgreCompositorManager.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgrePlugin.h>
#include <OgreShadowCameraSetupLiSPSM.h>
#include <OgreRenderSystemCapabilitiesManager.h>
#include <OgreRenderSystemCapabilitiesSerializer.h>
#include <OgreResourceBackgroundQueue.h>
#include <OgreSkeletonManager.h>

#include <OgreWindowEventUtilities.h>
#ifdef HAVE_XLIB
#include <X11/Xlib.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#endif

#define LIREN_RENDER_TEXTURE_UNLOAD_TIME 10

/*****************************************************************************/

int LIRenRender::init(LIRenVideomode* mode)
{
	char* log1;
	Ogre::String data1 (paths->module_data);

	/* Initialize the private data. */
	unload_timer = 0.0f;
	mesh_builders = lialg_strdic_new ();

	/* Disable console output. */
	log = new Ogre::LogManager ();
	log1 = lisys_path_concat (paths->module_data_save, "render.log", NULL);
	if (log1 != NULL)
	{
		log->createLog (log1, true, false, false);
		lisys_free (log1);
	}
	else
		log->createLog ("render.log", true, false, false);

	/* Create the window. */
	Ogre::RenderSystemCapabilities* retry = NULL;
	while (!init_window (mode, retry))
	{
		deinit_window ();
		if (!retry)
			return 0;
	}

	/* Initialize the scene manager. */
	scene_manager = root->createSceneManager("OctreeSceneManager", "DefaultSceneManager");
	scene_manager->setAmbientLight (Ogre::ColourValue (0.5, 0.5, 0.5));
	scene_manager->setShadowTechnique (Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
	scene_manager->setShadowTextureSelfShadow (false);
	scene_manager->setShadowCasterRenderBackFaces (false);
	scene_manager->setShadowFarDistance (50.0f);
	scene_manager->setShadowDirLightTextureOffset (0.6f);
	Ogre::LiSPSMShadowCameraSetup* shadow_camera = new Ogre::LiSPSMShadowCameraSetup ();
	shadow_camera->setOptimalAdjustFactor (2.0f);
	scene_manager->setShadowCameraSetup (Ogre::ShadowCameraSetupPtr (shadow_camera));
	scene_manager->setShadowTextureCount (3);
	scene_manager->setShadowTextureSize (1024);
	scene_root = scene_manager->getRootSceneNode ();

	/* Initialize the camera and viewport. */
	float view_rect[4] = {0, 0.0f, 1.0f, 1.0f};
	float camera_pos[3] = {0.0, 0, 0};
	add_viewport("Camera", view_rect, camera_pos);

	/* Initialize the user interface. */
	overlay_mgr = new LIRenOverlayManager (this);

	/* Create the group for temporary resources. */
	/* This group is used for temporary resources such as meshes or
	   materials instantiated and overridden for specific meshes. We
	   want this kind of resources to be completely purged when unused. */
	Ogre::ResourceGroupManager& mgr = Ogre::ResourceGroupManager::getSingleton ();
	mgr.createResourceGroup (LIREN_RESOURCES_TEMPORARY);

	/* Allow overriding of resources. */
	resource_loading_listener = new LIRenResourceLoadingListener (paths);
	mgr.setLoadingListener (resource_loading_listener);

	/* Inialize texture and material managers. */
	texture_manager = &Ogre::TextureManager::getSingleton ();
	texture_manager->setDefaultNumMipmaps (5);
	material_manager = &Ogre::MaterialManager::getSingleton ();
	material_utils = new LIRenMaterialUtils(this);

	return 1;
}

void LIRenRender::deinit ()
{
	/* Free the animation updater. */
	delete animation_updater;
	animation_updater = NULL;

	/* Free the mesh builders. */
	/* Some meshes may be being loaded in other threads so the Ogre root
	   needs to be shut down first to guarantee clean shutdown. */
	root->shutdown ();
	garbage_collect_builders ();
	lialg_strdic_free (mesh_builders);

	/* Free the Ogre root. */
	delete root;
	delete resource_loading_listener;
	delete material_utils;
	delete overlay_mgr;
	delete log;
}

/**
 * \brief Enables a compositor script.
 * \param name Compositor script name.
 */
void LIRenRender::add_compositor (
	const char* name)
{
	for(size_t i = 0; i < viewports.size(); i++) {
		if(Ogre::CompositorManager::getSingleton ().addCompositor (viewports[i].viewport, name)) {
			Ogre::CompositorManager::getSingleton ().setCompositorEnabled (viewports[i].viewport, name, true);
		} else {
			printf("Could not add compositor %s to view %s.\n", name, viewports[i].name.c_str());
		}
	}
}

/**
 * \brief Disables a compositor script.
 * \param name Compositor script name.
 */
void LIRenRender::remove_compositor (
	const char* name)
{
	for(size_t i = 0; i < viewports.size(); i++) {
		Ogre::CompositorManager::getSingleton ().removeCompositor (viewports[i].viewport, name);
	}
}

/**
 * \brief Add a viewport, or updates a viewport if it already exists.
 * \param name The name of the viewport to remove.
 * \param view_rect An array of 4 floats with the left, top, width, and height of the viewport.
 * \param camera_pos An array of 3 floats with the unrotated x, y, and z position of the camera relative to the viewer.
 */
void LIRenRender::add_viewport (
	const char* name,
	float view_rect[4],
	float camera_pos[3])
{
	for(size_t i = 0; i < viewports.size(); i++) {
		if(viewports[i].name == name) {
			viewports[i].viewport->setDimensions(view_rect[0], view_rect[1], view_rect[2], view_rect[3]);
			viewports[i].position = Ogre::Vector3(camera_pos[0], camera_pos[1], camera_pos[2]);
			return;
		}
	}
	int zorder = 0;
	if(viewports.size() > 0) {
		zorder = viewports.back().zorder + 1;
	}
	Ogre::Camera* camera = scene_manager->createCamera(name);
	LIRenViewport view = {
		name,
		camera,
		Ogre::Vector3(camera_pos[0], camera_pos[1], camera_pos[2]),
		render_window->addViewport(camera, zorder, view_rect[0], view_rect[1], view_rect[2], view_rect[3]),
		zorder};
	viewports.push_back(view);
}

/**
 * \brief Remove a single named viewport.
 * \param name The name of the viewport to remove.
 */
void LIRenRender::remove_viewport (
	const char* name)
{
	for(size_t i = 0; i < viewports.size(); i++) {
		if(viewports[i].name == name) {
			render_window->removeViewport(viewports[i].zorder);
			scene_manager->destroyCamera(viewports[i].camera);
			viewports.erase(viewports.begin() + i);
			break;
		}
	}
}

/**
 * \brief Removes all viewports.
 */
void LIRenRender::remove_all_viewports ()
{
	for(size_t i = 0; i < viewports.size(); i++) {
		render_window->removeViewport(viewports[i].zorder);
		scene_manager->destroyCamera(viewports[i].camera);
	}
	viewports.clear();
}




/**
 * \brief Registers an object.
 * \param object Object.
 */
void LIRenRender::add_object (
	LIRenObject* object)
{
	lialg_u32dic_insert (objects, object->get_id (), object);
	if (animation_updater)
		animation_updater->object_added (object);
}

/**
 * \brief Unregisters an object.
 * \param object Object.
 */
void LIRenRender::remove_object (
	LIRenObject* object)
{
	if (animation_updater)
		animation_updater->object_removed (object);
	lialg_u32dic_remove (objects, object->get_id ());
}

/**
 * \brief Finds a model by ID.
 * \param id Model ID.
 * \return Model.
 */
LIRenModel* LIRenRender::find_model (
	int id)
{
	return (LIRenModel*) lialg_u32dic_find (models, id);
}

int LIRenRender::layout_text (
	const char* font,
	const char* text,
	int         width_limit,
	int**       result_glyphs,
	int*        result_glyphs_num)
{
	int i;
	int h;
	LIFntFont* font_;
	LIFntLayout* layout;

	font_ = (LIFntFont*) lialg_strdic_find (fonts, font);
	if (font_ == NULL)
		return 0;

	layout = lifnt_layout_new ();
	if (layout == NULL)
		return 0;

	lifnt_layout_set_width_limit (layout, width_limit);
	lifnt_layout_append_string (layout, font_, text);
	lifnt_layout_update (layout);
	if (layout->n_glyphs)
	{
		*result_glyphs_num = layout->n_glyphs;
		*result_glyphs = (int*) lisys_calloc (layout->n_glyphs, 3 * sizeof (int));
		if (*result_glyphs == NULL)
			return 0;
		h = lifnt_font_get_height (font_);
		for (i = 0 ; i < layout->n_glyphs ; i++)
		{
			(*result_glyphs)[3 * i] = layout->glyphs[i].x;
			(*result_glyphs)[3 * i + 1] = layout->glyphs[i].y - h;
			(*result_glyphs)[3 * i + 2] = layout->glyphs[i].advance;
		}
	}
	else
	{
		*result_glyphs_num = 0;
		*result_glyphs = NULL;
	}
	lifnt_layout_free (layout);

	return 1;
}

int LIRenRender::load_font (
	const char* name,
	const char* file,
	int         size)
{
	LIFntFont* font;

	/* Check for existing. */
	font = (LIFntFont*) lialg_strdic_find (fonts, name);
	if (font != NULL)
		return 1;

	/* Load the font. */
	font = lifnt_font_new (name, file, size);
	if (font == NULL)
		return 0;

	/* Add to the dictionary. */
	if (!lialg_strdic_insert (fonts, name, font))
	{
		lifnt_font_free (font);
		return 0;
	}

	return 1;
}

void LIRenRender::load_resources ()
{
	LIAlgList* ptr;

	Ogre::ResourceGroupManager& mgr = Ogre::ResourceGroupManager::getSingleton ();

	/* Create the group for permanent resources. */
	/* This group is used for resources that are managed by Ogre. They
	   include textures and various scripts detected at initialization.
	   Out of these, we only want to unload textures and even for them
	   we want to keep the resource info available at all times. */
	Ogre::String group = LIREN_RESOURCES_PERMANENT;
	if (paths->paths != NULL)
	{
		for (ptr = paths->paths ; ptr->next != NULL ; ptr = ptr->next)
			{}
		for ( ; ptr != NULL ; ptr = ptr->prev)
		{
			const char* dir = (const char*) ptr->data;
			mgr.addResourceLocation (dir, "FileSystem", group, false);
		}
	}

	/* Load the resources. */
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups ();
}

int LIRenRender::measure_text (
	const char* font,
	const char* text,
	int         width_limit,
	int*        result_width,
	int*        result_height)
{
	LIFntFont* font_;
	LIFntLayout* layout;

	font_ = (LIFntFont*) lialg_strdic_find (fonts, font);
	if (font_ == NULL)
		return 0;

	layout = lifnt_layout_new ();
	if (layout == NULL)
		return 0;

	if (width_limit != -1)
		lifnt_layout_set_width_limit (layout, width_limit);
	lifnt_layout_append_string (layout, font_, text);
	lifnt_layout_update (layout);
	*result_width = lifnt_layout_get_width (layout);
	*result_height = lifnt_layout_get_height (layout);
	lifnt_layout_free (layout);

	return 1;
}

/**
 * \brief Projects a point in the world space to the screen space.
 * \param world Point in the world space.
 * \param screen Return location for the point in the screen space.
 */
void LIRenRender::project(const LIMatVector* world, LIMatVector* screen)
{
	//todo: properly handle the case of when there is more than 1 view port.
	Ogre::Matrix4 proj = viewports[0].camera->getProjectionMatrix();
	Ogre::Matrix4 view = viewports[0].camera->getViewMatrix();
	Ogre::Vector3 w (world->x, world->y, world->z);
	Ogre::Vector3 s = proj * view * w;
	screen->x = (0.5f + 0.5f * s.x) * mode.width;
	screen->y = (0.5f - 0.5f * s.y) * mode.height;
	screen->z = s.z;
}

/**
 * \brief Reloads all images, shaders and other graphics state.
 *
 * This function is called when the video mode changes in Windows. It
 * reloads all data that was lost when the context was erased.
 *
 * \param pass Reload pass.
 */
void LIRenRender::reload(int pass)
{
}

/**
 * \brief Renders the overlays.
 */
void LIRenRender::render()
{
	/* Render a frame. */
	root->renderOneFrame ();
}

int LIRenRender::screenshot(const char* path)
{
	render_window->writeContentsToFile(path);

	return 1;
}

/**
 * \brief Updates the renderer state.
 * \param secs Number of seconds since the last update.
 * \return Nonzero when running, zero if the window was closed.
 */
int LIRenRender::update(float secs)
{
	LIAlgU32dicIter iter1;
	LIRenObject* object;

	/* Update objects. */
	LIALG_U32DIC_FOREACH (iter1, objects)
	{
		object = (LIRenObject*) iter1.value;
		object->update (secs);
	}

	/* Update the backend. */
	Ogre::WindowEventUtilities::messagePump ();
	if (render_window->isClosed ())
		return 0;
	update_mode ();

	/* Update the aspect ratio of the camera. */
	for(size_t i = 0; i < viewports.size(); i++) {
		Ogre::Real w = viewports[i].viewport->getActualWidth ();
		Ogre::Real h = viewports[i].viewport->getActualHeight ();
		viewports[i].camera->setAspectRatio (w / h);
	}

	/* Free unused resources. */
	/* Ogre internals seem to be sloppy with using resource pointers. At
	   least the terrain system will crash if we try to remove all unused
	   resources. We need to limit unloading to our own resource group. */
	/* Ogre seems to not have a function for removing unreferenced
	   resources from a specific group so we need to do it manually. */
	unload_timer += secs;
	if (unload_timer > 1.0f)
	{
		unload_timer = 0.0f;
		unload_unused_resources (Ogre::MeshManager::getSingleton ());
		unload_unused_resources (Ogre::SkeletonManager::getSingleton ());
		unload_unused_resources (Ogre::MaterialManager::getSingleton ());
		unload_unused_resources (Ogre::TextureManager::getSingleton ());
		garbage_collect_builders ();
	}

	/* Apply updated animations. */
	if (animation_updater != NULL)
	{
		animation_updater->join();
		delete animation_updater;
		animation_updater = NULL;
	}

	return 1;
}

/**
 * \brief Updates the animations of the scene.
 * \param secs Number of seconds since the last update.
 */
void LIRenRender::update_animations(float secs)
{
	if (animation_updater == NULL)
		animation_updater = new LIRenAnimationUpdater (this, secs);
}

int LIRenRender::get_anisotropy() const
{
	return anisotropy;
}

void LIRenRender::set_anisotropy(int value)
{
	if (value != anisotropy)
	{
		anisotropy = value;
		material_manager->setDefaultAnisotropy (value);
		if (value)
			material_manager->setDefaultTextureFiltering (Ogre::TFO_ANISOTROPIC);
		else
			material_manager->setDefaultTextureFiltering (Ogre::TFO_BILINEAR);
		/* TODO: Update texture units? */
	}
}

/**
 * \brief Sets the far plane distance of the camera.
 * \param value Distance.
 */
void LIRenRender::set_camera_far(float value)
{
	for(size_t i = 0; i < viewports.size(); i++) 
	{
		viewports[i].camera->setFarClipDistance (value);
	}
}

/**
 * \brief Sets the near plane distance of the camera.
 * \param value Distance.
 */
void LIRenRender::set_camera_near(float value)
{
	for(size_t i = 0; i < viewports.size(); i++) 
	{
		viewports[i].camera->setNearClipDistance (value);
	}
}

/**
 * \brief Sets the position and orientation of the camera.
 * \param value Transformation.
 */
void LIRenRender::set_camera_transform (
	const LIMatTransform* value)
{
	Ogre::Vector3 pos(value->position.x, value->position.y, value->position.z);
	Ogre::Quaternion rot(value->rotation.w, value->rotation.x, value->rotation.y, value->rotation.z);
	for(size_t i = 0; i < viewports.size(); i++) {
		viewports[i].camera->setPosition (pos + rot * viewports[i].position);
		viewports[i].camera->setOrientation(rot);
	}
}

/**
 * \brief Sets the name of the preferred Ogre material scheme.
 * \param value Material scheme name.
 */
void LIRenRender::set_material_scheme(const char* value)
{
	for(size_t i = 0; i < viewports.size(); i++) {
		viewports[i].viewport->setMaterialScheme (value);
	}
}

/**
 * \brief Sets the ambient color.
 * \param value Array of 3 floats.
 */
void LIRenRender::set_scene_ambient(const float* value)
{
	scene_manager->setAmbientLight (Ogre::ColourValue (value[0], value[1], value[2]));
}

/**
 * \brief Sets the skybox material.
 * \param value Material name.
 */
void LIRenRender::set_skybox(const char* value)
{
	if (value != NULL)
	{
		try
		{
			scene_manager->setSkyBox (true, value, 10.0f, true);
			return;
		}
		catch (...)
		{
		}
	}
	scene_manager->setSkyBox (false, "");
}

void LIRenRender::get_stats(LIRenStats* result) const
{
	//todo: Handle the case of more than 1 view port.
	result->batch_count = viewports[0].viewport->_getNumRenderedBatches ();
	result->face_count = viewports[0].viewport->_getNumRenderedFaces ();
	result->material_count = count_resources (Ogre::MaterialManager::getSingleton ());
	result->material_count_loaded = count_resources_loaded (Ogre::MaterialManager::getSingleton ());
	result->mesh_count = count_resources (Ogre::MeshManager::getSingleton ());
	result->mesh_memory = (int) Ogre::MeshManager::getSingleton ().getMemoryUsage ();
	result->skeleton_count = count_resources (Ogre::SkeletonManager::getSingleton ());
	result->texture_count = count_resources (Ogre::TextureManager::getSingleton ());
	result->texture_count_loaded = count_resources_loaded (Ogre::TextureManager::getSingleton ());
	result->texture_memory = (int) Ogre::TextureManager::getSingleton ().getMemoryUsage ();

	/* Count entities. */
	result->entity_count = 0;
	Ogre::SceneManager::MovableObjectIterator iterator = scene_manager->getMovableObjectIterator ("Entity");
	while (iterator.hasMoreElements ())
	{
		result->entity_count++;
		iterator.getNext ();
	}

	/* Count attachments. */
	result->attachment_count = 0;
	LIAlgU32dicIter iter;
	LIALG_U32DIC_FOREACH (iter, objects)
	{
		LIRenObject* object = (LIRenObject*) iter.value;
		result->attachment_count += object->get_attachment_count ();
	}
}

void LIRenRender::update_texture (
	const char* name,
	int width,
	int height,
	const void* pixels)
{
	Ogre::Image img;
	img.loadDynamicImage ((Ogre::uchar*) pixels, width, height, 1, Ogre::PF_A8B8G8R8);
	texture_manager->loadImage(name, LIREN_RESOURCES_PERMANENT, img, Ogre::TEX_TYPE_2D, 0);
}

void LIRenRender::set_title(const char* value)
{
#ifdef HAVE_XLIB
	Display* display;
	Window window;
	render_window->getCustomAttribute ("DISPLAY", &display);
	render_window->getCustomAttribute ("WINDOW", &window);
	XStoreName (display, window, value);
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND window;
	render_window->getCustomAttribute ("WINDOW", &window);
	SetWindowText (window, value);
#else
	/* TODO */
#endif
}

int LIRenRender::set_videomode(LIRenVideomode* mode)
{
	if (mode->fullscreen)
		render_window->setFullscreen (true, mode->width, mode->height);
	else
		render_window->setFullscreen (false, mode->width, mode->height);
	return 1;
}

int LIRenRender::get_videomodes(LIRenVideomode** modes,
	int*             modes_num) const
{
	/* Count modes. */
	const Ogre::StringVector& list = render_system->
		getConfigOptions()["Video Mode"].possibleValues;
	if (!list.size ())
		return 0;

	/* Allocate the result. */
	*modes = (LIRenVideomode*) lisys_calloc (list.size (), sizeof (LIRenVideomode));
	if (*modes == NULL)
		return 0;
	*modes_num = list.size ();

	/* Convert modes. */
	for (int i = 0 ; i < *modes_num ; i++)
	{
		int w, h;
		if (sscanf (list[i].c_str (), "%d x %d", &w, &h) == 2)
		{
			(*modes)[i].width = w;
			(*modes)[i].height = h;
			(*modes)[i].fullscreen = 1;
			(*modes)[i].sync = mode.sync;
			(*modes)[i].multisamples = mode.multisamples;
		}
	}

	return 1;
}

/*****************************************************************************/

bool LIRenRender::init_window (
	LIRenVideomode*                  mode,
	Ogre::RenderSystemCapabilities*& retry)
{
	/* Initialize the Ogre root. */
	root = new Ogre::Root("", "", "");

	/* Load plugins. */
	load_plugin ("RenderSystem_GL");
	load_plugin ("Plugin_OctreeSceneManager");
	load_plugin ("Plugin_ParticleFX");

	/* Make sure that the required plugins were loaded. */
	if (!check_plugin ("GL RenderSystem"))
	{
		retry = NULL;
		return 0;
	}

	/* Initialize the render system. */
	render_system = root->getRenderSystemByName ("OpenGL Rendering Subsystem");
	if (!(render_system->getName () == "OpenGL Rendering Subsystem"))
	{
		retry = NULL;
		return 0;
	}

	/* Choose the video mode. */
	Ogre::String fsaa = Ogre::StringConverter::toString (mode->multisamples);
	Ogre::String video_mode =
		Ogre::StringConverter::toString (mode->width) + " x " +
		Ogre::StringConverter::toString (mode->height);
	render_system->setConfigOption ("Full Screen", mode->fullscreen? "Yes" : "No");
	render_system->setConfigOption ("VSync", mode->sync? "Yes" : "No");
	render_system->setConfigOption ("Video Mode", video_mode);
	render_system->setConfigOption ("FSAA", fsaa);

	/* Initialize the render window. */
	root->setRenderSystem (render_system);

	/* Override the render capabilities. */
	/* This is used to disable shaders either due to a bad video card or a
	   manual trigger via an environment variable. */
	if (retry)
		root->useCustomRenderSystemCapabilities (retry);

	/* Create the main window. */
	render_window = root->initialise (true, "Lips of Suna");

	/* Hack for old Intel cards. */
	/* Cards that report OpenGL older than 2.1 really should be using shaders
	   at all. Intel GMA 31xx takes 20 seconds to render the main menu with
	   shaders enabled and some driver versions will crash straight out. */
	if (!retry)
	{
		/* Check for the driver version. */
		const Ogre::RenderSystemCapabilities* caps = render_system->getCapabilities ();
		Ogre::DriverVersion version = caps->getDriverVersion ();
		if (version.major < 2 || (version.major == 2 && version.minor < 1))
		{
			printf ("WARNING: Driver reported version %d.%d.%d.%d but 2.1 is required\n",
				version.major, version.minor, version.release, version.build);
			if (!getenv ("LOS_FORCE_ENABLE_SHADERS"))
			{
				printf ("WARNING: Disabling shaders because OpenGL 2.1 is not supported.\n");
				retry = render_system->createRenderSystemCapabilities ();
			}
		}

		/* Check for manual shader disable. */
		if (!retry && getenv ("LOS_FORCE_DISABLE_SHADERS"))
		{
			printf ("WARNING: Forcing shaders to be disabled.\n");
			retry = render_system->createRenderSystemCapabilities ();
		}

		/* Create new rendercaps and retry if shaders were disabled. */
		if (retry)
		{
			retry->unsetCapability (Ogre::RSC_VERTEX_PROGRAM);
			retry->unsetCapability (Ogre::RSC_FRAGMENT_PROGRAM);
			retry->removeShaderProfile ("arbfp1");
			retry->removeShaderProfile ("arbvp1");
			retry->removeShaderProfile ("glsl");
			return false;
		}
	}

	/* Update the video mode. */
	this->mode = *mode;
	update_mode ();

	return true;
}

void LIRenRender::deinit_window ()
{
	delete root;
	root = NULL;
	render_system = NULL;
	render_window = NULL;
}

int LIRenRender::count_resources (
	const Ogre::ResourceManager& manager) const
{
	int count;

	Ogre::ResourceManager::ResourceMapIterator iter = const_cast<Ogre::ResourceManager&>(manager).getResourceIterator ();
	for (count = 0 ; iter.hasMoreElements () ; iter.moveNext ())
		count++;

	return count;
}

int LIRenRender::count_resources_loaded (
	const Ogre::ResourceManager& manager) const
{
	int count;

	Ogre::ResourceManager::ResourceMapIterator iter = const_cast<Ogre::ResourceManager&>(manager).getResourceIterator ();
	for (count = 0 ; iter.hasMoreElements () ; iter.moveNext ())
	{
		if (iter.peekNextValue ()->isLoaded ())
			count++;
	}

	return count;
}

void LIRenRender::load_plugin (
	const char* name)
{
#ifdef OGRE_PLUGIN_DIR
	Ogre::String path(OGRE_PLUGIN_DIR "/");
#else
	Ogre::String path(Ogre::String(paths->root) + "/plugins/");
#endif

	try
	{
		root->loadPlugin (path + name);
	}
	catch (Ogre::InternalErrorException e)
	{
		try
		{
			root->loadPlugin (path + name + Ogre::String("_d"));
		}
		catch (...)
		{
			throw e;
		}
	}
}

int LIRenRender::check_plugin (
	const char* name)
{
	Ogre::Root::PluginInstanceList ip = root->getInstalledPlugins ();
	for (Ogre::Root::PluginInstanceList::iterator k = ip.begin(); k != ip.end(); k++)
	{
		if ((*k)->getName () == name)
			return 1;
	}

	return 0;
}

void LIRenRender::garbage_collect_builders ()
{
	LIAlgStrdicIter iter;

	Ogre::MeshManager& mgr = Ogre::MeshManager::getSingleton ();

	/* Ogre will happily leak our manual resource loaders. To avoid them being
	   permanently leaked, we have stored them a dictionary by mesh name. If
	   the mesh no longer exists, we free the associated builder. */
	LIALG_STRDIC_FOREACH (iter, mesh_builders)
	{
		Ogre::MeshPtr ptr = mgr.getByName (iter.key, LIREN_RESOURCES_TEMPORARY);
		if (ptr.isNull ())
		{
			lialg_strdic_remove (mesh_builders, iter.key);
			OGRE_DELETE ((LIRenMeshBuilder*) iter.value);
		}
	}
}

void LIRenRender::unload_unused_resources (
	Ogre::ResourceManager& manager)
{
	Ogre::ResourceManager::ResourceMapIterator iter = manager.getResourceIterator ();
	while (iter.hasMoreElements ())
	{
		/* Check if the resource is loaded. */
		/* Resources that are not loaded may be subject to removal if they
		   belong to the group of temporary resources. Resources in the
		   permanent group are never removed, nor are resources that are
		   currently being background loaded. */
		Ogre::ResourcePtr resource = iter.getNext ();
		if (resource->isLoading ())
			continue;
		if (!resource->isLoaded () && resource->getGroup () == LIREN_RESOURCES_PERMANENT)
			continue;

		/* Check if the resource is in use. */
		/* The resource pointer we create here adds one extra reference. */
		if (resource.useCount () > Ogre::ResourceGroupManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS + 1)
			continue;

		/* Remove or unload the resource. */
		/* Resources in the temporary group are completely removed since we
		   don't want to keep any records of garbage collected meshes and such.
		   Resources in the permanent group can be unloaded but their records
		   are kept so that predefined materials and scripts don't get deleted.
		   The rest are Ogre's internal resources that shouldn't be touched. */
		if (resource->getGroup () == LIREN_RESOURCES_TEMPORARY)
			manager.remove (resource->getHandle ());
		else if (resource->getGroup () == LIREN_RESOURCES_PERMANENT)
			manager.unload (resource->getHandle ());
	}
}

void LIRenRender::update_mode ()
{
	unsigned int w, h, d;
	int x, y;

	render_window->getMetrics (w, h, d, x, y);
	mode.width = w;
	mode.height = h;
	mode.fullscreen = render_window->isFullScreen ();
}

/** @} */
/** @} */
/** @} */
