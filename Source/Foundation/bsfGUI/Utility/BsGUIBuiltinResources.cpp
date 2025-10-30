//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsGUIBuiltinResources.h"
#include "GUI/BsGUISkin.h"
#include "Image/BsSpriteTexture.h"
#include "RenderAPI/BsTexture.h"
#include "Resources/BsResources.h"
#include "Utility/BsPaths.h"
#include "Material/BsShader.h"
#include "Material/BsMaterial.h"
#include "Image/BsPixelData.h"
#include "CoreThread/BsCoreThread.h"

namespace bs
{
	constexpr const char* GUIBuiltinResources::IconTextureName;
	constexpr const char* GUIBuiltinResources::MultiLineLabelStyle;

	GUIBuiltinResources::~GUIBuiltinResources()
	{
		mCursorArrow = nullptr;
		mCursorArrowDrag = nullptr;
		mCursorArrowLeftRight = nullptr;
		mCursorIBeam = nullptr;
		mCursorDeny = nullptr;
		mCursorWait = nullptr;
		mCursorSizeNESW = nullptr;
		mCursorSizeNS = nullptr;
		mCursorSizeNWSE = nullptr;
		mCursorSizeWE = nullptr;
		mFrameworkIcon = nullptr;
	}

	GUIBuiltinResources::GUIBuiltinResources()
	{
		// Set up paths
		mBuiltinDataFolder = Paths::getDataPath();
		mEngineSkinSpritesFolder = mBuiltinDataFolder + SKIN_FOLDER + SPRITE_FOLDER;
		mEngineShaderFolder = mBuiltinDataFolder + SHADER_FOLDER;
		mEngineCursorFolder = mBuiltinDataFolder + CURSOR_FOLDER;

		// Load shaders
		mShaderSpriteText = getShader(ShaderSpriteTextFile);
		mShaderSpriteImage = getShader(ShaderSpriteImageFile);
		mShaderSpriteLine = getShader(ShaderSpriteLineFile);

		// Load sprite textures
		mWhiteSpriteTexture = getSkinTexture(WhiteTex);

		// Create dummy sprite texture
		SPtr<PixelData> dummyPixelData = PixelData::create(2, 2, 1, PF_RGBA8);
		dummyPixelData->setColorAt(Color::Red, 0, 0);
		dummyPixelData->setColorAt(Color::Red, 0, 1);
		dummyPixelData->setColorAt(Color::Red, 1, 0);
		dummyPixelData->setColorAt(Color::Red, 1, 1);
		HTexture dummyTexture = Texture::create(dummyPixelData);
		mDummySpriteTexture = SpriteTexture::create(dummyTexture);

		// Load GUI skin
		mSkin = gResources().load<GUISkin>(mBuiltinDataFolder + (String(GUI_SKIN_FILE) + u8".json.asset"));
		mEmptySkin = GUISkin::create();

		/************************************************************************/
		/* 							CURSOR		                     			*/
		/************************************************************************/

		HTexture cursorArrowTex = getCursorTexture(CursorArrowTex);
		HTexture cursorArrowDragTex = getCursorTexture(CursorArrowDragTex);
		HTexture cursorArrowLeftRightTex = getCursorTexture(CursorArrowLeftRightTex);
		HTexture cursorIBeamTex = getCursorTexture(CursorIBeamTex);
		HTexture cursorDenyTex = getCursorTexture(CursorDenyTex);
		HTexture cursorWaitTex = getCursorTexture(CursorWaitTex);
		HTexture cursorSizeNESWTex = getCursorTexture(CursorSizeNESWTex);
		HTexture cursorSizeNSTex = getCursorTexture(CursorSizeNSTex);
		HTexture cursorSizeNWSETex = getCursorTexture(CursorSizeNWSETex);
		HTexture cursorSizeWETex = getCursorTexture(CursorSizeWETex);

		mCursorArrow = cursorArrowTex->getProperties().allocBuffer(0, 0);
		cursorArrowTex->readData(mCursorArrow);

		mCursorArrowDrag = cursorArrowDragTex->getProperties().allocBuffer(0, 0);
		cursorArrowDragTex->readData(mCursorArrowDrag);

		mCursorArrowLeftRight = cursorArrowLeftRightTex->getProperties().allocBuffer(0, 0);
		cursorArrowLeftRightTex->readData(mCursorArrowLeftRight);

		mCursorIBeam = cursorIBeamTex->getProperties().allocBuffer(0, 0);
		cursorIBeamTex->readData(mCursorIBeam);

		mCursorDeny = cursorDenyTex->getProperties().allocBuffer(0, 0);
		cursorDenyTex->readData(mCursorDeny);

		mCursorWait = cursorWaitTex->getProperties().allocBuffer(0, 0);
		cursorWaitTex->readData(mCursorWait);

		mCursorSizeNESW = cursorSizeNESWTex->getProperties().allocBuffer(0, 0);
		cursorSizeNESWTex->readData(mCursorSizeNESW);

		mCursorSizeNS = cursorSizeNSTex->getProperties().allocBuffer(0, 0);
		cursorSizeNSTex->readData(mCursorSizeNS);

		mCursorSizeNWSE = cursorSizeNWSETex->getProperties().allocBuffer(0, 0);
		cursorSizeNWSETex->readData(mCursorSizeNWSE);

		mCursorSizeWE = cursorSizeWETex->getProperties().allocBuffer(0, 0);
		cursorSizeWETex->readData(mCursorSizeWE);

		/************************************************************************/
		/* 							ICON		                     			*/
		/************************************************************************/

		Path iconPath = mBuiltinDataFolder + ICON_FOLDER;
		iconPath.append(String(IconTextureName) + u8".asset");

		HTexture iconTex = gResources().load<Texture>(iconPath);

		mFrameworkIcon = iconTex->getProperties().allocBuffer(0, 0);
		iconTex->readData(mFrameworkIcon);

		gCoreThread().submit(true);
	}

	HSpriteTexture GUIBuiltinResources::getSkinTexture(const String& name) const
	{
		Path texturePath = mEngineSkinSpritesFolder;
		texturePath.append(u8"sprite_" + name + u8".asset");

		return gResources().load<SpriteTexture>(texturePath);
	}

	HShader GUIBuiltinResources::getShader(const Path& path) const
	{
		Path programPath = mEngineShaderFolder;
		programPath.append(path);
		programPath.setExtension(programPath.getExtension() + ".asset");

		return gResources().load<Shader>(programPath);
	}

	HTexture GUIBuiltinResources::getCursorTexture(const String& name) const
	{
		Path cursorPath = mEngineCursorFolder;
		cursorPath.append(name + u8".asset");

		return gResources().load<Texture>(cursorPath);
	}

	const PixelData& GUIBuiltinResources::getCursorArrow(Vector2I& hotSpot)
	{
		hotSpot = CursorArrowHotspot;
		return *mCursorArrow.get();
	}

	const PixelData& GUIBuiltinResources::getCursorArrowDrag(Vector2I& hotSpot)
	{
		hotSpot = CursorArrowDragHotspot;
		return *mCursorArrowDrag.get();
	}

	const PixelData& GUIBuiltinResources::getCursorWait(Vector2I& hotSpot)
	{
		hotSpot = CursorWaitHotspot;
		return *mCursorWait.get();
	}

	const PixelData& GUIBuiltinResources::getCursorIBeam(Vector2I& hotSpot)
	{
		hotSpot = CursorIBeamHotspot;
		return *mCursorIBeam.get();
	}

	const PixelData& GUIBuiltinResources::getCursorSizeNESW(Vector2I& hotSpot)
	{
		hotSpot = CursorSizeNESWHotspot;
		return *mCursorSizeNESW.get();
	}

	const PixelData& GUIBuiltinResources::getCursorSizeNS(Vector2I& hotSpot)
	{
		hotSpot = CursorSizeNSHotspot;
		return *mCursorSizeNS.get();
	}

	const PixelData& GUIBuiltinResources::getCursorSizeNWSE(Vector2I& hotSpot)
	{
		hotSpot = CursorSizeNWSEHotspot;
		return *mCursorSizeNWSE.get();
	}

	const PixelData& GUIBuiltinResources::getCursorSizeWE(Vector2I& hotSpot)
	{
		hotSpot = CursorSizeWEHotspot;
		return *mCursorSizeWE.get();
	}

	const PixelData& GUIBuiltinResources::getCursorDeny(Vector2I& hotSpot)
	{
		hotSpot = CursorDenyHotspot;
		return *mCursorDeny.get();
	}

	const PixelData& GUIBuiltinResources::getCursorMoveLeftRight(Vector2I& hotSpot)
	{
		hotSpot = CursorArrowLeftRightHotspot;
		return *mCursorArrowLeftRight.get();
	}

	const PixelData& GUIBuiltinResources::getFrameworkIcon()
	{
		return *mFrameworkIcon.get();
	}

	HMaterial GUIBuiltinResources::createSpriteTextMaterial() const
	{
		return Material::create(mShaderSpriteText);
	}

	HMaterial GUIBuiltinResources::createSpriteImageMaterial() const
	{
		return Material::create(mShaderSpriteImage);
	}

	HMaterial GUIBuiltinResources::createSpriteLineMaterial() const
	{
		return Material::create(mShaderSpriteLine);
	}

	GUIBuiltinResources& gGUIBuiltinResources()
	{
		return GUIBuiltinResources::instance();
	}
}
