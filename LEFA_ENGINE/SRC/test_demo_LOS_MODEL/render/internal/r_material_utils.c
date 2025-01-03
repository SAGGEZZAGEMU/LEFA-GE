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
 * \addtogroup LIRenMeshBuilder MeshBuilder
 * @{
 */

#include "lipsofsuna/system.h"
#include "render.hpp"
#include "render-material-utils.hpp"

LIRenMaterialUtils::LIRenMaterialUtils (LIRenRender* render) : render(render)
{
}

/**
 * \brief Creates a material.
 * \param mat Model material.
 * \return Ogre material.
 */
Ogre::MaterialPtr LIRenMaterialUtils::create_material (
	const LIMdlMaterial* mat)
{
	bool existing = false;
	bool override = true;
	Ogre::MaterialPtr material;
	Ogre::String unique_name = render->id.next ();

	/* Try to load an existing material. */
	if (mat->material != NULL && mat->material[0] != '\0')
	{
		Ogre::String name = Ogre::String (mat->material);
		material = render->material_manager->getByName (name);
		if (!material.isNull())
			existing = true;
	}

	/* Try to default to diff1 if no material found yet. */
	/* Although the model could specify almost all the important properties,
	   we strongly prefer to use a base material. This can be avoided by not
	   specifying the `diff1' shader, though there should be no reason for that. */
	if (!existing)
	{
		material = render->material_manager->getByName ("diff1");
		if (!material.isNull())
			existing = true;
	}

	/* Create a new material if an existing one was not found. */
	if (!existing)
	{
		Ogre::String group = LIREN_RESOURCES_TEMPORARY;
		material = render->material_manager->create (unique_name, group);
		if (mat->flags & LIMDL_MATERIAL_FLAG_TRANSPARENCY)
			material->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);
		if (mat->flags & LIMDL_MATERIAL_FLAG_CULLFACE)
			material->setCullingMode (Ogre::CULL_CLOCKWISE);
		else
			material->setCullingMode (Ogre::CULL_NONE);
	}

	/* Instantiate the material if it needs to be overridden. */
	/* The original material is in the group of permanent resources but
	   the instantiated material needs to be put into the temporary group
	   so that it can be removed when the model is garbage collected. */
	if (existing)
	{
		if (check_material_override (material))
			material = material->clone (unique_name, true, LIREN_RESOURCES_TEMPORARY);
		else
			override = false;
	}

	/* Override the fields of techniques. */
	if (override)
	{
		for (int i = 0 ; i < material->getNumTechniques () ; i++)
		{
			// Override passes.
			Ogre::Technique* technique = material->getTechnique (i);
			override_technique (mat, technique, existing);

			// Override shadow caster materials.
			Ogre::MaterialPtr caster = technique->getShadowCasterMaterial ();
			if (!caster.isNull () && check_material_override (caster))
			{
				Ogre::String unique_caster = render->id.next ();
				Ogre::MaterialPtr new_caster = caster->clone (unique_caster, true, LIREN_RESOURCES_TEMPORARY);
				for (int i = 0 ; i < new_caster->getNumTechniques () ; i++)
					override_technique (mat, new_caster->getTechnique (i), true);
				technique->setShadowCasterMaterial (new_caster);
			}
		}
	}

	return material;
}

/**
 * \brief Creates a material with a replaced texture.
 * \param name Base material name.
 * \param texture Texture name.
 * \return Ogre material on success. Null material pointer on error.
 */
Ogre::MaterialPtr LIRenMaterialUtils::create_instanced_material (const char* name, const char* texture)
{
	// Find the base material.
	Ogre::MaterialPtr base = render->material_manager->getByName (name);
	if (base.isNull())
		return base;

	// Duplicate the material if overridable.
	if (!has_overridable_texture (base))
		return base;
	Ogre::MaterialPtr material = base->clone (render->id.next (), true, LIREN_RESOURCES_TEMPORARY);

	// Override the texture units.
	for (int tech_idx = 0 ; tech_idx < material->getNumTechniques() ; ++tech_idx)
	{
		Ogre::Technique* tech = material->getTechnique(tech_idx);
		for (int pass_idx = 0 ; pass_idx < tech->getNumPasses() ; ++pass_idx)
		{
			Ogre::Pass* pass = tech->getPass(pass_idx);
			for (int unit_idx = 0 ; unit_idx < pass->getNumTextureUnitStates() ; ++unit_idx)
			{
				// Try to override the texture.
				Ogre::TextureUnitState* state = pass->getTextureUnitState(unit_idx);
				if (check_name_override (state->getName ()))
					state->setTextureName (texture_name_to_filename(texture));
			}
		}
	}

	return material;
}

/**
 * \brief Checks if the material has any overridable textures.
 * \param material Material.
 * \return True if found. False otherwise.
 */
bool LIRenMaterialUtils::has_overridable_texture (const Ogre::MaterialPtr& material)
{
	for (int tech_idx = 0 ; tech_idx < material->getNumTechniques() ; ++tech_idx)
	{
		Ogre::Technique* tech = const_cast<Ogre::Technique*>(material->getTechnique(tech_idx));
		for (int pass_idx = 0 ; pass_idx < tech->getNumPasses() ; ++pass_idx)
		{
			const Ogre::Pass* pass = tech->getPass(pass_idx);
			for (int unit_idx = 0 ; unit_idx < pass->getNumTextureUnitStates() ; ++unit_idx)
			{
				// Check if the unit is overridable.
				const Ogre::TextureUnitState* state = pass->getTextureUnitState(unit_idx);
				if (!check_name_override (state->getName ()))
					continue;
				return true;
			}
		}
	}

	return false;
}

/**
 * \brief Checks if the material has the given overridable texture.
 * \param material Material.
 * \param name Texture name without extension.
 * \return True if found. False otherwise.
 */
bool LIRenMaterialUtils::has_overridable_texture (const Ogre::MaterialPtr& material, const Ogre::String& name)
{
	for (int tech_idx = 0 ; tech_idx < material->getNumTechniques() ; ++tech_idx)
	{
		Ogre::Technique* tech = const_cast<Ogre::Technique*>(material->getTechnique(tech_idx));
		for (int pass_idx = 0 ; pass_idx < tech->getNumPasses() ; ++pass_idx)
		{
			const Ogre::Pass* pass = tech->getPass(pass_idx);
			for (int unit_idx = 0 ; unit_idx < pass->getNumTextureUnitStates() ; ++unit_idx)
			{
				// Check if the unit is overridable.
				const Ogre::TextureUnitState* state = pass->getTextureUnitState(unit_idx);
				if (!check_name_override (state->getName ()))
					continue;

				// Check if the texture matches.
				//
				// The first condition is for internally created materials that
				// are of form "_LOS/id". The second condition is for Ogre
				// textures whose names are of form "NAME.png".
				const Ogre::String& filename = state->getTextureName();
				if (name == filename || texture_filename_to_name(filename) == name)
					return true;
			}
		}
	}

	return false;
}

/**
 * \brief Replaces an overridable texture.
 * \param material Material.
 * \param name Texture name without extension.
 * \param new_name Ogre texture name.
 * \return True if found. False otherwise.
 */
void LIRenMaterialUtils::replace_texture (Ogre::MaterialPtr& material, const Ogre::String& name, const Ogre::String& new_name)
{
	for (int tech_idx = 0 ; tech_idx < material->getNumTechniques() ; ++tech_idx)
	{
		Ogre::Technique* tech = material->getTechnique(tech_idx);
		for (int pass_idx = 0 ; pass_idx < tech->getNumPasses() ; ++pass_idx)
		{
			Ogre::Pass* pass = tech->getPass(pass_idx);
			for (int unit_idx = 0 ; unit_idx < pass->getNumTextureUnitStates() ; ++unit_idx)
			{
				// Check if the unit is overridable.
				Ogre::TextureUnitState* state = pass->getTextureUnitState(unit_idx);
				if (!check_name_override (state->getName ()))
					continue;

				// Check if the texture matches.
				//
				// The first condition is for internally created materials that
				// are of form "_LOS/id". The second condition is for Ogre
				// textures whose names are of form "NAME.png".
				const Ogre::String& filename = state->getTextureName();
				if (filename == name || texture_filename_to_name(filename) == name)
					state->setTextureName (new_name);
			}
		}
	}
}

/**
 * \brief Checks if the material is overridable.
 * \param material Ogre material.
 * \return True if overridable. False otherwise.
 */
bool LIRenMaterialUtils::check_material_override (
	const Ogre::MaterialPtr& material)
{
	for (int k = 0 ; k < material->getNumTechniques () ; k++)
	{
		Ogre::Technique* tech = material->getTechnique (k);
		for (int i = 0 ; i < tech->getNumPasses () ; i++)
		{
			// Check if the pass needs to be overridden.
			Ogre::Pass* pass = tech->getPass (i);
			if (check_name_override (pass->getName ()))
				return true;

			// Check if any textures need to be overridden.
			for (int i = 0 ; i < pass->getNumTextureUnitStates () ; i++)
			{
				Ogre::TextureUnitState* state = pass->getTextureUnitState (i);
				if (check_name_override (state->getName ()))
					return true;
			}

			// Check if the shadow caster material needs to be overridden.
			const Ogre::MaterialPtr caster = tech->getShadowCasterMaterial ();
			if (!caster.isNull () && check_material_override (caster))
				return true;
		}
	}

	return false;
}

bool LIRenMaterialUtils::check_name_override (
	const Ogre::String& name)
{
	if (name.size () < 3)
		return 0;
	if (name[0] != 'L' || name[1] != 'O' || name[2] != 'S')
		return 0;
	return 1;
}

void LIRenMaterialUtils::override_technique (
	const LIMdlMaterial* mat,
	Ogre::Technique*     technique,
	bool                 existing)
{
	if (existing)
	{
		for (int j = 0 ; j < technique->getNumPasses () ; j++)
		{
			Ogre::Pass* pass = technique->getPass (j);
			override_pass (mat, pass);
		}
	}
	else
	{
		Ogre::Pass* pass = technique->getPass (0);
		initialize_pass (mat, pass);
	}
}

void LIRenMaterialUtils::initialize_pass (
	const LIMdlMaterial* mat,
	Ogre::Pass*          pass)
{
	/* Initialize pass properties. */
	pass->setSelfIllumination (mat->emission, mat->emission, mat->emission);
	pass->setShininess (mat->shininess);
	pass->setDiffuse (Ogre::ColourValue (mat->diffuse[0], mat->diffuse[1], mat->diffuse[2], mat->diffuse[3]));
	pass->setSpecular (Ogre::ColourValue (mat->specular[0], mat->specular[1], mat->specular[2], mat->specular[3]));
	pass->setVertexColourTracking (Ogre::TVC_DIFFUSE);

	/* Initialize texture units. */
	for (int i = 0 ; i < mat->textures.count && i < 1 ; i++)
	{
		Ogre::String texname = Ogre::String (mat->textures.array[i].string);
		pass->createTextureUnitState (texture_name_to_filename(texname));
	}
}

void LIRenMaterialUtils::override_pass (
	const LIMdlMaterial* mat,
	Ogre::Pass*          pass)
{
	/* Set pass properties. */
	/* If this is a newly created material or the name of the first pass
	   starts with the string "LOS", we override some of the parameters. */
	if (check_name_override (pass->getName ()))
	{
		pass->setSelfIllumination (mat->emission, mat->emission, mat->emission);
		pass->setShininess (mat->shininess);
		Ogre::TrackVertexColourType track = pass->getVertexColourTracking ();
		if (track != Ogre::TVC_DIFFUSE)
			pass->setDiffuse (Ogre::ColourValue (mat->diffuse[0], mat->diffuse[1], mat->diffuse[2], mat->diffuse[3]));
		if (track != Ogre::TVC_SPECULAR)
			pass->setSpecular (Ogre::ColourValue (mat->specular[0], mat->specular[1], mat->specular[2], mat->specular[3]));
	}

	/* Override texture units. */
	/* Texture units whose names start with "LOS" are considered overridable. */
	int j = 0;
	for (int i = 0 ; i < pass->getNumTextureUnitStates () ; i++)
	{
		if (j >= mat->textures.count)
			break;
		Ogre::TextureUnitState* state = pass->getTextureUnitState (i);
		if (check_name_override (state->getName ()))
		{
			Ogre::String texname = Ogre::String (mat->textures.array[j].string);
			state->setTextureName (texture_name_to_filename(texname));
			j++;
		}
	}
}

/**
 * \brief Gets the filename by texture name.
 * \param name Texture name without extension.
 * \return Texture filename.
 */
Ogre::String LIRenMaterialUtils::texture_name_to_filename(const Ogre::String& name)
{
	/* Check if a PNG version is available. */
	Ogre::String pngname = name + ".png";
	const char* path = lipth_paths_find_file (render->paths, pngname.c_str ());
	int gotpng;
	if (path != NULL)
		gotpng = lisys_filesystem_access (path, LISYS_ACCESS_READ);
	else
		gotpng = 0;

	/* Use either a PNG or a DDS file. */
	/* PNG is favored over DDS so that artists don't need to bother
	   with converting their textures when testing them. */
	if (gotpng)
		return pngname;
	else
		return name + ".dds";
}

/**
 * \brief Gets the texture name by filename.
 * \param filename Texture filename.
 * \return Texture name without extension.
 */
Ogre::String LIRenMaterialUtils::texture_filename_to_name(const Ogre::String& filename)
{
	for (int i = filename.length() - 1 ; i >= 0 ; --i)
	{
		if (filename[i] == '.')
			return filename.substr (0, i);
	}
	return "";
}

/** @} */
/** @} */
/** @} */
