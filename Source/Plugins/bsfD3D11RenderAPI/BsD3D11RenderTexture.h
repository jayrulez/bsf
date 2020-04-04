//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsD3D11Prerequisites.h"
#include "Image/BsTexture.h"
#include "RenderAPI/BsRenderTexture.h"

namespace bs
{
	/** @addtogroup D3D11
	 *  @{
	 */

	/**
	 * DirectX 11 implementation of a render texture.
	 *
	 * @note	Sim thread only.
	 */
	class D3D11RenderTexture : public RenderTexture
	{
	public:

		D3D11RenderTexture(const RENDER_TEXTURE_DESC& desc);
		virtual ~D3D11RenderTexture() { }
		D3D11RenderTexture(const RENDER_TEXTURE_DESC& desc, UINT32 deviceIdx);

		/** @copydoc RenderTexture::getCustomAttribute */
		void getCustomAttribute(const String& name, void* data) const override;

	protected:
		friend class D3D11TextureManager;

		/** @copydoc RenderTexture::getProperties */
		const RenderTargetProperties& getPropertiesInternal() const override { return mProperties; }

		RenderTextureProperties mProperties;
	};

	/** @} */
}
