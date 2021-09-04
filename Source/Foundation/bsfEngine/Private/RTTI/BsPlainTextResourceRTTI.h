//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "Resources/BsPlainTextResource.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class BS_EXPORT PlainTextResourceRTTI : public RTTIType <PlainTextResource, Resource, PlainTextResourceRTTI>
	{
	private:
		WString& getString(PlainTextResource* obj) { return obj->mString; }
		void setString(PlainTextResource* obj, WString& val) { obj->mString = val; }
	public:
		PlainTextResourceRTTI()
		{
			addPlainField("mString", 0, &PlainTextResourceRTTI::getString, &PlainTextResourceRTTI::setString);
		}

		const String& getRTTIName() override
		{
			static String name = "PlainTextResource";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_PlainText;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return PlainTextResource::_createPtr(L""); // Initial string doesn't matter, it'll get overwritten
		}
	};

	/** @} */
	/** @endcond */
}
