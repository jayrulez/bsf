//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentPrerequisites.h"
#include "BsDiligentRenderAPIFactory.h"

namespace bs
{
    extern "C" BS_PLUGIN_EXPORT const char* getPluginName()
    {
        return ct::DiligentRenderAPIFactory::SystemName;
    }
}
