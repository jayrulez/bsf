//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsPhysicsMeshResource.h"
#include "Private/RTTI/BsPhysicsMeshResourceRTTI.h"
#include "Resources/BsResources.h"
#include "Physics/BsPhysics.h"

namespace bs
{
	PhysicsMeshResource::PhysicsMeshResource(const SPtr<MeshData>& meshData, PhysicsMeshType type)
		:mInitMeshData(meshData), mType(type)
	{
		// Derived class is responsible for initializing mInternal
	}

	PhysicsMeshType PhysicsMeshResource::getType() const
	{
		return mInternal->mType;
	}

	SPtr<MeshData> PhysicsMeshResource::getMeshData() const
	{
		return mInternal->getMeshData();
	}

	PhysicsMeshResourceHandle PhysicsMeshResource::create(const SPtr<MeshData>& meshData, PhysicsMeshType type)
	{
		SPtr<PhysicsMeshResource> newMesh = _createPtr(meshData, type);

		return static_resource_cast<PhysicsMeshResource>(gResources()._createResourceHandle(newMesh));
	}

	SPtr<PhysicsMeshResource> PhysicsMeshResource::_createPtr(const SPtr<MeshData>& meshData, PhysicsMeshType type)
	{
		SPtr<PhysicsMeshResource> newMesh = gPhysics().createMesh(meshData, type);
		newMesh->_setThisPtr(newMesh);
		newMesh->initialize();

		return newMesh;
	}

	void PhysicsMeshResource::initialize()
	{
		mInitMeshData = nullptr;

		Resource::initialize();
	}

	RTTITypeBase* PhysicsMeshResource::getRTTIStatic()
	{
		return PhysicsMeshResourceRTTI::instance();
	}

	RTTITypeBase* PhysicsMeshResource::getRTTI() const
	{
		return getRTTIStatic();
	}

	FPhysicsMesh::FPhysicsMesh(const SPtr<MeshData>& meshData, PhysicsMeshType type)
		:mType(type)
	{
		
	}

	FPhysicsMesh::~FPhysicsMesh()
	{
		
	}

	RTTITypeBase* FPhysicsMesh::getRTTIStatic()
	{
		return FPhysicsMeshRTTI::instance();
	}

	RTTITypeBase* FPhysicsMesh::getRTTI() const
	{
		return getRTTIStatic();
	}
}
