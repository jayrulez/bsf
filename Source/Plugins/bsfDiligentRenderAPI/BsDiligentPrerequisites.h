//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"

#if BS_PLATFORM == BS_PLATFORM_WIN32
#include <Windows.h>
#elif BS_PLATFORM == BS_PLATFORM_LINUX
#elif BS_PLATFORM == BS_PLATFORM_ANDROID
#elif BS_PLATFORM == BS_PLATFORM_OSX
#endif

/** @addtogroup Plugins
 *  @{
 */

/** @defgroup DiligentRenderAPI bsfDiligentRenderAPI
 *	Diligent (no-op) render API implementation.
 */

/** @} */
namespace bs { namespace ct {
	class DiligentRenderAPI;
	class Win32RenderWindow;
	class Win32VideoMode;
}}