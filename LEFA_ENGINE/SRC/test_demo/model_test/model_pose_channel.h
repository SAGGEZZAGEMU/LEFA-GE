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

#ifndef __MODEL_POSE_CHANNEL_H__
#define __MODEL_POSE_CHANNEL_H__

//#include "lipsofsuna/algorithm.h"
#include"ANOTHER/algorithm.h"
#include"model_animation.h"
#include"model_types.h"

enum _LIMdlPoseChannelState
{
	LIMDL_POSE_CHANNEL_STATE_INVALID,
	LIMDL_POSE_CHANNEL_STATE_PLAYING,
	LIMDL_POSE_CHANNEL_STATE_PAUSED
};

enum _LIMdlPoseBlendMode
{
	LIMDL_POSE_BLEND_ADD,
	LIMDL_POSE_BLEND_MIX,
	LIMDL_POSE_BLEND_REPLACE
};

enum _LIMdlFadeInMode
{
	LIMDL_POSE_FADE_IN_AFTER_START,
	LIMDL_POSE_FADE_IN_BEFORE_START,
	LIMDL_POSE_FADE_IN_INSTANT
};

enum _LIMdlFadeOutMode
{
	LIMDL_POSE_FADE_OUT_AFTER_END,
	LIMDL_POSE_FADE_OUT_AFTER_END_REPEAT,
	LIMDL_POSE_FADE_OUT_BEFORE_END,
	LIMDL_POSE_FADE_OUT_INSTANT
};

struct _LIMdlPoseChannel
{
	int blend_mode;
	int state;
	int repeat;
	int repeats;
	float repeat_start;
	float repeat_end;
	float time;
	float time_scale;
	int priority_scale;
	int priority_transform;
	float weight_scale;
	float weight_transform;
	float fade_timer;
	struct
	{
		int active;
		int mode;
		float duration;
	} fade_in;
	struct
	{
		int active;
		int mode;
		float duration;
	} fade_out;
	LIAlgStrdic* priorities;
	LIAlgStrdic* weights;
	LIMdlAnimation* animation;
};

LIAPICALL (LIMdlPoseChannel*, limdl_pose_channel_new, (
	LIMdlAnimation* animation));

LIAPICALL (LIMdlPoseChannel*, limdl_pose_channel_new_copy, (
	LIMdlPoseChannel* channel));

LIAPICALL (void, limdl_pose_channel_free, (
	LIMdlPoseChannel* self));

LIAPICALL (int, limdl_pose_channel_compare_scale, (
	const LIMdlPoseChannel* self,
	const LIMdlPoseChannel* chan));

LIAPICALL (int, limdl_pose_channel_compare_transform, (
	const LIMdlPoseChannel* self,
	const LIMdlPoseChannel* chan));

LIAPICALL (int, limdl_pose_channel_play, (
	LIMdlPoseChannel* self,
	float             secs));

LIAPICALL (float, limdl_pose_channel_get_fading, (
	const LIMdlPoseChannel* self));

LIAPICALL (float, limdl_pose_channel_get_current_time, (
	const LIMdlPoseChannel* self));

LIAPICALL (float, limdl_pose_channel_get_total_time, (
	const LIMdlPoseChannel* self));

LIAPICALL (void, limdl_pose_channel_get_node_priority, (
	LIMdlPoseChannel* self,
	const char*       node,
	int*              scale,
	int*              transform));

LIAPICALL (int, limdl_pose_channel_set_node_priority, (
	LIMdlPoseChannel* self,
	const char*       node,
	int               value));

LIAPICALL (int, limdl_pose_channel_set_node_weight, (
	LIMdlPoseChannel* self,
	const char*       node,
	float             value));

LIAPICALL (void, limdl_pose_channel_get_weight, (
	const LIMdlPoseChannel* self,
	const char*             node,
	float*                  scale,
	float*                  transform));

#endif
