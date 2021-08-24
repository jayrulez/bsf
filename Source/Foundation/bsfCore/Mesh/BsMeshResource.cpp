//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Mesh/BsMesh.h"
#include "Private/RTTI/BsMeshResourceRTTI.h"
#include "Mesh/BsMeshData.h"
#include "Debug/BsDebug.h"
#include "RenderAPI/Managers/BsHardwareBufferManager.h"
#include "RenderAPI/Managers/BsMeshManager.h"
#include "CoreThread/BsCoreThread.h"
#include "Threading/BsAsyncOp.h"
#include "RenderAPI/BsVertexDataDesc.h"
#include "Resources/BsResources.h"
#include "RenderAPI/BsRenderAPI.h"

namespace bs
{
	MESH_DESC MESH_DESC::DEFAULT = MESH_DESC();

	MeshResource::MeshResource(const MESH_DESC& desc)
		:MeshResourceBase(desc.numVertices, desc.numIndices, desc.subMeshes), mVertexDesc(desc.vertexDesc), mUsage(desc.usage),
		mIndexType(desc.indexType), mSkeleton(desc.skeleton), mMorphShapes(desc.morphShapes)
	{

	}

	MeshResource::MeshResource(const SPtr<MeshData>& initialMeshData, const MESH_DESC& desc)
		:MeshResourceBase(initialMeshData->getNumVertices(), initialMeshData->getNumIndices(), desc.subMeshes),
		mCPUData(initialMeshData), mVertexDesc(initialMeshData->getVertexDesc()),
		mUsage(desc.usage), mIndexType(initialMeshData->getIndexType()), mSkeleton(desc.skeleton),
		mMorphShapes(desc.morphShapes)
	{ }

	MeshResource::MeshResource()
		:MeshResourceBase(0, 0, DOT_TRIANGLE_LIST)
	{ }

	AsyncOp MeshResource::writeData(const SPtr<MeshData>& data, bool discardEntireBuffer)
	{
		updateBounds(*data);
		updateCPUBuffer(0, *data);

		data->_lock();

		std::function<void(const SPtr<ct::Mesh>&, const SPtr<MeshData>&, bool, AsyncOp&)> func =
			[&](const SPtr<ct::Mesh>& mesh, const SPtr<MeshData>& _meshData, bool _discardEntireBuffer, AsyncOp& asyncOp)
		{
			mesh->writeData(*_meshData, _discardEntireBuffer, false);
			_meshData->_unlock();
			asyncOp._completeOperation();

		};

		return gCoreThread().queueReturnCommand(std::bind(func, getCore(),
			data, discardEntireBuffer, std::placeholders::_1));
	}

	AsyncOp MeshResource::readData(const SPtr<MeshData>& data)
	{
		data->_lock();

		std::function<void(const SPtr<ct::Mesh>&, const SPtr<MeshData>&, AsyncOp&)> func =
			[&](const SPtr<ct::Mesh>& mesh, const SPtr<MeshData>& _meshData, AsyncOp& asyncOp)
		{
			// Make sure any queued command start executing before reading
			ct::RenderAPI::instance().submitCommandBuffer(nullptr);

			mesh->readData(*_meshData);
			_meshData->_unlock();
			asyncOp._completeOperation();

		};

		return gCoreThread().queueReturnCommand(std::bind(func, getCore(),
			data, std::placeholders::_1));
	}

	SPtr<MeshData> MeshResource::allocBuffer() const
	{
		SPtr<MeshData> meshData = bs_shared_ptr_new<MeshData>(mProperties.mNumVertices, mProperties.mNumIndices,
			mVertexDesc, mIndexType);

		return meshData;
	}

	void MeshResource::initialize()
	{
		if (mCPUData != nullptr)
			updateBounds(*mCPUData);

		MeshResourceBase::initialize();

		if ((mUsage & MU_CPUCACHED) != 0 && mCPUData == nullptr)
			createCPUBuffer();
	}

	void MeshResource::updateBounds(const MeshData& meshData)
	{
		mProperties.mBounds = meshData.calculateBounds();
		markCoreDirty();
	}

	SPtr<ct::Mesh> MeshResource::getCore() const
	{
		return std::static_pointer_cast<ct::Mesh>(mCoreSpecific);
	}

	SPtr<ct::CoreObject> MeshResource::createCore() const
	{
		MESH_DESC desc;
		desc.numVertices = mProperties.mNumVertices;
		desc.numIndices = mProperties.mNumIndices;
		desc.vertexDesc = mVertexDesc;
		desc.subMeshes = mProperties.mSubMeshes;
		desc.usage = mUsage;
		desc.indexType = mIndexType;
		desc.skeleton = mSkeleton;
		desc.morphShapes = mMorphShapes;

		ct::Mesh* obj = new (bs_alloc<ct::Mesh>()) ct::Mesh(mCPUData, desc, GDF_DEFAULT);

		SPtr<ct::CoreObject> meshCore = bs_shared_ptr<ct::Mesh>(obj);
		meshCore->_setThisPtr(meshCore);

		if ((mUsage & MU_CPUCACHED) == 0)
			mCPUData = nullptr;

		return meshCore;
	}

	void MeshResource::updateCPUBuffer(UINT32 subresourceIdx, const MeshData& pixelData)
	{
		if ((mUsage & MU_CPUCACHED) == 0)
			return;

		if (subresourceIdx > 0)
		{
			BS_LOG(Error, Mesh, "Invalid subresource index: {0}. Supported range: 0 .. 1.", subresourceIdx);
			return;
		}

		if (pixelData.getNumIndices() != mProperties.getNumIndices() ||
			pixelData.getNumVertices() != mProperties.getNumVertices() ||
			pixelData.getIndexType() != mIndexType ||
			pixelData.getVertexDesc()->getVertexStride() != mVertexDesc->getVertexStride())
		{
			BS_LOG(Error, Mesh, "Provided buffer is not of valid dimensions or format in order to update this mesh.");
			return;
		}

		if (mCPUData->getSize() != pixelData.getSize())
			BS_EXCEPT(InternalErrorException, "Buffer sizes don't match.");

		UINT8* dest = mCPUData->getData();
		UINT8* src = pixelData.getData();

		memcpy(dest, src, pixelData.getSize());
	}

	void MeshResource::createCPUBuffer()
	{
		mCPUData = allocBuffer();
	}

	MeshResourceHandle MeshResource::dummy()
	{
		return MeshManager::instance().getDummyMesh();
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* MeshResource::getRTTIStatic()
	{
		return MeshResourceRTTI::instance();
	}

	RTTITypeBase* MeshResource::getRTTI() const
	{
		return MeshResource::getRTTIStatic();
	}

	/************************************************************************/
	/* 								STATICS		                     		*/
	/************************************************************************/

	MeshResourceHandle MeshResource::create(UINT32 numVertices, UINT32 numIndices, const SPtr<VertexDataDesc>& vertexDesc,
		int usage, DrawOperationType drawOp, IndexType indexType)
	{
		MESH_DESC desc;
		desc.numVertices = numVertices;
		desc.numIndices = numIndices;
		desc.vertexDesc = vertexDesc;
		desc.usage = usage;
		desc.subMeshes.push_back(SubMesh(0, numIndices, drawOp));
		desc.indexType = indexType;

		SPtr<MeshResource> meshPtr = _createPtr(desc);
		return static_resource_cast<MeshResource>(gResources()._createResourceHandle(meshPtr));
	}

	MeshResourceHandle MeshResource::create(const MESH_DESC& desc)
	{
		SPtr<MeshResource> meshPtr = _createPtr(desc);
		return static_resource_cast<MeshResource>(gResources()._createResourceHandle(meshPtr));
	}

	MeshResourceHandle MeshResource::create(const SPtr<MeshData>& initialMeshData, const MESH_DESC& desc)
	{
		SPtr<MeshResource> meshPtr = _createPtr(initialMeshData, desc);
		return static_resource_cast<MeshResource>(gResources()._createResourceHandle(meshPtr));
	}

	MeshResourceHandle MeshResource::create(const SPtr<MeshData>& initialMeshData, int usage, DrawOperationType drawOp)
	{
		SPtr<MeshResource> meshPtr = _createPtr(initialMeshData, usage, drawOp);
		return static_resource_cast<MeshResource>(gResources()._createResourceHandle(meshPtr));
	}

	SPtr<MeshResource> MeshResource::_createPtr(const MESH_DESC& desc)
	{
		SPtr<MeshResource> mesh = bs_core_ptr<MeshResource>(new (bs_alloc<MeshResource>()) MeshResource(desc));
		mesh->_setThisPtr(mesh);
		mesh->initialize();

		return mesh;
	}

	SPtr<MeshResource> MeshResource::_createPtr(const SPtr<MeshData>& initialMeshData, const MESH_DESC& desc)
	{
		SPtr<MeshResource> mesh = bs_core_ptr<MeshResource>(new (bs_alloc<MeshResource>()) MeshResource(initialMeshData, desc));
		mesh->_setThisPtr(mesh);
		mesh->initialize();

		return mesh;
	}

	SPtr<MeshResource> MeshResource::_createPtr(const SPtr<MeshData>& initialMeshData, int usage, DrawOperationType drawOp)
	{
		MESH_DESC desc;
		desc.usage = usage;
		desc.subMeshes.push_back(SubMesh(0, initialMeshData->getNumIndices(), drawOp));

		SPtr<MeshResource> mesh = bs_core_ptr<MeshResource>(new (bs_alloc<MeshResource>()) MeshResource(initialMeshData, desc));
		mesh->_setThisPtr(mesh);
		mesh->initialize();

		return mesh;
	}

	SPtr<MeshResource> MeshResource::createEmpty()
	{
		SPtr<MeshResource> mesh = bs_core_ptr<MeshResource>(new (bs_alloc<MeshResource>()) MeshResource());
		mesh->_setThisPtr(mesh);

		return mesh;
	}
}
