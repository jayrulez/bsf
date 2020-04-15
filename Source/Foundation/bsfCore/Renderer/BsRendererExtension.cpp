//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/BsRendererExtension.h"
#include "Renderer/BsRendererManager.h"
#include "Renderer/BsRenderer.h"

namespace bs
{
	void RendererExtension::_initializer(RendererExtension* obj, const Any& data)
	{
		RendererManager::instance().getActive()->addPlugin(obj);
		obj->initialize(data);
	}

	void RendererExtension::_deleter(RendererExtension* obj)
	{
		RendererManager::instance().getActive()->removePlugin(obj);

		obj->destroy();
		obj->~RendererExtension();

		bs_free(obj);
	}
}
