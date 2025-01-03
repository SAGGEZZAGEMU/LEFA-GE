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

#ifndef __MODEL_POSE_SKELETON_H__
#define __MODEL_POSE_SKELETON_H__

//#include"lipsofsuna/system.h"
//#include"lipsofsuna/algorithm.h"
//#include"lipsofsuna/math.h"
#include"model_nodes.h"
#include"model_types.h"

typedef struct _LIMdlPoseSkeleton LIMdlPoseSkeleton;
struct _LIMdlPoseSkeleton
{
	LIMdlNodes nodes;
	LIAlgStrdic* lookup;
};

LIAPICALL (LIMdlPoseSkeleton*, limdl_pose_skeleton_new, (
	const LIMdlModel** models,
	int                count));

LIAPICALL (void, limdl_pose_skeleton_free, (
	LIMdlPoseSkeleton* self));

LIAPICALL (LIMdlNode*, limdl_pose_skeleton_find_node, (
	const LIMdlPoseSkeleton* self,
	const char*              name));

LIAPICALL (void, limdl_pose_skeleton_rebuild_from_models, (
	LIMdlPoseSkeleton* self,
	const LIMdlModel** models,
	int                count));

LIAPICALL (void, limdl_pose_skeleton_rebuild_from_skeletons, (
	LIMdlPoseSkeleton*        self,
	const LIMdlPoseSkeleton** skeletons,
	int                       count));

LIAPICALL (void, limdl_pose_skeleton_update, (
	LIMdlPoseSkeleton* self,
	LIMdlPose*         pose));

#endif
