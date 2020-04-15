//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Managers/BsTextureManager.h"
#include "Error/BsException.h"
#include "Image/BsPixelUtil.h"
#include "RenderAPI/BsRenderAPI.h"

namespace bs
{
	void TextureManager::onStartUp()
	{
		TEXTURE_DESC desc;
		desc.type = TEX_TYPE_2D;
		desc.width = 2;
		desc.height = 2;
		desc.format = PF_RGBA8;
		desc.usage = TU_STATIC;

		// White built-in texture
		SPtr<Texture> whiteTexture = createTexture(desc);

		SPtr<PixelData> whitePixelData = PixelData::create(2, 2, 1, PF_RGBA8);
		whitePixelData->setColorAt(Color::White, 0, 0);
		whitePixelData->setColorAt(Color::White, 0, 1);
		whitePixelData->setColorAt(Color::White, 1, 0);
		whitePixelData->setColorAt(Color::White, 1, 1);

		whiteTexture->writeData(*whitePixelData);
		Texture::WHITE = whiteTexture;

		// Black built-in texture
		SPtr<Texture> blackTexture = createTexture(desc);

		SPtr<PixelData> blackPixelData = PixelData::create(2, 2, 1, PF_RGBA8);
		blackPixelData->setColorAt(Color::ZERO, 0, 0);
		blackPixelData->setColorAt(Color::ZERO, 0, 1);
		blackPixelData->setColorAt(Color::ZERO, 1, 0);
		blackPixelData->setColorAt(Color::ZERO, 1, 1);

		blackTexture->writeData(*blackPixelData);
		Texture::BLACK = blackTexture;

		// Normal (Y = Up) built-in texture
		SPtr<Texture> normalTexture = createTexture(desc);
		SPtr<PixelData> normalPixelData = PixelData::create(2, 2, 1, PF_RGBA8);

		Color encodedNormal(0.5f, 0.5f, 1.0f);
		normalPixelData->setColorAt(encodedNormal, 0, 0);
		normalPixelData->setColorAt(encodedNormal, 0, 1);
		normalPixelData->setColorAt(encodedNormal, 1, 0);
		normalPixelData->setColorAt(encodedNormal, 1, 1);

		normalTexture->writeData(*normalPixelData);
		Texture::NORMAL = normalTexture;
	}

	void TextureManager::onShutDown()
	{
		// Need to make sure these are freed while still on the core thread
		Texture::WHITE = nullptr;
		Texture::BLACK = nullptr;
		Texture::NORMAL = nullptr;
	}

	SPtr<Texture> TextureManager::createTexture(const TEXTURE_DESC& desc, const SPtr<PixelData>& pixelData, GpuDeviceFlags deviceMask)
	{
		SPtr<Texture> newTex = createTextureInternal(desc, pixelData, deviceMask);

		newTex->initialize();

		return newTex;
	}

	SPtr<Texture> TextureManager::createTexture(const TEXTURE_DESC& desc, GpuDeviceFlags deviceMask)
	{
		SPtr<Texture> newTex = createTextureInternal(desc, nullptr, deviceMask);

		newTex->initialize();

		return newTex;
	}

	SPtr<Texture> TextureManager::_createEmpty()
	{
		SPtr<Texture> empty = _createEmptyImpl();

		return empty;
	}

	SPtr<RenderTexture> RenderTextureManager::createRenderTexture(const TEXTURE_DESC& colorDesc, bool createDepth, PixelFormat depthStencilFormat)
	{
		TEXTURE_DESC textureDesc = colorDesc;
		textureDesc.usage = TU_RENDERTARGET;
		textureDesc.numMips = 0;

		SPtr<Texture> texture = Texture::create(textureDesc);

		HTexture depthStencil;
		if (createDepth)
		{
			textureDesc.format = depthStencilFormat;
			textureDesc.hwGamma = false;
			textureDesc.usage = TU_DEPTHSTENCIL;

			depthStencil = Texture::createHandle(textureDesc);
		}

		RENDER_TEXTURE_DESC desc;
		desc.colorSurfaces[0].texture = texture;
		desc.colorSurfaces[0].face = 0;
		desc.colorSurfaces[0].numFaces = 1;
		desc.colorSurfaces[0].mipLevel = 0;

		desc.depthStencilSurface.texture = depthStencil.getInternalPtr();//XXXTEX
		desc.depthStencilSurface.face = 0;
		desc.depthStencilSurface.numFaces = 1;
		desc.depthStencilSurface.mipLevel = 0;

		SPtr<RenderTexture> newRT = createRenderTexture(desc);

		return newRT;
	}

	SPtr<RenderTexture> RenderTextureManager::createRenderTexture(const RENDER_TEXTURE_DESC& desc, UINT32 deviceIdx)
	{
		SPtr<RenderTexture> newRT = createRenderTextureInternal(desc, deviceIdx);
		newRT->initialize();

		return newRT;
	}
}
