//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"
#include "Math/BsBounds.h"
#include "RenderAPI/BsSubMesh.h"
#include "Mesh/BsMeshResourceBase.h"

namespace bs
{
	namespace ct
	{
	/**
	 * Core version of a class used as a basis for all implemenations of meshes.
	 *
	 * @see		bs::MeshBase
	 *
	 * @note	Core thread.
	 */
	class BS_CORE_EXPORT MeshBase : public CoreObject
	{
	public:
		MeshBase(UINT32 numVertices, UINT32 numIndices, const Vector<SubMesh>& subMeshes);
		virtual ~MeshBase() { }

		/**	Get vertex data used for rendering. */
		virtual SPtr<VertexData> getVertexData() const = 0;

		/**	Get index data used for rendering. */
		virtual SPtr<IndexBuffer> getIndexBuffer() const = 0;

		/**
		 * Returns an offset into the vertex buffers that is returned by getVertexData() that signifies where this meshes
		 * vertices begin.
		 * 			
		 * @note	Used when multiple meshes share the same buffers.
		 */
		virtual UINT32 getVertexOffset() const { return 0; }

		/**
		 * Returns an offset into the index buffer that is returned by getIndexData() that signifies where this meshes
		 * indices begin.
		 * 			
		 * @note	Used when multiple meshes share the same buffers.
		 */
		virtual UINT32 getIndexOffset() const { return 0; }

		/** Returns a structure that describes how are the vertices stored in the mesh's vertex buffer. */
		virtual SPtr<VertexDataDesc> getVertexDesc() const = 0;

		/**
		 * Called whenever this mesh starts being used on the GPU.
		 * 			
		 * @note	Needs to be called after all commands referencing this mesh have been sent to the GPU.
		 */
		virtual void _notifyUsedOnGPU() { }

		/**	Returns properties that contain information about the mesh. */
		const MeshProperties& getProperties() const { return mProperties; }

	protected:
		/** @copydoc CoreObject::syncToCore */
		void syncToCore(const CoreSyncData& data) override;

		MeshProperties mProperties;
	};
	}

	/** @} */
}
