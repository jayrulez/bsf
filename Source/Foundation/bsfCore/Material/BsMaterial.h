//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Resources/BsResource.h"
#include "Resources/BsIResourceListener.h"
#include "Material/BsMaterialParam.h"
#include "Material/BsMaterialParams.h"
#include "Material/BsTechnique.h"
#include "Animation/BsAnimationCurve.h"
#include "Math/BsVector2.h"
#include "Math/BsVector3.h"
#include "Math/BsVector4.h"
#include "Math/BsMatrix3.h"
#include "Math/BsMatrix4.h"
#include "Material/BsMaterialResource.h"

namespace bs
{
	namespace ct
	{
	/** @addtogroup Material-Internal
	 *  @{
	 */

	/** @copydoc MaterialBase */
	class BS_CORE_EXPORT Material : public CoreObject, public TMaterial<true>
	{
	public:
		~Material() = default;

		/** @copydoc bs::Material::setShader */
		void setShader(const SPtr<Shader>& shader);

		/**
		 * Set of parameters that determine which subset of techniques in the assigned shader should be used. Only the
		 * techniques that have the provided parameters with the provided values will match. This will control which
		 * technique is considered the default technique and which subset of techniques are searched during a call to
		 * findTechnique().
		 */
		void setVariation(const ShaderVariation& variation);

		/** Creates a new material with the specified shader. */
		static SPtr<Material> create(const SPtr<Shader>& shader);
	private:
		friend class bs::MaterialResource;

		Material() = default;
		Material(const SPtr<Shader>& shader, const ShaderVariation& variation);
		Material(const SPtr<Shader>& shader, const Vector<SPtr<Technique>>& techniques,
			const SPtr<MaterialParams>& materialParams, const ShaderVariation& variation);

		/** @copydoc CoreObject::syncToCore */
		void syncToCore(const CoreSyncData& data) override;
	};

	/** @} */	
	}
}
