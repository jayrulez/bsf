//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Mesh/BsMeshBase.h"
#include "CoreThread/BsCoreThread.h"

namespace bs
{
	namespace ct
	{
		MeshBase::MeshBase(UINT32 numVertices, UINT32 numIndices, const Vector<SubMesh>& subMeshes)
			:mProperties(numVertices, numIndices, subMeshes)
		{ }

		void MeshBase::syncToCore(const CoreSyncData& data)
		{
			mProperties.mBounds = data.getData<Bounds>();
		}
	}
}
