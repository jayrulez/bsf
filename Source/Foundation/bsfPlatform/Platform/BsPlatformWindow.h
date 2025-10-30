//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Platform/BsPlatformPrerequisites.h"
#include "Math/BsVector2I.h"

namespace bs
{
	/** @addtogroup Platform
	 *  @{
	 */

	/** @addtogroup Platform-Internal
	 *  @{
	 */

	/**
	 * Common descriptor used for creating a platform-specific native window. Platform-specific implementations may extend
	 * this with additional fields.
	 */
	struct BS_PLATFORM_EXPORT PLATFORM_WINDOW_DESC
	{
		PLATFORM_WINDOW_DESC() = default;

		String title; /**< Title of the window. */
		UINT32 width = 0; /**< Width of the window in pixels. */
		UINT32 height = 0; /**< Height of the window in pixels. */
		INT32 left = -1; /**< Window origin on X axis in pixels. -1 == screen center. */
		INT32 top = -1; /**< Window origin on Y axis in pixels. -1 == screen center. */
		bool fullscreen = false; /**< Should the window be opened in fullscreen mode. */
		bool hidden = false; /**< Should the window be hidden initially. */
		bool showTitleBar = true; /**< Determines if the title-bar should be shown or not. */
		bool showBorder = true; /**< Determines if the window border should be shown or not. */
		bool allowResize = true; /**< Determines if the user can resize the window by dragging on the window edges. */
		bool modal = false; /**< When a modal window is open all other windows will be locked until modal window is closed. */
	};

	/**
	 * Abstract base class for platform-specific window implementations. Provides common interface for window management
	 * operations like resize, move, minimize, maximize, etc.
	 */
	class BS_PLATFORM_EXPORT PlatformWindow
	{
	public:
		virtual ~PlatformWindow() = default;

		/**	Returns position of the left-most border of the window, relative to the screen. */
		virtual INT32 getLeft() const = 0;

		/**	Returns position of the top-most border of the window, relative to the screen. */
		virtual INT32 getTop() const = 0;

		/**	Returns width of the window in pixels. */
		virtual UINT32 getWidth() const = 0;

		/**	Returns height of the window in pixels. */
		virtual UINT32 getHeight() const = 0;

		/**	Hides the window. */
		virtual void hide() = 0;

		/**	Shows (unhides) the window. */
		virtual void show() = 0;

		/**	Minimizes the window to the taskbar. */
		virtual void minimize() = 0;

		/**	Maximizes the window over the entire current screen. */
		virtual void maximize() = 0;

		/**	Restores the window to original position and size if it is minimized or maximized. */
		virtual void restore() = 0;

		/**	Change the size of the window. */
		virtual void resize(UINT32 width, UINT32 height) = 0;

		/**	Reposition the window. */
		virtual void move(INT32 left, INT32 top) = 0;

		/**	Converts screen position into window local position. */
		virtual Vector2I screenToWindowPos(const Vector2I& screenPos) const = 0;

		/**	Converts window local position to screen position. */
		virtual Vector2I windowToScreenPos(const Vector2I& windowPos) const = 0;

		/**
		 * Returns a platform-specific window handle. The type depends on the platform:
		 * - Windows: HWND
		 * - Linux: ::Window (X11)
		 * - macOS: NSWindow*
		 */
		virtual void* getNativeHandle() const = 0;
	};

	/** @} */
	/** @} */
}
