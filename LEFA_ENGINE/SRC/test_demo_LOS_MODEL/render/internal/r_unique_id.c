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
 * \addtogroup LIRen Render
 * @{
 * \addtogroup LIRenInternal Internal
 * @{
 * \addtogroup LIRenUniqueId UniqueId
 * @{
 */

#include "render_unique_id.h"

LIRenUniqueId::LIRenUniqueId() : index (0)
{
}

LIRenUniqueId::~LIRenUniqueId()
{
}

Ogre::String LIRenUniqueId::next()
{
	int i;
	long rem;
	char res[256];
	static const char* chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";

	strcpy (res, "_LOS/");
	rem = index;
	for (i = 5 ; i < 256 ; i++)
	{
		res[i] = chars[rem % 64];
		rem /= 64;
		if (!rem)
			break;
	}
	lisys_assert (i < 255);
	res[i + 1] = '\0';
	index++;

	return Ogre::String (res);
}

/** @} */
/** @} */
/** @} */
