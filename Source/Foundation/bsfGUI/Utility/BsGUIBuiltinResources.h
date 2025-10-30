//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsGUIPrerequisites.h"
#include "Utility/BsModule.h"
#include "Math/BsVector2I.h"

namespace bs
{
	/** @addtogroup GUI
	 *  @{
	 */

	/**	Holds references to built-in GUI resources used by the engine. */
	class BS_GUI_EXPORT GUIBuiltinResources : public Module<GUIBuiltinResources>
	{
	public:
		GUIBuiltinResources();
		~GUIBuiltinResources();

		/**	Returns the default skin used by engine GUI elements. */
		const HGUISkin& getGUISkin() const { return mSkin; }

		/**	Returns an empty skin used to be used when no other is available. */
		const HGUISkin& getEmptyGUISkin() const { return mEmptySkin; }

		/**	Returns a small entirely white texture. */
		const HSpriteTexture& getWhiteSpriteTexture() const { return mWhiteSpriteTexture; }

		/**	Returns a 2x2 sprite texture that can be used when no other is available. */
		const HSpriteTexture& getDummySpriteTexture() const { return mDummySpriteTexture; }

		/**	Returns image data for an arrow cursor, along with its hotspot. */
		const PixelData& getCursorArrow(Vector2I& hotSpot);

		/**	Returns image data for an arrow with dragged object cursor, along with its hotspot. */
		const PixelData& getCursorArrowDrag(Vector2I& hotSpot);

		/**	Returns image data for a wait cursor, along with its hotspot. */
		const PixelData& getCursorWait(Vector2I& hotSpot);

		/**	Returns image data for an "I" beam cursor, along with its hotspot. */
		const PixelData& getCursorIBeam(Vector2I& hotSpot);

		/**	Returns image data for a NESW resize cursor, along with its hotspot. */
		const PixelData& getCursorSizeNESW(Vector2I& hotSpot);

		/**	Returns image data for a NS resize cursor, along with its hotspot. */
		const PixelData& getCursorSizeNS(Vector2I& hotSpot);

		/**	Returns image data for a NWSE resize cursor, along with its hotspot. */
		const PixelData& getCursorSizeNWSE(Vector2I& hotSpot);

		/**	Returns image data for a WE resize cursor, along with its hotspot. */
		const PixelData& getCursorSizeWE(Vector2I& hotSpot);

		/**	Returns image data for a deny cursor, along with its hotspot. */
		const PixelData& getCursorDeny(Vector2I& hotSpot);

		/**	Returns image data for a move left-right cursor, along with its hotspot. */
		const PixelData& getCursorMoveLeftRight(Vector2I& hotSpot);

		/**	Returns the default application icon. */
		const PixelData& getFrameworkIcon();

		/**	Creates a material used for textual sprite rendering (for example text in GUI). */
		HMaterial createSpriteTextMaterial() const;

		/**	Creates a material used for image sprite rendering (for example images in GUI). */
		HMaterial createSpriteImageMaterial() const;

		/** Creates a material used for antialiased line rendering (for example curve rendering in GUI). */
		HMaterial createSpriteLineMaterial() const;

		/**	Loads a shader at the specified path.
		 *
		 * @param[in]	path	Path relative to the default shader folder with no file extension.
		 */
		HShader getShader(const Path& path) const;

		static constexpr const char* IconTextureName = "bsfIcon.png";
		static constexpr const char* MultiLineLabelStyle = "MultiLineLabel";

		static constexpr const char* SHADER_FOLDER = "Shaders/";
		static constexpr const char* CURSOR_FOLDER = "Cursors/";
		static constexpr const char* ICON_FOLDER = "Icons/";
		static constexpr const char* ICON3D_FOLDER = "Icons3D/";
		static constexpr const char* SKIN_FOLDER = "Skin/";
		static constexpr const char* SPRITE_FOLDER = "Sprites/";
		static constexpr const char* ANIMATED_SPRITES_FOLDER = "AnimatedSprites/";

		static constexpr const char* GUI_SKIN_FILE = u8"GUISkin";

	private:
		/**	Loads a GUI skin texture with the specified filename. */
		HSpriteTexture getSkinTexture(const String& name) const;

		/**	Loads a cursor texture with the specified filename. */
		HTexture getCursorTexture(const String& name) const;

		HGUISkin mEmptySkin;
		HGUISkin mSkin;

		SPtr<PixelData> mCursorArrow;
		SPtr<PixelData> mCursorArrowDrag;
		SPtr<PixelData> mCursorArrowLeftRight;
		SPtr<PixelData> mCursorIBeam;
		SPtr<PixelData> mCursorDeny;
		SPtr<PixelData> mCursorWait;
		SPtr<PixelData> mCursorSizeNESW;
		SPtr<PixelData> mCursorSizeNS;
		SPtr<PixelData> mCursorSizeNWSE;
		SPtr<PixelData> mCursorSizeWE;
		SPtr<PixelData> mFrameworkIcon;

		HSpriteTexture mWhiteSpriteTexture;
		HSpriteTexture mDummySpriteTexture;

		HShader mShaderSpriteText;
		HShader mShaderSpriteImage;
		HShader mShaderSpriteLine;

		Path mBuiltinDataFolder;
		Path mEngineSkinSpritesFolder;
		Path mEngineShaderFolder;
		Path mEngineCursorFolder;

		static inline const String WhiteTex = u8"White.psd";

		static inline const String CursorArrowTex = u8"Arrow.psd";
		static inline const String CursorArrowDragTex = u8"ArrowDrag.psd";
		static inline const String CursorArrowLeftRightTex = u8"ArrowLeftRight.psd";
		static inline const String CursorIBeamTex = u8"IBeam.psd";
		static inline const String CursorDenyTex = u8"Deny.psd";
		static inline const String CursorWaitTex = u8"Wait.psd";
		static inline const String CursorSizeNESWTex = u8"SizeNESW.psd";
		static inline const String CursorSizeNSTex = u8"SizeNS.psd";
		static inline const String CursorSizeNWSETex = u8"SizeNWSE.psd";
		static inline const String CursorSizeWETex = u8"SizeWE.psd";

		static inline const Vector2I CursorArrowHotspot = Vector2I(10, 8);
		static inline const Vector2I CursorArrowDragHotspot = Vector2I(8, 4);
		static inline const Vector2I CursorArrowLeftRightHotspot = Vector2I(13, 9);
		static inline const Vector2I CursorIBeamHotspot = Vector2I(16, 15);
		static inline const Vector2I CursorDenyHotspot = Vector2I(15, 15);
		static inline const Vector2I CursorWaitHotspot = Vector2I(15, 15);
		static inline const Vector2I CursorSizeNESWHotspot = Vector2I(16, 15);
		static inline const Vector2I CursorSizeNSHotspot = Vector2I(16, 15);
		static inline const Vector2I CursorSizeNWSEHotspot = Vector2I(16, 15);
		static inline const Vector2I CursorSizeWEHotspot = Vector2I(16, 15);

		static inline const String ShaderSpriteTextFile = u8"SpriteText.bsl";
		static inline const String ShaderSpriteImageFile = u8"SpriteImage.bsl";
		static inline const String ShaderSpriteLineFile = u8"SpriteLine.bsl";
	};

	/**	Provides easy access to GUIBuiltinResources. */
	BS_GUI_EXPORT GUIBuiltinResources& gGUIBuiltinResources();

	/** @} */
}
