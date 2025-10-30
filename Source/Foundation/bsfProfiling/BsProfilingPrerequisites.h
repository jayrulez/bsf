//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

#if (BS_PLATFORM == BS_PLATFORM_WIN32) && !defined(__MINGW32__)
#	ifdef BS_PROFILING_EXPORTS
#		define BS_PROFILING_EXPORT __declspec(dllexport)
#	else
#		define BS_PROFILING_EXPORT __declspec(dllimport)
#	endif
#elif defined(__GNUC__)
#	if defined(BS_PROFILING_EXPORTS)
#		define BS_PROFILING_EXPORT __attribute__ ((visibility ("default")))
#	else
#		define BS_PROFILING_EXPORT
#	endif
#else
#	define BS_PROFILING_EXPORT
#endif

namespace bs
{
	class ProfilerCPU;
	class ProfilerGPU;
	class ProfilingManager;
	struct ProfilerReport;

	// Special types for use by profilers
	typedef std::basic_string<char, std::char_traits<char>, StdAlloc<char, ProfilerAlloc>> ProfilerString;

	template <typename T, typename A = StdAlloc<T, ProfilerAlloc>>
	using ProfilerVector = std::vector<T, A>;

	template <typename T, typename A = StdAlloc<T, ProfilerAlloc>>
	using ProfilerStack = std::stack<T, std::deque<T, A>>;
}
