//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsTimerQuery.h"
#include "RenderAPI/Managers/BsQueryManager.h"

namespace bs {
	SPtr<TimerQuery> TimerQuery::create(UINT32 deviceIdx)
	{
		return QueryManager::instance().createTimerQuery(deviceIdx);
	}
}
