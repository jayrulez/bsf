//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Material/BsMaterial.h"
#include "Material/BsShader.h"
#include "Material/BsTechnique.h"
#include "Material/BsPass.h"
#include "RenderAPI/BsRenderAPI.h"
#include "Resources/BsResources.h"
#include "Math/BsMatrixNxM.h"
#include "Math/BsVector3I.h"
#include "Math/BsVector4I.h"
#include "Material/BsMaterialParams.h"
#include "Material/BsGpuParamsSet.h"
#include "Animation/BsAnimationCurve.h"
#include "CoreThread/BsCoreObjectSync.h"
#include "Private/RTTI/BsShaderVariationRTTI.h"
#include "Serialization/BsBinarySerializer.h"
#include "FileSystem/BsDataStream.h"

namespace bs
{
	namespace ct
	{
	Material::Material(const SPtr<Shader>& shader, const ShaderVariation& variation)
	{
		mVariation = variation;
		setShader(shader);
	}
	
	Material::Material(const SPtr<Shader>& shader, const Vector<SPtr<Technique>>& techniques,
		const SPtr<MaterialParams>& materialParams, const ShaderVariation& variation)
	{
		mShader = shader;
		mParams = materialParams;
		mTechniques = techniques;
		mVariation = variation;
	}

	void Material::setShader(const SPtr<Shader>& shader)
	{
		mShader = shader;

		initializeTechniques();
	}

	void Material::setVariation(const ShaderVariation& variation)
	{
		mVariation = variation;
	}
	
	void Material::syncToCore(const CoreSyncData& data)
	{
		Bitstream stream(data.getBuffer(), data.getBufferSize());

		bool syncAllParams;
		rtti_read(syncAllParams, stream);

		UINT64 initialParamVersion = mParams != nullptr ? mParams->getParamVersion() : 1;
		if(syncAllParams)
			mParams = nullptr;

		SPtr<Shader>* shader = (SPtr<Shader>*)stream.cursor();

		mShader = *shader;
		shader->~SPtr<Shader>();
		stream.skipBytes(sizeof(SPtr<Shader>));

		UINT32 numTechniques;
		rtti_read(numTechniques, stream);

		mTechniques.resize(numTechniques);
		for(UINT32 i = 0; i < numTechniques; i++)
		{
			SPtr<Technique>* technique = (SPtr<Technique>*)stream.cursor();
			mTechniques[i] = *technique;
			technique->~SPtr<Technique>();
			stream.skipBytes(sizeof(SPtr<Technique>));
		}

		UINT32 paramsSize = 0;
		rtti_read(paramsSize, stream);
		if (mParams == nullptr && mShader != nullptr)
			mParams = bs_shared_ptr_new<MaterialParams>(mShader, initialParamVersion);

		if(mParams != nullptr && paramsSize > 0)
			mParams->setSyncData(stream.cursor(), paramsSize);

		stream.skipBytes(paramsSize);

		mVariation.clearParams();
		csync_read(mVariation, stream);
	}

	SPtr<Material> Material::create(const SPtr<Shader>& shader)
	{
		Material* material = new (bs_alloc<Material>()) Material(shader, ShaderVariation::EMPTY);
		SPtr<Material> materialPtr = bs_shared_ptr<Material>(material);
		materialPtr->_setThisPtr(materialPtr);
		materialPtr->initialize();

		return materialPtr;
	}
	}
}
