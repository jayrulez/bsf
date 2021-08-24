//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"
#include "String/BsStringID.h"
#include "Resources/BsResourceMetaData.h"
#include "Material/BsTechnique.h"
#include "Material/BsShaderResource.h"

namespace bs
{
	namespace ct
	{
	/** @addtogroup Material-Internal
	 *  @{
	 */

	/** Core thread version of Shader. */
	class BS_CORE_EXPORT Shader : public CoreObject, public TShader<true>
	{
	public:
		/** @copydoc bs::ShaderResource::create */
		static SPtr<Shader> create(const String& name, const SHADER_DESC& desc);

	protected:
		friend class bs::ShaderResource;

		Shader(const String& name, const SHADER_DESC& desc, UINT32 id);

		static std::atomic<UINT32> mNextShaderId;
	};

	/** @} */
	}
}
