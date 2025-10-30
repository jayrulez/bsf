//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsResourcesPrerequisites.h"

#if (BS_PLATFORM == BS_PLATFORM_WIN32) && !defined(__MINGW32__)
#	ifdef BS_LOCALIZATION_EXPORTS
#		define BS_LOCALIZATION_EXPORT __declspec(dllexport)
#	else
#		define BS_LOCALIZATION_EXPORT __declspec(dllimport)
#	endif
#elif defined(__GNUC__)
#	if defined(BS_LOCALIZATION_EXPORTS)
#		define BS_LOCALIZATION_EXPORT __attribute__ ((visibility ("default")))
#	else
#		define BS_LOCALIZATION_EXPORT
#	endif
#else
#	define BS_LOCALIZATION_EXPORT
#endif

namespace bs
{
	class HString;
	class StringTable;
	class StringTableManager;

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<StringTable> HStringTable;

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for Localization module RTTI */
	enum TypeID_Localization
	{
		TID_StringTable = 1083,
		TID_LanguageData = 1084,
		TID_LocalizedStringData = 1085,
	};
}
