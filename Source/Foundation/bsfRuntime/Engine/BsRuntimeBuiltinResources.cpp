//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Engine/BsRuntimeBuiltinResources.h"
#include "Resources/BsResources.h"
#include "Resources/BsResourceManifest.h"
#include "FileSystem/BsFileSystem.h"
#include "Utility/BsPaths.h"

namespace bs
{
	RuntimeBuiltinResources::RuntimeBuiltinResources()
	{
		// Set up paths
		mBuiltinRawDataFolder = Paths::getDataPath() + u8"Raw/";
		mBuiltinDataFolder = Paths::getDataPath();
		ResourceManifestPath = mBuiltinDataFolder + "ResourceManifest.asset";

		// Load manifest
		if (FileSystem::exists(ResourceManifestPath))
			mResourceManifest = ResourceManifest::load(ResourceManifestPath, mBuiltinDataFolder);

		if (mResourceManifest == nullptr)
			mResourceManifest = ResourceManifest::create("BuiltinResources");

		gResources().registerResourceManifest(mResourceManifest);
	}

	RuntimeBuiltinResources::~RuntimeBuiltinResources()
	{
	}

	RuntimeBuiltinResources& gRuntimeBuiltinResources()
	{
		return RuntimeBuiltinResources::instance();
	}
}
