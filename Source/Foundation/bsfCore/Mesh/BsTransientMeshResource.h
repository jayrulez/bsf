//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Mesh/BsMeshBase.h"

namespace bs
{
	/** @addtogroup Resources
	 *  @{
	 */

	/**
	 * Represents a single mesh entry in the MeshHeap. This can be used as a normal mesh but due to the nature of the
	 * mesh heap it is not the type of mesh you should use for storing static data.
	 *
	 * Transient meshes don't keep internal index/vertex buffers but instead use the ones provided by their parent mesh heap.
	 *
	 * @note	Sim thread.
	 */
	class BS_CORE_EXPORT TransientMeshResource : public MeshResourceBase
	{
	public:
		virtual ~TransientMeshResource();

		/** Retrieves a core implementation of a mesh usable only from the core thread. */
		SPtr<ct::TransientMesh> getCore() const;

	protected:
		friend class MeshHeap;

		/**
		 * Constructs a new transient mesh.
		 *
		 * @see		MeshHeap::alloc
		 */
		TransientMeshResource(const SPtr<MeshHeap>& parentHeap, UINT32 id, UINT32 numVertices,
			UINT32 numIndices, DrawOperationType drawOp = DOT_TRIANGLE_LIST);

		/** Marks the mesh as destroyed so we know that we don't need to destroy it ourselves. */
		void markAsDestroyed() { mIsDestroyed = true; }

		/** @copydoc MeshBase::createCore */
		SPtr<ct::CoreObject> createCore() const override;

	protected:
		bool mIsDestroyed;
		SPtr<MeshHeap> mParentHeap;
		UINT32 mId;
	};

	/** @} */
}
