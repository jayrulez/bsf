//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Image/BsTexture.h"
#include "Private/RTTI/BsTextureResourceRTTI.h"
#include "FileSystem/BsDataStream.h"
#include "Error/BsException.h"
#include "Debug/BsDebug.h"
#include "CoreThread/BsCoreThread.h"
#include "Threading/BsAsyncOp.h"
#include "Resources/BsResources.h"
#include "Image/BsPixelUtil.h"

namespace bs
{
	namespace ct
	{
	SPtr<Texture> Texture::WHITE;
	SPtr<Texture> Texture::BLACK;
	SPtr<Texture> Texture::NORMAL;

	Texture::Texture(const TEXTURE_DESC& desc, const SPtr<PixelData>& initData, GpuDeviceFlags deviceMask)
		:mProperties(desc), mInitData(initData)
	{ }

	void Texture::initialize()
	{
		if (mInitData != nullptr)
		{
			writeData(*mInitData, 0, 0, true);
			mInitData->_unlock();
			mInitData = nullptr;
		}

		CoreObject::initialize();
	}

	void Texture::writeData(const PixelData& src, UINT32 mipLevel, UINT32 face, bool discardEntireBuffer,
		UINT32 queueIdx)
	{
		THROW_IF_NOT_CORE_THREAD;

		if(discardEntireBuffer)
		{
			if((mProperties.getUsage() & TU_DYNAMIC) == 0)
			{
				// Buffer discard is enabled but buffer was not created as dynamic. Disabling discard.
				discardEntireBuffer = false;
			}
		}

		writeDataImpl(src, mipLevel, face, discardEntireBuffer, queueIdx);
	}

	void Texture::readData(PixelData& dest, UINT32 mipLevel, UINT32 face, UINT32 deviceIdx, UINT32 queueIdx)
	{
		THROW_IF_NOT_CORE_THREAD;

		PixelData& pixelData = static_cast<PixelData&>(dest);

		UINT32 mipWidth, mipHeight, mipDepth;
		PixelUtil::getSizeForMipLevel(mProperties.getWidth(), mProperties.getHeight(), mProperties.getDepth(),
			mipLevel, mipWidth, mipHeight, mipDepth);

		if (pixelData.getWidth() != mipWidth || pixelData.getHeight() != mipHeight ||
			pixelData.getDepth() != mipDepth || pixelData.getFormat() != mProperties.getFormat())
		{
			BS_LOG(Error, Texture,
				"Provided buffer is not of valid dimensions or format in order to read from this texture.");
			return;
		}

		readDataImpl(pixelData, mipLevel, face, deviceIdx, queueIdx);
	}

	PixelData Texture::lock(GpuLockOptions options, UINT32 mipLevel, UINT32 face, UINT32 deviceIdx, UINT32 queueIdx)
	{
		THROW_IF_NOT_CORE_THREAD;

		if (mipLevel > mProperties.getNumMipmaps())
		{
			BS_LOG(Error, Texture, "Invalid mip level: {0}. Min is 0, max is {1}", mipLevel, mProperties.getNumMipmaps());
			return PixelData(0, 0, 0, PF_UNKNOWN);
		}

		if (face >= mProperties.getNumFaces())
		{
			BS_LOG(Error, Texture, "Invalid face index: {0}. Min is 0, max is {1}", face, mProperties.getNumFaces());
			return PixelData(0, 0, 0, PF_UNKNOWN);
		}

		return lockImpl(options, mipLevel, face, deviceIdx, queueIdx);
	}

	void Texture::unlock()
	{
		THROW_IF_NOT_CORE_THREAD;

		unlockImpl();
	}

	void Texture::copy(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc, const SPtr<CommandBuffer>& commandBuffer)
	{
		THROW_IF_NOT_CORE_THREAD;

		if (target->mProperties.getTextureType() != mProperties.getTextureType())
		{
			BS_LOG(Error, Texture, "Source and destination textures must be of same type.");
			return;
		}

		if (mProperties.getFormat() != target->mProperties.getFormat()) // Note: It might be okay to use different formats of the same size
		{
			BS_LOG(Error, Texture, "Source and destination texture formats must match.");
			return;
		}

		if (target->mProperties.getNumSamples() > 1 && mProperties.getNumSamples() != target->mProperties.getNumSamples())
		{
			BS_LOG(Error, Texture,
				"When copying to a multisampled texture, source texture must have the same number of samples.");
			return;
		}

		if (desc.srcFace >= mProperties.getNumFaces())
		{
			BS_LOG(Error, Texture, "Invalid source face index.");
			return;
		}

		if (desc.dstFace >= target->mProperties.getNumFaces())
		{
			BS_LOG(Error, Texture, "Invalid destination face index.");
			return;
		}

		if (desc.srcMip > mProperties.getNumMipmaps())
		{
			BS_LOG(Error, Texture, "Source mip level out of range. Valid range is [0, {0}].", mProperties.getNumMipmaps());
			return;
		}

		if (desc.dstMip > target->mProperties.getNumMipmaps())
		{
			BS_LOG(Error, Texture, "Destination mip level out of range. Valid range is [0, {0}].",
				target->mProperties.getNumMipmaps());
			return;
		}

		UINT32 srcWidth, srcHeight, srcDepth;
		PixelUtil::getSizeForMipLevel(
			mProperties.getWidth(),
			mProperties.getHeight(),
			mProperties.getDepth(),
			desc.srcMip,
			srcWidth,
			srcHeight,
			srcDepth);

		UINT32 dstWidth, dstHeight, dstDepth;
		PixelUtil::getSizeForMipLevel(
			target->mProperties.getWidth(),
			target->mProperties.getHeight(),
			target->mProperties.getDepth(),
			desc.dstMip,
			dstWidth,
			dstHeight,
			dstDepth);

		if(desc.dstPosition.x < 0 || desc.dstPosition.x >= (INT32)dstWidth ||
			desc.dstPosition.y < 0 || desc.dstPosition.y >= (INT32)dstHeight ||
			desc.dstPosition.z < 0 || desc.dstPosition.z >= (INT32)dstDepth)
		{
			BS_LOG(Error, Texture, "Destination position falls outside the destination texture.");
			return;
		}

		bool entireSurface = desc.srcVolume.getWidth() == 0 ||
			desc.srcVolume.getHeight() == 0 ||
			desc.srcVolume.getDepth() == 0;

		UINT32 dstRight = (UINT32)desc.dstPosition.x;
		UINT32 dstBottom = (UINT32)desc.dstPosition.y;
		UINT32 dstBack = (UINT32)desc.dstPosition.z;
		if(!entireSurface)
		{
			if(desc.srcVolume.left >= srcWidth || desc.srcVolume.right > srcWidth ||
				desc.srcVolume.top >= srcHeight || desc.srcVolume.bottom > srcHeight ||
				desc.srcVolume.front >= srcDepth || desc.srcVolume.back > srcDepth)
			{
				BS_LOG(Error, Texture, "Source volume falls outside the source texture.");
				return;
			}

			dstRight += desc.srcVolume.getWidth();
			dstBottom += desc.srcVolume.getHeight();
			dstBack += desc.srcVolume.getDepth();
		}
		else
		{
			dstRight += srcWidth;
			dstBottom += srcHeight;
			dstBack += srcDepth;
		}

		if(dstRight > dstWidth || dstBottom > dstHeight || dstBack > dstDepth)
		{
			BS_LOG(Error, Texture, "Destination volume falls outside the destination texture.");
			return;
		}

		copyImpl(target, desc, commandBuffer);
	}

	void Texture::clear(const Color& value, UINT32 mipLevel, UINT32 face, UINT32 queueIdx)
	{
		THROW_IF_NOT_CORE_THREAD;

		if (face >= mProperties.getNumFaces())
		{
			BS_LOG(Error, Texture, "Invalid face index.");
			return;
		}

		if (mipLevel > mProperties.getNumMipmaps())
		{
			BS_LOG(Error, Texture, "Mip level out of range. Valid range is [0, {0}].", mProperties.getNumMipmaps());
			return;
		}

		clearImpl(value, mipLevel, face, queueIdx);
	}

	void Texture::clearImpl(const Color& value, UINT32 mipLevel, UINT32 face, UINT32 queueIdx)
	{
		SPtr<PixelData> data = mProperties.allocBuffer(face, mipLevel);
		data->setColors(value);
		
		writeData(*data, mipLevel, face, true, queueIdx);
	}

	/************************************************************************/
	/* 								TEXTURE VIEW                      		*/
	/************************************************************************/

	SPtr<TextureView> Texture::createView(const TEXTURE_VIEW_DESC& desc)
	{
		return bs_shared_ptr<TextureView>(new (bs_alloc<TextureView>()) TextureView(desc));
	}

	void Texture::clearBufferViews()
	{
		mTextureViews.clear();
	}

	SPtr<TextureView> Texture::requestView(UINT32 mostDetailMip, UINT32 numMips, UINT32 firstArraySlice,
										   UINT32 numArraySlices, GpuViewUsage usage)
	{
		THROW_IF_NOT_CORE_THREAD;

		const TextureProperties& texProps = getProperties();

		TEXTURE_VIEW_DESC key;
		key.mostDetailMip = mostDetailMip;
		key.numMips = numMips == 0 ? (texProps.getNumMipmaps() + 1) : numMips;
		key.firstArraySlice = firstArraySlice;
		key.numArraySlices = numArraySlices == 0 ? texProps.getNumFaces() : numArraySlices;
		key.usage = usage;

		auto iterFind = mTextureViews.find(key);
		if (iterFind == mTextureViews.end())
		{
			mTextureViews[key] = createView(key);

			iterFind = mTextureViews.find(key);
		}

		return iterFind->second;
	}

	/************************************************************************/
	/* 								STATICS	                      			*/
	/************************************************************************/
	SPtr<Texture> Texture::createPtr(const TEXTURE_DESC& desc, GpuDeviceFlags deviceMask)
	{
		return TextureManager::instance().createTexture(desc, deviceMask);
	}

	SPtr<Texture> Texture::createPtr(const SPtr<PixelData>& pixelData, int usage, bool hwGammaCorrection,
		GpuDeviceFlags deviceMask)
	{
		TEXTURE_DESC desc;
		desc.type = pixelData->getDepth() > 1 ? TEX_TYPE_3D : TEX_TYPE_2D;
		desc.width = pixelData->getWidth();
		desc.height = pixelData->getHeight();
		desc.depth = pixelData->getDepth();
		desc.format = pixelData->getFormat();
		desc.usage = usage;
		desc.hwGamma = hwGammaCorrection;

		SPtr<Texture> newTex = TextureManager::instance().createTextureInternal(desc, pixelData, deviceMask);
		newTex->initialize();

		return newTex;
	}
	}
}
