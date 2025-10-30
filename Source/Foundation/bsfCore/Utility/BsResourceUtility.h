//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsResourcesPrerequisites.h"
#include "Resources/BsResourceHandle.h"

namespace bs
{
	/** @addtogroup Resources
	 *  @{
	 */

	/** Contains information about a resource dependency, including the dependant resource and number of references to it. */
	struct ResourceDependency
	{
		ResourceDependency() = default;

		HResource resource;
		UINT32 numReferences = 0;
	};

	/** Utility methods for working with resources. */
	class BS_RESOURCES_EXPORT ResourceUtility
	{
	public:
		/**
		 * Finds all unique resources referenced by the specified object.
		 *
		 * @param[in]	object		Object to search for resource dependencies.
		 * @param[in]	recursive	Determines whether or not child objects will also be searched (if the object has any
		 *							children).
		 * @return					A list of unique, non-null resources.
		 */
		static Vector<ResourceDependency> findResourceDependencies(IReflectable& object, bool recursive = true);
	};

	/** @} */
}
