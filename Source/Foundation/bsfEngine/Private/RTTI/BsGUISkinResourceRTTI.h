//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "GUI/BsGUISkinResource.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class BS_EXPORT GUISkinEntry : public IReflectable
	{
	public:
		GUISkinEntry() = default;
		GUISkinEntry(const String& name, const GUIElementStyle& style);

		String name;
		GUIElementStyle style;

	public:
		friend class GUISkinEntryRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;
	};

	class BS_EXPORT GUISkinEntryRTTI : public RTTIType <GUISkinEntry, IReflectable, GUISkinEntryRTTI>
	{
	private:
		String& getName(GUISkinEntry* obj) { return obj->name; }
		void setName(GUISkinEntry* obj, String& val) { obj->name = val; }

		GUIElementStyle& getStyle(GUISkinEntry* obj) { return obj->style; }
		void setStyle(GUISkinEntry* obj, GUIElementStyle& val) { obj->style = val; }

	public:
		GUISkinEntryRTTI()
		{
			addPlainField("name", 0, &GUISkinEntryRTTI::getName, &GUISkinEntryRTTI::setName);
			addReflectableField("style", 1, &GUISkinEntryRTTI::getStyle, &GUISkinEntryRTTI::setStyle);
		}

		const String& getRTTIName() override
		{
			static String name = "GUISkinEntry";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_GUISkinEntry;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<GUISkinEntry>();
		}
	};

	class BS_EXPORT GUISkinResourceRTTI : public RTTIType <GUISkinResource, Resource, GUISkinResourceRTTI>
	{
	private:
		GUISkinEntry& getStyle(GUISkinResource* obj, UINT32 idx)
		{
			return mStyles[idx];
		}

		void setStyle(GUISkinResource* obj, UINT32 idx, GUISkinEntry& val) { obj->mStyles[val.name] = val.style; }
		UINT32 getStyleArraySize(GUISkinResource* obj) { return (UINT32)obj->mStyles.size(); }
		void setStyleArraySize(GUISkinResource* obj, UINT32 size) { /* Do nothing */ }

	public:
		GUISkinResourceRTTI()
		{
			addReflectableArrayField("mStyles", 0, &GUISkinResourceRTTI::getStyle, &GUISkinResourceRTTI::getStyleArraySize,
				&GUISkinResourceRTTI::setStyle, &GUISkinResourceRTTI::setStyleArraySize);
		}

		void onSerializationStarted(IReflectable* obj, SerializationContext* context) override
		{
			GUISkinResource* skin = static_cast<GUISkinResource*>(obj);

			for (auto& style : skin->mStyles)
				mStyles.push_back(GUISkinEntry(style.first, style.second));
		}

		const String& getRTTIName() override
		{
			static String name = "GUISkinResource";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_GUISkin;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return GUISkinResource::_createPtr();
		}

	private:
		Vector<GUISkinEntry> mStyles;
	};

	/** @} */
	/** @endcond */
}
