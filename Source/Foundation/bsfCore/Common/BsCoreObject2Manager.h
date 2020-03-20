//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Utility/BsModule.h"
#include "Common/BsCoreObject2.h"

namespace bs
{
	/** @addtogroup Common
	 *  @{
	 */

	// TODO Low priority - Add debug option that would remember a call stack for each resource initialization,
	// so when we fail to release one we know which one it is.
	
	/**
	 * Manager that keeps track of all active CoreObject2%s.
	 * 			
	 * @note	Internal class.
	 * @note	Thread safe unless specified otherwise.
	 */
	class BS_CORE_EXPORT CoreObject2Manager : public Module<CoreObject2Manager>
	{
	public:
		CoreObject2Manager();
		~CoreObject2Manager();

		/** Generates a new unique ID for a core object. */
		UINT64 generateId();

		/** Registers a new CoreObject2 notifying the manager the object	is created. */
		void registerObject(CoreObject2* object);

		/** Unregisters a CoreObject2 notifying the manager the object is destroyed. */
		void unregisterObject(CoreObject2* object);

	private:
		UINT64 mNextAvailableID;
		Map<UINT64, CoreObject2*> mObjects;

		Mutex mObjectsMutex;
	};

	/** @} */
}

