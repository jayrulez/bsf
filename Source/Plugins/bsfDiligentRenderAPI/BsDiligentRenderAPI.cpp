//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentRenderAPI.h"
#include "CoreThread/BsCoreThread.h"
#include "Math/BsMatrix4.h"
#include "RenderAPI/BsGpuParamDesc.h"
#include "RenderAPI/BsGpuParams.h"
#include "Managers/BsGpuProgramManager.h"
#include "BsDiligentCommandBuffer.h"
#include "BsDiligentTexture.h"
#include "BsDiligentBuffers.h"
#include "BsDiligentRenderTargets.h"
#include "BsDiligentRenderStates.h"
#include "BsDiligentQueries.h"

namespace bs { namespace ct
{
	const StringID& DiligentRenderAPI::getName() const
	{
		static StringID strName("DiligentRenderAPI");
		return strName;
	}

	void DiligentRenderAPI::initialize()
	{
		THROW_IF_NOT_CORE_THREAD;

		mVideoModeInfo = bs_shared_ptr_new<VideoModeInfo>();

		GPUInfo gpuInfo;
		gpuInfo.numGPUs = 0;

		PlatformUtility::_setGPUInfo(gpuInfo);

		CommandBufferManager::startUp<DiligentCommandBufferManager>();

		bs::TextureManager::startUp<bs::DiligentTextureManager>();
		TextureManager::startUp<DiligentTextureManager>();

		// Create hardware buffer manager		
		bs::HardwareBufferManager::startUp();
		HardwareBufferManager::startUp<DiligentHardwareBufferManager>();

		// Create render window manager
		bs::RenderWindowManager::startUp<bs::DiligentRenderWindowManager>();
		RenderWindowManager::startUp();

		// Create render state manager
		RenderStateManager::startUp<DiligentRenderStateManager>();

		// Pretend as if we can parse HLSL
		mDiligentProgramFactory = bs_new<DiligentProgramFactory>();
		GpuProgramManager::instance().addFactory("hlsl", mDiligentProgramFactory);

		mNumDevices = 1;
		mCurrentCapabilities = bs_newN<RenderAPICapabilities>(mNumDevices);
		mCurrentCapabilities->deviceName = "Diligent";
		mCurrentCapabilities->renderAPIName = getName();
		mCurrentCapabilities->deviceVendor = GPU_UNKNOWN;
				
		RenderAPI::initialize();
	}

	void DiligentRenderAPI::initializeWithWindow(const SPtr<RenderWindow>& primaryWindow)
	{
		QueryManager::startUp<DiligentQueryManager>();

		RenderAPI::initializeWithWindow(primaryWindow);
	}

	void DiligentRenderAPI::destroyCore()
	{
		THROW_IF_NOT_CORE_THREAD;

		mActiveRenderTarget = nullptr;

		if(mDiligentProgramFactory != nullptr)
		{
			bs_delete(mDiligentProgramFactory);
			mDiligentProgramFactory = nullptr;
		}

		QueryManager::shutDown();
		RenderStateManager::shutDown();
		RenderWindowManager::shutDown();
		bs::RenderWindowManager::shutDown();
		HardwareBufferManager::shutDown();
		bs::HardwareBufferManager::shutDown();
		TextureManager::shutDown();
		bs::TextureManager::shutDown();
		CommandBufferManager::shutDown();

		RenderAPI::destroyCore();
	}

	void DiligentRenderAPI::convertProjectionMatrix(const Matrix4& matrix, Matrix4& dest)
	{
		dest = matrix;
	}

	GpuParamBlockDesc DiligentRenderAPI::generateParamBlockDesc(const String& name, Vector<GpuParamDataDesc>& params)
	{
		GpuParamBlockDesc block;
		block.blockSize = 0;
		block.isShareable = true;
		block.name = name;
		block.slot = 0;
		block.set = 0;

		for (auto& param : params)
		{
			const GpuParamDataTypeInfo& typeInfo = bs::GpuParams::PARAM_SIZES.lookup[param.type];

			if (param.arraySize > 1)
			{
				// Arrays perform no packing and their elements are always padded and aligned to four component vectors
				UINT32 size;
				if(param.type == GPDT_STRUCT)
					size = Math::divideAndRoundUp(param.elementSize, 16U) * 4;
				else
					size = Math::divideAndRoundUp(typeInfo.size, 16U) * 4;

				block.blockSize = Math::divideAndRoundUp(block.blockSize, 4U) * 4;

				param.elementSize = size;
				param.arrayElementStride = size;
				param.cpuMemOffset = block.blockSize;
				param.gpuMemOffset = 0;

				// Last array element isn't rounded up to four component vectors unless it's a struct
				if(param.type != GPDT_STRUCT)
				{
					block.blockSize += size * (param.arraySize - 1);
					block.blockSize += typeInfo.size / 4;
				}
				else
					block.blockSize += param.arraySize * size;
			}
			else
			{
				UINT32 size;
				if(param.type == GPDT_STRUCT)
				{
					// Structs are always aligned and arounded up to 4 component vectors
					size = Math::divideAndRoundUp(param.elementSize, 16U) * 4;
					block.blockSize = Math::divideAndRoundUp(block.blockSize, 4U) * 4;
				}
				else
				{
					size = typeInfo.baseTypeSize * (typeInfo.numRows * typeInfo.numColumns) / 4;

					// Pack everything as tightly as possible as long as the data doesn't cross 16 byte boundary
					UINT32 alignOffset = block.blockSize % 4;
					if (alignOffset != 0 && size > (4 - alignOffset))
					{
						UINT32 padding = (4 - alignOffset);
						block.blockSize += padding;
					}
				}

				param.elementSize = size;
				param.arrayElementStride = size;
				param.cpuMemOffset = block.blockSize;
				param.gpuMemOffset = 0;

				block.blockSize += size;
			}

			param.paramBlockSlot = 0;
			param.paramBlockSet = 0;
		}

		// Constant buffer size must always be a multiple of 16
		if (block.blockSize % 4 != 0)
			block.blockSize += (4 - (block.blockSize % 4));

		return block;
	}
}}
