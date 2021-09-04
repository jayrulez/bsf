//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUISkinResource.h"
#include "GUI/BsGUIElementStyle.h"
#include "Debug/BsDebug.h"
#include "Resources/BsResources.h"
#include "Private/RTTI/BsGUISkinResourceRTTI.h"

namespace bs
{
	GUIElementStyle GUISkinResource::DefaultStyle;

	GUISkinResource::GUISkinResource()
		:Resource(false)
	{ }

	GUISkinResource::GUISkinResource(const GUISkinResource& skin)
		:Resource(false)
	{ }

	bool GUISkinResource::hasStyle(const String& name) const
	{
		auto iterFind = mStyles.find(name);

		if (iterFind != mStyles.end())
			return true;

		return false;
	}

	const GUIElementStyle* GUISkinResource::getStyle(const String& guiElemType) const
	{
		auto iterFind = mStyles.find(guiElemType);

		if(iterFind != mStyles.end())
			return &iterFind->second;

		BS_LOG(Warning, GUI, "Cannot find GUI style with name: {0}. Returning default style.", guiElemType);

		return &DefaultStyle;
	}

	void GUISkinResource::setStyle(const String& guiElemType, const GUIElementStyle& style)
	{
		mStyles[guiElemType] = style;
	}

	void GUISkinResource::removeStyle(const String& guiElemType)
	{
		mStyles.erase(guiElemType);
	}

	Vector<String> GUISkinResource::getStyleNames() const
	{
		Vector<String> output;
		for (auto& pair : mStyles)
			output.push_back(pair.first);

		return output;
	}

	GUISkinResourceHandle GUISkinResource::create()
	{
		SPtr<GUISkinResource> newSkin = _createPtr();

		return static_resource_cast<GUISkinResource>(gResources()._createResourceHandle(newSkin));
	}

	SPtr<GUISkinResource> GUISkinResource::_createPtr()
	{
		SPtr<GUISkinResource> newSkin = bs_core_ptr<GUISkinResource>(new (bs_alloc<GUISkinResource>()) GUISkinResource());
		newSkin->_setThisPtr(newSkin);
		newSkin->initialize();

		return newSkin;
	}

	RTTITypeBase* GUISkinResource::getRTTIStatic()
	{
		return GUISkinResourceRTTI::instance();
	}

	RTTITypeBase* GUISkinResource::getRTTI() const
	{
		return GUISkinResource::getRTTIStatic();
	}
}
