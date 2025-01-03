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

#ifndef __R_MATERIAL_UTILS_H__
#define __R_MATERIAL_UTILS_H__

//#include "lipsofsuna/system.h"
#include"bloody_termitez/model_test/ANOTHER/allsystem.h"
//#include "lipsofsuna/model.h"
#include"bloody_termitez/model_test/all_model.h"
//#include <OgreMaterial.h>
//#include <OgreString.h>

struct LIRenMaterialUtils
{
	LIRenRender* render;
};

/*
class LIRenRender;

class LIRenMaterialUtils
{
public:
	LIRenMaterialUtils (LIRenRender* render);
public:
	Ogre::MaterialPtr create_material (const LIMdlMaterial* mat);
	Ogre::MaterialPtr create_instanced_material (const char* name, const char* texture);
	bool has_overridable_texture (const Ogre::MaterialPtr& material);
	bool has_overridable_texture (const Ogre::MaterialPtr& material, const Ogre::String& name);
	void replace_texture (Ogre::MaterialPtr& material, const Ogre::String& name, const Ogre::String& new_name);
public:
	bool check_material_override (const Ogre::MaterialPtr& material);
	bool check_name_override (const Ogre::String& name);
	void override_technique (const LIMdlMaterial* mat, Ogre::Technique* technique, bool existing);
	void initialize_pass (const LIMdlMaterial* mat, Ogre::Pass* pass);
	void override_pass (const LIMdlMaterial* mat, Ogre::Pass* pass);
	Ogre::String texture_name_to_filename(const Ogre::String& name);
	Ogre::String texture_filename_to_name(const Ogre::String& filename);
private:
	LIRenRender* render;
};
*/

#endif
