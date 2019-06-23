//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentTexture.h"
#include "BsDiligentRenderTargets.h"

namespace bs
{
	SPtr<RenderTexture> DiligentTextureManager::createRenderTextureImpl(const RENDER_TEXTURE_DESC& desc)
	{
		return bs_core_ptr_new<DiligentRenderTexture>(desc);
	}

	PixelFormat DiligentTextureManager::getNativeFormat(TextureType ttype, PixelFormat format, int usage, bool hwGamma)
	{
		return format;
	}

	namespace ct 
	{
		SPtr<Texture> DiligentTextureManager::createTextureInternal(const TEXTURE_DESC& desc, const SPtr<PixelData>& initialData,
			GpuDeviceFlags deviceMask)
		{
			SPtr<DiligentTexture> texPtr = bs_shared_ptr_new<DiligentTexture>(desc, initialData, deviceMask);
			texPtr->_setThisPtr(texPtr);

			return texPtr;
		}

		SPtr<RenderTexture> DiligentTextureManager::createRenderTextureInternal(const RENDER_TEXTURE_DESC& desc, UINT32 deviceIdx)
		{
			SPtr<DiligentRenderTexture> texPtr = bs_shared_ptr_new<DiligentRenderTexture>(desc, deviceIdx);
			texPtr->_setThisPtr(texPtr);

			return texPtr;
		}

		DiligentTexture::DiligentTexture(const TEXTURE_DESC& desc, const SPtr<PixelData>& initialData, GpuDeviceFlags deviceMask)
			: Texture(desc, initialData, deviceMask)
		{ }

		DiligentTexture::~DiligentTexture()
		{
			clearBufferViews();
		}

		PixelData DiligentTexture::lockImpl(GpuLockOptions options, UINT32 mipLevel, UINT32 face, UINT32 deviceIdx, UINT32 queueIdx)
		{
			UINT32 mipWidth = std::max(1u, mProperties.getWidth() >> mipLevel);
			UINT32 mipHeight = std::max(1u, mProperties.getHeight() >> mipLevel);
			UINT32 mipDepth = std::max(1u, mProperties.getDepth() >> mipLevel);

			mMappedBuffer = bs_new<PixelData>(mipWidth, mipHeight, mipDepth, mProperties.getFormat());
			mMappedBuffer->allocateInternalBuffer();

			PixelData output(mipWidth, mipHeight, mipDepth, mProperties.getFormat());
			output.setExternalBuffer(mMappedBuffer->getData());

			return output;
		}

		void DiligentTexture::unlockImpl()
		{
			bs_delete(mMappedBuffer);
			mMappedBuffer = nullptr;
		}
	}
}
