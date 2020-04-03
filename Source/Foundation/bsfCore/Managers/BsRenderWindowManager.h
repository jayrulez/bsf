//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Utility/BsModule.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Utility/BsEvent.h"

namespace bs
{
	/** @addtogroup RenderAPI-Internal
	 *  @{
	 */

	/** Handles creation and internal updates relating to render windows. */
	class BS_CORE_EXPORT RenderWindowManager : public Module<RenderWindowManager>
	{
	public:
		RenderWindowManager();
		~RenderWindowManager() = default;

		/**
		 * Creates a new render window using the specified options. Optionally makes the created window a child of another
		 * window.
		 */
		SPtr<RenderWindow> create(RENDER_WINDOW_DESC& desc, SPtr<RenderWindow> parentWindow);

		/** Called once per frame. Dispatches events. */
		void _update();

		/**	Returns a list of all open render windows. */
		Vector<RenderWindow*> getRenderWindows() const;

		/** Called by the core thread when window is destroyed. */
		void notifyWindowDestroyed(RenderWindow* window);

		/**	Called by the core thread when window receives focus. */
		void notifyFocusReceived(RenderWindow* window);

		/**	Called by the core thread when window loses focus. */
		void notifyFocusLost(RenderWindow* window);

		/**	Called by the core thread when window is moved or resized. */
		void notifyMovedOrResized(RenderWindow* window);

		/**	Called by the core thread when mouse leaves a window. */
		void notifyMouseLeft(RenderWindow* window);

		/** Called by the core thread when the user requests for the window to close. */
		void notifyCloseRequested(RenderWindow* coreWindow);

		/** Returns the window that is currently the top-most modal window. Returns null if no modal windows are active. */
		RenderWindow* getTopMostModal() const;

		/** Event that is triggered when a window gains focus. */
		Event<void(RenderWindow&)> onFocusGained;

		/**	Event that is triggered when a window loses focus. */
		Event<void(RenderWindow&)> onFocusLost;

		/**	Event that is triggered when mouse leaves a window. */
		Event<void(RenderWindow&)> onMouseLeftWindow;
	protected:
		friend class RenderWindow;

		/**	Called whenever a window is created. */
		void windowCreated(RenderWindow* window);

		/**	Called by the core thread when window is destroyed. */
		void windowDestroyed(RenderWindow* window);

		/** @copydoc create */
		virtual SPtr<RenderWindow> createImpl(RENDER_WINDOW_DESC& desc, UINT32 windowId, const SPtr<RenderWindow>& parentWindow) = 0;

	protected:
		friend class RenderWindow;

		mutable Mutex mWindowMutex;
		Map<UINT32, RenderWindow*> mWindows;
		Vector<RenderWindow*> mModalWindowStack;

		Vector<RenderWindow*> mCreatedWindows;
		std::atomic_uint mNextWindowId;

		RenderWindow* mWindowInFocus = nullptr;
		RenderWindow* mNewWindowInFocus = nullptr;
		Vector<RenderWindow*> mMovedOrResizedWindows;
		Vector<RenderWindow*> mMouseLeftWindows;
		Vector<RenderWindow*> mCloseRequestedWindows;
		UnorderedSet<RenderWindow*> mDirtyProperties;
	};

	/** @} */
}
