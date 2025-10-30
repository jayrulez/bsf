//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_GUI_EXPORT
#    else
#      if defined(BS_GUI_EXPORTS)
#        define BS_GUI_EXPORT __declspec(dllexport)
#      else
#        define BS_GUI_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_GUI_EXPORT
#    else
#      if defined(BS_GUI_EXPORTS)
#        define BS_GUI_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_GUI_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_GUI_HIDDEN
#else
#  define BS_GUI_EXPORT __attribute__ ((visibility ("default")))
#  define BS_GUI_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

namespace bs
{
	class GUIElement;
	class GUIWidget;
	class GUIManager;

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for GUI module RTTI */
	enum TypeID_GUI
	{
		TID_CGUIWidget = 1081,
	};
}
