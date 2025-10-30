//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsResourcesPrerequisites.h"

#if (BS_PLATFORM == BS_PLATFORM_WIN32) && !defined(__MINGW32__)
#	ifdef BS_TEXT_EXPORTS
#		define BS_TEXT_EXPORT __declspec(dllexport)
#	else
#		if defined( __MINGW32__ )
#			define BS_TEXT_EXPORT
#		else
#			define BS_TEXT_EXPORT __declspec(dllimport)
#		endif
#	endif
#elif defined ( BS_GCC_VISIBILITY )
#	define BS_TEXT_EXPORT  __attribute__ ((visibility("default")))
#else
#	define BS_TEXT_EXPORT
#endif

namespace bs
{
	class TextData;
	class FontImportOptions;
	struct FontDesc;
	class Font;
	class FontManager;
	struct FontBitmap;
	class FontRTTI;

	// Desc structs
	struct FONT_DESC;

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<Font> HFont;

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for Text module RTTI */
	enum TypeID_Text
	{
		TID_Font = 1051,
		TID_FONT_DESC = 1052,
		TID_CHAR_DESC = 1053,
		TID_FontImportOptions = 1056,
		TID_FontBitmap = 1057,
	};
}
