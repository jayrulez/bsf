//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Image/BsSpriteTexture.h"
#include "Private/RTTI/BsSpriteTextureResourceRTTI.h"
#include "Image/BsTexture.h"
#include "Resources/BsResources.h"
#include "Resources/BsBuiltinResources.h"
#include "CoreThread/BsCoreObjectSync.h"

namespace bs
{
	Rect2 SpriteTextureBase::evaluate(float t) const
	{
		if(mPlayback == SpriteAnimationPlayback::None)
			return Rect2(mUVOffset.x, mUVOffset.y, mUVScale.x, mUVScale.y);

		UINT32 row;
		UINT32 column;
		getAnimationFrame(t, row, column);

		Rect2 output;

		// Note: These could be pre-calculated
		output.width = mUVScale.x / mAnimation.numColumns;
		output.height = mUVScale.y / mAnimation.numRows;

		output.x = mUVOffset.x + column * output.width;
		output.y = mUVOffset.y + row * output.height;

		return output;
	}

	void SpriteTextureBase::getAnimationFrame(float t, UINT32& row, UINT32& column) const
	{
		if(mPlayback == SpriteAnimationPlayback::None)
		{
			row = 0;
			column = 0;

			return;
		}

		// Note: Duration could be pre-calculated
		float duration = 0.0f;
		if (mAnimation.fps > 0)
			duration = mAnimation.count / (float)mAnimation.fps;

		switch(mPlayback)
		{
		default:
		case SpriteAnimationPlayback::Normal:
			t = Math::clamp(t, 0.0f, duration);
			break;
		case SpriteAnimationPlayback::Loop:
			t = Math::repeat(t, duration);
			break;
		case SpriteAnimationPlayback::PingPong:
			t = Math::pingPong(t, duration);
			break;
		}

		const float pct = t / duration;
		UINT32 frame = 0;
		
		if(mAnimation.count > 0)
			frame = Math::clamp(Math::floorToPosInt(pct * mAnimation.count), 0U, mAnimation.count - 1);

		row = frame / mAnimation.numColumns;
		column = frame % mAnimation.numColumns;
	}

	template <bool Core>
	template <class P>
	void TSpriteTexture<Core>::rttiEnumFields(P p)
	{
		p(mUVOffset);
		p(mUVScale);
		p(mAnimation);
		p(mPlayback);
		p(mAtlasTexture);
	}

	SpriteTextureResource::SpriteTextureResource(const Vector2& uvOffset, const Vector2& uvScale, const TextureResourceHandle& texture)
		:TSpriteTexture(uvOffset, uvScale, texture)
	{ }

	const SpriteTextureResourceHandle& SpriteTextureResource::dummy()
	{
		return BuiltinResources::instance().getDummySpriteTexture();
	}

	bool SpriteTextureResource::checkIsLoaded(const SpriteTextureResourceHandle& tex)
	{
		return tex != nullptr && tex.isLoaded(false) && tex->getTexture() != nullptr && tex->getTexture().isLoaded(false);
	}

	void SpriteTextureResource::setTexture(const TextureResourceHandle& texture)
	{
		removeResourceDependency(mAtlasTexture);
		mAtlasTexture = texture;
		addResourceDependency(mAtlasTexture);

		markDependenciesDirty();
	}

	UINT32 SpriteTextureResource::getWidth() const
	{
		return Math::roundToInt(mAtlasTexture->getProperties().getWidth() * mUVScale.x);
	}

	UINT32 SpriteTextureResource::getHeight() const
	{
		return Math::roundToInt(mAtlasTexture->getProperties().getHeight() * mUVScale.y);
	}

	UINT32 SpriteTextureResource::getFrameWidth() const
	{
		return getWidth() / std::max(1U, mAnimation.numColumns);
	}

	UINT32 SpriteTextureResource::getFrameHeight() const
	{
		return getHeight() / std::max(1U, mAnimation.numRows);
	}

	void SpriteTextureResource::_markCoreDirty()
	{
		markCoreDirty();
	}

	void SpriteTextureResource::initialize()
	{
		addResourceDependency(mAtlasTexture);

		Resource::initialize();
	}

	SPtr<ct::CoreObject> SpriteTextureResource::createCore() const
	{
		SPtr<ct::Texture> texturePtr;
		if(mAtlasTexture.isLoaded())
			texturePtr = mAtlasTexture->getCore();

		ct::SpriteTexture* spriteTexture = new (bs_alloc<ct::SpriteTexture>()) ct::SpriteTexture(mUVOffset, mUVScale,
			std::move(texturePtr), mAnimation, mPlayback);

		SPtr<ct::SpriteTexture> spriteTexPtr = bs_shared_ptr<ct::SpriteTexture>(spriteTexture);
		spriteTexPtr->_setThisPtr(spriteTexPtr);

		return spriteTexPtr;
	}

	CoreSyncData SpriteTextureResource::syncToCore(FrameAlloc* allocator)
	{
		UINT32 size = csync_size(*this);

		UINT8* buffer = allocator->alloc(size);
		Bitstream stream(buffer, size);
		csync_write(*this, stream);

		return CoreSyncData(buffer, size);
	}

	void SpriteTextureResource::getCoreDependencies(Vector<CoreObject*>& dependencies)
	{
		if (mAtlasTexture.isLoaded())
			dependencies.push_back(mAtlasTexture.get());
	}

	SPtr<ct::SpriteTexture> SpriteTextureResource::getCore() const
	{
		return std::static_pointer_cast<ct::SpriteTexture>(mCoreSpecific);
	}

	SpriteTextureResourceHandle SpriteTextureResource::create(const TextureResourceHandle& texture)
	{
		SPtr<SpriteTextureResource> texturePtr = _createPtr(texture);

		return static_resource_cast<SpriteTextureResource>(gResources()._createResourceHandle(texturePtr));
	}

	SpriteTextureResourceHandle SpriteTextureResource::create(const Vector2& uvOffset, const Vector2& uvScale, const TextureResourceHandle& texture)
	{
		SPtr<SpriteTextureResource> texturePtr = _createPtr(uvOffset, uvScale, texture);

		return static_resource_cast<SpriteTextureResource>(gResources()._createResourceHandle(texturePtr));
	}

	SPtr<SpriteTextureResource> SpriteTextureResource::_createPtr(const TextureResourceHandle& texture)
	{
		SPtr<SpriteTextureResource> texturePtr = bs_core_ptr<SpriteTextureResource>
			(new (bs_alloc<SpriteTextureResource>()) SpriteTextureResource(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), texture));

		texturePtr->_setThisPtr(texturePtr);
		texturePtr->initialize();

		return texturePtr;
	}

	SPtr<SpriteTextureResource> SpriteTextureResource::_createPtr(const Vector2& uvOffset, const Vector2& uvScale, const TextureResourceHandle& texture)
	{
		SPtr<SpriteTextureResource> texturePtr = bs_core_ptr<SpriteTextureResource>
			(new (bs_alloc<SpriteTextureResource>()) SpriteTextureResource(uvOffset, uvScale, texture));

		texturePtr->_setThisPtr(texturePtr);
		texturePtr->initialize();

		return texturePtr;
	}

	SPtr<SpriteTextureResource> SpriteTextureResource::createEmpty()
	{
		SPtr<SpriteTextureResource> texturePtr = bs_core_ptr<SpriteTextureResource>
			(new (bs_alloc<SpriteTextureResource>()) SpriteTextureResource(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), TextureResourceHandle()));

		texturePtr->_setThisPtr(texturePtr);

		return texturePtr;
	}

	RTTITypeBase* SpriteTextureResource::getRTTIStatic()
	{
		return SpriteTextureResourceRTTI::instance();
	}

	RTTITypeBase* SpriteTextureResource::getRTTI() const
	{
		return SpriteTextureResource::getRTTIStatic();
	}
}
