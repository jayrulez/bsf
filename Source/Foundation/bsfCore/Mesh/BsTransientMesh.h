//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Mesh/BsMeshBase.h"

namespace bs
{
	namespace ct
	{
	/** @addtogroup Resources-Internal
	 *  @{
	 */

	/**
	 * Core thread portion of a bs::TransientMesh.
	 *
	 * @note	Core thread.
	 */
	class BS_CORE_EXPORT TransientMesh : public MeshBase
	{
	public:
		TransientMesh(const SPtr<MeshHeap>& parentHeap, UINT32 id, UINT32 numVertices,
			UINT32 numIndices, const Vector<SubMesh>& subMeshes);

		/** @copydoc MeshBase::getVertexData */
		SPtr<VertexData> getVertexData() const override;

		 /** @copydoc MeshBase::getIndexBuffer */
		SPtr<IndexBuffer> getIndexBuffer() const override;

		/** @copydoc MeshBase::getVertexDesc */
		SPtr<VertexDataDesc> getVertexDesc() const override;

		/**	Returns the ID that uniquely identifies this mesh in the parent heap. */
		UINT32 getMeshHeapId() const { return mId; }

		/** @copydoc MeshBase::getVertexOffset */
		UINT32 getVertexOffset() const override;

		 /** @copydoc MeshBase::getIndexOffset */
		UINT32 getIndexOffset() const override;

		 /** @copydoc MeshBase::_notifyUsedOnGPU */
		void _notifyUsedOnGPU() override;

	protected:
		friend class bs::TransientMeshResource;

		SPtr<MeshHeap> mParentHeap;
		UINT32 mId;
	};

	/** @} */
	}
}
