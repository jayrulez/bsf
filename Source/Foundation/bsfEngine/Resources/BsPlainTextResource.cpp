//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Resources/BsPlainTextResource.h"
#include "Resources/BsResources.h"
#include "Private/RTTI/BsPlainTextResourceRTTI.h"

namespace bs
{
	PlainTextResource::PlainTextResource(const WString& data)
		:Resource(false), mString(data)
	{

	}

	PlainTextResourceHandle PlainTextResource::create(const WString& data)
	{
		return static_resource_cast<PlainTextResource>(gResources()._createResourceHandle(_createPtr(data)));
	}

	SPtr<PlainTextResource> PlainTextResource::_createPtr(const WString& data)
	{
		SPtr<PlainTextResource> plainTextPtr = bs_core_ptr<PlainTextResource>(
			new (bs_alloc<PlainTextResource>()) PlainTextResource(data));
		plainTextPtr->_setThisPtr(plainTextPtr);
		plainTextPtr->initialize();

		return plainTextPtr;
	}

	RTTITypeBase* PlainTextResource::getRTTIStatic()
	{
		return PlainTextResourceRTTI::instance();
	}

	RTTITypeBase* PlainTextResource::getRTTI() const
	{
		return PlainTextResource::getRTTIStatic();
	}
}
