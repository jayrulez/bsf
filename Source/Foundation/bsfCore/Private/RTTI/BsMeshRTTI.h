//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "BsCoreApplication.h"
#include "Mesh/BsMesh.h"
#include "Animation/BsSkeleton.h"
#include "Animation/BsMorphShapes.h"
#include "CoreThread/BsCoreThread.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class MeshResourceRTTI : public RTTIType<MeshResource, MeshResourceBase, MeshResourceRTTI>
	{
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_REFLPTR(mVertexDesc, 0)
			BS_RTTI_MEMBER_PLAIN(mIndexType, 1)
			BS_RTTI_MEMBER_PLAIN(mUsage, 2)
			BS_RTTI_MEMBER_REFLPTR(mSkeleton, 4)
			BS_RTTI_MEMBER_REFLPTR(mMorphShapes, 5)
		BS_END_RTTI_MEMBERS

		SPtr<MeshData> getMeshData(MeshResource* obj)
		{
			SPtr<MeshData> meshData = obj->allocBuffer();

			obj->readData(meshData);
			gCoreThread().submitAll(true);

			return meshData;
		}

		void setMeshData(MeshResource* obj, SPtr<MeshData> meshData)
		{
			obj->mCPUData = meshData;
		}

	public:
		MeshResourceRTTI()
		{
			addReflectablePtrField("mMeshData", 3, &MeshResourceRTTI::getMeshData, &MeshResourceRTTI::setMeshData);
		}

		void onDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			MeshResource* mesh = static_cast<MeshResource*>(obj);
			mesh->initialize();
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return MeshResource::createEmpty();
		}

		const String& getRTTIName() override
		{
			static String name = "Mesh";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_Mesh;
		}
	};

	/** @} */
	/** @endcond */
}
