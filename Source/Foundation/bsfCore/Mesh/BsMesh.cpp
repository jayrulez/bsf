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
	namespace ct
	{
		Mesh::Mesh(const SPtr<MeshData>& initialMeshData, const MESH_DESC& desc, GpuDeviceFlags deviceMask)
			: MeshBase(desc.numVertices, desc.numIndices, desc.subMeshes), mVertexData(nullptr), mIndexBuffer(nullptr)
			, mVertexDesc(desc.vertexDesc), mUsage(desc.usage), mIndexType(desc.indexType), mDeviceMask(deviceMask)
			, mTempInitialMeshData(initialMeshData), mSkeleton(desc.skeleton), mMorphShapes(desc.morphShapes)

		{ }

		Mesh::~Mesh()
		{
			THROW_IF_NOT_CORE_THREAD;

			mVertexData = nullptr;
			mIndexBuffer = nullptr;
			mVertexDesc = nullptr;
			mTempInitialMeshData = nullptr;
		}

		void Mesh::initialize()
		{
			THROW_IF_NOT_CORE_THREAD;

			bool isDynamic = (mUsage & MU_DYNAMIC) != 0;
			int usage = isDynamic ? GBU_DYNAMIC : GBU_STATIC;

			INDEX_BUFFER_DESC ibDesc;
			ibDesc.indexType = mIndexType;
			ibDesc.numIndices = mProperties.mNumIndices;
			ibDesc.usage = (GpuBufferUsage)usage;

			mIndexBuffer = IndexBuffer::create(ibDesc, mDeviceMask);

			mVertexData = bs_shared_ptr_new<VertexData>();
			mVertexData->vertexCount = mProperties.mNumVertices;
			mVertexData->vertexDeclaration = VertexDeclaration::create(mVertexDesc, mDeviceMask);

			for (UINT32 i = 0; i <= mVertexDesc->getMaxStreamIdx(); i++)
			{
				if (!mVertexDesc->hasStream(i))
					continue;

				VERTEX_BUFFER_DESC vbDesc;
				vbDesc.vertexSize = mVertexData->vertexDeclaration->getProperties().getVertexSize(i);
				vbDesc.numVerts = mVertexData->vertexCount;
				vbDesc.usage = (GpuBufferUsage)usage;

				SPtr<VertexBuffer> vertexBuffer = VertexBuffer::create(vbDesc, mDeviceMask);
				mVertexData->setBuffer(i, vertexBuffer);
			}

			// TODO Low priority - DX11 (and maybe OpenGL)? allow an optimization that allows you to set
			// buffer data upon buffer construction, instead of setting it in a second step like I do here
			if (mTempInitialMeshData != nullptr)
			{
				writeData(*mTempInitialMeshData, isDynamic);
				mTempInitialMeshData = nullptr;
			}

			MeshBase::initialize();
		}

		SPtr<VertexData> Mesh::getVertexData() const
		{
			THROW_IF_NOT_CORE_THREAD;

			return mVertexData;
		}

		SPtr<IndexBuffer> Mesh::getIndexBuffer() const
		{
			THROW_IF_NOT_CORE_THREAD;

			return mIndexBuffer;
		}

		SPtr<VertexDataDesc> Mesh::getVertexDesc() const
		{
			THROW_IF_NOT_CORE_THREAD;

			return mVertexDesc;
		}

		void Mesh::writeData(const MeshData& meshData, bool discardEntireBuffer, bool performUpdateBounds, UINT32 queueIdx)
		{
			THROW_IF_NOT_CORE_THREAD;

			if (discardEntireBuffer)
			{
				if ((mUsage & MU_STATIC) != 0)
				{
					BS_LOG(Warning, Mesh, "Buffer discard is enabled but buffer was not created as dynamic. Disabling discard.");
					discardEntireBuffer = false;
				}
			}
			else
			{
				if ((mUsage & MU_DYNAMIC) != 0)
				{
					BS_LOG(Warning, Mesh, "Buffer discard is not enabled but buffer was created as dynamic. Enabling discard.");
					discardEntireBuffer = true;
				}
			}

			// Indices
			const IndexBufferProperties& ibProps = mIndexBuffer->getProperties();

			UINT32 indicesSize = meshData.getIndexBufferSize();
			UINT8* srcIdxData = meshData.getIndexData();

			if (meshData.getIndexElementSize() != ibProps.getIndexSize())
			{
				BS_LOG(Error, Mesh, "Provided index size doesn't match meshes index size. Needed: {0}. Got: {1}",
					ibProps.getIndexSize(), meshData.getIndexElementSize());

				return;
			}

			if (indicesSize > mIndexBuffer->getSize())
			{
				indicesSize = mIndexBuffer->getSize();
				BS_LOG(Error, Mesh, "Index buffer values are being written out of valid range.");
			}

			mIndexBuffer->writeData(0, indicesSize, srcIdxData, discardEntireBuffer ? BWT_DISCARD : BWT_NORMAL, queueIdx);

			// Vertices
			for (UINT32 i = 0; i <= mVertexDesc->getMaxStreamIdx(); i++)
			{
				if (!mVertexDesc->hasStream(i))
					continue;

				if (!meshData.getVertexDesc()->hasStream(i))
					continue;

				// Ensure both have the same sized vertices
				UINT32 myVertSize = mVertexDesc->getVertexStride(i);
				UINT32 otherVertSize = meshData.getVertexDesc()->getVertexStride(i);
				if (myVertSize != otherVertSize)
				{
					BS_LOG(Error, Mesh, "Provided vertex size for stream {0} doesn't match meshes vertex size. "
						"Needed: {1}. Got: {2}", i, myVertSize, otherVertSize);

					continue;
				}

				SPtr<VertexBuffer> vertexBuffer = mVertexData->getBuffer(i);

				UINT32 bufferSize = meshData.getStreamSize(i);
				UINT8* srcVertBufferData = meshData.getStreamData(i);

				if (bufferSize > vertexBuffer->getSize())
				{
					bufferSize = vertexBuffer->getSize();
					BS_LOG(Error, Mesh, "Vertex buffer values for stream \"{0}\" are being written out of valid range.", i);
				}

				vertexBuffer->writeData(0, bufferSize, srcVertBufferData, discardEntireBuffer ? BWT_DISCARD : BWT_NORMAL,
					queueIdx);
			}

			if (performUpdateBounds)
				updateBounds(meshData);
		}

		void Mesh::readData(MeshData& meshData, UINT32 deviceIdx, UINT32 queueIdx)
		{
			THROW_IF_NOT_CORE_THREAD;

			IndexType indexType = IT_32BIT;
			if (mIndexBuffer)
				indexType = mIndexBuffer->getProperties().getType();

			if (mIndexBuffer)
			{
				const IndexBufferProperties& ibProps = mIndexBuffer->getProperties();

				if (meshData.getIndexElementSize() != ibProps.getIndexSize())
				{
					BS_LOG(Error, Mesh, "Provided index size doesn't match meshes index size. Needed: {0}. Got: {1}",
						ibProps.getIndexSize(), meshData.getIndexElementSize());
					return;
				}

				UINT8* idxData = static_cast<UINT8*>(mIndexBuffer->lock(GBL_READ_ONLY, deviceIdx, queueIdx));
				UINT32 idxElemSize = ibProps.getIndexSize();

				UINT8* indices = nullptr;

				if (indexType == IT_16BIT)
					indices = (UINT8*)meshData.getIndices16();
				else
					indices = (UINT8*)meshData.getIndices32();

				UINT32 numIndicesToCopy = std::min(mProperties.mNumIndices, meshData.getNumIndices());

				UINT32 indicesSize = numIndicesToCopy * idxElemSize;
				if (indicesSize > meshData.getIndexBufferSize())
				{
					BS_LOG(Error, Mesh, "Provided buffer doesn't have enough space to store mesh indices.");
					return;
				}

				memcpy(indices, idxData, numIndicesToCopy * idxElemSize);

				mIndexBuffer->unlock();
			}

			if (mVertexData)
			{
				auto vertexBuffers = mVertexData->getBuffers();

				UINT32 streamIdx = 0;
				for (auto iter = vertexBuffers.begin(); iter != vertexBuffers.end(); ++iter)
				{
					if (!meshData.getVertexDesc()->hasStream(streamIdx))
						continue;

					SPtr<VertexBuffer> vertexBuffer = iter->second;
					const VertexBufferProperties& vbProps = vertexBuffer->getProperties();

					// Ensure both have the same sized vertices
					UINT32 myVertSize = mVertexDesc->getVertexStride(streamIdx);
					UINT32 otherVertSize = meshData.getVertexDesc()->getVertexStride(streamIdx);
					if (myVertSize != otherVertSize)
					{
						BS_LOG(Error, Mesh, "Provided vertex size for stream {0} doesn't match meshes vertex size. "
							"Needed: {1}. Got: {2}", streamIdx, myVertSize, otherVertSize);

						continue;
					}

					UINT32 numVerticesToCopy = meshData.getNumVertices();
					UINT32 bufferSize = vbProps.getVertexSize() * numVerticesToCopy;

					if (bufferSize > vertexBuffer->getSize())
					{
						BS_LOG(Error, Mesh, "Vertex buffer values for stream \"{0}\" are being read out of valid range.",
							streamIdx);
						continue;
					}

					UINT8* vertDataPtr = static_cast<UINT8*>(vertexBuffer->lock(GBL_READ_ONLY, deviceIdx, queueIdx));

					UINT8* dest = meshData.getStreamData(streamIdx);
					memcpy(dest, vertDataPtr, bufferSize);

					vertexBuffer->unlock();

					streamIdx++;
				}
			}
		}

		void Mesh::updateBounds(const MeshData& meshData)
		{
			mProperties.mBounds = meshData.calculateBounds();

			// TODO - Sync this to sim-thread possibly?
		}

		SPtr<Mesh> Mesh::create(UINT32 numVertices, UINT32 numIndices, const SPtr<VertexDataDesc>& vertexDesc,
			int usage, DrawOperationType drawOp, IndexType indexType, GpuDeviceFlags deviceMask)
		{
			MESH_DESC desc;
			desc.numVertices = numVertices;
			desc.numIndices = numIndices;
			desc.vertexDesc = vertexDesc;
			desc.subMeshes.push_back(SubMesh(0, numIndices, drawOp));
			desc.usage = usage;
			desc.indexType = indexType;

			SPtr<Mesh> mesh = bs_shared_ptr<Mesh>(new (bs_alloc<Mesh>()) Mesh(nullptr, desc, deviceMask));
			mesh->_setThisPtr(mesh);
			mesh->initialize();

			return mesh;
		}

		SPtr<Mesh> Mesh::create(const MESH_DESC& desc, GpuDeviceFlags deviceMask)
		{
			SPtr<Mesh> mesh = bs_shared_ptr<Mesh>(new (bs_alloc<Mesh>()) Mesh(nullptr, desc, deviceMask));

			mesh->_setThisPtr(mesh);
			mesh->initialize();

			return mesh;
		}

		SPtr<Mesh> Mesh::create(const SPtr<MeshData>& initialMeshData, const MESH_DESC& desc, GpuDeviceFlags deviceMask)
		{
			MESH_DESC descCopy = desc;
			descCopy.numVertices = initialMeshData->getNumVertices();
			descCopy.numIndices = initialMeshData->getNumIndices();
			descCopy.vertexDesc = initialMeshData->getVertexDesc();
			descCopy.indexType = initialMeshData->getIndexType();

			SPtr<Mesh> mesh =
				bs_shared_ptr<Mesh>(new (bs_alloc<Mesh>()) Mesh(initialMeshData, descCopy, deviceMask));

			mesh->_setThisPtr(mesh);
			mesh->initialize();

			return mesh;
		}

		SPtr<Mesh> Mesh::create(const SPtr<MeshData>& initialMeshData, int usage, DrawOperationType drawOp,
			GpuDeviceFlags deviceMask)
		{
			MESH_DESC desc;
			desc.numVertices = initialMeshData->getNumVertices();
			desc.numIndices = initialMeshData->getNumIndices();
			desc.vertexDesc = initialMeshData->getVertexDesc();
			desc.indexType = initialMeshData->getIndexType();
			desc.subMeshes.push_back(SubMesh(0, initialMeshData->getNumIndices(), drawOp));
			desc.usage = usage;

			SPtr<Mesh> mesh =
				bs_shared_ptr<Mesh>(new (bs_alloc<Mesh>()) Mesh(initialMeshData, desc, deviceMask));

			mesh->_setThisPtr(mesh);
			mesh->initialize();

			return mesh;
		}
	}
}
