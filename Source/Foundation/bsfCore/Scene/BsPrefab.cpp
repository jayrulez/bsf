//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Scene/BsPrefab.h"
#include "Private/RTTI/BsPrefabRTTI.h"
#include "Resources/BsResources.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsPrefabUtility.h"
#include "BsCoreApplication.h"

namespace bs
{
	PrefabResource::PrefabResource()
		:Resource(false)
	{
		
	}

	PrefabResource::~PrefabResource()
	{
		if (mRoot != nullptr)
			mRoot->destroy(true);
	}

	PrefabResourceHandle PrefabResource::create(const HSceneObject& sceneObject, bool isScene)
	{
		SPtr<PrefabResource> newPrefab = createEmpty();
		newPrefab->mIsScene = isScene;

		PrefabUtility::clearPrefabIds(sceneObject, true, false);
		newPrefab->initialize(sceneObject);

		PrefabResourceHandle handle = static_resource_cast<PrefabResource>(gResources()._createResourceHandle(newPrefab));
		newPrefab->mUUID = handle.getUUID();
		sceneObject->mPrefabLinkUUID = newPrefab->mUUID;
		newPrefab->_getRoot()->mPrefabLinkUUID = newPrefab->mUUID;

		return handle;
	}

	SPtr<PrefabResource> PrefabResource::createEmpty()
	{
		SPtr<PrefabResource> newPrefab = bs_core_ptr<PrefabResource>(new (bs_alloc<PrefabResource>()) PrefabResource());
		newPrefab->_setThisPtr(newPrefab);

		return newPrefab;
	}

	void PrefabResource::initialize(const HSceneObject& sceneObject)
	{
		sceneObject->mPrefabDiff = nullptr;
		PrefabUtility::generatePrefabIds(sceneObject);

		// If there are any child prefab instances, make sure to update their diffs so they are saved with this prefab
		Stack<HSceneObject> todo;
		todo.push(sceneObject);

		while (!todo.empty())
		{
			HSceneObject current = todo.top();
			todo.pop();

			UINT32 childCount = current->getNumChildren();
			for (UINT32 i = 0; i < childCount; i++)
			{
				HSceneObject child = current->getChild(i);

				if (!child->mPrefabLinkUUID.empty())
					PrefabUtility::recordPrefabDiff(child);
				else
					todo.push(child);
			}
		}

		// Clone the hierarchy for internal storage
		if (mRoot != nullptr)
			mRoot->destroy(true);

		mRoot = sceneObject->clone(false, true);
		mRoot->mParent = nullptr;
		mRoot->mLinkId = -1;

		// Remove objects with "dont save" flag
		todo.push(mRoot);

		while (!todo.empty())
		{
			HSceneObject current = todo.top();
			todo.pop();

			if (current->hasFlag(SOF_DontSave))
				current->destroy();
			else
			{
				UINT32 numChildren = current->getNumChildren();
				for (UINT32 i = 0; i < numChildren; i++)
					todo.push(current->getChild(i));
			}
		}
	}

	void PrefabResource::update(const HSceneObject& sceneObject)
	{
		initialize(sceneObject);
		sceneObject->mPrefabLinkUUID = mUUID;
		mRoot->mPrefabLinkUUID = mUUID;

		mHash++;
	}

	void PrefabResource::_updateChildInstances() const
	{
		Stack<HSceneObject> todo;
		todo.push(mRoot);

		while (!todo.empty())
		{
			HSceneObject current = todo.top();
			todo.pop();

			UINT32 childCount = current->getNumChildren();
			for (UINT32 i = 0; i < childCount; i++)
			{
				HSceneObject child = current->getChild(i);

				if (!child->mPrefabLinkUUID.empty())
					PrefabUtility::updateFromPrefab(child);
				else
					todo.push(child);
			}
		}
	}

	HSceneObject PrefabResource::_instantiate(bool preserveUUIDs) const
	{
		if (mRoot == nullptr)
			return HSceneObject();

#if BS_IS_BANSHEE3D
		if (gCoreApplication().isEditor())
		{
			// Update any child prefab instances in case their prefabs changed
			_updateChildInstances();
		}
#endif

		HSceneObject clone = _clone(preserveUUIDs);
		clone->_instantiate();
		
		return clone;
	}

	HSceneObject PrefabResource::_clone(bool preserveUUIDs) const
	{
		if (mRoot == nullptr)
			return HSceneObject();

		mRoot->mPrefabHash = mHash;
		mRoot->mLinkId = -1;

		return mRoot->clone(false, preserveUUIDs);
	}

	RTTITypeBase* PrefabResource::getRTTIStatic()
	{
		return PrefabResourceRTTI::instance();
	}

	RTTITypeBase* PrefabResource::getRTTI() const
	{
		return PrefabResource::getRTTIStatic();
	}
}
