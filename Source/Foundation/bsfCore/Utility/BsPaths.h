//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

namespace bs
{
	/** @addtogroup Utility-Engine-Internal
	 *  @{
	 */

	/** Contains common engine paths and utility method for searching for paths. */
	class BS_CORE_EXPORT Paths
	{
	public:	
		/**	Returns the absolute path where the builtin framework-specific assets are located. */
		static const Path& getDataPath();

		/** Returns the absolute path where the engine binaries are located in. */
		static const Path& getBinariesPath();
		
		/**
		 * Searches common locations for a specified path by querying if the file/directory exists and returns the found
		 * path.
		 * 			
		 * @param[in]	path	Relative path to search for (for example "Data\").
		 * @return				Path at which the relative path was found at. This path will be relative to the working
		 *						directory.
		 */
		static Path findPath(const Path& path);

		/** Path to the root data directory. Relative to working directory, or RAW_APP_ROOT. */
		static inline const Path FRAMEWORK_DATA_PATH = "Data/";
	};

	/** @} */
}
