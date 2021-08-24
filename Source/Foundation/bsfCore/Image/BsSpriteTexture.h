//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"
#include "Math/BsVector2.h"
#include "Image/BsSpriteTextureResource.h"

namespace bs
{
	namespace ct
	{
		/** @addtogroup Resources-Internal
		 *  @{
		 */

		/**
		 * Core thread version of a bs::SpriteTexture.
		 *
		 * @note	Core thread.
		 */
		class BS_CORE_EXPORT SpriteTexture : public CoreObject, public TSpriteTexture<true>
		{
		public:
			/**	Determines the internal texture that the sprite texture references. */
			void setTexture(const SPtr<ct::Texture>& texture) { mAtlasTexture = texture; }

			/**	@copydoc setTexture() */
			const SPtr<ct::Texture>& getTexture() const { return mAtlasTexture; }

		private:
			friend class bs::SpriteTextureResource;

			SpriteTexture(const Vector2& uvOffset, const Vector2& uvScale, SPtr<Texture> texture,
				const SpriteSheetGridAnimation& anim, SpriteAnimationPlayback playback);

			/** @copydoc CoreObject::syncToCore */
			void syncToCore(const CoreSyncData& data) override;
		};

		/** @} */
	}
}
