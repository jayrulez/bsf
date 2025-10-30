//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

namespace bs
{
	/** @addtogroup Utility-GUI
	 *  @{
	 */

	/** Types of GUI meshes that can be output by GUI elements. */
	enum class GUIMeshType
	{
		/** Mesh containing a set of vertices as 2D position followed by 2D UV coordinates. */
		Triangle,
		/** Mesh containing a set of vertices as 2D position followed by vertex indices. */
		Line
	};

	/**	Determines type of data to display on the profiler overlay. */
	enum class ProfilerOverlayType
	{
		/** Display CPU samples on the overlay. */
		CPUSamples,

		/** Display GPU samples on the overlay. */
		GPUSamples
	};

	/** @} */
}
