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

	TextureResource::TextureResource(const TEXTURE_DESC& desc)
		:mProperties(desc)
	{
		
	}

	TextureResource::TextureResource(const TEXTURE_DESC& desc, const SPtr<PixelData>& pixelData)
		: mProperties(desc), mInitData(pixelData)
	{
		if (mInitData != nullptr)
			mInitData->_lock();
	}

	void TextureResource::initialize()
	{
		mSize = calculateSize();

		// Allocate CPU buffers if needed
		if ((mProperties.getUsage() & TU_CPUCACHED) != 0)
		{
			createCPUBuffers();

			if (mInitData != nullptr)
				updateCPUBuffers(0, *mInitData);
		}

		Resource::initialize();
	}

	SPtr<ct::CoreObject> TextureResource::createCore() const
	{
		const TextureProperties& props = getProperties();

		SPtr<ct::CoreObject> coreObj = ct::TextureManager::instance().createTextureInternal(props.mDesc, mInitData);

		if ((mProperties.getUsage() & TU_CPUCACHED) == 0)
			mInitData = nullptr;

		return coreObj;
	}

	AsyncOp TextureResource::writeData(const SPtr<PixelData>& data, UINT32 face, UINT32 mipLevel, bool discardEntireBuffer)
	{
		UINT32 subresourceIdx = mProperties.mapToSubresourceIdx(face, mipLevel);
		updateCPUBuffers(subresourceIdx, *data);

		data->_lock();

		std::function<void(const SPtr<ct::Texture>&, UINT32, UINT32, const SPtr<PixelData>&, bool, AsyncOp&)> func =
			[&](const SPtr<ct::Texture>& texture, UINT32 _face, UINT32 _mipLevel, const SPtr<PixelData>& _pixData,
				bool _discardEntireBuffer, AsyncOp& asyncOp)
		{
			texture->writeData(*_pixData, _mipLevel, _face, _discardEntireBuffer);
			_pixData->_unlock();
			asyncOp._completeOperation();

		};

		return gCoreThread().queueReturnCommand(std::bind(func, getCore(), face, mipLevel,
			data, discardEntireBuffer, std::placeholders::_1));
	}

	AsyncOp TextureResource::readData(const SPtr<PixelData>& data, UINT32 face, UINT32 mipLevel)
	{
		data->_lock();

		std::function<void(const SPtr<ct::Texture>&, UINT32, UINT32, const SPtr<PixelData>&, AsyncOp&)> func =
			[&](const SPtr<ct::Texture>& texture, UINT32 _face, UINT32 _mipLevel, const SPtr<PixelData>& _pixData,
				AsyncOp& asyncOp)
		{
			// Make sure any queued command start executing before reading
			ct::RenderAPI::instance().submitCommandBuffer(nullptr);

			texture->readData(*_pixData, _mipLevel, _face);
			_pixData->_unlock();
			asyncOp._completeOperation();

		};

		return gCoreThread().queueReturnCommand(std::bind(func, getCore(), face, mipLevel,
			data, std::placeholders::_1));
	}

	TAsyncOp<SPtr<PixelData>> TextureResource::readData(UINT32 face, UINT32 mipLevel)
	{
		TAsyncOp<SPtr<PixelData>> op;

		auto func = [texture = getCore(), face, mipLevel, op]() mutable
		{
			// Make sure any queued command start executing before reading
			ct::RenderAPI::instance().submitCommandBuffer(nullptr);

			SPtr<PixelData> output = texture->getProperties().allocBuffer(face, mipLevel);
			texture->readData(*output, mipLevel, face);

			op._completeOperation(output);

		};

		gCoreThread().queueCommand(func);
		return op;
	}

	UINT32 TextureResource::calculateSize() const
	{
		return mProperties.getNumFaces() * PixelUtil::getMemorySize(mProperties.getWidth(),
			mProperties.getHeight(), mProperties.getDepth(), mProperties.getFormat());
	}

	void TextureResource::updateCPUBuffers(UINT32 subresourceIdx, const PixelData& pixelData)
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

	void TextureResource::readCachedData(PixelData& dest, UINT32 face, UINT32 mipLevel)
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

	void TextureResource::createCPUBuffers()
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

	SPtr<ct::Texture> TextureResource::getCore() const
	{
		return std::static_pointer_cast<ct::Texture>(mCoreSpecific);
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* TextureResource::getRTTIStatic()
	{
		return TextureResourceRTTI::instance();
	}

	RTTITypeBase* TextureResource::getRTTI() const
	{
		return TextureResource::getRTTIStatic();
	}

	/************************************************************************/
	/* 								STATICS	                      			*/
	/************************************************************************/
	TextureResourceHandle TextureResource::createHandle(const TEXTURE_DESC& desc)
	{
		SPtr<TextureResource> texturePtr = _createPtr(desc);

		return static_resource_cast<TextureResource>(gResources()._createResourceHandle(texturePtr));
	}
	
	TextureResourceHandle TextureResource::createHandle(const SPtr<PixelData>& pixelData, int usage, bool hwGammaCorrection)
	{
		SPtr<TextureResource> texturePtr = _createPtr(pixelData, usage, hwGammaCorrection);

		return static_resource_cast<TextureResource>(gResources()._createResourceHandle(texturePtr));
	}

	SPtr<TextureResource> TextureResource::_createPtr(const TEXTURE_DESC& desc)
	{
		return TextureManager::instance().createTexture(desc);
	}

	SPtr<TextureResource> TextureResource::_createPtr(const SPtr<PixelData>& pixelData, int usage, bool hwGammaCorrection)
	{
		TEXTURE_DESC desc;
		desc.type = pixelData->getDepth() > 1 ? TEX_TYPE_3D : TEX_TYPE_2D;
		desc.width = pixelData->getWidth();
		desc.height = pixelData->getHeight();
		desc.depth = pixelData->getDepth();
		desc.format = pixelData->getFormat();
		desc.usage = usage;
		desc.hwGamma = hwGammaCorrection;

		return TextureManager::instance().createTexture(desc, pixelData);
	}
}
