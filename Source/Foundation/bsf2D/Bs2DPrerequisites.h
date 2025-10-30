//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsImagePrerequisites.h"

#if (BS_PLATFORM == BS_PLATFORM_WIN32) && !defined(__MINGW32__)
#	ifdef BS_2D_EXPORTS
#		define BS_2D_EXPORT __declspec(dllexport)
#	else
#		if defined( __MINGW32__ )
#			define BS_2D_EXPORT
#		else
#			define BS_2D_EXPORT __declspec(dllimport)
#		endif
#	endif
#elif defined ( BS_GCC_VISIBILITY )
#	define BS_2D_EXPORT  __attribute__ ((visibility("default")))
#else
#	define BS_2D_EXPORT
#endif

namespace bs
{
	class ImageSprite;
	class Sprite;
	class TextSprite;
	class SpriteMaterial;
	class SpriteMaterials;
	class SpriteManager;
	class SpriteTexture;
}
