//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"
#include "RenderAPI/BsHardwareBuffer.h"
#include "Image/BsPixelUtil.h"
#include "RenderAPI/BsTextureView.h"
#include "Math/BsVector3I.h"
#include "Image/BsTextureResource.h"

namespace bs
{
	namespace ct
	{
	/** @addtogroup Resources-Internal
	 *  @{
	 */

	/**
	 * Core thread version of a bs::Texture.
	 *
	 * @note	Core thread.
	 */
	class BS_CORE_EXPORT Texture : public CoreObject
	{
	public:
		Texture(const TEXTURE_DESC& desc, const SPtr<PixelData>& initData, GpuDeviceFlags deviceMask);
		virtual ~Texture() {}


		/** @copydoc CoreObject::initialize */
		void initialize() override;

		/**
		 * Locks the buffer for reading or writing.
		 *
		 * @param[in]	options 	Options for controlling what you may do with the locked data.
		 * @param[in]	mipLevel	(optional) Mipmap level to lock.
		 * @param[in]	face		(optional) Texture face to lock.
		 * @param[in]	deviceIdx	Index of the device whose memory to map. If the buffer doesn't exist on this device,
		 *							the method returns null.
		 * @param[in]	queueIdx	Device queue to perform the read/write operations on. See @ref queuesDoc.
		 * 			
		 * @note	
		 * If you are just reading or writing one block of data use readData()/writeData() methods as they can be much faster
		 * in certain situations.
		 */
		PixelData lock(GpuLockOptions options, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
					   UINT32 queueIdx = 0);

		/**
		 * Unlocks a previously locked buffer. After the buffer is unlocked, any data returned by lock becomes invalid.
		 *
		 * @see	lock()
		 */
		void unlock();

		/**
		 * Copies the contents a subresource in this texture to another texture. Texture format and size of the subresource
		 * must match.
		 *
		 * You are allowed to copy from a multisampled to non-multisampled surface, which will resolve the multisampled
		 * surface before copying.
		 *
		 * @param[in]	target				Texture that contains the destination subresource.
		 * @param[in]	desc				Structure used for customizing the copy operation.
		 * @param[in]	commandBuffer		Command buffer to queue the copy operation on. If null, main command buffer is
		 *									used.
		 */
		void copy(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc = TEXTURE_COPY_DESC::DEFAULT,
			const SPtr<CommandBuffer>& commandBuffer = nullptr);

		/**
		 * Sets all the pixels of the specified face and mip level to the provided value.
		 *
		 * @param[in]	value			Color to clear the pixels to.
		 * @param[in]	mipLevel		Mip level to clear.
		 * @param[in]	face			Face (array index or cubemap face) to clear.
		 * @param[in]	queueIdx		Device queue to perform the write operation on. See @ref queuesDoc.
		 */
		void clear(const Color& value, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 queueIdx = 0);

		/**
		 * Reads data from the texture buffer into the provided buffer.
		 * 		
		 * @param[out]	dest		Previously allocated buffer to read data into.
		 * @param[in]	mipLevel	(optional) Mipmap level to read from.
		 * @param[in]	face		(optional) Texture face to read from.
		 * @param[in]	deviceIdx	Index of the device whose memory to read. If the buffer doesn't exist on this device,
		 *							no data will be read.
		 * @param[in]	queueIdx	Device queue to perform the read operation on. See @ref queuesDoc.
		 */
		void readData(PixelData& dest, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
							  UINT32 queueIdx = 0);

		/**
		 * Writes data from the provided buffer into the texture buffer.
		 * 		
		 * @param[in]	src					Buffer to retrieve the data from.
		 * @param[in]	mipLevel			(optional) Mipmap level to write into.
		 * @param[in]	face				(optional) Texture face to write into.
		 * @param[in]	discardWholeBuffer	(optional) If true any existing texture data will be discard. This can improve
		 *									performance of the write operation.
		 * @param[in]	queueIdx			Device queue to perform the write operation on. See @ref queuesDoc.
		 */
		void writeData(const PixelData& src, UINT32 mipLevel = 0, UINT32 face = 0, bool discardWholeBuffer = false,
							   UINT32 queueIdx = 0);

		/**	Returns properties that contain information about the texture. */
		const TextureProperties& getProperties() const { return mProperties; }

		/************************************************************************/
		/* 								STATICS		                     		*/
		/************************************************************************/

		/**
		 * @copydoc bs::Texture::create(const TEXTURE_DESC&)
		 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
		 */
		static SPtr<Texture> createPtr(const TEXTURE_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

		/**
		 * @copydoc bs::Texture::create(const SPtr<PixelData>&, int, bool)
		 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
		 */
		static SPtr<Texture> createPtr(const SPtr<PixelData>& pixelData, int usage = TU_DEFAULT,
			bool hwGammaCorrection = false, GpuDeviceFlags deviceMask = GDF_DEFAULT);

		/************************************************************************/
		/* 								TEXTURE VIEW                      		*/
		/************************************************************************/

		/**
		 * Requests a texture view for the specified mip and array ranges. Returns an existing view of one for the specified
		 * ranges already exists, otherwise creates a new one. You must release all views by calling releaseView() when done.
		 *
		 * @note	Core thread only.
		 */
		SPtr<TextureView> requestView(UINT32 mostDetailMip, UINT32 numMips, UINT32 firstArraySlice, UINT32 numArraySlices,
									  GpuViewUsage usage);

		/** Returns a plain white texture. */
		static SPtr<Texture> WHITE;

		/** Returns a plain black texture. */
		static SPtr<Texture> BLACK;

		/** Returns a plain normal map texture with normal pointing up (in Y direction). */
		static SPtr<Texture> NORMAL;
	protected:
		/** @copydoc lock */
		virtual PixelData lockImpl(GpuLockOptions options, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
			UINT32 queueIdx = 0) = 0;

		/** @copydoc unlock */
		virtual void unlockImpl() = 0;

		/** @copydoc copy */
		virtual void copyImpl(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc,
			const SPtr<CommandBuffer>& commandBuffer) = 0;

		/** @copydoc readData */
		virtual void readDataImpl(PixelData& dest, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
			UINT32 queueIdx = 0) = 0;

		/** @copydoc writeData */
		virtual void writeDataImpl(const PixelData& src, UINT32 mipLevel = 0, UINT32 face = 0,
			bool discardWholeBuffer = false, UINT32 queueIdx = 0) = 0;

		/** @copydoc clear */
		virtual void clearImpl(const Color& value, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 queueIdx = 0);

		/************************************************************************/
		/* 								TEXTURE VIEW                      		*/
		/************************************************************************/

		/**	Creates a view of a specific subresource in a texture. */
		virtual SPtr<TextureView> createView(const TEXTURE_VIEW_DESC& desc);

		/** Releases all internal texture view references. */
		void clearBufferViews();

		UnorderedMap<TEXTURE_VIEW_DESC, SPtr<TextureView>, TextureView::HashFunction, TextureView::EqualFunction> mTextureViews;
		TextureProperties mProperties;
		SPtr<PixelData> mInitData;
	};

	/** @} */
	}
}
