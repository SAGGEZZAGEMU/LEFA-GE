/* Lips of Suna
 * Copyright© 2007-2012 Lips of Suna development team.
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
 * \addtogroup LIPth Paths
 * @{
 * \addtogroup LIPthPaths Paths
 * @{
 */

#include"../allsystem.h"
#include"paths.h"

static int private_validate_dir(char** path);

static int private_create_save_path (
	LIPthPaths* paths,
	const char* path);

/*****************************************************************************/

/**
 * \brief Creates a new paths object.
 *
 * \param path Package root directory or NULL for auto-detect.
 * \param name Module name.
 * \return Paths or NULL.
 */
LIPthPaths* lipth_paths_new (
	const char* path,
	const char* name)
{
	char* tmp;
	LIPthPaths* self;

	/* Allocate self. */
	self = lisys_calloc (1, sizeof (LIPthPaths));
	if (self == NULL)
		return NULL;

	/* Allocate the file lookup table. */
	self->files = lialg_strdic_new ();
	if (self->files == NULL)
	{
		lipth_paths_free (self);
		return NULL;
	}

	/* Set the module name. */
	self->module_name = lisys_string_dup (name);
	if (self->module_name == NULL)
	{
		lipth_paths_free (self);
		return NULL;
	}

	/* Set the root directory. */
	if (path != NULL)
	{
		self->root = lisys_string_dup (path);
		if (self->root == NULL)
		{
			lipth_paths_free (self);
			return NULL;
		}
	}
	else
	{
		self->root = lipth_paths_get_root ();
		if (self->root == NULL)
		{
			lipth_paths_free (self);
			return NULL;
		}
	}

	/* Get the data directory root. */
	/* This is where modules and system scripts are located. If relative paths
	   are enabled, the directory of the executable is searched for a data
	   directory. Failing that, XDG data directories are searched for the game
	   directory. As the last resort, the hardcoded data directory is used. */
#ifdef LI_RELATIVE_PATHS
	self->global_data = lisys_path_concat (self->root, "data", NULL);
	if (!private_validate_dir (&self->global_data))
#endif
	{
		self->global_data = lisys_paths_get_data_global ("lipsofsuna");
		private_validate_dir (&self->global_data);
		if (self->global_data == NULL)
		{
#ifdef LIDATADIR
			self->global_data = lisys_string_dup (LIDATADIR);
			if (!private_validate_dir (&self->global_data))
#endif
			{
				lisys_error_set (EINVAL, "cannot find the data directory");
				lipth_paths_free (self);
				return NULL;
			}
		}
	}

	/* Get the data directory of the current module. */
	/* This is where the data files of the module are installed. The directory
	   functions as a fallback for both data and configuration files. */
	self->module_data = lisys_path_concat (self->global_data, name, NULL);
	if (!private_validate_dir (&self->module_data))
	{
		lisys_error_set (EINVAL, "cannot find the module data directory");
		lipth_paths_free (self);
		return NULL;
	}

	/* Get the data save directory. */
	/* This is where save files are written and where the user can copy any
	   custom data files or mods. It's the first place where data files are
	   looked for. */
	tmp = lisys_paths_get_data_home ();
	if (tmp == NULL)
	{
		lipth_paths_free (self);
		return NULL;
	}
	self->module_data_save = lisys_path_concat (tmp, "lipsofsuna", name, NULL);
	lisys_free (tmp);
	if (self->module_data_save == NULL)
	{
		lipth_paths_free (self);
		return NULL;
	}

	/* Get the config directory. */
	/* This is where configuration files are stored. It's separate from the save
	   directory since in Linux that's the case. In Windows it's actually the
	   save directory. */
	tmp = lisys_paths_get_config_home ();
	if (tmp == NULL)
	{
		lipth_paths_free (self);
		return NULL;
	}
	self->module_config = lisys_path_concat (tmp, "lipsofsuna", name, NULL);
	lisys_free (tmp);
	if (self->module_config == NULL)
	{
		lipth_paths_free (self);
		return NULL;
	}

	/* Get the extension directory. */
	/* This directory is reserved for potential third party extension libraries.
	   The path is lib/extensions either in the relative path, XDG data path or
	   the hardcoded extension directory. */
#ifdef LI_RELATIVE_PATHS
	self->global_exts = lisys_path_concat (self->global_data, "lib", "extensions", NULL);
	if (!private_validate_dir (&self->global_exts))
#endif
	{
		self->global_exts = lisys_paths_get_data_global ("lipsofsuna/lib/extensions");
#ifdef LIEXTSDIR
		if (!private_validate_dir (&self->global_exts))
		{
			self->global_exts = lisys_string_dup (LIEXTSDIR);
			private_validate_dir (&self->global_exts);
		}
#endif
	}

	/* Create the save directories. */
	if (!private_create_save_path (self, self->module_data_save) ||
	    !private_create_save_path (self, self->module_config))
	{
		lipth_paths_free (self);
		return NULL;
	}

	/* Add the base data directory. */
	lipth_paths_add_path (self, "");

	return self;
}

/**
 * \brief Frees the paths object.
 *
 * \param self Paths object.
 */
void lipth_paths_free (
	LIPthPaths* self)
{
	LIAlgList* ptr;
	LIAlgStrdicIter iter;

	/* Free path names. */
	if (self->paths != NULL)
	{
		for (ptr = self->paths ; ptr != NULL ; ptr = ptr->next)
			lisys_free (ptr->data);
		lialg_list_free (self->paths);
	}

	/* Free file names. */
	if (self->files != NULL)
	{
		LIALG_STRDIC_FOREACH (iter, self->files)
			lisys_free (iter.value);
		lialg_strdic_free (self->files);
	}

	lisys_free (self->global_exts);
	lisys_free (self->global_data);
	lisys_free (self->module_name);
	lisys_free (self->module_config);
	lisys_free (self->module_data);
	lisys_free (self->module_data_save);
	lisys_free (self->root);
	lisys_free (self);
}

/**
 * \brief Adds a data directory lookup path.
 * \param self Paths.
 * \param path Module root relative path.
 */
int lipth_paths_add_path (
	LIPthPaths* self,
	const char* path)
{
	int ret = 1;
	char* path1;

	/* Add the module directory. */
	path1 = lisys_path_concat (self->module_data, path, NULL);
	if (path1 != NULL)
	{
		if (!lipth_paths_add_path_abs (self, path1))
			ret = 0;
		lisys_free (path1);
	}
	else
		ret = 0;

	/* Add the override directory. */
	path1 = lisys_path_concat (self->module_data_save, path, NULL);
	if (path1 != NULL)
	{
		lipth_paths_add_path_abs (self, path1);
		lisys_free (path1);
	}
	else
		ret = 0;

	return ret;
}

/**
 * \brief Adds a data directory lookup path.
 * \param self Paths.
 * \param path Absolute path.
 */
int lipth_paths_add_path_abs (
	LIPthPaths* self,
	const char* path)
{
	int i;
	int c;
	char* p;
	const char* name;
	LISysDir* dir;
	LIAlgList* ptr;
	LIAlgStrdicNode* node;

	/* Add the path to the list. */
	/* The path isn't added if it already exists. Its files are scanned
	   and can override previously registered files, however. */
	for (ptr = self->paths ; ptr != NULL ; ptr = ptr->next)
	{
		if (!strcmp (ptr->data, path))
			break;
	}
	if (ptr == NULL)
	{
		p = lisys_string_dup (path);
		if (p == NULL)
			return 0;
		lialg_list_prepend (&self->paths, p);
	}

	/* Scan the files in the directory. */
	dir = lisys_dir_open (path);
	if (dir == NULL)
		return 0;
	lisys_dir_set_filter (dir, lisys_dir_filter_files, NULL);
	if (!lisys_dir_scan (dir))
	{
		lisys_dir_free (dir);
		return 0;
	}

	/* Add the files to the lookup table. */
	/* If there are duplicate files, the new ones replace the old ones.
	   This allows mods to override files based on the registration order. */
	c = lisys_dir_get_count (dir);
	for (i = 0 ; i < c ; i++)
	{
		/* Construct the path name. */
		name = lisys_dir_get_name (dir, i);
		p = lisys_dir_get_path (dir, i);
		if (p == NULL)
			continue;

		/* Override or add. */
		node = lialg_strdic_find_node (self->files, name);
		if (node != NULL)
		{
			lisys_free (node->value);
			node->value = p;
		}
		else
			lialg_strdic_insert (self->files, name, p);
	}
	lisys_dir_free (dir);

	return 1;
}

/**
 * \brief Creates a new file in the override directory.
 * \param self Paths.
 * \param name Filename.
 * \param config One for a config file, zero for a data file.
 * \return path Absolute path or NULL.
 */
const char* lipth_paths_create_file (
	LIPthPaths* self,
	const char* name,
	int         config)
{
	char* path;
	LIAlgStrdicNode* node;

	/* Format the path. */
	if (config)
		path = lisys_path_concat (self->module_config, name, NULL);
	else
		path = lisys_path_concat (self->module_data_save, name, NULL);
	if (path == NULL)
		return NULL;

	/* Register the file. */
	node = lialg_strdic_find_node (self->files, name);
	if (node != NULL)
	{
		lisys_free (node->value);
		node->value = path;
	}
	else
		lialg_strdic_insert (self->files, name, path);

	return path;
}

/**
 * \brief Finds a data file by name.
 * \param self Paths.
 * \param name File name.
 * \return path Absolute path or NULL.
 */
const char* lipth_paths_find_file (
	LIPthPaths* self,
	const char* name)
{
	return lialg_strdic_find (self->files, name);
}

/**
 * \brief Finds the data of a config file by a path relative to the data directory root.
 * \param self Paths.
 * \param path Path relative to the data directory root.
 * \param config Nonzero for a config file, zero for a data file.
 * \return Absolute path or NULL.
 */
char* lipth_paths_find_path (
	const LIPthPaths* self,
	const char*       path,
	int               config)
{
	char* path1;

	/* Try the config/save path. */
	if (config)
		path1 = lisys_path_concat (self->module_config, path, NULL);
	else
		path1 = lisys_path_concat (self->module_data_save, path, NULL);
	if (path1 == NULL)
		return NULL;
	if (lisys_filesystem_access (path1, LISYS_ACCESS_READ))
		return path1;
	lisys_free (path1);

	/* Try the data path. */
	path1 = lisys_path_concat (self->module_data, path, NULL);
	if (lisys_filesystem_access (path1, LISYS_ACCESS_READ))
		   return path1;
	lisys_free (path1);

	return NULL;
}

/**
 * \brief Gets the game root directory.
 * \return Path or NULL.
 */
char* lipth_paths_get_root ()
{
#ifdef LI_RELATIVE_PATHS
	return lisys_relative_exedir ();
#else
	return lisys_string_dup (LIDATADIR);
#endif
}

/*****************************************************************************/

static int private_validate_dir (
	char** path)
{
	LISysStat stat;

	if (*path == NULL)
		return 0;
	if (!lisys_filesystem_stat (*path, &stat))
	{
		lisys_free (*path);
		*path = NULL;
		return 0;
	}
	if (stat.type != LISYS_STAT_DIRECTORY && stat.type != LISYS_STAT_LINK)
	{
		lisys_free (*path);
		*path = NULL;
		return 0;
	}

	return 1;
}

static int private_create_save_path (
	LIPthPaths* self,
	const char* path)
{
	/* Check if the save directory exists. */
	if (lisys_filesystem_access (path, LISYS_ACCESS_EXISTS))
	{
		if (!lisys_filesystem_access (path, LISYS_ACCESS_WRITE))
		{
			lisys_error_set (EINVAL, "save path `%s' is not writable", path);
			return 0;
		}
		return 1;
	}

	/* Create the save directory. */
	if (!lisys_filesystem_makepath (path))
		return 0;

	return 1;
}

/** @} */
/** @} */
