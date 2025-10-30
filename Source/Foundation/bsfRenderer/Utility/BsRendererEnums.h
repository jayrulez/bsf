//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

namespace bs
{
	/** @addtogroup Utility-Renderer
	 *  @{
	 */

	/**	Projection type to use by the camera. */
	enum BS_SCRIPT_EXPORT() ProjectionType
	{
		/** Projection type where object size remains constant and parallel lines remain parallel. */
		PT_ORTHOGRAPHIC		BS_SCRIPT_EXPORT(n:Orthographic),
		/** Projection type that emulates human vision. Objects farther away appear smaller. */
		PT_PERSPECTIVE		BS_SCRIPT_EXPORT(n:Perspective)
	};

	/**
	 * Suggested queue priority numbers used for sorting objects in the render queue. Objects with higher priority will
	 * be renderer sooner.
	 */
	enum class QueuePriority
	{
		Opaque = 100000,
		Transparent = 90000,
		Skybox = 80000,
		Overlay = 70000
	};

	/** Type of sorting to perform on an object when added to a render queue. */
	enum class QueueSortType
	{
		FrontToBack, /**< All objects with the same priority will be rendered front to back based on their center. */
		BackToFront, /**< All objects with the same priority will be rendered back to front based on their center. */
		None /**< Objects will not be sorted and will be processed in the order they were added to the queue. */
	};

	/**	Flags that may be assigned to a shader that let the renderer know how to interpret the shader. */
	enum class ShaderFlag
	{
		Transparent = 0x1, /**< Signifies that the shader is rendering a transparent object. */
		Forward = 0x2 /**< Signifies the shader should use the forward rendering pipeline, if relevant. */
	};

	typedef Flags<ShaderFlag> ShaderFlags;
	BS_FLAGS_OPERATORS(ShaderFlag)

	/** Determines the type of the source image for generating cubemaps. */
	enum class BS_SCRIPT_EXPORT(m:Utility,api:bsf,api:bed) CubemapSourceType
	{
		/** Source is a single image that will be replicated on all cubemap faces. */
		Single,

		/**
		 * Source is a list of 6 images, either sequentially next to each other or in a cross format. The system will
		 * automatically guess the layout and orientation based on the aspect ratio.
		 */
		Faces,

		/** Source is a single spherical panoramic image. */
		Spherical,

		/** Source is a single cylindrical panoramic image. */
		Cylindrical
	};

	/**	Type of scaling modes for GUI images. */
	enum class TextureScaleMode
	{
		StretchToFit, /**< Image will stretch non-uniformly in all dimensions in order to cover the assigned area fully. */
		ScaleToFit, /**< Image will scale uniformly until one dimension is aligned with the assigned area. Remaining dimension might have empty space. */
		CropToFit, /**< Image will scale uniformly until both dimensions are larger or aligned with the assigned area. Remaining dimension might be cropped. */
		RepeatToFit /**< Image will keep its original size, but will repeat in order to fill the assigned area. */
	};

	/** @} */
}
