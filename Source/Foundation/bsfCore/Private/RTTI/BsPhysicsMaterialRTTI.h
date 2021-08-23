//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "Physics/BsPhysicsMaterial.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT PhysicsMaterialResourceRTTI : public RTTIType<PhysicsMaterialResource, Resource, PhysicsMaterialResourceRTTI>
	{
	private:
		float& getStaticFriction(PhysicsMaterialResource* obj)
		{
			return mStaticFriction;
		}

		void setStaticFriction(PhysicsMaterialResource* obj, float& size) { obj->setStaticFriction(size); }

		float& getDynamicFriction(PhysicsMaterialResource* obj)
		{
			return mDynamicFriction;
		}

		void setDynamicFriction(PhysicsMaterialResource* obj, float& size) { obj->setDynamicFriction(size); }

		float& getRestitutionCoefficient(PhysicsMaterialResource* obj)
		{
			return mRestitutionCoefficient;
		}

		void setRestitutionCoefficient(PhysicsMaterialResource* obj, float& size) { obj->setRestitutionCoefficient(size); }

	public:
		PhysicsMaterialResourceRTTI()
		{
			addPlainField("staticFriction", 0, &PhysicsMaterialResourceRTTI::getStaticFriction, &PhysicsMaterialResourceRTTI::setStaticFriction);
			addPlainField("dynamicFriction", 1, &PhysicsMaterialResourceRTTI::getDynamicFriction, &PhysicsMaterialResourceRTTI::setDynamicFriction);
			addPlainField("restitutionCoefficient", 2, &PhysicsMaterialResourceRTTI::getRestitutionCoefficient, &PhysicsMaterialResourceRTTI::setRestitutionCoefficient);
		}

		void onSerializationStarted(IReflectable* obj, SerializationContext* context) override
		{
			PhysicsMaterialResource* material = static_cast<PhysicsMaterialResource*>(obj);

			mStaticFriction = material->getStaticFriction();
			mDynamicFriction = material->getDynamicFriction();
			mRestitutionCoefficient = material->getRestitutionCoefficient();
		}

		const String& getRTTIName() override
		{
			static String name = "PhysicsMaterialResource";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_PhysicsMaterial;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return PhysicsMaterialResource::_createPtr();
		}

	private:
		float mStaticFriction;
		float mDynamicFriction;
		float mRestitutionCoefficient;
	};

	/** @} */
	/** @endcond */
}
