//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentRenderTargets.h"

namespace bs
{
	SPtr<RenderWindow> DiligentRenderWindowManager::createImpl(RENDER_WINDOW_DESC& desc, UINT32 windowId, 
		const SPtr<RenderWindow>& parentWindow)
	{
		if(parentWindow != nullptr)
		{
			UINT64 hWnd;
			parentWindow->getCustomAttribute("WINDOW", &hWnd);
			desc.platformSpecific["parentWindowHandle"] = toString(hWnd);
		}

		// Create the window
		DiligentRenderWindow* renderWindow = new (bs_alloc<DiligentRenderWindow>()) DiligentRenderWindow(desc, windowId);
		return bs_core_ptr<DiligentRenderWindow>(renderWindow);
	}

	DiligentRenderWindow::DiligentRenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId)
		:RenderWindow(desc, windowId), mProperties(desc)
	{ }

	SPtr<ct::DiligentRenderWindow> DiligentRenderWindow::getCore() const
	{
		return std::static_pointer_cast<ct::DiligentRenderWindow>(mCoreSpecific);
	}

	void DiligentRenderWindow::syncProperties()
	{
		ScopedSpinLock lock(getCore()->mLock);
		mProperties = getCore()->mSyncedProperties;
	}

	SPtr<ct::CoreObject> DiligentRenderWindow::createCore() const
	{
		RENDER_WINDOW_DESC desc = mDesc;
		SPtr<ct::CoreObject> coreObj = bs_shared_ptr_new<ct::DiligentRenderWindow>(desc, mWindowId);
		coreObj->_setThisPtr(coreObj);

		return coreObj;
	}

	void DiligentRenderWindow::getCustomAttribute(const String& name, void* pData) const
	{
		if (name == "WINDOW")
		{
			UINT64 *pHwnd = (UINT64*)pData;
			*pHwnd = 0;
			return;
		}

		RenderWindow::getCustomAttribute(name, pData);
	}

	namespace ct
	{
		DiligentRenderWindow::DiligentRenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId)
			: RenderWindow(desc, windowId), mProperties(desc), mSyncedProperties(desc)
		{ }


		void DiligentRenderWindow::syncProperties()
		{
			ScopedSpinLock lock(mLock);
			mProperties = mSyncedProperties;
		}

		void DiligentRenderWindow::getCustomAttribute(const String& name, void* pData) const
		{
			if(name == "WINDOW")
			{
				UINT64 *pWnd = (UINT64*)pData;
				*pWnd = 0;
				return;
			}

			RenderWindow::getCustomAttribute(name, pData);
		}
	}
}