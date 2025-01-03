/* Lips of Suna
 * Copyright© 2007-2010 Lips of Suna development team.
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
 * \addtogroup LISysFilesystem Filesystem
 * @{
 */

/* INIT UNISTD.H EMU */

#include<stdlib.h>
#include<io.h>
/*#include<getopt.h> /* getopt at: https://gist.github.com/ashelly/7776712 */

#include<process.h> /* for getpid() and the exec..() family */
#include<direct.h> /* for _getcwd() and _chdir() */

#define srandom srand
#define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1       /* execute permission - unsupported in windows*/
#define F_OK    0       /* Test for existence.  */

#define access _access
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
//#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek
/* read, write, and close are NOT being #defined here, because while there are file handle specific versions for Windows, they probably don't work for sockets. You need to look at your app and consider whether to call e.g. closesocket(). */

#ifdef _WIN64
#define ssize_t __int64
#else
#define ssize_t long
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


/* should be in some equivalent to <sys/types.h> */
/*
typedef __int8            int8_t;
typedef __int16           int16_t; 
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
*/



/* END UNISTD.H EMU */

#define HAVE_WINDOWS_H

#ifdef HAVE_CONFIG_H
#include"config.h"
#endif

#include<stdlib.h>
#include<string.h>

#ifdef HAVE_FCNTL_H
#include<fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include<unistd.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include<sys/stat.h>
#endif

#ifdef HAVE_WINDOWS_H
#include<windows.h>
#endif

#include"system.h"
#include"system_error.h"
#include"system_filesystem.h"




/**
 * \brief Checks if the file is accessible.
 * \param path Path to the file.
 * \param mode Access mode.
 * \return Nonzero on success.
 */
int lisys_filesystem_access(const char* path,
									 int         mode)
{
	int m = 0;

	if (mode & LISYS_ACCESS_EXISTS) m |= F_OK;
	if (mode & LISYS_ACCESS_READ) m |= R_OK;
	if (mode & LISYS_ACCESS_WRITE) m |= W_OK;
	if (mode & LISYS_ACCESS_EXEC) m |= X_OK;

	return !access (path, m);
}

/**
 * \brief Creates a directory.
 * \param path Path.
 * \return Nonzero on success.
 */
int lisys_filesystem_makedir(const char* path)
{
#ifdef HAVE_WINDOWS_H
	if (mkdir (path) == -1)
#else
	if (mkdir (path, 0770) == -1)
#endif
	{
		lisys_error_set (errno, "cannot create directory `%s'", path);
		return 0;
	}

	return 1;
}

/**
 * \brief Creates a directory and its parent directories.
 * \param path Path.
 * \return Nonzero on success.
 */
int lisys_filesystem_makepath(const char* path)
{
	char* tmp;
	char* end;

	/* Create a copy of the path so that we can slice it. */
	tmp = strdup (path);
	if (tmp == NULL)
	{
		lisys_error_set (ENOMEM, NULL);
		return 0;
	}

	/* Create each level of the path. */
	for (end = tmp ; 1 ; end++)
	{
		end = strchr (end, '/');
		if (end == tmp)
			continue;
		if (end != NULL)
			*end = '\0';
		if (!lisys_filesystem_access (tmp, LISYS_ACCESS_EXISTS))
		{
			if (!lisys_filesystem_makedir (tmp))
			{
				free (tmp);
				return 0;
			}
		}
		if (end != NULL)
			*end = '/';
		else
			break;
	}
	free (tmp);

	return 1;
}

/**
 * \brief Reads the target of a symbolic link.
 * \param path Path to the link.
 * \return Link target or NULL.
 */
char* lisys_filesystem_readlink(const char* path)
{
#ifdef HAVE_READLINK
	int len;
	char* tmp;
	char* name;

	len = 128;
	name = NULL;
	while (1)
	{
		tmp = realloc (name, len);
		if (tmp == NULL)
		{
			lisys_error_set (ENOMEM, NULL);
			free (name);
			return NULL;
		}
		name = tmp;
		name[len - 1] = '\0';
		if (readlink (path, name, len) <= 0)
		{
			lisys_error_set (errno, "cannot read link `%s'", path);
			free (name);
			return NULL;
		}
		if (strlen (tmp) < len)
			break;
		len <<= 1;
	}

	return name;
#else
	lisys_error_set (ENOTSUP, NULL);
	return NULL;
#endif
}

/**
 * \brief Stats a path.
 * \param path Path to stat.
 * \param result Return location for the stat data.
 * \return Nonzero on success.
 */
int lisys_filesystem_stat(const char* path,
								  LISysStat*  result)
{
	struct stat st;

	/* Stat the file. */
	if (stat (path, &st) != 0)
	{
		lisys_error_set (EIO, "cannot stat `%s'", path);
		return 0;
	}

	/* Convert results. */
	if (S_ISREG (st.st_mode)) result->type = LISYS_STAT_FILE;
	else if (S_ISDIR (st.st_mode)) result->type = LISYS_STAT_DIRECTORY;
	else if (S_ISCHR (st.st_mode)) result->type = LISYS_STAT_CHAR;
	else if (S_ISBLK (st.st_mode)) result->type = LISYS_STAT_BLOCK;
#ifdef HAVE_LSTAT
	else if (S_ISLNK (st.st_mode)) result->type = LISYS_STAT_LINK;
#endif
	else { lisys_assert (0); }
	result->uid = st.st_uid;
	result->gid = st.st_gid;
	result->mode = st.st_mode & 0777;
	result->size = st.st_size;
	result->mtime = st.st_mtime;

	return 1;
}

/**
 * \brief Stats a path.
 * \param path Path to stat.
 * \param result Return location for the stat data.
 * \return Nonzero on success.
 */
int lisys_filesystem_lstat(const char* path,
									LISysStat*  result)
{
#ifdef HAVE_LSTAT
	struct stat st;

	/* Stat the file. */
	if (lstat (path, &st) != 0)
	{
		lisys_error_set (EIO, "cannot stat `%s'", path);
		return 0;
	}

	/* Convert results. */
	if (S_ISREG (st.st_mode)) result->type = LISYS_STAT_FILE;
	else if (S_ISDIR (st.st_mode)) result->type = LISYS_STAT_DIRECTORY;
	else if (S_ISCHR (st.st_mode)) result->type = LISYS_STAT_CHAR;
	else if (S_ISBLK (st.st_mode)) result->type = LISYS_STAT_BLOCK;
#ifdef HAVE_LSTAT
	else if (S_ISLNK (st.st_mode)) result->type = LISYS_STAT_LINK;
#endif
	else { lisys_assert (0); }
	result->uid = st.st_uid;
	result->gid = st.st_gid;
	result->mode = st.st_mode & 0777;
	result->size = st.st_size;
	result->mtime = st.st_mtime;
	result->rdev = st.st_rdev;

	return 1;
#else
	return lisys_filesystem_stat (path, result);
#endif
}

/** @} */
/** @} */
