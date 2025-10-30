//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

#if (BS_PLATFORM == BS_PLATFORM_WIN32) && !defined(__MINGW32__)
#	ifdef BS_IMAGE_EXPORTS
#		define BS_IMAGE_EXPORT __declspec(dllexport)
#	else
#		if defined( __MINGW32__ )
#			define BS_IMAGE_EXPORT
#		else
#			define BS_IMAGE_EXPORT __declspec(dllimport)
#		endif
#	endif
#elif defined ( BS_GCC_VISIBILITY )
#	define BS_IMAGE_EXPORT  __attribute__ ((visibility("default")))
#else
#	define BS_IMAGE_EXPORT
#endif

namespace bs
{
	struct PixelData;
	class PixelUtil;
	struct PixelVolume;
}
