//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsRuntimePrerequisites.h"
#include "Utility/BsModule.h"

namespace bs
{
	/** @addtogroup Resources-Runtime
	 *  @{
	 */

	/**
	 * Manages the resource manifest for built-in resources.
	 * For accessing actual builtin resources, use the specialized modules:
	 * - gTextBuiltinResources() for font resources
	 * - gRendererBuiltinResources() for meshes, textures, and shaders
	 * - gGUIBuiltinResources() for GUI skins, sprites, and cursors
	 */
	class BS_RUNTIME_EXPORT RuntimeBuiltinResources : public Module<RuntimeBuiltinResources>
	{
	public:
		RuntimeBuiltinResources();
		~RuntimeBuiltinResources();

		/** Returns the resource manifest for builtin resources. */
		SPtr<ResourceManifest> getResourceManifest() const { return mResourceManifest; }

	private:
		SPtr<ResourceManifest> mResourceManifest;
		Path mBuiltinRawDataFolder;
		Path mBuiltinDataFolder;
		Path ResourceManifestPath;
	};

	/**	Provides easy access to RuntimeBuiltinResources. */
	BS_RUNTIME_EXPORT RuntimeBuiltinResources& gRuntimeBuiltinResources();

	/** @} */
}
