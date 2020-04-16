//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Mesh/BsMeshBase.h"
#include "Private/RTTI/BsMeshBaseRTTI.h"

namespace bs
{
	MeshProperties::MeshProperties()
		:mNumVertices(0), mNumIndices(0)
	{
		mSubMeshes.reserve(10);
	}

	MeshProperties::MeshProperties(UINT32 numVertices, UINT32 numIndices, DrawOperationType drawOp)
		:mNumVertices(numVertices),
		mNumIndices(numIndices)
	{
		mSubMeshes.push_back(SubMesh(0, numIndices, drawOp));
	}

	MeshProperties::MeshProperties(UINT32 numVertices, UINT32 numIndices, const Vector<SubMesh>& subMeshes)
		:mNumVertices(numVertices),
		mNumIndices(numIndices)
	{
		mSubMeshes = subMeshes;
	}

	const SubMesh& MeshProperties::getSubMesh(UINT32 subMeshIdx) const
	{
		if (subMeshIdx >= mSubMeshes.size())
		{
			BS_EXCEPT(InvalidParametersException, "Invalid sub-mesh index ("
				+ toString(subMeshIdx) + "). Number of sub-meshes available: " + toString((int)mSubMeshes.size()));
		}

		return mSubMeshes[subMeshIdx];
	}

	UINT32 MeshProperties::getNumSubMeshes() const
	{
		return (UINT32)mSubMeshes.size();
	}

	MeshBase::MeshBase(UINT32 numVertices, UINT32 numIndices, DrawOperationType drawOp)
		:mProperties(numVertices, numIndices, drawOp)
	{ }

	MeshBase::MeshBase(UINT32 numVertices, UINT32 numIndices, const Vector<SubMesh>& subMeshes)
		:mProperties(numVertices, numIndices, subMeshes)
	{ }

	MeshBase::~MeshBase()
	{ }

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* MeshBase::getRTTIStatic()
	{
		return MeshBaseRTTI::instance();
	}

	RTTITypeBase* MeshBase::getRTTI() const
	{
		return MeshBase::getRTTIStatic();
	}
}
