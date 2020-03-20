//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Common/BsCoreObject2Manager.h"
#include "Common/BsCoreObject2.h"
#include "Error/BsException.h"

namespace bs
{
	CoreObject2Manager::CoreObject2Manager()
		:mNextAvailableID(1)
	{

	}

	CoreObject2Manager::~CoreObject2Manager()
	{
#if BS_DEBUG_MODE
		Lock lock(mObjectsMutex);

		if(mObjects.size() > 0)
		{
			// All objects MUST be destroyed at this point, otherwise there might be memory corruption.
			// (Reason: This is called on application shutdown and at that point we also unload any dynamic libraries,
			// which will invalidate any pointers to objects created from those libraries. Therefore we require of the user to
			// clean up all objects manually before shutting down the application).
			BS_EXCEPT(InternalErrorException, "Core object manager shut down, but not all objects were released. Application must release ALL " \
				"engine objects before shutdown.");
		}
#endif
	}

	UINT64 CoreObject2Manager::generateId()
	{
		Lock lock(mObjectsMutex);

		return mNextAvailableID++;
	}

	void CoreObject2Manager::registerObject(CoreObject2* object)
	{
		Lock lock(mObjectsMutex);

		UINT64 objId = object->getInternalID();
		mObjects[objId] = object;
	}

	void CoreObject2Manager::unregisterObject(CoreObject2* object)
	{
		assert(object != nullptr && !object->isDestroyed());

		UINT64 internalId = object->getInternalID();

		{
			Lock lock(mObjectsMutex);

			mObjects.erase(internalId);
		}
	}
}
