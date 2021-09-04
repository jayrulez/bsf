//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsMathRTTI.h"
#include "Particles/BsVectorFieldResource.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT VectorFieldResourceRTTI : public RTTIType<VectorFieldResource, Resource, VectorFieldResourceRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN_NAMED(countX, mDesc.countX, 0)
			BS_RTTI_MEMBER_PLAIN_NAMED(countY, mDesc.countY, 1)
			BS_RTTI_MEMBER_PLAIN_NAMED(countZ, mDesc.countZ, 2)
			BS_RTTI_MEMBER_PLAIN_NAMED(bounds, mDesc.bounds, 3)
			BS_RTTI_MEMBER_REFLPTR(mTexture, 4)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "VectorFieldResource";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_VectorField;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return VectorFieldResource::_createEmpty();
		}

	protected:
		void onDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			VectorFieldResource* vectorField = static_cast<VectorFieldResource*>(obj);
			vectorField->initialize();
		}
	};

	/** @} */
	/** @endcond */
}
