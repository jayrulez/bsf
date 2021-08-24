//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Material/BsShader.h"
#include "Material/BsTechnique.h"
#include "Error/BsException.h"
#include "Debug/BsDebug.h"
#include "Resources/BsResources.h"
#include "RenderAPI/BsGpuParams.h"
#include "Material/BsPass.h"
#include "RenderAPI/BsSamplerState.h"
#include "Image/BsTexture.h"

namespace bs
{
	namespace ct
	{
	std::atomic<UINT32> Shader::mNextShaderId;

	Shader::Shader(const String& name, const SHADER_DESC& desc, UINT32 id)
		:TShader(name, desc, id)
	{

	}

	SPtr<Shader> Shader::create(const String& name, const SHADER_DESC& desc)
	{
		UINT32 id = mNextShaderId.fetch_add(1, std::memory_order_relaxed);
		assert(id < std::numeric_limits<UINT32>::max() && "Created too many shaders, reached maximum id.");

		Shader* shaderCore = new (bs_alloc<Shader>()) Shader(name, desc, id);
		SPtr<Shader> shaderCorePtr = bs_shared_ptr<Shader>(shaderCore);
		shaderCorePtr->_setThisPtr(shaderCorePtr);
		shaderCorePtr->initialize();

		return shaderCorePtr;
	}
	}
}
