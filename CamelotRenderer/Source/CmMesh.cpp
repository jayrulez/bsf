#include "CmMesh.h"
#include "CmMeshRTTI.h"
#include "CmMeshData.h"
#include "CmVector2.h"
#include "CmVector3.h"
#include "CmDebug.h"
#include "CmHardwareBufferManager.h"
#include "CmRenderSystem.h"
#include "CmAsyncOp.h"

#if CM_DEBUG_MODE
#define THROW_IF_NOT_RENDER_THREAD throwIfNotRenderThread();
#else
#define THROW_IF_NOT_RENDER_THREAD 
#endif

namespace CamelotEngine
{
	Mesh::Mesh()
		:mVertexData(nullptr), mIndexData(nullptr)
	{

	}

	Mesh::~Mesh()
	{
		THROW_IF_NOT_RENDER_THREAD;

		if(mVertexData)
			delete mVertexData;

		if(mIndexData)
			delete mIndexData;
	}

	void Mesh::setMeshData(MeshDataPtr meshData)
	{
		RenderSystem::instancePtr()->queueCommand(boost::bind(&Mesh::setMeshData_internal, this, meshData));
	}

	void Mesh::setMeshData_internal(MeshDataPtr meshData)
	{
		THROW_IF_NOT_RENDER_THREAD;

		if(meshData == nullptr)
		{
			CM_EXCEPT(InternalErrorException, "Cannot load mesh. Mesh data is null.");
		}

		// Submeshes
		for(UINT32 i = 0; i < meshData->subMeshes.size(); i++)
			mSubMeshes.push_back(SubMesh(meshData->subMeshes[i].indexOffset, meshData->subMeshes[i].indexCount));

		// Indices
		mIndexData = new IndexData();

		mIndexData->indexCount = meshData->indexCount;
		mIndexData->indexBuffer = HardwareBufferManager::instance().createIndexBuffer(
			HardwareIndexBuffer::IT_32BIT,
			mIndexData->indexCount, 
			HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		UINT32* idxData = static_cast<UINT32*>(mIndexData->indexBuffer->lock(HBL_READ_WRITE));

		for(UINT32 i = 0; i < mIndexData->indexCount; i++)
		{
			idxData[i] = (UINT32)meshData->index[i];
		}

		mIndexData->indexBuffer->unlock();

		// Vertices
		mVertexData = new VertexData();

		mVertexData->vertexStart = 0;
		mVertexData->vertexCount = meshData->vertexCount;

		mVertexData->vertexDeclaration = meshData->declaration->clone();

		for(auto iter = meshData->vertexBuffers.begin(); iter != meshData->vertexBuffers.end(); ++iter)
		{
			int streamIdx = iter->first; 

			HardwareVertexBufferPtr vertexBuffer = HardwareBufferManager::instance().createVertexBuffer(
				mVertexData->vertexDeclaration->getVertexSize(streamIdx),
				mVertexData->vertexCount,
				HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			mVertexData->vertexBufferBinding->setBinding(streamIdx, vertexBuffer);

			UINT32 vertexSize = vertexBuffer->getVertexSize();
			UINT8* vertBufferData = static_cast<UINT8*>(vertexBuffer->lock(HBL_READ_WRITE));

			UINT32 numElements = mVertexData->vertexDeclaration->getElementCount();

			for(UINT32 j = 0; j < numElements; j++)
			{
				const VertexElement* element = mVertexData->vertexDeclaration->getElement(j);
				VertexElementSemantic semantic = element->getSemantic();
				UINT32 offset = element->getOffset();
				UINT32 elemSize = element->getSize();

				std::shared_ptr<MeshData::VertexData> vertexData = meshData->vertexBuffers[streamIdx];

				UINT8* source = nullptr;
				switch(semantic)
				{
				case VES_POSITION:
					if(vertexData->vertex)
						source = (UINT8*)vertexData->vertex;

					break;
				case VES_DIFFUSE:
					if(vertexData->color)
						source = (UINT8*)vertexData->color;

					break;
				case VES_NORMAL:
					if(vertexData->normal)
						source = (UINT8*)vertexData->normal;	

					break;
				case VES_TANGENT:
					if(vertexData->tangent)
						source = (UINT8*)vertexData->tangent;	

					break;
				case VES_BITANGENT:
					if(vertexData->bitangent)
						source = (UINT8*)vertexData->bitangent;	

					break;
				case VES_TEXTURE_COORDINATES:
					if(element->getIndex() == 0)
					{
						if(vertexData->uv0)
							source = (UINT8*)vertexData->uv0;	
					}
					else if(element->getIndex() == 1)
					{
						if(vertexData->uv1)
							source = (UINT8*)vertexData->uv1;	
					}

					break;
				default:
					break;
				}

				if(source != nullptr)
				{
					for(UINT32 k = 0; k < mVertexData->vertexCount; k++)
						memcpy(&vertBufferData[k * vertexSize + offset], &source[k * elemSize], elemSize);
				}
				else
				{
					LOGWRN("Vertex declaration contains semantic (" + toString(semantic) + ") but mesh doesn't have data for it. Data for the semantic will be zeroed out.");

					for(UINT32 k = 0; k < mVertexData->vertexCount; k++)
						memset(&vertBufferData[k * vertexSize + offset], 0, elemSize);
				}
			}

			vertexBuffer->unlock();
		}
	}

	MeshDataPtr Mesh::getMeshData()
	{
		AsyncOp op = RenderSystem::instancePtr()->queueReturnCommand(boost::bind(&Mesh::getMeshData_internal, this, _1), true);

		return op.getReturnValue<MeshDataPtr>();
	}

	void Mesh::getMeshData_internal(AsyncOp& asyncOp)
	{
		MeshDataPtr meshData(new MeshData());

		meshData->declaration = mVertexData->vertexDeclaration->clone();
		
		for(UINT32 i = 0; i < mSubMeshes.size(); i++)
		{
			MeshData::SubMeshData subMesh;
			subMesh.indexCount = mSubMeshes[i].indexCount;
			subMesh.indexOffset = mSubMeshes[i].indexOffset;

			meshData->subMeshes.push_back(subMesh);
		}

		if(mIndexData)
		{
			meshData->indexCount = mIndexData->indexCount - mIndexData->indexStart;
			meshData->index = new int[meshData->indexCount];

			UINT16* idxData = static_cast<UINT16*>(mIndexData->indexBuffer->lock(HBL_READ_ONLY));

			for(UINT32 i = 0; i < mIndexData->indexCount; i++)
				meshData->index[i] = (UINT32)idxData[i];

			mIndexData->indexBuffer->unlock();
		}

		if(mVertexData)
		{
			meshData->vertexCount = mVertexData->vertexCount - mVertexData->vertexStart;
			
			UINT16 maxBufferIdx = mVertexData->vertexBufferBinding->getLastBoundIndex();
			for(UINT16 i = 0; i < maxBufferIdx; i++)
			{
				if(!mVertexData->vertexBufferBinding->isBufferBound(i))
					continue;

				HardwareVertexBufferPtr vertexBuffer = mVertexData->vertexBufferBinding->getBuffer(i);
				UINT32 vertexSize = vertexBuffer->getVertexSize();
				UINT8* vertDataIter = static_cast<UINT8*>(vertexBuffer->lock(HBL_READ_ONLY));

				std::shared_ptr<MeshData::VertexData> vertexData(new MeshData::VertexData(meshData->vertexCount, i));
				meshData->vertexBuffers[i] = vertexData;

				UINT32 numElements = mVertexData->vertexDeclaration->getElementCount();
				for(UINT32 j = 0; j < numElements; j++)
				{
					const VertexElement* element = mVertexData->vertexDeclaration->getElement(j);
					VertexElementSemantic semantic = element->getSemantic();
					UINT32 offset = element->getOffset();
					UINT32 elemSize = element->getSize();

					UINT8* dest = nullptr;
					switch(semantic)
					{
					case VES_POSITION:
						vertexData->vertex = new Vector3[meshData->vertexCount];
						dest = (UINT8*)vertexData->vertex;

						break;
					case VES_DIFFUSE:
						vertexData->color = new Color[meshData->vertexCount];
						dest = (UINT8*)vertexData->color;

						break;
					case VES_NORMAL:
						vertexData->normal = new Vector3[meshData->vertexCount];
						dest = (UINT8*)vertexData->normal;	

						break;
					case VES_TANGENT:
						vertexData->tangent = new Vector3[meshData->vertexCount];
						dest = (UINT8*)vertexData->tangent;	

						break;
					case VES_BITANGENT:
						vertexData->bitangent = new Vector3[meshData->vertexCount];
						dest = (UINT8*)vertexData->bitangent;	

						break;
					case VES_TEXTURE_COORDINATES:
						if(element->getIndex() == 0)
						{
							vertexData->uv0 = new Vector2[meshData->vertexCount];
							dest = (UINT8*)vertexData->uv0;	
						}
						else if(element->getIndex() == 1)
						{
							vertexData->uv1 = new Vector2[meshData->vertexCount];
							dest = (UINT8*)vertexData->uv1;	
						}

						break;
					default:
						LOGWRN("Vertex declaration contains semantic (" + toString(semantic) + ") but mesh data can't store it.");

						break;
					}

					if(dest != nullptr)
					{
						for(UINT32 k = 0; k < mVertexData->vertexCount; k++)
							memcpy(&dest[k * elemSize], &vertDataIter[k * vertexSize + offset], elemSize);
					}
				}

				vertexBuffer->unlock();
			}
		}		

		asyncOp.completeOperation(meshData);
	}

	RenderOperation Mesh::getRenderOperation(UINT32 subMeshIdx) const
	{
		if(subMeshIdx < 0 || subMeshIdx >= mSubMeshes.size())
		{
			CM_EXCEPT(InvalidParametersException, "Invalid sub-mesh index (" 
				+ toString(subMeshIdx) + "). Number of sub-meshes available: " + toString(mSubMeshes.size()));
		}

		// TODO - BIG TODO - Completely ignores subMeshIdx and always renders the entire thing
		RenderOperation ro;
		ro.indexData = mIndexData;
		ro.vertexData = mVertexData;
		ro.useIndexes = true;
		ro.operationType = RenderOperation::OT_TRIANGLE_LIST;

		return ro;
	}
	void Mesh::initialize()
	{
		RenderSystem::instancePtr()->queueCommand(boost::bind(&Mesh::initialize_internal, this));
	}

	void Mesh::initialize_internal()
	{
		THROW_IF_NOT_RENDER_THREAD;

		// TODO Low priority - Initialize an empty mesh. A better way would be to only initialize the mesh
		// once we set the proper mesh data (then we don't have to do it twice), but this makes the code less complex.
		// Consider changing it if there are performance issues.
		setMeshData_internal(getNullMeshData());

		Resource::initialize_internal();
	}

	void Mesh::throwIfNotRenderThread() const
	{
		if(CM_THREAD_CURRENT_ID != RenderSystem::instancePtr()->getRenderThreadId())
			CM_EXCEPT(InternalErrorException, "Calling an internal texture method from a non-render thread!");
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* Mesh::getRTTIStatic()
	{
		return MeshRTTI::instance();
	}

	RTTITypeBase* Mesh::getRTTI() const
	{
		return Mesh::getRTTIStatic();
	}

	/************************************************************************/
	/* 								STATICS		                     		*/
	/************************************************************************/

	MeshPtr Mesh::create()
	{
		MeshPtr mesh = MeshPtr(new Mesh());
		mesh->initialize();

		return mesh;
	}

	MeshPtr Mesh::createEmpty()
	{
		MeshPtr mesh = MeshPtr(new Mesh());

		return mesh;
	}

	MeshDataPtr Mesh::getNullMeshData()
	{
		static MeshDataPtr NULL_MESH_DATA = nullptr;

		if(NULL_MESH_DATA == nullptr)
		{
			NULL_MESH_DATA = MeshDataPtr(new MeshData());

			NULL_MESH_DATA->indexCount = 3;
			NULL_MESH_DATA->vertexCount = 1;

			NULL_MESH_DATA->index = new int[3];
			NULL_MESH_DATA->index[0] = 0;
			NULL_MESH_DATA->index[1] = 0;
			NULL_MESH_DATA->index[2] = 0;

			std::shared_ptr<MeshData::VertexData> vertexData = std::shared_ptr<MeshData::VertexData>(new MeshData::VertexData(1));
			NULL_MESH_DATA->vertexBuffers.insert(std::make_pair(0, vertexData));
			vertexData->vertex = new Vector3[1];
			vertexData->vertex[0] = Vector3(0, 0, 0);

			NULL_MESH_DATA->declaration->addElement(0, 0, VET_FLOAT3, VES_POSITION);

			MeshData::SubMeshData subMesh;
			subMesh.indexOffset = 0;
			subMesh.indexCount = 3;
			NULL_MESH_DATA->subMeshes.push_back(subMesh);
		}

		return NULL_MESH_DATA;
	}
}

#undef THROW_IF_NOT_RENDER_THREAD