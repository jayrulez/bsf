//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Resources/BsScriptCodeResource.h"
#include "Resources/BsResources.h"
#include "Private/RTTI/BsScriptCodeResourceRTTI.h"

namespace bs
{
	ScriptCodeResource::ScriptCodeResource(const WString& data, bool editorScript)
		:Resource(false), mString(data), mEditorScript(editorScript)
	{

	}

	ScriptCodeResourceHandle ScriptCodeResource::create(const WString& data, bool editorScript)
	{
		return static_resource_cast<ScriptCodeResource>(gResources()._createResourceHandle(_createPtr(data, editorScript)));
	}

	SPtr<ScriptCodeResource> ScriptCodeResource::_createPtr(const WString& data, bool editorScript)
	{
		SPtr<ScriptCodeResource> scriptCodePtr = bs_core_ptr<ScriptCodeResource>(
			new (bs_alloc<ScriptCodeResource>()) ScriptCodeResource(data, editorScript));
		scriptCodePtr->_setThisPtr(scriptCodePtr);
		scriptCodePtr->initialize();

		return scriptCodePtr;
	}

	RTTITypeBase* ScriptCodeResource::getRTTIStatic()
	{
		return ScriptCodeResourceRTTI::instance();
	}

	RTTITypeBase* ScriptCodeResource::getRTTI() const
	{
		return ScriptCodeResource::getRTTIStatic();
	}
}
