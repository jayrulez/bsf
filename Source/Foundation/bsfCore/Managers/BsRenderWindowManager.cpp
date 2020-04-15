//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Managers/BsRenderWindowManager.h"
#include "Platform/BsPlatform.h"
#include "BsCoreApplication.h"

using namespace std::placeholders;

namespace bs
{
	RenderWindowManager::RenderWindowManager()
	{
		mNextWindowId = 0;
	}

	SPtr<RenderWindow> RenderWindowManager::create(RENDER_WINDOW_DESC& desc, SPtr<RenderWindow> parentWindow)
	{
		UINT32 id = RenderWindowManager::instance().mNextWindowId.fetch_add(1, std::memory_order_relaxed);

		SPtr<RenderWindow> renderWindow = createImpl(desc, id, parentWindow);
		renderWindow->_setThisPtr(renderWindow);
		
		{
			Lock lock(mWindowMutex);

			mWindows[renderWindow->mWindowId] = renderWindow.get();
		}

		if (renderWindow->getProperties().isModal)
			mModalWindowStack.push_back(renderWindow.get());

		renderWindow->initialize();
		
		return renderWindow;
	}

	void RenderWindowManager::windowCreated(RenderWindow* window)
	{
		Lock lock(mWindowMutex);

		mCreatedWindows.push_back(window);
	}

	void RenderWindowManager::windowDestroyed(RenderWindow* window)
	{
		{
			Lock lock(mWindowMutex);

			auto iterFind = std::find(begin(mCreatedWindows), end(mCreatedWindows), window);

			if (iterFind == mCreatedWindows.end())
				BS_EXCEPT(InternalErrorException, "Trying to destroy a window that is not in the created windows list.");

			mCreatedWindows.erase(iterFind);
		}
	}

	void RenderWindowManager::notifyWindowDestroyed(RenderWindow* window)
	{
		{
			Lock lock(mWindowMutex);

			auto iterFind = std::find(begin(mMovedOrResizedWindows), end(mMovedOrResizedWindows), window);

			if(iterFind != mMovedOrResizedWindows.end())
				mMovedOrResizedWindows.erase(iterFind);

			if (mNewWindowInFocus == window)
				mNewWindowInFocus = nullptr;

			mWindows.erase(window->mWindowId);
		}

		{
			auto iterFind = std::find(begin(mModalWindowStack), end(mModalWindowStack), window);
			if(iterFind != mModalWindowStack.end())
				mModalWindowStack.erase(iterFind);
		}
	}

	void RenderWindowManager::notifyFocusReceived(RenderWindow* window)
	{
		Lock lock(mWindowMutex);

		mNewWindowInFocus = window;
	}

	void RenderWindowManager::notifyFocusLost(RenderWindow* coreWindow)
	{
		Lock lock(mWindowMutex);

		mNewWindowInFocus = nullptr;
	}

	void RenderWindowManager::notifyMovedOrResized(RenderWindow* window)
	{
		Lock lock(mWindowMutex);

		if (window == nullptr)
			return;

		auto iterFind = std::find(begin(mMovedOrResizedWindows), end(mMovedOrResizedWindows), window);
		if (iterFind == end(mMovedOrResizedWindows))
			mMovedOrResizedWindows.push_back(window);
	}

	void RenderWindowManager::notifyMouseLeft(RenderWindow* window)
	{
		Lock lock(mWindowMutex);

		auto iterFind = std::find(begin(mMouseLeftWindows), end(mMouseLeftWindows), window);

		if (iterFind == end(mMouseLeftWindows))
			mMouseLeftWindows.push_back(window);
	}

	void RenderWindowManager::notifyCloseRequested(RenderWindow* window)
	{
		Lock lock(mWindowMutex);

		auto iterFind = std::find(begin(mCloseRequestedWindows), end(mCloseRequestedWindows), window);

		if (iterFind == end(mCloseRequestedWindows))
			mCloseRequestedWindows.push_back(window);
	}

	void RenderWindowManager::_update()
	{
		RenderWindow* newWinInFocus = nullptr;
		Vector<RenderWindow*> movedOrResizedWindows;
		Vector<RenderWindow*> mouseLeftWindows;
		Vector<RenderWindow*> closeRequestedWindows;

		{
			Lock lock(mWindowMutex);
			newWinInFocus = mNewWindowInFocus;

			std::swap(mMovedOrResizedWindows, movedOrResizedWindows);
			std::swap(mMouseLeftWindows, mouseLeftWindows);

			std::swap(mCloseRequestedWindows, closeRequestedWindows);
		}

		if(mWindowInFocus != newWinInFocus)
		{
			if(mWindowInFocus != nullptr)
				onFocusLost(*mWindowInFocus);

			if(newWinInFocus != nullptr)
				onFocusGained(*newWinInFocus);

			mWindowInFocus = newWinInFocus;
		}

		for (auto& window : movedOrResizedWindows)
			window->onResized();

		if (!onMouseLeftWindow.empty())
		{
			for (auto& window : mouseLeftWindows)
				onMouseLeftWindow(*window);
		}

		SPtr<RenderWindow> primaryWindow = gCoreApplication().getPrimaryWindow();
		for(auto& entry : closeRequestedWindows)
		{
			// Default behaviour for primary window is to quit the app on close
			if(entry == primaryWindow.get() && entry->onCloseRequested.empty())
			{
				gCoreApplication().quitRequested();
				continue;
			}

			entry->onCloseRequested();
		}
	}

	Vector<RenderWindow*> RenderWindowManager::getRenderWindows() const
	{
		Lock lock(mWindowMutex);

		Vector<RenderWindow*> windows;
		for (auto& windowPair : mWindows)
			windows.push_back(windowPair.second);

		return windows;
	}

	RenderWindow* RenderWindowManager::getTopMostModal() const
	{
		if (mModalWindowStack.empty())
			return nullptr;
		
		return mModalWindowStack.back();
	}
}
