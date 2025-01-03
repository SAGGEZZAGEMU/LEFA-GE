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

#ifndef __MODEL_POSE_H__
#define __MODEL_POSE_H__

//#include"lipsofsuna/algorithm.h"
#include"ANOTHER/algorithm.h"
//#include"model.h"
#include"model_animation.h"
#include"model_bone.h"
#include"model_pose_channel.h"
#include"model_types.h"
#include"model_vertex.h"

#define LIMDL_POSE_FADE_AUTOMATIC -1.0f

typedef struct _LIMdlPoseVertex LIMdlPoseVertex;
struct _LIMdlPoseVertex
{
	int weight_count;
	float weight_total;
};

struct _LIMdlPose
{
	LIAlgRandom random;
	LIAlgU32dic* channels;
};

LIAPICALL (LIMdlPose*, limdl_pose_new, ());

LIAPICALL (LIMdlPose*, limdl_pose_new_copy, (LIMdlPose* pose));

LIAPICALL (void, limdl_pose_free, (LIMdlPose* self));

LIAPICALL(void, limdl_pose_calculate_node_tranformation, 
		  (const LIMdlPose* self,
		   const char*      node,
		   LIMatTransform*  result_transform,
		   float*           result_scale));

LIAPICALL(void, limdl_pose_clear_channel_node_priorities, (LIMdlPose*  self,
	int         channel));

LIAPICALL (void, limdl_pose_clear_channel_node_weights, (
	LIMdlPose*  self,
	int         channel));

LIAPICALL (void, limdl_pose_destroy_all, (
	LIMdlPose* self));

LIAPICALL (void, limdl_pose_destroy_channel, (
	LIMdlPose* self,
	int        channel));

LIAPICALL (void, limdl_pose_fade_channel, (
	LIMdlPose* self,
	int        channel,
	float      rate));

LIAPICALL (void, limdl_pose_merge_channel, (
	LIMdlPose*              self,
	int                     channel,
	int                     keep,
	const LIMdlPoseChannel* info));

LIAPICALL (void, limdl_pose_update, (
	LIMdlPose* self,
	float      secs));

LIAPICALL (int, limdl_pose_get_channel_blend_mode, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_blend_mode, (
	LIMdlPose* self,
	int        channel,
	int        value));

LIAPICALL (LIMdlAnimation*, limdl_pose_get_channel_animation, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_animation, (
	LIMdlPose*      self,
	int             channel,
	LIMdlAnimation* animation));

LIAPICALL (float, limdl_pose_get_channel_repeat_end, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_repeat_end, (
	LIMdlPose* self,
	int        channel,
	float      value));

LIAPICALL (float, limdl_pose_get_channel_repeat_start, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_repeat_start, (
	LIMdlPose* self,
	int        channel,
	float      value));

LIAPICALL (float, limdl_pose_get_channel_fade_in, (
	LIMdlPose* self,
	int        channel));

LIAPICALL (void, limdl_pose_set_channel_fade_in, (
	LIMdlPose* self,
	int        channel,
	int        mode,
	float      value));

LIAPICALL (float, limdl_pose_get_channel_fade_out, (
	LIMdlPose* self,
	int        channel));

LIAPICALL (void, limdl_pose_set_channel_fade_out, (
	LIMdlPose* self,
	int        channel,
	int        mode,
	float      value));

LIAPICALL (const char*, limdl_pose_get_channel_name, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (float, limdl_pose_get_channel_position, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_position, (
	LIMdlPose* self,
	int        channel,
	float      value));

LIAPICALL (int*, limdl_pose_get_channel_priority_node, (
	const LIMdlPose* self,
	int              channel,
	const char*      node));

LIAPICALL (int, limdl_pose_set_channel_priority_node, (
	LIMdlPose*  self,
	int         channel,
	const char* node,
	int         value));

LIAPICALL (LIAlgStrdic*, limdl_pose_get_channel_priority_nodes, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (int, limdl_pose_get_channel_priority_scale, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_priority_scale, (
	LIMdlPose* self,
	int        channel,
	int        value));

LIAPICALL (int, limdl_pose_get_channel_priority_transform, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_priority_transform, (
	LIMdlPose* self,
	int        channel,
	int      value));

LIAPICALL (float*, limdl_pose_get_channel_weight_node, (
	const LIMdlPose* self,
	int              channel,
	const char*      node));

LIAPICALL (int, limdl_pose_set_channel_weight_node, (
	LIMdlPose*  self,
	int         channel,
	const char* node,
	float       value));

LIAPICALL (LIAlgStrdic*, limdl_pose_get_channel_weight_nodes, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (float, limdl_pose_get_channel_weight_scale, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_weight_scale, (
	LIMdlPose* self,
	int        channel,
	float      value));

LIAPICALL (float, limdl_pose_get_channel_weight_transform, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_weight_transform, (
	LIMdlPose* self,
	int        channel,
	float      value));

LIAPICALL (int, limdl_pose_get_channel_repeats, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_repeats, (
	LIMdlPose* self,
	int        channel,
	int        value));

LIAPICALL (LIMdlPoseChannelState, limdl_pose_get_channel_state, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_state, (
	LIMdlPose*            self,
	int                   channel,
	LIMdlPoseChannelState value));

LIAPICALL (float, limdl_pose_get_channel_time_scale, (
	const LIMdlPose* self,
	int              channel));

LIAPICALL (void, limdl_pose_set_channel_time_scale, (
	LIMdlPose* self,
	int        channel,
	float      value));

#endif
