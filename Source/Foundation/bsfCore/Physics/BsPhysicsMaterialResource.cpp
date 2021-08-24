//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsPhysicsMaterialResource.h"
#include "Private/RTTI/BsPhysicsMaterialResourceRTTI.h"
#include "Resources/BsResources.h"
#include "Physics/BsPhysics.h"

namespace bs
{
	PhysicsMaterialResourceHandle PhysicsMaterialResource::create(float staticFriction, float dynamicFriction, float restitution)
	{
		SPtr<PhysicsMaterialResource> newMaterial = _createPtr(staticFriction, dynamicFriction, restitution);

		return static_resource_cast<PhysicsMaterialResource>(gResources()._createResourceHandle(newMaterial));
	}

	SPtr<PhysicsMaterialResource> PhysicsMaterialResource::_createPtr(float staticFriction, float dynamicFriction, float restitution)
	{
		SPtr<PhysicsMaterialResource> newMaterial = gPhysics().createMaterial(staticFriction, dynamicFriction, restitution);
		newMaterial->_setThisPtr(newMaterial);
		newMaterial->initialize();

		return newMaterial;
	}

	RTTITypeBase* PhysicsMaterialResource::getRTTIStatic()
	{
		return PhysicsMaterialResourceRTTI::instance();
	}

	RTTITypeBase* PhysicsMaterialResource::getRTTI() const
	{
		return getRTTIStatic();
	}
}
