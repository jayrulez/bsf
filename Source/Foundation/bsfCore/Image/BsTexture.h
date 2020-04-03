//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"
#include "RenderAPI/BsHardwareBuffer.h"
#include "Image/BsPixelUtil.h"
#include "RenderAPI/BsTextureView.h"
#include "Math/BsVector3I.h"

namespace bs
{
	/** @addtogroup Resources
	 *  @{
	 */

	/**	Flags that describe how is a texture used. */
	enum BS_SCRIPT_EXPORT(m:Rendering) TextureUsage
	{
		/** A regular texture that is not often or ever updated from the CPU. */
		TU_STATIC			BS_SCRIPT_EXPORT(n:Default)			= GBU_STATIC,
		/** A regular texture that is often updated by the CPU. */
		TU_DYNAMIC			BS_SCRIPT_EXPORT(n:Dynamic)			= GBU_DYNAMIC,
		/** Texture that can be rendered to by the GPU. */
		TU_RENDERTARGET		BS_SCRIPT_EXPORT(n:Render)			= 0x200,
		/** Texture used as a depth/stencil buffer by the GPU. */
		TU_DEPTHSTENCIL		BS_SCRIPT_EXPORT(n:DepthStencil)	= 0x400,
		/** Texture that allows load/store operations from the GPU program. */
		TU_LOADSTORE		BS_SCRIPT_EXPORT(n:LoadStore)		= 0x800,
		/** All mesh data will also be cached in CPU memory, making it available for fast read access from the CPU. */
		TU_CPUCACHED		BS_SCRIPT_EXPORT(n:CPUCached)		= 0x1000,
		/** Allows the CPU to directly read the texture data buffers from the GPU. */
		TU_CPUREADABLE		BS_SCRIPT_EXPORT(n:CPUReadable)		= 0x2000,
		/** Allows you to retrieve views of the texture using a format different from one specified on creation. */
		TU_MUTABLEFORMAT	BS_SCRIPT_EXPORT(n:MutableFormat)	= 0x4000,
		/** Default (most common) texture usage. */
		TU_DEFAULT			BS_SCRIPT_EXPORT(ex:true)			= TU_STATIC
	};

	/**	Texture mipmap options. */
	enum TextureMipmap
	{
		MIP_UNLIMITED = 0x7FFFFFFF /**< Create all mip maps down to 1x1. */
	};

	/** Descriptor structure used for initialization of a Texture. */
	struct TEXTURE_DESC
	{
		/** Type of the texture. */
		TextureType type = TEX_TYPE_2D;

		/** Format of pixels in the texture. */
		PixelFormat format = PF_RGBA8;

		/** Width of the texture in pixels. */
		UINT32 width = 1;

		/** Height of the texture in pixels. */
		UINT32 height = 1;

		/** Depth of the texture in pixels (Must be 1 for 2D textures). */
		UINT32 depth = 1;

		/** Number of mip-maps the texture has. This number excludes the full resolution map. */
		UINT32 numMips = 0;

		/** Describes how the caller plans on using the texture in the pipeline. */
		INT32 usage = TU_DEFAULT;

		/**
		 * If true the texture data is assumed to have been gamma corrected and will be converted back to linear space when
		 * sampled on GPU.
		 */
		bool hwGamma = false;

		/** Number of samples per pixel. Set to 1 or 0 to use the default of a single sample per pixel. */
		UINT32 numSamples = 0;

		/** Number of texture slices to create if creating a texture array. Ignored for 3D textures. */
		UINT32 numArraySlices = 1;
	};

	/** Structure used for specifying information about a texture copy operation. */
	struct TEXTURE_COPY_DESC
	{
		/**
		 * Face from which to copy. This can be an entry in an array of textures, or a single face of a cube map. If cubemap
		 * array, then each array entry takes up six faces.
		 */
		UINT32 srcFace = 0;

		/** Mip level from which to copy. */
		UINT32 srcMip = 0;

		/** Pixel volume from which to copy from. This defaults to all pixels of the face. */
		PixelVolume srcVolume = PixelVolume(0, 0, 0, 0, 0, 0);

		/**
		 * Face to which to copy. This can be an entry in an array of textures, or a single face of a cube map. If cubemap
		 * array, then each array entry takes up six faces.
		 */
		UINT32 dstFace = 0;

		/** Mip level to which to copy. */
		UINT32 dstMip = 0;

		/**
		 * Coordinates to write the source pixels to. The destination texture must have enough pixels to fit the entire
		 * source volume.
		 */
		Vector3I dstPosition;

		BS_CORE_EXPORT static TEXTURE_COPY_DESC DEFAULT;
	};

	/** Properties of a Texture. Shared between sim and core thread versions of a Texture. */
	class BS_CORE_EXPORT TextureProperties
	{
	public:
		TextureProperties() = default;
		TextureProperties(const TEXTURE_DESC& desc);

		/**	Gets the type of texture. */
		TextureType getTextureType() const { return mDesc.type; }

		/**
		 * Gets the number of mipmaps to be used for this texture. This number excludes the top level map (which is always
		 * assumed to be present).
		 */
		UINT32 getNumMipmaps() const { return mDesc.numMips; }

		/**
		 * Determines does the texture contain gamma corrected data. If true then the GPU will automatically convert the
		 * pixels to linear space before reading from the texture, and convert them to gamma space when writing to the
		 * texture.
		 */
		bool isHardwareGammaEnabled() const { return mDesc.hwGamma; }

		/**	Gets the number of samples used for multisampling (0 or 1 if multisampling is not used). */
		UINT32 getNumSamples() const { return mDesc.numSamples; }

		/**	Returns the height of the texture.  */
		UINT32 getHeight() const { return mDesc.height; }

		/**	Returns the width of the texture. */
		UINT32 getWidth() const { return mDesc.width; }

		/**	Returns the depth of the texture (only applicable for 3D textures). */
		UINT32 getDepth() const { return mDesc.depth; }

		/**	Returns a value that signals the engine in what way is the texture expected to be used. */
		int getUsage() const { return mDesc.usage; }

		/**	Returns the pixel format for the texture surface. */
		PixelFormat getFormat() const { return mDesc.format; }

		/**	Returns true if the texture has an alpha layer. */
		bool hasAlpha() const;

		/**
		 * Returns the number of faces this texture has. This includes array slices (if texture is an array texture),
		 * as well as cube-map faces.
		 */
		UINT32 getNumFaces() const;

		/** Returns the number of array slices of the texture (if the texture is an array texture). */
		UINT32 getNumArraySlices() const { return mDesc.numArraySlices; }

		/**
		 * Allocates a buffer that exactly matches the format of the texture described by these properties, for the provided
		 * face and mip level. This is a helper function, primarily meant for creating buffers when reading from, or writing
		 * to a texture.
		 * 			
		 * @note	Thread safe.
		 */
		SPtr<PixelData> allocBuffer(UINT32 face, UINT32 mipLevel) const;

	protected:
		friend class TextureRTTI;
		friend class Texture;

		/**
		 * Maps a sub-resource index to an exact face and mip level. Sub-resource indexes are used when reading or writing
		 * to the resource.
		 */
		void mapFromSubresourceIdx(UINT32 subresourceIdx, UINT32& face, UINT32& mip) const;

		/**
		 * Map a face and a mip level to a sub-resource index you can use for updating or reading a specific sub-resource.
		 */
		UINT32 mapToSubresourceIdx(UINT32 face, UINT32 mip) const;

		TEXTURE_DESC mDesc;
	};

	/**
	 * Abstract class representing a texture. Specific render systems have their own Texture implementations. Internally
	 * represented as one or more surfaces with pixels in a certain number of dimensions, backed by a hardware buffer.
	 *
	 * @note	Sim thread.
	 */

	/** @} */


	class BS_CORE_EXPORT BS_SCRIPT_EXPORT(m :Rendering) Texture : public Resource
	{
	public:
		virtual ~Texture() = default;

		AsyncOp writeData(const SPtr<PixelData>& data, UINT32 face = 0, UINT32 mipLevel = 0, bool discardEntireBuffer = false);

		AsyncOp readData(const SPtr<PixelData>& data, UINT32 face = 0, UINT32 mipLevel = 0);

		BS_SCRIPT_EXPORT(n:GetGPUPixels)
		TAsyncOp<SPtr<PixelData>> readData(UINT32 face = 0, UINT32 mipLevel = 0);

		void readCachedData(PixelData& data, UINT32 face = 0, UINT32 mipLevel = 0);

		static HTexture createHandle(const TEXTURE_DESC& desc);

		static HTexture createHandle(const SPtr<PixelData>& pixelData, int usage = TU_DEFAULT, bool hwGammaCorrection = false);

		static SPtr<Texture> create(const TEXTURE_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);
		static SPtr<Texture> create(const SPtr<PixelData>& pixelData, int usage = TU_DEFAULT, bool hwGammaCorrection = false, GpuDeviceFlags deviceMask = GDF_DEFAULT);


		static SPtr<Texture> _createPtr(const TEXTURE_DESC& desc);

		static SPtr<Texture> _createPtr(const SPtr<PixelData>& pixelData, int usage = TU_DEFAULT, bool hwGammaCorrection = false);

		/******/

		void initialize() override;

		PixelData lock(GpuLockOptions options, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0, UINT32 queueIdx = 0);

		void unlock();

		void copy(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc = TEXTURE_COPY_DESC::DEFAULT, const SPtr<ct::CommandBuffer>& commandBuffer = nullptr);

		void clear(const Color& value, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 queueIdx = 0);

		void readData(PixelData& dest, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0,
			UINT32 queueIdx = 0);

		void writeData(const PixelData& src, UINT32 mipLevel = 0, UINT32 face = 0, bool discardWholeBuffer = false, UINT32 queueIdx = 0);

		const TextureProperties& getProperties() const
		{
			return mProperties;
		}

		/************************************************************************/
		/* 								TEXTURE VIEW                      		*/
		/************************************************************************/
		SPtr<ct::TextureView> requestView(UINT32 mostDetailMip, UINT32 numMips, UINT32 firstArraySlice, UINT32 numArraySlices,
			GpuViewUsage usage);

		static SPtr<Texture> WHITE;
		static SPtr<Texture> BLACK;
		static SPtr<Texture> NORMAL;
	protected:
		friend class TextureManager;

		/*****/
		Texture(const TEXTURE_DESC& desc);
		Texture(const TEXTURE_DESC& desc, const SPtr<PixelData>& pixelData, GpuDeviceFlags deviceMask = GDF_DEFAULT);

		SPtr<ct::CoreObject> createCore() const override;

		UINT32 calculateSize() const;

		void createCPUBuffers();

		void updateCPUBuffers(UINT32 subresourceIdx, const PixelData& data);
		/*****/

		virtual PixelData lockImpl(GpuLockOptions options, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0, UINT32 queueIdx = 0)
		{
			return PixelData();
		}

		virtual void unlockImpl()
		{
		}

		virtual void copyImpl(const SPtr<Texture>& target, const TEXTURE_COPY_DESC& desc, const SPtr<ct::CommandBuffer>& commandBuffer)
		{
		}

		virtual void readDataImpl(PixelData& dest, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 deviceIdx = 0, UINT32 queueIdx = 0)
		{
		}

		virtual void writeDataImpl(const PixelData& src, UINT32 mipLevel = 0, UINT32 face = 0, bool discardWholeBuffer = false, UINT32 queueIdx = 0)
		{
		}

		virtual void clearImpl(const Color& value, UINT32 mipLevel = 0, UINT32 face = 0, UINT32 queueIdx = 0);

		/************************************************************************/
		/* 								TEXTURE VIEW                      		*/
		/************************************************************************/

		virtual SPtr<ct::TextureView> createView(const ct::TEXTURE_VIEW_DESC& desc);

		void clearBufferViews();

		UnorderedMap<ct::TEXTURE_VIEW_DESC, SPtr<ct::TextureView>, ct::TextureView::HashFunction, ct::TextureView::EqualFunction> mTextureViews;
		TextureProperties mProperties;
		SPtr<PixelData> mInitData;
		//mutable SPtr<PixelData> mInitData;
		Vector<SPtr<PixelData>> mCPUSubresourceData;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	public:
		Texture() = default; // Serialization only

		friend class TextureRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};
}
