//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Mesh/BsMeshBase.h"
#include "Mesh/BsMeshData.h"
#include "RenderAPI/BsVertexData.h"
#include "RenderAPI/BsSubMesh.h"
#include "Math/BsBounds.h"
#include "Mesh/BsMeshResource.h"

namespace bs
{
	namespace ct
	{
	/** @addtogroup Resources-Internal
	 *  @{
	 */

	/**
	 * Core thread portion of a bs::Mesh.
	 *
	 * @note	Core thread.
	 */
	class BS_CORE_EXPORT Mesh : public MeshBase
	{
	public:
		Mesh(const SPtr<MeshData>& initialMeshData, const MESH_DESC& desc, GpuDeviceFlags deviceMask);

		~Mesh();

		/** @copydoc CoreObject::initialize */
		void initialize() override;

		/** @copydoc MeshBase::getVertexData */
		SPtr<VertexData> getVertexData() const override;

		/** @copydoc MeshBase::getIndexBuffer */
		SPtr<IndexBuffer> getIndexBuffer() const override;

		/** @copydoc MeshBase::getVertexDesc */
		SPtr<VertexDataDesc> getVertexDesc() const override;

		/** Returns a skeleton that can be used for animating the mesh. */
		SPtr<Skeleton> getSkeleton() const { return mSkeleton; }

		/** Returns an object containing all shapes used for morph animation, if any are available. */
		SPtr<MorphShapes> getMorphShapes() const { return mMorphShapes; }

		/**
		 * Updates the current mesh with the provided data.
		 *
		 * @param[in]	data				Data to update the mesh with.
		 * @param[in]	discardEntireBuffer When true the existing contents of the resource you are updating will be
		 *									discarded. This can make the operation faster. Resources with certain buffer
		 *									types might require this flag to be in a specific state otherwise the operation
		 *									will fail.
		 * @param[in]	updateBounds		If true the internal bounds of the mesh will be recalculated based on the
		 *									provided data.
		 * @param[in]	queueIdx			Device queue to perform the write operation on. See @ref queuesDoc.
		 */
		virtual void writeData(const MeshData& data, bool discardEntireBuffer, bool updateBounds = true,
			UINT32 queueIdx = 0);

		/**
		 * Reads the current mesh data into the provided @p data parameter. Data buffer needs to be pre-allocated.
		 *
		 * @param[out]	data				Pre-allocated buffer of proper vertex/index format and size where data will be
		 *									read to. You can use Mesh::allocBuffer() to allocate a buffer of a correct
		 *									format and size.
		 * @param[in]	deviceIdx			Index of the device whose memory to read. If the buffer doesn't exist on this
		 *									device, no data will be read.
		 * @param[in]	queueIdx			Device queue to perform the read operation on. See @ref queuesDoc.
		 */
		virtual void readData(MeshData& data, UINT32 deviceIdx = 0, UINT32 queueIdx = 0);

		/**
		 * Creates a new empty mesh. Created mesh will have no sub-meshes.
		 *
		 * @param[in]	numVertices		Number of vertices in the mesh.
		 * @param[in]	numIndices		Number of indices in the mesh.
		 * @param[in]	vertexDesc		Vertex description structure that describes how are vertices organized in the
		 *								vertex buffer. When binding a mesh to the pipeline you must ensure vertex
		 *								description at least partially matches the input description of the currently
		 *								bound vertex GPU program.
		 * @param[in]	usage			Optimizes performance depending on planned usage of the mesh.
		 * @param[in]	drawOp			Determines how should the provided indices be interpreted by the pipeline. Default
		 *								option is a triangle list, where three indices represent a single triangle.
		 * @param[in]	indexType		Size of indices, use smaller size for better performance, however be careful not to
		 *								go over the number of vertices limited by the size.
		 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
		 */
		static SPtr<Mesh> create(UINT32 numVertices, UINT32 numIndices, const SPtr<VertexDataDesc>& vertexDesc,
			int usage = MU_STATIC, DrawOperationType drawOp = DOT_TRIANGLE_LIST, IndexType indexType = IT_32BIT,
			GpuDeviceFlags deviceMask = GDF_DEFAULT);

		/**
		 * Creates a new empty mesh.
		 *
		 * @param[in]	desc			Descriptor containing the properties of the mesh to create.
		 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
		 */
		static SPtr<Mesh> create(const MESH_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

		/**
		 * Creates a new mesh from an existing mesh data. Created mesh will match the vertex and index buffers described
		 * by the mesh data exactly.
		 *
		 * @param[in]	initialData		Vertex and index data to initialize the mesh with.
		 * @param[in]	desc			Descriptor containing the properties of the mesh to create. Vertex and index count,
		 *								vertex descriptor and index type properties are ignored and are read from provided
		 *								mesh data instead.
		 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
		 */
		static SPtr<Mesh> create(const SPtr<MeshData>& initialData, const MESH_DESC& desc,
			GpuDeviceFlags deviceMask = GDF_DEFAULT);

		/**
		 * Creates a new mesh from an existing mesh data. Created mesh will match the vertex and index buffers described
		 * by the mesh data exactly. Mesh will have no sub-meshes.
		 *
		 * @param[in]	initialData		Vertex and index data to initialize the mesh with.
		 * @param[in]	usage			Optimizes performance depending on planned usage of the mesh.
		 * @param[in]	drawOp			Determines how should the provided indices be interpreted by the pipeline. Default
		 *								option is a triangle strip, where three indices represent a single triangle.
		 * @param[in]	deviceMask		Mask that determines on which GPU devices should the object be created on.
		 */
		static SPtr<Mesh> create(const SPtr<MeshData>& initialData, int usage = MU_STATIC,
			DrawOperationType drawOp = DOT_TRIANGLE_LIST, GpuDeviceFlags deviceMask = GDF_DEFAULT);

	protected:
		friend class bs::MeshResource;

		/** Updates bounds by calculating them from the vertices in the provided mesh data object. */
		void updateBounds(const MeshData& meshData);

		SPtr<VertexData> mVertexData;
		SPtr<IndexBuffer> mIndexBuffer;

		SPtr<VertexDataDesc> mVertexDesc;
		int mUsage;
		IndexType mIndexType;
		GpuDeviceFlags mDeviceMask;
		SPtr<MeshData> mTempInitialMeshData;
		SPtr<Skeleton> mSkeleton; // Immutable
		SPtr<MorphShapes> mMorphShapes; // Immutable
	};

	/** @} */
	}
}
