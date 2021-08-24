//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "Material/BsShaderIncludeResource.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT ShaderIncludeResourceRTTI : public RTTIType <ShaderIncludeResource, Resource, ShaderIncludeResourceRTTI>
	{
	private:
		String& getString(ShaderIncludeResource* obj) { return obj->mString; }
		void setString(ShaderIncludeResource* obj, String& val) { obj->mString = val; }
	public:
		ShaderIncludeResourceRTTI()
		{
			addPlainField("mString", 0, &ShaderIncludeResourceRTTI::getString, &ShaderIncludeResourceRTTI::setString);
		}

		const String& getRTTIName() override
		{
			static String name = "ShaderIncludeResource";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_ShaderInclude;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return ShaderIncludeResource::_createPtr(""); // Initial string doesn't matter, it'll get overwritten
		}
	};

	/** @} */
	/** @endcond */
}
