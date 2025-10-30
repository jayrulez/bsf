//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_RUNTIME_EXPORT
#    else
#      if defined(BS_RUNTIME_EXPORTS)
#        define BS_RUNTIME_EXPORT __declspec(dllexport)
#      else
#        define BS_RUNTIME_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_RUNTIME_EXPORT
#    else
#      if defined(BS_RUNTIME_EXPORTS)
#        define BS_RUNTIME_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_RUNTIME_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_RUNTIME_HIDDEN
#else
#  define BS_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#  define BS_RUNTIME_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

namespace bs
{
	class Application;
	class CoreApplication;

	/** Default thread policy for the framework. Performs special startup/shutdown on threads managed by thread pool. */
	class BS_RUNTIME_EXPORT ThreadDefaultPolicy
	{
	public:
		static void onThreadStarted(const String& name)
		{
			MemStack::beginThread();
		}

		static void onThreadEnded(const String& name)
		{
			MemStack::endThread();
		}
	};
}
