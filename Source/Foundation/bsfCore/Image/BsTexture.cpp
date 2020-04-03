//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Image/BsTexture.h"
#include "Private/RTTI/BsTextureRTTI.h"
#include "FileSystem/BsDataStream.h"
#include "Error/BsException.h"
#include "Debug/BsDebug.h"
#include "Threading/BsAsyncOp.h"
#include "Resources/BsResources.h"
#include "Image/BsPixelUtil.h"

namespace bs
{
	TEXTURE_COPY_DESC TEXTURE_COPY_DESC::DEFAULT = TEXTURE_COPY_DESC();

	TextureProperties::TextureProperties(const TEXTURE_DESC& desc)
		:mDesc(desc)
	{

	}

	bool TextureProperties::hasAlpha() const
	{
		return PixelUtil::hasAlpha(mDesc.format);
	}

	UINT32 TextureProperties::getNumFaces() const
	{
		UINT32 facesPerSlice = getTextureType() == TEX_TYPE_CUBE_MAP ? 6 : 1;

		return facesPerSlice * mDesc.numArraySlices;
	}

	void TextureProperties::mapFromSubresourceIdx(UINT32 subresourceIdx, UINT32& face, UINT32& mip) const
	{
		UINT32 numMipmaps = getNumMipmaps() + 1;

		face = Math::floorToInt((subresourceIdx) / (float)numMipmaps);
		mip = subresourceIdx % numMipmaps;
	}

	UINT32 TextureProperties::mapToSubresourceIdx(UINT32 face, UINT32 mip) const
	{
		return face * (getNumMipmaps() + 1) + mip;
	}

	SPtr<PixelData> TextureProperties::allocBuffer(UINT32 face, UINT32 mipLevel) const
	{
		UINT32 width = getWidth();
		UINT32 height = getHeight();
		UINT32 depth = getDepth();

		for (UINT32 j = 0; j < mipLevel; j++)
		{
			if (width != 1) width /= 2;
			if (height != 1) height /= 2;
			if (depth != 1) depth /= 2;
		}

		SPtr<PixelData> dst = bs_shared_ptr_new<PixelData>(width, height, depth, getFormat());
		dst->allocateInternalBuffer();

		return dst;
	}

	SPtr<Texture> Texture::WHITE;
	SPtr<Texture> Texture::BLACK;
	SPtr<Texture> Texture::NORMAL;

	Texture::Texture(const TEXTURE_DESC& desc)
		:mProperties(desc)
	{

	}

	Texture::Texture(const TEXTURE_DESC& desc, const SPtr<PixelData>& pixelData, GpuDeviceFlags deviceMask)
		: mProperties(desc), mInitData(pixelData)
	{
		if (mInitData != nullptr)
			mInitData->_lock();
	}

	HTexture Texture::createHandle(const TEXTURE_DESC& desc)
	{
		SPtr<Texture> texturePtr = _createPtr(desc);

		return static_resource_cast<Texture>(gResources()._createResourceHandle(texturePtr));
	}

	HTexture Texture::createHandle(const SPtr<PixelData>& pixelData, int usage, bool hwGammaCorrection)
	{
		SPtr<Texture> texturePtr = _createPtr(pixelData, usage, hwGammaCorrection);

		return static_resource_cast<Texture>(gResources()._createResourceHandle(texturePtr));
	}

	SPtr<Texture> Texture::create(const TEXTURE_DESC& desc, GpuDeviceFlags deviceMask)
	{
		return TextureManager::instance().createTexture(desc, deviceMask);
	}

	SPtr<Texture> Texture::create(const SPtr<PixelData>& pixelData, int usage, bool hwGammaCorrection, GpuDeviceFlags deviceMask)
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

	SPtr<Texture> Texture::_createPtr(const TEXTURE_DESC& desc)
	{
		return Texture::create(desc);
	}

	SPtr<Texture> Texture::_createPtr(const SPtr<PixelData>& pixelData, int usage, bool hwGammaCorrection)
	{
		return Texture::create(pixelData, usage, hwGammaCorrection);
	}

	void Texture::initialize()
	{
		mSize = calculateSize();

		// Allocate CPU buffers if needed
		if ((mProperties.getUsage() & TU_CPUCACHED) != 0)
		{
			createCPUBuffers();

			if (mInitData != nullptr)
				updateCPUBuffers(0, *mInitData);
		}

		if (mInitData != nullptr)
		{
			writeData(*mInitData, 0, 0, true);
			mInitData->_unlock();

			if ((mProperties.getUsage() & TU_CPUCACHED) == 0)
				mInitData = nullptr;
		}

		Resource::initialize();
		CoreObject::initialize();
	}

	SPtr<ct::CoreObject> Texture::createCore() const
	{
		return nullptr;
	}

	AsyncOp Texture::writeData(const SPtr<PixelData>& data, UINT32 face, UINT32 mipLevel, bool discardEntireBuffer)
	{
		AsyncOp op;

		UINT32 subresourceIdx = mProperties.mapToSubresourceIdx(face, mipLevel);
		updateCPUBuffers(subresourceIdx, *data);

		data->_lock();

		writeData(*data, mipLevel, face, discardEntireBuffer);
		data->_unlock();
		op._completeOperation();
		return op;
	}

	AsyncOp Texture::readData(const SPtr<PixelData>& data, UINT32 face, UINT32 mipLevel)
	{
		AsyncOp op;

		data->_lock();
		// Make sure any queued command start executing before reading
		ct::RenderAPI::instance().submitCommandBuffer(nullptr);

		readData(*data, mipLevel, face);
		data->_unlock();
		op._completeOperation();

		return op;
	}

	TAsyncOp<SPtr<PixelData>> Texture::readData(UINT32 face, UINT32 mipLevel)
	{
		TAsyncOp<SPtr<PixelData>> op;

		auto func = [texture = this, face, mipLevel, op]() mutable
		{
			// Make sure any queued command start executing before reading
			ct::RenderAPI::instance().submitCommandBuffer(nullptr);

			SPtr<PixelData> output = texture->getProperties().allocBuffer(face, mipLevel);
			texture->readData(*output, mipLevel, face);

			op._completeOperation(output);

		};

		func();

		//gCoreThread().queueCommand(func);
		return op;
	}

	void Texture::readCachedData(PixelData& dest, UINT32 face, UINT32 mipLevel)
	{
		if ((mProperties.getUsage() & TU_CPUCACHED) == 0)
		{
			BS_LOG(Error, Texture, "Attempting to read CPU data from a texture that is created without CPU caching.");
			return;
		}

		UINT32 mipWidth, mipHeight, mipDepth;
		PixelUtil::getSizeForMipLevel(mProperties.getWidth(), mProperties.getHeight(), mProperties.getDepth(),
			mipLevel, mipWidth, mipHeight, mipDepth);

		if (dest.getWidth() != mipWidth || dest.getHeight() != mipHeight ||
			dest.getDepth() != mipDepth || dest.getFormat() != mProperties.getFormat())
		{
			BS_LOG(Error, Texture, "Provided buffer is not of valid dimensions or format in order to read from this texture.");
			return;
		}

		UINT32 subresourceIdx = mProperties.mapToSubresourceIdx(face, mipLevel);
		if (subresourceIdx >= (UINT32)mCPUSubresourceData.size())
		{
			BS_LOG(Error, Texture, "Invalid subresource index: {0}. Supported range: 0 .. {1}",
				subresourceIdx, (UINT32)mCPUSubresourceData.size());
			return;
		}

		if (mCPUSubresourceData[subresourceIdx]->getSize() != dest.getSize())
			BS_EXCEPT(InternalErrorException, "Buffer sizes don't match.");

		UINT8* srcPtr = mCPUSubresourceData[subresourceIdx]->getData();
		UINT8* destPtr = dest.getData();

		memcpy(destPtr, srcPtr, dest.getSize());
	}

	UINT32 Texture::calculateSize() const
	{
		return mProperties.getNumFaces() * PixelUtil::getMemorySize(mProperties.getWidth(),
			mProperties.getHeight(), mProperties.getDepth(), mProperties.getFormat());
	}

	void Texture::createCPUBuffers()
	{
		UINT32 numFaces = mProperties.getNumFaces();
		UINT32 numMips = mProperties.getNumMipmaps() + 1;

		UINT32 numSubresources = numFaces * numMips;
		mCPUSubresourceData.resize(numSubresources);

		for (UINT32 i = 0; i < numFaces; i++)
		{
			UINT32 curWidth = mProperties.getWidth();
			UINT32 curHeight = mProperties.getHeight();
			UINT32 curDepth = mProperties.getDepth();

			for (UINT32 j = 0; j < numMips; j++)
			{
				UINT32 subresourceIdx = mProperties.mapToSubresourceIdx(i, j);

				mCPUSubresourceData[subresourceIdx] = bs_shared_ptr_new<PixelData>(curWidth, curHeight, curDepth, mProperties.getFormat());
				mCPUSubresourceData[subresourceIdx]->allocateInternalBuffer();

				if (curWidth > 1)
					curWidth = curWidth / 2;

				if (curHeight > 1)
					curHeight = curHeight / 2;

				if (curDepth > 1)
					curDepth = curDepth / 2;
			}
		}
	}

	void Texture::updateCPUBuffers(UINT32 subresourceIdx, const PixelData& pixelData)
	{
		if ((mProperties.getUsage() & TU_CPUCACHED) == 0)
			return;

		if (subresourceIdx >= (UINT32)mCPUSubresourceData.size())
		{
			BS_LOG(Error, Texture, "Invalid subresource index: {0}. Supported range: 0 .. {1}",
				subresourceIdx, (UINT32)mCPUSubresourceData.size());
			return;
		}

		UINT32 mipLevel;
		UINT32 face;
		mProperties.mapFromSubresourceIdx(subresourceIdx, face, mipLevel);

		UINT32 mipWidth, mipHeight, mipDepth;
		PixelUtil::getSizeForMipLevel(mProperties.getWidth(), mProperties.getHeight(), mProperties.getDepth(),
			mipLevel, mipWidth, mipHeight, mipDepth);

		if (pixelData.getWidth() != mipWidth || pixelData.getHeight() != mipHeight ||
			pixelData.getDepth() != mipDepth || pixelData.getFormat() != mProperties.getFormat())
		{
			BS_LOG(Error, Texture, "Provided buffer is not of valid dimensions or format in order to update this texture.");
			return;
		}

		if (mCPUSubresourceData[subresourceIdx]->getSize() != pixelData.getSize())
			BS_EXCEPT(InternalErrorException, "Buffer sizes don't match.");

		UINT8* dest = mCPUSubresourceData[subresourceIdx]->getData();
		UINT8* src = pixelData.getData();

		memcpy(dest, src, pixelData.getSize());
	}

	PixelData Texture::lock(GpuLockOptions options, UINT32 mipLevel, UINT32 face, UINT32 deviceIdx, UINT32 queueIdx)
	{
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
		unlockImpl();
	}

	void Texture::copy(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc, const SPtr<ct::CommandBuffer>& commandBuffer)
	{
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

		if (desc.dstPosition.x < 0 || desc.dstPosition.x >= (INT32)dstWidth ||
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
		if (!entireSurface)
		{
			if (desc.srcVolume.left >= srcWidth || desc.srcVolume.right > srcWidth ||
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

		if (dstRight > dstWidth || dstBottom > dstHeight || dstBack > dstDepth)
		{
			BS_LOG(Error, Texture, "Destination volume falls outside the destination texture.");
			return;
		}

		copyImpl(target, desc, commandBuffer);
	}

	void Texture::clear(const Color& value, UINT32 mipLevel, UINT32 face, UINT32 queueIdx)
	{
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

	void Texture::readData(PixelData& dest, UINT32 mipLevel, UINT32 face, UINT32 deviceIdx, UINT32 queueIdx)
	{
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

	void Texture::writeData(const PixelData& src, UINT32 mipLevel, UINT32 face, bool discardWholeBuffer, UINT32 queueIdx)
	{
		if (discardWholeBuffer)
		{
			if ((mProperties.getUsage() & TU_DYNAMIC) == 0)
			{
				// Buffer discard is enabled but buffer was not created as dynamic. Disabling discard.
				discardWholeBuffer = false;
			}
		}

		writeDataImpl(src, mipLevel, face, discardWholeBuffer, queueIdx);
	}

	SPtr<ct::TextureView> Texture::requestView(UINT32 mostDetailMip, UINT32 numMips, UINT32 firstArraySlice, UINT32 numArraySlices, GpuViewUsage usage)
	{
		const TextureProperties& texProps = getProperties();

		ct::TEXTURE_VIEW_DESC key;
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

	SPtr<ct::TextureView> Texture::createView(const ct::TEXTURE_VIEW_DESC& desc)
	{
		return bs_shared_ptr<ct::TextureView>(new (bs_alloc<ct::TextureView>()) ct::TextureView(desc));
	}

	void Texture::clearBufferViews()
	{
		mTextureViews.clear();
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* Texture::getRTTIStatic()
	{
		return TextureRTTI::instance();
	}

	RTTITypeBase* Texture::getRTTI() const
	{
		return Texture::getRTTIStatic();
	}

	HTexture Texture::getHandle() const
	{
		SPtr<Texture> texturePtr = std::static_pointer_cast<Texture>(getThisPtr());// fix this, just for compilation
		return static_resource_cast<Texture>(gResources()._createResourceHandle(texturePtr));
	}
}
