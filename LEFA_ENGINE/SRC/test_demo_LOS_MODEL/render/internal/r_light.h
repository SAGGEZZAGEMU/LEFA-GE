#ifndef _R_LIGHT_H_
#define _R_LIGHT_H_

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


//#include "lipsofsuna/math.h"
//#include <OgreLight.h>

#include"bloody_termitez/model_test/ANOTHER/math_only.h"

//class LIRenRender;

/*
class LIRenLight
{
public:
	int id;
	int directional;
	LIMatTransform transform;
	LIRenRender* render;
	Ogre::Light* light;
};
*/


typedef struct _LIRenLight  LIRenLight;
struct _LIRenLight  
{
	int id;
	int directional;
	LIMatTransform transform;
	LIRenRender* render;
	//Ogre::Light* light;

};



LIAPICALL (LIRenLight*, liren_light_new, (LIRenRender* render,
										  const float* ambient,
	const float* diffuse,
	const float* specular,
	const float* equation,
	float        cutoff,
	float        exponent,
	int          shadow));

LIAPICALL (void, liren_light_free, (
	LIRenLight* self));

LIAPICALL (void, liren_light_set_ambient, (LIRenLight*  self,
										   const float* value));

LIAPICALL (void, liren_light_set_diffuse, (
	LIRenLight*  self,
	const float* value));

LIAPICALL (void, liren_light_set_directional, (
	LIRenLight* self,
	int         value));

LIAPICALL (void, liren_light_set_enabled, (
	LIRenLight* self,
	int         value));

LIAPICALL (void, liren_light_set_equation, (
	LIRenLight*  self,
	const float* value));

LIAPICALL (int, liren_light_get_id, (
	LIRenLight* self));

LIAPICALL (void, liren_light_set_priority, (
	LIRenLight* self,
	float       value));

LIAPICALL (void, liren_light_set_shadow, (
	LIRenLight* self,
	int         value));

LIAPICALL (void, liren_light_set_shadow_far, (
	LIRenLight* self,
	float       value));

LIAPICALL (void, liren_light_set_shadow_near, (
	LIRenLight* self,
	float       value));

LIAPICALL (void, liren_light_set_specular, (
	LIRenLight*  self,
	const float* value));

LIAPICALL (void, liren_light_set_spot_cutoff, (
	LIRenLight* self,
	float       value));

LIAPICALL (void, liren_light_set_spot_exponent, (
	LIRenLight* self,
	float       value));

LIAPICALL (void, liren_light_get_transform, (
	LIRenLight*     self,
	LIMatTransform* value));

LIAPICALL (void, liren_light_set_transform, (
	LIRenLight*           self,
	const LIMatTransform* value));

#endif /* _R_LIGHT_H_ */
