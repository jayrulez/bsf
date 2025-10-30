//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

#if (BS_PLATFORM == BS_PLATFORM_WIN32) && !defined(__MINGW32__)
#	ifdef BS_INPUT_EXPORTS
#		define BS_INPUT_EXPORT __declspec(dllexport)
#	else
#		define BS_INPUT_EXPORT __declspec(dllimport)
#	endif
#elif defined(__GNUC__)
#	if defined(BS_INPUT_EXPORTS)
#		define BS_INPUT_EXPORT __attribute__ ((visibility ("default")))
#	else
#		define BS_INPUT_EXPORT
#	endif
#else
#	define BS_INPUT_EXPORT
#endif

namespace bs
{
	class Input;
	class InputConfiguration;
	class VirtualInput;
	class Keyboard;
	class Mouse;
	class Gamepad;

	struct PointerEvent;
}
