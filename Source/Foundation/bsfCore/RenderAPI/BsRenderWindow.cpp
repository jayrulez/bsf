//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsRenderWindow.h"
#include "Managers/BsRenderWindowManager.h"
#include "RenderAPI/BsViewport.h"
#include "Platform/BsPlatform.h"
#include "Private/RTTI/BsRenderTargetRTTI.h"

namespace bs
{
	RenderWindowProperties::RenderWindowProperties(const RENDER_WINDOW_DESC& desc)
	{
		width = desc.videoMode.width;
		height = desc.videoMode.height;
		hwGamma = desc.gamma;
		vsync = desc.vsync;
		vsyncInterval = desc.vsyncInterval;
		multisampleCount = desc.multisampleCount;
		left = desc.left;
		top = desc.top;
		isFullScreen = desc.fullscreen;
		isHidden = desc.hidden;
		isModal = desc.modal;
		isWindow = true;
		requiresTextureFlipping = false;
	}

	void RenderWindow::destroy()
	{
		RenderWindowManager::instance().notifyWindowDestroyed(this);

		RenderTarget::destroy();
	}

	RenderWindow::RenderWindow(const RENDER_WINDOW_DESC& desc, UINT32 windowId)
		: mDesc(desc), mWindowId(windowId)
	{
		RenderWindowManager::instance().windowCreated(this);
	}

	RenderWindow::~RenderWindow()
	{
		RenderWindowManager::instance().windowDestroyed(this);
	}

	SPtr<RenderWindow> RenderWindow::create(RENDER_WINDOW_DESC& desc, SPtr<RenderWindow> parentWindow)
	{
		return RenderWindowManager::instance().create(desc, parentWindow);
	}

	void RenderWindow::setHidden(bool hidden)
	{
		RenderWindowProperties& props = const_cast<RenderWindowProperties&>(getProperties());

		props.isHidden = hidden;
		{
			ScopedSpinLock lock(mLock);
		}
	}

	void RenderWindow::setActive(bool state)
	{
	}

	void RenderWindow::hide()
	{
		setHidden(true);

		getMutableProperties().isHidden = true;
	}

	void RenderWindow::show()
	{
		setHidden(false);

		getMutableProperties().isHidden = false;

	}

	RenderWindowProperties& RenderWindow::getMutableProperties()
	{
		return const_cast<RenderWindowProperties&>(getProperties());
	}

	const RenderWindowProperties& RenderWindow::getProperties() const
	{
		return static_cast<const RenderWindowProperties&>(getPropertiesInternal());
	}

	void RenderWindow::_notifyWindowEvent(WindowEventType type)
	{
		RenderWindowProperties& props = const_cast<RenderWindowProperties&>(getProperties());

		switch (type)
		{
		case WindowEventType::Resized:
		{
			_windowMovedOrResized();

			{
				ScopedSpinLock lock(mLock);
			}

			bs::RenderWindowManager::instance().notifyMovedOrResized(this);

			break;
		}
		case WindowEventType::Moved:
		{
			_windowMovedOrResized();

			{
				ScopedSpinLock lock(mLock);
			}

			bs::RenderWindowManager::instance().notifyMovedOrResized(this);

			break;
		}
		case WindowEventType::FocusReceived:
		{
			{
				ScopedSpinLock lock(mLock);
			}

			props.hasFocus = true;

			bs::RenderWindowManager::instance().notifyFocusReceived(this);
			break;
		}
		case WindowEventType::FocusLost:
		{
			{
				ScopedSpinLock lock(mLock);
			}

			props.hasFocus = false;

			bs::RenderWindowManager::instance().notifyFocusLost(this);
			break;
		}
		case WindowEventType::Minimized:
		{
			{
				ScopedSpinLock lock(mLock);
			}

			props.isMaximized = false;

			break;
		}
		case WindowEventType::Maximized:
		{
			{
				ScopedSpinLock lock(mLock);
			}

			props.isMaximized = true;

			break;
		}
		case WindowEventType::Restored:
		{
			{
				ScopedSpinLock lock(mLock);
			}

			props.isMaximized = false;

			break;
		}
		case WindowEventType::MouseLeft:
		{
			bs::RenderWindowManager::instance().notifyMouseLeft(this);
			break;
		}
		case WindowEventType::CloseRequested:
		{
			bs::RenderWindowManager::instance().notifyCloseRequested(this);
			break;
		}
		}
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* RenderWindow::getRTTIStatic()
	{
		return RenderWindowRTTI::instance();
	}

	RTTITypeBase* RenderWindow::getRTTI() const
	{
		return RenderWindow::getRTTIStatic();
	}
}
