//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Material/BsShaderIncludeResource.h"
#include "Resources/BsResources.h"
#include "Private/RTTI/BsShaderIncludeResourceRTTI.h"

namespace bs
{
	ShaderIncludeResource::ShaderIncludeResource(const String& includeString)
		:Resource(false), mString(includeString)
	{

	}

	ShaderIncludeResourceHandle ShaderIncludeResource::create(const String& includeString)
	{
		return static_resource_cast<ShaderIncludeResource>(gResources()._createResourceHandle(_createPtr(includeString)));
	}

	SPtr<ShaderIncludeResource> ShaderIncludeResource::_createPtr(const String& includeString)
	{
		SPtr<ShaderIncludeResource> shaderIncludePtr = bs_core_ptr<ShaderIncludeResource>(
			new (bs_alloc<ShaderIncludeResource>()) ShaderIncludeResource(includeString));
		shaderIncludePtr->_setThisPtr(shaderIncludePtr);
		shaderIncludePtr->initialize();

		return shaderIncludePtr;
	}

	RTTITypeBase* ShaderIncludeResource::getRTTIStatic()
	{
		return ShaderIncludeResourceRTTI::instance();
	}

	RTTITypeBase* ShaderIncludeResource::getRTTI() const
	{
		return ShaderIncludeResource::getRTTIStatic();
	}
}
