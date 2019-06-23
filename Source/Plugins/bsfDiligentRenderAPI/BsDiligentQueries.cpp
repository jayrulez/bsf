//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentQueries.h"

namespace bs { namespace ct
{
	SPtr<EventQuery> DiligentQueryManager::createEventQuery(UINT32 deviceIdx) const
	{
		SPtr<EventQuery> query = SPtr<DiligentEventQuery>(bs_new<DiligentEventQuery>(), &QueryManager::deleteEventQuery, 
			StdAlloc<DiligentEventQuery>());
		mEventQueries.push_back(query.get());

		return query;
	}

	SPtr<TimerQuery> DiligentQueryManager::createTimerQuery(UINT32 deviceIdx) const
	{
		SPtr<TimerQuery> query = SPtr<DiligentTimerQuery>(bs_new<DiligentTimerQuery>(), &QueryManager::deleteTimerQuery, 
			StdAlloc<DiligentTimerQuery>());
		mTimerQueries.push_back(query.get());

		return query;
	}

	SPtr<OcclusionQuery> DiligentQueryManager::createOcclusionQuery(bool binary, UINT32 deviceIdx) const
	{
		SPtr<OcclusionQuery> query = SPtr<DiligentOcclusionQuery>(bs_new<DiligentOcclusionQuery>(binary), 
			&QueryManager::deleteOcclusionQuery, StdAlloc<DiligentOcclusionQuery>());
		mOcclusionQueries.push_back(query.get());

		return query;
	}
}}
