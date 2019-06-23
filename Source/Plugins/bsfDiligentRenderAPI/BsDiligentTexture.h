//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsDiligentPrerequisites.h"
#include "Image/BsTexture.h"
#include "Managers/BsTextureManager.h"

namespace bs
{
	/** @addtogroup DiligentRenderAPI
	 *  @{
	 */

	/**	Handles creation of Diligent textures. */
	class DiligentTextureManager final : public TextureManager
	{
	public:
		/** @copydoc TextureManager::getNativeFormat */
		PixelFormat getNativeFormat(TextureType ttype, PixelFormat format, int usage, bool hwGamma) override;

	protected:		
		/** @copydoc TextureManager::createRenderTextureImpl */
		SPtr<RenderTexture> createRenderTextureImpl(const RENDER_TEXTURE_DESC& desc) override;
	};

	namespace ct
	{
		/**	Handles creation of Diligent textures. */
		class DiligentTextureManager : public TextureManager
		{
		protected:
			/** @copydoc TextureManager::createTextureInternal */
			SPtr<Texture> createTextureInternal(const TEXTURE_DESC& desc, 
				const SPtr<PixelData>& initialData = nullptr, GpuDeviceFlags deviceMask = GDF_DEFAULT) override;

			/** @copydoc TextureManager::createRenderTextureInternal */
			SPtr<RenderTexture> createRenderTextureInternal(const RENDER_TEXTURE_DESC& desc, 
				UINT32 deviceIdx = 0) override;
		};

		/**	Diligent implementation of a texture. */
		class DiligentTexture final : public Texture
		{
		public:
			DiligentTexture(const TEXTURE_DESC& desc, const SPtr<PixelData>& initialData, GpuDeviceFlags deviceMask);
			~DiligentTexture();

		protected:
			/** @copydoc Texture::lockImpl */
			PixelData lockImpl(GpuLockOptions options, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
				UINT32 queueIdx = 0) override;

			/** @copydoc Texture::unlockImpl */
			void unlockImpl() override;

			/** @copydoc Texture::copyImpl */
			void copyImpl(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc,
				const SPtr<CommandBuffer>& commandBuffer) override { }

			/** @copydoc Texture::readData */
			void readDataImpl(PixelData& dest, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
				UINT32 queueIdx = 0) override { }

			/** @copydoc Texture::writeData */
			void writeDataImpl(const PixelData& src, UINT32 mipLevel = 0, UINT32 face = 0, bool discardWholeBuffer = false,
				UINT32 queueIdx = 0) override { }

		protected:
			PixelData* mMappedBuffer = nullptr;
		};
	}

	/** @} */
}
