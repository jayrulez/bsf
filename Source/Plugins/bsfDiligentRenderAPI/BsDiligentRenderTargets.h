//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsDiligentPrerequisites.h"
#include "Managers/BsRenderWindowManager.h"
#include "RenderAPI/BsRenderTexture.h"

namespace bs
{
	/** Diligent implementation of a render texture. */
	class DiligentRenderTexture : public RenderTexture
	{
	public:
		DiligentRenderTexture(const RENDER_TEXTURE_DESC& desc)
			:RenderTexture(desc), mProperties(desc, false)
		{ }

		virtual ~DiligentRenderTexture() = default;

	protected:
		/** @copydoc RenderTexture::getProperties */
		const RenderTargetProperties& getPropertiesInternal() const override { return mProperties; }

		RenderTextureProperties mProperties;	
	};

	namespace ct
	{
		/** Diligent implementation of a render texture. */
		class DiligentRenderTexture : public RenderTexture
		{
		public:
			DiligentRenderTexture(const RENDER_TEXTURE_DESC& desc, UINT32 deviceIdx)
				:RenderTexture(desc, deviceIdx), mProperties(desc, false)
			{ }

			virtual ~DiligentRenderTexture() = default;

		protected:
			/** @copydoc RenderTexture::getProperties */
			const RenderTargetProperties& getPropertiesInternal() const override { return mProperties; }

			RenderTextureProperties mProperties;	
		};
	}

	/** @} */
}
