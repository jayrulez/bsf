//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Common/BsCoreObject2.h"
#include "CoreThread/BsCoreThread.h"
#include "Common/BsCoreObject2Manager.h"

using namespace std::placeholders;

namespace bs
{
	CoreObject2::CoreObject2()
		: mFlags(CGO2_NONE)
		, mInternalID(CoreObject2Manager::instance().generateId())
	{
	}

	CoreObject2::~CoreObject2()
	{
		if(!isDestroyed())
		{
			// Object must be released with destroy() otherwise engine can still try to use it, even if it was destructed
			// (e.g. if an object has one of its methods queued in a command queue, and is destructed, you will be accessing invalid memory)
			BS_EXCEPT(InternalErrorException, "Destructor called but object is not destroyed. This will result in nasty issues.");
		}

#if BS_DEBUG_MODE
		if(!mThis.expired())
		{
			BS_EXCEPT(InternalErrorException, "Shared pointer to this object still has active references but " \
				"the object is being deleted? You shouldn't delete CoreObject2s manually.");
		}
#endif
	}

	void CoreObject2::destroy()
	{
		CoreObject2Manager::instance().unregisterObject(this);
		setIsDestroyed(true);
	}

	void CoreObject2::initialize()
	{
		CoreObject2Manager::instance().registerObject(this);

		mFlags |= CGO2_INITIALIZED;
	}

	void CoreObject2::_setThisPtr(SPtr<CoreObject2> ptrThis)
	{
		mThis = ptrThis;
	}
}
