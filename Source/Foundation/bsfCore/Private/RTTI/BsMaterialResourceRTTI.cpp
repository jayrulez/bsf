//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Private/RTTI/BsMaterialResourceRTTI.h"
#include "Material/BsMaterialParams.h"

namespace bs
{
	void MaterialResourceRTTI::onDeserializationEnded(IReflectable* obj, SerializationContext* context)
	{
		MaterialResource* material = static_cast<MaterialResource*>(obj);
		material->initialize();

		if(!mMatParams)
			return;

		material->initializeTechniques();

		if (material->getNumTechniques() > 0)
			material->setParams(mMatParams);
	}

	SPtr<IReflectable> MaterialResourceRTTI::newRTTIObject()
	{
		return MaterialResource::createEmpty();
	}
}
