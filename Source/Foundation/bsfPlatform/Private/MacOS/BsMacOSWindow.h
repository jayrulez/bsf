//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Platform/BsPlatformPrerequisites.h"
#include "Platform/BsPlatformWindow.h"
#include "Math/BsVector2I.h"
#include <Math/BsRect2I.h>

#ifdef BS_COCOA_INTERNALS
#import <Cocoa/Cocoa.h>
#endif

#ifdef BS_COCOA_INTERNALS
@class BSWindowDelegate;
@class BSWindowListener;
@class BSView;
@class BSWindow;
@class CALayer;
#endif

namespace bs
{
	class CocoaDropTarget;

	/** @addtogroup Platform
	 *  @{
	 */

	/** @addtogroup Platform-Internal
	 *  @{
	 */

	/**	Descriptor used for creating a MacOS-specific native window. Extends PLATFORM_WINDOW_DESC with Cocoa-specific fields. */
	struct WINDOW_DESC : PLATFORM_WINDOW_DESC
	{
		INT32 x = -1; /**< Window origin on X axis (same as left in base class). */
		INT32 y = -1; /**< Window origin on Y axis (same as top in base class). */
		bool showDecorations = true; /**< Show window decorations (title bar, border, etc.). */
		bool floating = false; /**< Window floats on top of all other windows. */
		SPtr<PixelData> background; /**< Optional background image. */
	};

	/**
	 * Represents a Cocoa window. Note this class should only be used from the sim thread as Cocoa does not support
	 * event handling, windows or views outside of the main thread.
	 */
	class BS_PLATFORM_EXPORT CocoaWindow : public PlatformWindow
	{
	public:
#ifdef BS_COCOA_INTERNALS
		struct Pimpl
		{
			NSWindow* window = nil;
			BSView* view = nil;
			BSWindowListener* responder = nil;
			BSWindowDelegate* delegate = nil;
            CALayer* layer = nil;
			UINT32 numDropTargets = 0;
			bool isModal = false;
			NSUInteger style = 0;
			bool isFullscreen;
			NSRect windowedRect;
			NSModalSession modalSession = nil;
			void* userData = nullptr;
		};
#else
		struct Pimpl;
#endif

		CocoaWindow(const WINDOW_DESC& desc);
		~CocoaWindow();

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

		// MacOS-specific methods
		/** Returns the current area of the window, relative to the top-left origin of the screen. */
		Rect2I getArea() const;

		/** Switches from fullscreen to windowed mode. */
		void setWindowed();

		/** Switches from windowed to fullscreen mode. */
		void setFullscreen();

		/**
		 * @name Internal
		 * @{
		 */

		/**
		 * Destroys the window, cleaning up any resources and removing it from the display. No further methods should be
		 * called on this object after it has been destroyed. This called automatically in the destructor.
		 */
		void _destroy();

		/** Returns an identifier that unique identifies this window. */
		UINT32 _getWindowId() const { return mWindowId; }

		/**
		 * Sets a portion of the window in which the user can click and drag in order to move the window. This is needed
		 * when window has no title bar, yet you still want to allow the user to drag it by clicking on some specific area
		 * (e.g. a title bar you manually render).
		 *
		 * @param[in]	rects	Areas of the window (relative to the window origin in top-left corner) in which the drag
		 * 						operation in allowed.
		 */
		void _setDragZones(const Vector<Rect2I>& rects);

		/** Attaches non-specific user data that can later be retrieved through _getUserData(). */
		void _setUserData(void* data);

		/** Returns user data attached to the object when _setUserData was called. */
		void* _getUserData() const;

		/**
		 * Registers the window with the drag and drop manager and allows it to accept file drop operations. Each call
		 * to this method must eventually be followed with _unregisterForDragAndDrop.
		 */
		void _registerForDragAndDrop();

		/**
		 * Unregisters the window from the drag and drop manager. This will need to be called multiple times if
		 * _registerForDragAndDrop was called multiple times.
		 */
		void _unregisterForDragAndDrop();

		/** Lets the window know that the provided OpenGL context will be rendering to it. */
		void _registerGLContext(void* context);

		/** Assigns a CALayer to the windows' view, and enables layer backing on the view. */
		void _setLayer(void* layer);

		/** Returns the assigned CALayer to the window view. */
		void* _getLayer() const;

		/** Returns internal private data for use by friends. */
		Pimpl* _getPrivateData() const { return m; }

		/** @} */

	private:
		Pimpl* m;
		UINT32 mWindowId;
	};

	/** @} */
	/** @} */
}

