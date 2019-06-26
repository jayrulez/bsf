//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsDiligentPrerequisites.h"
#include "Managers/BsGpuProgramManager.h"

namespace bs {
	namespace ct
	{
		/** @addtogroup Diligent
		 *  @{
		 */

		 /**	Handles creation of Diligent GPU programs.  */
		class DiligentProgramFactory final : public GpuProgramFactory
		{
		public:
			DiligentProgramFactory() = default;
			~DiligentProgramFactory() = default;

			SPtr<GpuProgram> create(const GPU_PROGRAM_DESC& desc, GpuDeviceFlags deviceMask) override;
			SPtr<GpuProgram> create(GpuProgramType type, GpuDeviceFlags deviceMask) override;
			SPtr<GpuProgramBytecode> compileBytecode(const GPU_PROGRAM_DESC& desc) override;
		};

		/** @} */
	}
}