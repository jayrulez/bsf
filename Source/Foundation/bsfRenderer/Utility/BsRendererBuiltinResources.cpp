//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsRendererBuiltinResources.h"
#include "Material/BsShader.h"
#include "RenderAPI/BsTexture.h"
#include "Mesh/BsMesh.h"
#include "Resources/BsResources.h"
#include "Image/BsPixelData.h"
#include "Utility/BsPaths.h"

namespace bs
{
	constexpr const char* ShaderDiffuseFile = u8"Diffuse.bsl";
	constexpr const char* ShaderTransparentFile = u8"Transparent.bsl";
	constexpr const char* ShaderParticlesUnlitFile = u8"ParticlesUnlit.bsl";
	constexpr const char* ShaderParticlesLitFile = u8"ParticlesLit.bsl";
	constexpr const char* ShaderParticlesLitOpaqueFile = u8"ParticlesLitOpaque.bsl";
	constexpr const char* ShaderDecalFile = u8"Decal.bsl";

	RendererBuiltinResources::RendererBuiltinResources()
	{
		// Set up paths
		mBuiltinDataFolder = Paths::getDataPath();
		mEngineShaderFolder = mBuiltinDataFolder + SHADER_FOLDER;
		mEngineMeshFolder = mBuiltinDataFolder + MESH_FOLDER;

		// Load shaders
		mShaderDiffuse = getShader(ShaderDiffuseFile);
		mShaderTransparent = getShader(ShaderTransparentFile);
		mShaderParticlesUnlit = getShader(ShaderParticlesUnlitFile);
		mShaderParticlesLit = getShader(ShaderParticlesLitFile);
		mShaderParticlesLitOpaque = getShader(ShaderParticlesLitOpaqueFile);
		mShaderDecal = getShader(ShaderDecalFile);

		// Create dummy texture
		SPtr<PixelData> dummyPixelData = PixelData::create(2, 2, 1, PF_RGBA8);
		dummyPixelData->setColorAt(Color::Red, 0, 0);
		dummyPixelData->setColorAt(Color::Red, 0, 1);
		dummyPixelData->setColorAt(Color::Red, 1, 0);
		dummyPixelData->setColorAt(Color::Red, 1, 1);
		mDummyTexture = Texture::create(dummyPixelData);
	}

	HShader RendererBuiltinResources::getShader(const Path& path) const
	{
		Path programPath = mEngineShaderFolder;
		programPath.append(path);
		programPath.setExtension(programPath.getExtension() + ".asset");

		return gResources().load<Shader>(programPath);
	}

	HShader RendererBuiltinResources::getBuiltinShader(BuiltinShader type) const
	{
		switch(type)
		{
		case BuiltinShader::Standard:
			return mShaderDiffuse;
		case BuiltinShader::Transparent:
			return mShaderTransparent;
		case BuiltinShader::ParticlesUnlit:
			return mShaderParticlesUnlit;
		case BuiltinShader::ParticlesLit:
			return mShaderParticlesLit;
		case BuiltinShader::ParticlesLitOpaque:
			return mShaderParticlesLitOpaque;
		case BuiltinShader::Decal:
			return mShaderDecal;
		default:
			break;
		}

		return HShader();
	}

	HMesh RendererBuiltinResources::getMesh(BuiltinMesh mesh) const
	{
		Path meshPath = mEngineMeshFolder;

		switch (mesh)
		{
		case BuiltinMesh::Box:
			meshPath.append(MESH_BOX_FILE);
			break;
		case BuiltinMesh::Sphere:
			meshPath.append(MESH_SPHERE_FILE);
			break;
		case BuiltinMesh::Cone:
			meshPath.append(MESH_CONE_FILE);
			break;
		case BuiltinMesh::Cylinder:
			meshPath.append(MESH_CYLINDER_FILE);
			break;
		case BuiltinMesh::Quad:
			meshPath.append(MESH_QUAD_FILE);
			break;
		case BuiltinMesh::Disc:
			meshPath.append(MESH_DISC_FILE);
			break;
		}

		return gResources().load<Mesh>(meshPath);
	}

	HTexture RendererBuiltinResources::getTexture(BuiltinTexture type)
	{
		Path texturePath = Paths::getDataPath();
		texturePath.append(TEXTURE_FOLDER);

		switch (type)
		{
		case BuiltinTexture::Black:
			texturePath.append(TEXTURE_BLACK_FILE);
			break;
		case BuiltinTexture::White:
			texturePath.append(TEXTURE_WHITE_FILE);
			break;
		case BuiltinTexture::Normal:
			texturePath.append(TEXTURE_NORMAL_FILE);
			break;
		case BuiltinTexture::BokehFlare:
			texturePath.append(u8"BokehHex.png.asset");
			break;
		}

		return gResources().load<Texture>(texturePath);
	}

	Path RendererBuiltinResources::getRawShaderFolder()
	{
		return Paths::getDataPath() + "Raw/" + SHADER_FOLDER;
	}

	Path RendererBuiltinResources::getShaderIncludeFolder()
	{
		return Paths::getDataPath() + SHADER_INCLUDE_FOLDER;
	}

#if BS_IS_BANSHEE3D || defined BS_IS_ASSET_TOOL
	Path RendererBuiltinResources::getEditorShaderIncludeFolder()
	{
		return Paths::getEditorDataPath() + SHADER_INCLUDE_FOLDER;
	}
#endif

	RendererBuiltinResources& gRendererBuiltinResources()
	{
		return RendererBuiltinResources::instance();
	}
}
