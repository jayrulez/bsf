//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_ANIMATION_EXPORT
#    else
#      if defined(BS_ANIMATION_EXPORTS)
#        define BS_ANIMATION_EXPORT __declspec(dllexport)
#      else
#        define BS_ANIMATION_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_ANIMATION_EXPORT
#    else
#      if defined(BS_ANIMATION_EXPORTS)
#        define BS_ANIMATION_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_ANIMATION_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_ANIMATION_HIDDEN
#else
#  define BS_ANIMATION_EXPORT __attribute__ ((visibility ("default")))
#  define BS_ANIMATION_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

namespace bs
{
	class Animation;
	class AnimationClip;
	class Skeleton;
	class Bone;
	class MorphShapes;
	class MorphShape;
	class MorphChannel;

	template <class T> class TAnimationCurve;
	struct AnimationCurves;

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<AnimationClip> HAnimationClip;

	/************************************************************************/
	/* 						Game object handle references             		*/
	/************************************************************************/

	template<class T>
	class GameObjectHandle;

	// Game object handles
	typedef GameObjectHandle<CAnimation> HAnimation;
	typedef GameObjectHandle<CBone> HBone;

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for Animation module RTTI */
	enum TypeID_Animation
	{
		TID_AnimationClip = 1115,
		TID_AnimationCurve = 1116,
		TID_KeyFrame = 1117,
		TID_NamedAnimationCurve = 1118,
		TID_Skeleton = 1119,
		TID_SkeletonBoneInfo = 1120,
		TID_AnimationSplitInfo = 1121,
		TID_CAnimation = 1122,
		TID_AnimationEvent = 1123,
		TID_ImportedAnimationEvents = 1124,
		TID_CBone = 1125,
		TID_MorphShape = 1128,
		TID_MorphShapes = 1129,
		TID_MorphChannel = 1130,
	};
}
