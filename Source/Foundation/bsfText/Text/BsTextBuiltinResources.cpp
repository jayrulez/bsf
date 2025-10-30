//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Text/BsTextBuiltinResources.h"
#include "Text/BsFont.h"
#include "Resources/BsResources.h"
#include "Utility/BsPaths.h"

namespace bs
{
	TextBuiltinResources::TextBuiltinResources()
	{
		mBuiltinDataFolder = Paths::getDataPath();
		mFont = gResources().load<Font>(mBuiltinDataFolder + (String(DEFAULT_FONT_NAME) + u8".asset"));
	}

	TextBuiltinResources& gTextBuiltinResources()
	{
		return TextBuiltinResources::instance();
	}
}
