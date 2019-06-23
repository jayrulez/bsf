//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentRenderAPIFactory.h"
#include "RenderAPI/BsRenderAPI.h"
#include "BsDiligentRenderAPI.h"

namespace bs { namespace ct
{
	constexpr const char* DiligentRenderAPIFactory::SystemName;

	void DiligentRenderAPIFactory::create()
	{
		RenderAPI::startUp<DiligentRenderAPI>();
	}

	DiligentRenderAPIFactory::InitOnStart DiligentRenderAPIFactory::initOnStart;
}}
