//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Platform/BsPlatformPrerequisites.h"
#include "Platform/BsPlatformWindow.h"
#include "Math/BsVector2I.h"
#include <X11/X.h>
#include <X11/Xutil.h>

namespace bs
{
	/** @addtogroup Platform
	 *  @{
	 */

	/** @addtogroup Platform-Internal
	 *  @{
	 */

	/**	Descriptor used for creating a Linux-specific native window. Extends PLATFORM_WINDOW_DESC with X11-specific fields. */
	struct WINDOW_DESC : PLATFORM_WINDOW_DESC
	{
		INT32 x = -1; /**< Window origin on X axis (same as left in base class). */
		INT32 y = -1; /**< Window origin on Y axis (same as top in base class). */
		UINT32 screen = 0; /**< X11 screen number. */
		bool showDecorations = true; /**< Show window decorations (title bar, border, etc.). */
		bool showOnTaskBar = true; /**< Show window icon on taskbar. */
		::Window parent = 0; /**< Parent window handle. */
		::Window external = 0; /**< External window handle if window was created externally. */
		XVisualInfo visualInfo; /**< X11 visual information. */
		SPtr<PixelData> background; /**< Optional background image. */
	};

	/**
	 * Represents a X11 window. Note that all accesses (including creation and destruction) to objects of this class must
	 * be locked by the main X11 lock accessible through LinuxPlatform.
	 */
	class BS_PLATFORM_EXPORT LinuxWindow : public PlatformWindow
	{
	public:
		LinuxWindow(const WINDOW_DESC& desc);
		~LinuxWindow();

		// PlatformWindow interface implementation
		/**	Returns position of the left-most border of the window, relative to the screen. */
		INT32 getLeft() const override;

		/**	Returns position of the top-most border of the window, relative to the screen. */
		INT32 getTop() const override;

		/**	Returns width of the window in pixels. */
		UINT32 getWidth() const override;

		/**	Returns height of the window in pixels. */
		UINT32 getHeight() const override;

		/** Hides the window. */
		void hide() override;

		/** Shows (unhides) the window. */
		void show() override;

		/**	Minimizes the window. */
		void minimize() override;

		/**	Maximizes the window over the entire current screen. */
		void maximize() override;

		/**	Restores the window to original position and size if it is minimized or maximized. */
		void restore() override;

		/**	Change the size of the window. */
		void resize(UINT32 width, UINT32 height) override;

		/**	Reposition the window. */
		void move(INT32 left, INT32 top) override;

		/**	Converts screen position into window local position. */
		Vector2I screenToWindowPos(const Vector2I& screenPos) const override;

		/**	Converts window local position to screen position. */
		Vector2I windowToScreenPos(const Vector2I& windowPos) const override;

		/**	Returns the platform-specific window handle as a void pointer. */
		void* getNativeHandle() const override;

		// Linux-specific methods
		/** Sets the icon to display for the window. */
		void setIcon(const PixelData& icon);

		/**
		 * @name Internal
		 * @{
		 */

		/**
		 * Destroys the window, cleaning up any resources and removing it from the display. No further methods should be
		 * called on this object after it has been destroyed.
		 */
		void _destroy();

		/**
		 * Sets a portion of the window in which the user can click and drag in order to move the window. This is needed
		 * when window has no title bar, yet you still want to allow the user to drag it by clicking on some specific area
		 * (e.g. a title bar you manually render).
		 *
		 * @param[in]	rects	Areas of the window (relative to the window origin in top-left corner) in which the drag
		 * 						operation in allowed.
		 */
		void _setDragZones(const Vector<Rect2I>& rects);

		/**
		 * Notifies the window that user has started dragging the window using a custom drag zone. Provided parameter is the
		 * event that started the drag.
		 */
		void _dragStart(const XButtonEvent& event);

		/** Notifies the window the user has stopped the window drag operation. */
		void _dragEnd();

		/** Returns the internal X11 window handle. */
		::Window _getXWindow() const;

		/** Toggles between fullscreen and windowed mode. */
		void _setFullscreen(bool fullscreen);

		/** Attaches non-specific user data that can later be retrieved through _getUserData(). */
		void _setUserData(void* data);

		/** Returns user data attached to the object when _setUserData was called. */
		void* _getUserData() const;

		/** @} */

	private:
		/** Checks if the window is currently maximized. */
		bool isMaximized() const;

		/** Checks if the window is currently minimized (iconified). */
		bool isMinimized();

		/**
		 * Maximizes a window if @p enable is true. If false restores the window to size/position before maximization
		 * occurred.
		 */
		void maximize(bool enable);

		/**
		 * Minimizes a window if @p enable is true. If false restores the window to size/position before minimization
		 * occurred.
		 */
		void minimize(bool enable);

		/** Shows or hides the window icon from the taskbar. */
		void showOnTaskbar(bool enable);

		/**
		 * Shows or hides window decorations. Decorations include window title bar, border and similar. Essentially anything
		 * not part of the main rendering area.
		 */
		void setShowDecorations(bool show);

		/**
		 * Switches the window between modal and normal mode. Modal window prevents input to their parent window until
		 * it is dismissed.
		 */
		void setIsModal(bool modal);

		struct Pimpl;
		Pimpl* m;
	};

	/** @} */
	/** @} */
}
