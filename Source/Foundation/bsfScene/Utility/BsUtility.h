//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"

namespace bs
{
	class GameObjectDeserializationState;
	
	/** @addtogroup Utility-Core-Internal
	 *  @{
	 */

	/** Static class containing various utility methods that do not fit anywhere else. */
	class BS_CORE_EXPORT Utility
	{
	public:
		/**
		 * Finds all components of a specific type on a scene object and any of its children.
		 *
		 * @param[in]	object		Object which to search for components. All children will be searched as well.
		 * @param[in]	typeId		RTTI type ID of the component type to search for.
		 * @return					A list of all components of the specified type.
		 */
		static Vector<HComponent> findComponents(const HSceneObject& object, UINT32 typeId);

		/** Calculates how deep in the scene object hierarchy is the provided object. Zero means root. */
		static UINT32 getSceneObjectDepth(const HSceneObject& so);
	};

	/** Provides extra information and maintains state during serialization of various RTTI types in the core. */
	struct BS_CORE_EXPORT CoreSerializationContext : SerializationContext
	{
		SPtr<GameObjectDeserializationState> goState;
		bool goDeserializationActive = false;

		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};

	/** @} */
}
