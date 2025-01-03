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

#ifndef __MODEL_POSE_BUFFER_H__
#define __MODEL_POSE_BUFFER_H__

//#include"lipsofsuna/system.h"
//#include"lipsofsuna/math.h"
#include"model_pose_skeleton.h"
#include"model_types.h"

typedef struct _LIMdlPoseBufferBone LIMdlPoseBufferBone;
struct _LIMdlPoseBufferBone
{
	char* name;
	LIMatTransform transform;
	vec3 scale;
};

typedef struct _LIMdlPoseBuffer LIMdlPoseBuffer;
struct _LIMdlPoseBuffer
{
	struct { int count; LIMdlPoseBufferBone* array; } bones;
};

LIAPICALL (LIMdlPoseBuffer*, limdl_pose_buffer_new, (
	const LIMdlModel* model));

LIAPICALL (LIMdlPoseBuffer*, limdl_pose_buffer_new_copy, (
	const LIMdlPoseBuffer* buffer));

LIAPICALL (void, limdl_pose_buffer_free, (
	LIMdlPoseBuffer* self));

LIAPICALL (void, limdl_pose_buffer_copy_pose, (
	LIMdlPoseBuffer*       self,
	const LIMdlPoseBuffer* src));

LIAPICALL (void, limdl_pose_buffer_update, (
	LIMdlPoseBuffer*   self,
	LIMdlPoseSkeleton* skeleton));

#endif
