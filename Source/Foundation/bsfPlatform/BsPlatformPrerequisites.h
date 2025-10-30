//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Debug/BsDebug.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup Platform Platform
 *	Platform abstraction layer providing OS-specific functionality.
 *  @{
 */

/** @defgroup Platform-Engine Platform
  *	OS-specific functionality like file system monitoring, cursor management, and drag & drop.
  */

/** @} */
/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_PLATFORM_EXPORT
#    else
#      if defined(BS_PLATFORM_EXPORTS)
#        define BS_PLATFORM_EXPORT __declspec(dllexport)
#      else
#        define BS_PLATFORM_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_PLATFORM_EXPORT
#    else
#      if defined(BS_PLATFORM_EXPORTS)
#        define BS_PLATFORM_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_PLATFORM_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_PLATFORM_HIDDEN
#else // Linux/Mac settings
#  define BS_PLATFORM_EXPORT __attribute__ ((visibility ("default")))
#  define BS_PLATFORM_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	class Platform;
	class FolderMonitor;
	class DropTarget;
	class Cursor;
	struct CursorData;
	class RenderWindow;

	/**	Types of special input commands. */
	enum class InputCommandType
	{
		CursorMoveLeft, CursorMoveRight, CursorMoveUp, CursorMoveDown,
		SelectLeft, SelectRight, SelectUp, SelectDown,
		Escape, Delete, Backspace, Return, Confirm, Tab
	};
}
