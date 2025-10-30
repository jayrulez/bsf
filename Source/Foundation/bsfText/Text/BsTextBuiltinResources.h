//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsTextPrerequisites.h"
#include "Utility/BsModule.h"

namespace bs
{
	/** @addtogroup Text
	 *  @{
	 */

	/**	Holds references to built-in text/font resources used by the engine. */
	class BS_TEXT_EXPORT TextBuiltinResources : public Module<TextBuiltinResources>
	{
	public:
		TextBuiltinResources();
		~TextBuiltinResources() = default;

		/** Returns the default font used by the engine. */
		HFont getDefaultFont() const { return mFont; }

		static constexpr const char* DEFAULT_FONT_NAME = u8"arial.ttf";
		static constexpr const UINT32 DEFAULT_FONT_SIZE = 8;

	private:
		HFont mFont;

		Path mBuiltinDataFolder;
	};

	/**	Provides easy access to TextBuiltinResources. */
	BS_TEXT_EXPORT TextBuiltinResources& gTextBuiltinResources();

	/** @} */
}
