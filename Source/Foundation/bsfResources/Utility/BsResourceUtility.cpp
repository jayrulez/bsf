//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsResourceUtility.h"
#include "Reflection/BsRTTIType.h"
#include "Allocators/BsFrameAlloc.h"

namespace bs
{
	/**
	 * Checks if the specified type (or any of its derived classes) have any IReflectable pointer or value types as
	 * their fields.
	 */
	static bool hasReflectableChildren(RTTITypeBase* type)
	{
		UINT32 numFields = type->getNumFields();
		for (UINT32 i = 0; i < numFields; i++)
		{
			RTTIField* field = type->getField(i);
			if (field->schema.type == SerializableFT_Reflectable || field->schema.type == SerializableFT_ReflectablePtr)
				return true;
		}

		const Vector<RTTITypeBase*>& derivedClasses = type->getDerivedClasses();
		for (auto& derivedClass : derivedClasses)
		{
			numFields = derivedClass->getNumFields();
			for (UINT32 i = 0; i < numFields; i++)
			{
				RTTIField* field = derivedClass->getField(i);
				if (field->schema.type == SerializableFT_Reflectable || field->schema.type == SerializableFT_ReflectablePtr)
					return true;
			}
		}

		return false;
	}

	static void findResourceDependenciesInternal(IReflectable& obj, FrameAlloc& alloc, bool recursive,
		Map<UUID, ResourceDependency>& dependencies)
	{
		RTTITypeBase* rtti = obj.getRTTI();
		do {
			RTTITypeBase* rttiInstance = rtti->_clone(alloc);
			rttiInstance->onSerializationStarted(&obj, nullptr);

			const UINT32 numFields = rtti->getNumFields();
			for (UINT32 i = 0; i < numFields; i++)
			{
				RTTIField* field = rtti->getField(i);
				if (field->schema.info.flags.isSet(RTTIFieldFlag::SkipInReferenceSearch))
					continue;

				if (field->schema.type == SerializableFT_Reflectable)
				{
					auto reflectableField = static_cast<RTTIReflectableFieldBase*>(field);

					if (reflectableField->getType()->getRTTIId() == TID_ResourceHandle)
					{
						if (reflectableField->schema.isArray)
						{
							const UINT32 numElements = reflectableField->getArraySize(rttiInstance, &obj);
							for (UINT32 j = 0; j < numElements; j++)
							{
								HResource resource = (HResource&)reflectableField->getArrayValue(rttiInstance, &obj, j);
								if (!resource.getUUID().empty())
								{
									ResourceDependency& dependency = dependencies[resource.getUUID()];
									dependency.resource = resource;
									dependency.numReferences++;
								}
							}
						}
						else
						{
							HResource resource = (HResource&)reflectableField->getValue(rttiInstance, &obj);
							if (!resource.getUUID().empty())
							{
								ResourceDependency& dependency = dependencies[resource.getUUID()];
								dependency.resource = resource;
								dependency.numReferences++;
							}
						}
					}
					else if (recursive)
					{
						// Optimization, no need to retrieve its value and go deeper if it has no
						// reflectable children that may hold the reference.
						if (hasReflectableChildren(reflectableField->getType()))
						{
							if (reflectableField->schema.isArray)
							{
								const UINT32 numElements = reflectableField->getArraySize(rttiInstance, &obj);
								for (UINT32 j = 0; j < numElements; j++)
								{
									IReflectable& childObj = reflectableField->getArrayValue(rttiInstance, &obj, j);
									findResourceDependenciesInternal(childObj, alloc, true, dependencies);
								}
							}
							else
							{
								IReflectable& childObj = reflectableField->getValue(rttiInstance, &obj);
								findResourceDependenciesInternal(childObj, alloc, true, dependencies);
							}
						}
					}
				}
				else if (field->schema.type == SerializableFT_ReflectablePtr && recursive)
				{
					auto reflectablePtrField = static_cast<RTTIReflectablePtrFieldBase*>(field);

					// Optimization, no need to retrieve its value and go deeper if it has no
					// reflectable children that may hold the reference.
					if (hasReflectableChildren(reflectablePtrField->getType()))
					{
						if (reflectablePtrField->schema.isArray)
						{
							const UINT32 numElements = reflectablePtrField->getArraySize(rttiInstance, &obj);
							for (UINT32 j = 0; j < numElements; j++)
							{
								const SPtr<IReflectable>& childObj =
									reflectablePtrField->getArrayValue(rttiInstance, &obj, j);

								if (childObj != nullptr)
									findResourceDependenciesInternal(*childObj, alloc, true, dependencies);
							}
						}
						else
						{
							const SPtr<IReflectable>& childObj = reflectablePtrField->getValue(rttiInstance, &obj);

							if (childObj != nullptr)
								findResourceDependenciesInternal(*childObj, alloc, true, dependencies);
						}
					}
				}
			}

			rttiInstance->onSerializationEnded(&obj, nullptr);
			alloc.destruct(rttiInstance);

			rtti = rtti->getBaseClass();
		} while(rtti != nullptr);
	}

	Vector<ResourceDependency> ResourceUtility::findResourceDependencies(IReflectable& obj, bool recursive)
	{
		gFrameAlloc().markFrame();

		Map<UUID, ResourceDependency> dependencies;
		findResourceDependenciesInternal(obj, gFrameAlloc(), recursive, dependencies);

		gFrameAlloc().clear();

		Vector<ResourceDependency> dependencyList(dependencies.size());
		UINT32 i = 0;
		for (auto& entry : dependencies)
		{
			dependencyList[i] = entry.second;
			i++;
		}

		return dependencyList;
	}
}
