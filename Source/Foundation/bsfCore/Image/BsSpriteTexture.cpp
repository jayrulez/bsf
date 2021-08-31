//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Image/BsSpriteTexture.h"
#include "Image/BsTexture.h"
#include "Private/RTTI/BsSpriteTextureResourceRTTI.h"
#include "Resources/BsResources.h"
#include "Resources/BsBuiltinResources.h"
#include "CoreThread/BsCoreObjectSync.h"

namespace bs
{

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

	namespace ct
	{
		SpriteTexture::SpriteTexture(const Vector2& uvOffset, const Vector2& uvScale, SPtr<Texture> texture,
			const SpriteSheetGridAnimation& anim, SpriteAnimationPlayback playback)
			:TSpriteTexture(uvOffset, uvScale, texture)
		{
			mAnimation = anim;
			mPlayback = playback;
		}

		void SpriteTexture::syncToCore(const CoreSyncData& data)
		{
			Bitstream stream(data.getBuffer(), data.getBufferSize());
			csync_read(*this, stream);
		}
	}
}
