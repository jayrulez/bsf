//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Text/BsFontResource.h"
#include "Private/RTTI/BsFontResourceRTTI.h"
#include "Resources/BsResources.h"

namespace bs
{
	const CharDesc& FontBitmap::getCharDesc(UINT32 charId) const
	{
		auto iterFind = characters.find(charId);
		if(iterFind != characters.end())
		{
			return characters.at(charId);
		}

		return missingGlyph;
	}

	RTTITypeBase* FontBitmap::getRTTIStatic()
	{
		return FontBitmapRTTI::instance();
	}

	RTTITypeBase* FontBitmap::getRTTI() const
	{
		return FontBitmap::getRTTIStatic();
	}

	FontResource::FontResource()
		:Resource(false)
	{ }

	void FontResource::initialize(const Vector<SPtr<FontBitmap>>& fontData)
	{
		for(auto iter = fontData.begin(); iter != fontData.end(); ++iter)
		{
			mFontDataPerSize[(*iter)->size] = *iter;

			for (auto& texture : (*iter)->texturePages)
			{
				if (texture != nullptr)
					addResourceDependency(texture);
			}
		}

		Resource::initialize();
	}

	SPtr<FontBitmap> FontResource::getBitmap(UINT32 size) const
	{
		auto iterFind = mFontDataPerSize.find(size);

		if(iterFind == mFontDataPerSize.end())
			return nullptr;

		return iterFind->second;
	}

	INT32 FontResource::getClosestSize(UINT32 size) const
	{
		UINT32 minDiff = std::numeric_limits<UINT32>::max();
		UINT32 bestSize = size;

		for(auto iter = mFontDataPerSize.begin(); iter != mFontDataPerSize.end(); ++iter)
		{
			if(iter->first == size)
				return size;
			else if(iter->first > size)
			{
				UINT32 diff = iter->first - size;
				if(diff < minDiff)
				{
					minDiff = diff;
					bestSize = iter->first;
				}
			}
			else
			{
				UINT32 diff = size - iter->first;
				if(diff < minDiff)
				{
					minDiff = diff;
					bestSize = iter->first;
				}
			}
		}

		return bestSize;
	}

	void FontResource::getCoreDependencies(Vector<CoreObject*>& dependencies)
	{
		for (auto& fontDataEntry : mFontDataPerSize)
		{
			for (auto& texture : fontDataEntry.second->texturePages)
			{
				if (texture.isLoaded())
					dependencies.push_back(texture.get());
			}
		}
	}

	FontResourceHandle FontResource::create(const Vector<SPtr<FontBitmap>>& fontData)
	{
		SPtr<FontResource> newFont = _createPtr(fontData);

		return static_resource_cast<FontResource>(gResources()._createResourceHandle(newFont));
	}

	SPtr<FontResource> FontResource::_createPtr(const Vector<SPtr<FontBitmap>>& fontData)
	{
		SPtr<FontResource> newFont = bs_core_ptr<FontResource>(new (bs_alloc<FontResource>()) FontResource());
		newFont->_setThisPtr(newFont);
		newFont->initialize(fontData);

		return newFont;
	}

	SPtr<FontResource> FontResource::_createEmpty()
	{
		SPtr<FontResource> newFont = bs_core_ptr<FontResource>(new (bs_alloc<FontResource>()) FontResource());
		newFont->_setThisPtr(newFont);

		return newFont;
	}

	RTTITypeBase* FontResource::getRTTIStatic()
	{
		return FontResourceRTTI::instance();
	}

	RTTITypeBase* FontResource::getRTTI() const
	{
		return FontResource::getRTTIStatic();
	}
}
