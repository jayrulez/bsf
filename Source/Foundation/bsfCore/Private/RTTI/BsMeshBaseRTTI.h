//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "Mesh/BsMeshBase.h"
#include "Error/BsException.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	BS_ALLOW_MEMCPY_SERIALIZATION(SubMesh);

	class MeshResourceBaseRTTI : public RTTIType<MeshResourceBase, Resource, MeshResourceBaseRTTI>
	{
		SubMesh& getSubMesh(MeshResourceBase* obj, UINT32 arrayIdx) { return obj->mProperties.mSubMeshes[arrayIdx]; }
		void setSubMesh(MeshResourceBase* obj, UINT32 arrayIdx, SubMesh& value) { obj->mProperties.mSubMeshes[arrayIdx] = value; }
		UINT32 getNumSubmeshes(MeshResourceBase* obj) { return (UINT32)obj->mProperties.mSubMeshes.size(); }
		void setNumSubmeshes(MeshResourceBase* obj, UINT32 numElements) { obj->mProperties.mSubMeshes.resize(numElements); }

		UINT32& getNumVertices(MeshResourceBase* obj) { return obj->mProperties.mNumVertices; }
		void setNumVertices(MeshResourceBase* obj, UINT32& value) { obj->mProperties.mNumVertices = value; }

		UINT32& getNumIndices(MeshResourceBase* obj) { return obj->mProperties.mNumIndices; }
		void setNumIndices(MeshResourceBase* obj, UINT32& value) { obj->mProperties.mNumIndices = value; }

	public:
		MeshResourceBaseRTTI()
		{
			addPlainField("mNumVertices", 0, &MeshResourceBaseRTTI::getNumVertices, &MeshResourceBaseRTTI::setNumVertices);
			addPlainField("mNumIndices", 1, &MeshResourceBaseRTTI::getNumIndices, &MeshResourceBaseRTTI::setNumIndices);

			addPlainArrayField("mSubMeshes", 2, &MeshResourceBaseRTTI::getSubMesh,
				&MeshResourceBaseRTTI::getNumSubmeshes, &MeshResourceBaseRTTI::setSubMesh, &MeshResourceBaseRTTI::setNumSubmeshes);
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			BS_EXCEPT(InternalErrorException, "Cannot instantiate an abstract class.");
			return nullptr;
		}

		const String& getRTTIName() override
		{
			static String name = "MeshBase";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_MeshBase;
		}
	};

	/** @} */
	/** @endcond */
}
