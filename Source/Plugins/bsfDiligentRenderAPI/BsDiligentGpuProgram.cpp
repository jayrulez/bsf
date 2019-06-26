//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentGpuProgram.h"
#include "RenderAPI/BsGpuParams.h"
#include "RenderAPI/BsGpuParamDesc.h"
#include "Managers/BsGpuProgramManager.h"
#include "RenderAPI/BsVertexDeclaration.h"
#include "Managers/BsHardwareBufferManager.h"
#include "Profiling/BsRenderStats.h"
#include "FileSystem/BsFileSystem.h"
#include "FileSystem/BsDataStream.h"

namespace bs {
	namespace ct
	{
		DiligentGpuProgram::DiligentGpuProgram(const GPU_PROGRAM_DESC& desc, GpuDeviceFlags deviceMask)
			: GpuProgram(desc, deviceMask), mLanguage(desc.language), mDeviceMask(deviceMask)
		{

		}

		DiligentGpuProgram::~DiligentGpuProgram()
		{
			BS_INC_RENDER_STAT_CAT(ResDestroyed, RenderStatObject_GpuProgram);
		}

		void DiligentGpuProgram::initialize()
		{
			{
				Diligent::ShaderCreateInfo shaderCI;
				shaderCI.UseCombinedTextureSamplers = true;

				if (mLanguage == "hlsl")
				{
					shaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
				}
				else if (mLanguage == "glsl")
				{
					shaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_GLSL;
				}
				else if (mLanguage == "vksl")
				{
					shaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_DEFAULT;
				}
				
				switch (mType)
				{
				case GPT_VERTEX_PROGRAM:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
				case GPT_FRAGMENT_PROGRAM:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
				case GPT_GEOMETRY_PROGRAM:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_GEOMETRY;
				case GPT_DOMAIN_PROGRAM:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_DOMAIN;
				case GPT_HULL_PROGRAM:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_HULL;
				case GPT_COMPUTE_PROGRAM:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_COMPUTE;
				default:
					shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_UNKNOWN;
				}

				shaderCI.EntryPoint = mEntryPoint.data();
				shaderCI.Desc.Name = "Diligent GPU Program";
				shaderCI.Source = mSource.data();

				gDiligentRenderAPI().m_pDevice->CreateShader(shaderCI, &m_pS);
			}

			mIsCompiled = m_pS != nullptr;

			BS_INC_RENDER_STAT_CAT(ResCreated, RenderStatObject_GpuProgram);

			GpuProgram::initialize();
		}
	}
}
