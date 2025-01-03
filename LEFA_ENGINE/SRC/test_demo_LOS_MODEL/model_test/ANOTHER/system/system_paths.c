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

/**
 * \addtogroup LISys System
 * @{
 * \addtogroup LISysPaths Paths
 * @{
 */

#define HAVE_WINDOWS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#ifdef HAVE_WINDOWS_H
#define _WIN32_IE 0x0400
#include<windows.h>
#include<shlobj.h>
#endif

#include"system.h"
#include"system_error.h"
#include"system_filesystem.h"
#include"system_memory.h"
#include"system_path.h"
#include"system_paths.h"
#include"system_string.h"

/**
 * \brief Gets the home directory.
 *
 * If no home directory is specified, the current
 * working directory is returned instead.
 *
 * \return New string or NULL if out of memory.
 */
char* lisys_paths_get_home()
{
#ifdef __WIN32__
	int i;
	char tmp[MAX_PATH];

	if (!SHGetSpecialFolderPath (NULL, tmp, CSIDL_PROFILE, TRUE))
		return NULL;
	for (i = 0 ; tmp[i] != '\0' ; i++)
	{
		if (tmp[i] == '\\')
			tmp[i] = '/';
	}
	return lisys_string_dup (tmp);
#else
	char* home;

	home = getenv ("HOME");
	if (home != NULL)
		return lisys_string_dup  (home);
	return lisys_string_dup  (".");
#endif
}

/**
 * \brief Gets the home data directory.
 *
 * Follows the XDG Base Directory Specification:
 * http://www.freedesktop.org/Standards/basedir-spec
 *
 * \return New string or NULL.
 */
char* lisys_paths_get_data_home()
{
#ifdef __WIN32__
	int i;
	char tmp[MAX_PATH];

	if (!SHGetSpecialFolderPath (NULL, tmp, CSIDL_PERSONAL, TRUE))
		return NULL;
	for (i = 0 ; tmp[i] != '\0' ; i++)
	{
		if (tmp[i] == '\\')
			tmp[i] = '/';
	}
	return lisys_path_concat (tmp, "My Games", NULL);
#else
	char* ret;
	char* tmp;
	const char* dir;

	dir = getenv ("XDG_DATA_HOME");
	if (dir != NULL && dir[0] != '\0')
		return lisys_string_dup  (dir);
	tmp = lisys_paths_get_home ();
	if (tmp == NULL)
		return NULL;
	ret = lisys_path_concat (tmp, ".local/share", NULL);
	lisys_free (tmp);

	return ret;
#endif
}

/**
 * \brief Gets the home config directory.
 *
 * Follows the XDG Base Directory Specification:
 * http://www.freedesktop.org/Standards/basedir-spec
 *
 * \return New string or NULL.
 */
char* lisys_paths_get_config_home ()
{
#ifdef __WIN32__
	return lisys_paths_get_data_home ();
#else
	char* ret;
	char* tmp;
	const char* dir;

	dir = getenv ("XDG_CONFIG_HOME");
	if (dir != NULL && dir[0] != '\0')
		return lisys_string_dup  (dir);
	tmp = lisys_paths_get_home ();
	if (tmp == NULL)
		return NULL;
	ret = lisys_path_concat (tmp, ".config", NULL);
	lisys_free (tmp);

	return ret;
#endif
}

/**
 * \brief Gets the home cache directory.
 *
 * Follows the XDG Base Directory Specification:
 * http://www.freedesktop.org/Standards/basedir-spec
 *
 * \return New string or NULL.
 */
char* lisys_paths_get_cache_home ()
{
#ifdef __WIN32__
	return lisys_paths_get_data_home ();
#else
	char* ret;
	char* tmp;
	const char* dir;

	dir = getenv ("XDG_CACHE_HOME");
	if (dir != NULL && dir[0] != '\0')
		return lisys_string_dup  (dir);
	tmp = lisys_paths_get_home ();
	if (tmp == NULL)
		return NULL;
	ret = lisys_path_concat (tmp, ".cache", NULL);
	lisys_free (tmp);

	return ret;
#endif
}

/**
 * \brief Gets the global data directory.
 *
 * Follows the XDG Base Directory Specification:
 * http://www.freedesktop.org/Standards/basedir-spec
 *
 * \param path Relative path being searched for.
 * \return New string or NULL.
 */
char* lisys_paths_get_data_global (
	const char* path)
{
#ifdef __WIN32__
	return NULL;
#else
	int last;
	char* dup;
	char* ptr;
	char* ret;
	char* start;
	const char* dirs;

	/* Get the list of global data directories. */
	dirs = getenv ("XDG_DATA_DIRS");
	if (dirs == NULL || dirs[0] == '\0')
		return NULL;
	dup = lisys_string_dup  (dirs);
	if (dup == NULL)
		return NULL;

	/* Loop through all directories. */
	ptr = start = dup;
	ret = NULL;
	last = 0;
	while (1)
	{
		/* Search for the delimiter. */
		last = (*ptr == '\0');
		if (*ptr != ':' && !last)
		{
			ptr++;
			continue;
		}
		*ptr = '\0';

		/* Test if the path is valid. */
		ret = lisys_path_concat (start, path, NULL);
		if (ret != NULL)
		{
			if (lisys_filesystem_access (ret, LISYS_ACCESS_READ))
				break;
			lisys_free (ret);
			ret = NULL;
		}

		/* Check if more candidates exist. */
		if (last)
			break;
		ptr++;
		start = ptr;
	}

	/* Return the result or NULL. */
	lisys_free (dup);
	return ret;
#endif
}

/** @} */
/** @} */
