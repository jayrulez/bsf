//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsDiligentPrerequisites.h"
#include "BsDiligentRenderAPI.h"
#include "RenderAPI/BsGpuProgram.h"
#include "Graphics/GraphicsEngine/interface/Shader.h"

namespace bs {
	namespace ct
	{
		/** @addtogroup Diligent
		 *  @{
		 */

		/**	Abstraction of a Diligent shader object. */
		class DiligentGpuProgram : public GpuProgram
		{
		public:
			virtual ~DiligentGpuProgram();

			/** Returns the name of the program entry point function. */
			const String& getEntryPoint() const { return mEntryPoint; }

		protected:
			friend class DiligentProgramFactory;
			friend class DiligentRenderAPI;

			DiligentGpuProgram(const GPU_PROGRAM_DESC& desc, GpuDeviceFlags deviceMask);

			/** @copydoc GpuProgram::initialize */
			void initialize() override;

			Diligent::RefCntAutoPtr<Diligent::IShader> m_pS;

		private:
			GpuDeviceFlags mDeviceMask;
			String mLanguage;
		};

		/** @} */
	}
}