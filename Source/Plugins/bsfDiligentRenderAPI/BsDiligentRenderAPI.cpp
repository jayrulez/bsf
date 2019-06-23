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
#include "DiligentCore/Platforms/interface/PlatformDefinitions.h"

#if BS_PLATFORM == BS_PLATFORM_WIN32
#ifndef NOMINMAX
#   define NOMINMAX
#endif
#include <Windows.h>
#endif

#if D3D11_SUPPORTED
#   include "EngineFactoryD3D11.h"
#endif

#if D3D12_SUPPORTED
#   include "EngineFactoryD3D12.h"
#endif

#if GL_SUPPORTED || GLES_SUPPORTED
#   include "EngineFactoryOpenGL.h"
#endif

#if VULKAN_SUPPORTED
#   include "EngineFactoryVk.h"
#endif

namespace bs {
	namespace ct
	{
		DiligentRenderAPI::~DiligentRenderAPI()
		{
			m_DisplayModes.clear();
			m_pImmediateContext->Flush();
			m_pDeferredContexts.clear();
			m_pImmediateContext.Release();
			m_pSwapChain.Release();
			m_pDevice.Release();
		}

		void DiligentRenderAPI::initializeDiligentEngine(
#if PLATFORM_LINUX
			void *display,
#endif
			void *NativeWindowHandle)
		{
			Diligent::Uint32 AdapterId = 0;

			Diligent::SwapChainDesc SCDesc;
			SCDesc.SamplesCount = 1;

			std::vector<Diligent::IDeviceContext*> ppContexts;

			switch (m_DeviceType)
			{
#if D3D11_SUPPORTED
			case Diligent::DeviceType::D3D11:
			{
				Diligent::EngineD3D11CreateInfo EngineCI;
				getEngineInitializationAttribs(m_DeviceType, EngineCI);

#if ENGINE_DLL
				Diligent::GetEngineFactoryD3D11Type GetEngineFactoryD3D11 = nullptr;
				// Load the dll and import GetEngineFactoryD3D11() function
				LoadGraphicsEngineD3D11(GetEngineFactoryD3D11);
#endif
				auto* pFactoryD3D11 = GetEngineFactoryD3D11();
				m_pEngineFactory = pFactoryD3D11;
				Diligent::Uint32 NumAdapters = 0;
				pFactoryD3D11->EnumerateHardwareAdapters(NumAdapters, 0);
				std::vector<Diligent::HardwareAdapterAttribs> Adapters(NumAdapters);
				if (NumAdapters > 0)
					pFactoryD3D11->EnumerateHardwareAdapters(NumAdapters, Adapters.data());
				else
					LOGERR("Failed to find compatible hardware adapters");

				m_AdapterAttribs = Adapters[AdapterId];
				Diligent::Uint32 NumDisplayModes = 0;
				pFactoryD3D11->EnumerateDisplayModes(AdapterId, 0, Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB, NumDisplayModes, nullptr);
				m_DisplayModes.resize(NumDisplayModes);
				pFactoryD3D11->EnumerateDisplayModes(AdapterId, 0, Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB, NumDisplayModes, m_DisplayModes.data());

				ppContexts.resize(1 + EngineCI.NumDeferredContexts);
				pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &m_pDevice, ppContexts.data());

				if (NativeWindowHandle != nullptr)
					pFactoryD3D11->CreateSwapChainD3D11(m_pDevice, ppContexts[0], SCDesc, Diligent::FullScreenModeDesc{}, NativeWindowHandle, &m_pSwapChain);
			}
			break;
#endif

#if D3D12_SUPPORTED
			case Diligent::DeviceType::D3D12:
			{
#if ENGINE_DLL
				Diligent::GetEngineFactoryD3D12Type GetEngineFactoryD3D12 = nullptr;
				// Load the dll and import GetEngineFactoryD3D12() function
				LoadGraphicsEngineD3D12(GetEngineFactoryD3D12);
#endif
				auto* pFactoryD3D12 = GetEngineFactoryD3D12();
				m_pEngineFactory = pFactoryD3D12;
				Diligent::Uint32 NumAdapters = 0;
				pFactoryD3D12->EnumerateHardwareAdapters(NumAdapters, 0);
				std::vector<Diligent::HardwareAdapterAttribs> Adapters(NumAdapters);
				if (NumAdapters > 0)
					pFactoryD3D12->EnumerateHardwareAdapters(NumAdapters, Adapters.data());
				else
					LOGERR("Failed to find compatible hardware adapters");

				m_AdapterAttribs = Adapters[AdapterId];
				Diligent::Uint32 NumDisplayModes = 0;
				pFactoryD3D12->EnumerateDisplayModes(AdapterId, 0, Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB, NumDisplayModes, nullptr);
				m_DisplayModes.resize(NumDisplayModes);
				pFactoryD3D12->EnumerateDisplayModes(AdapterId, 0, Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB, NumDisplayModes, m_DisplayModes.data());

				Diligent::EngineD3D12CreateInfo EngD3D12Attribs;
				getEngineInitializationAttribs(m_DeviceType, EngD3D12Attribs);
				ppContexts.resize(1 + EngD3D12Attribs.NumDeferredContexts);
				pFactoryD3D12->CreateDeviceAndContextsD3D12(EngD3D12Attribs, &m_pDevice, ppContexts.data());

				if (!m_pSwapChain && NativeWindowHandle != nullptr)
					pFactoryD3D12->CreateSwapChainD3D12(m_pDevice, ppContexts[0], SCDesc, Diligent::FullScreenModeDesc{}, NativeWindowHandle, &m_pSwapChain);
			}
			break;
#endif

#if GL_SUPPORTED || GLES_SUPPORTED
			case Diligent::DeviceType::OpenGL:
			case Diligent::DeviceType::OpenGLES:
			{
#if !PLATFORM_MACOS
				BS_ASSERT(NativeWindowHandle != nullptr);
#endif
#if EXPLICITLY_LOAD_ENGINE_GL_DLL
				// Declare function pointer
				Diligent::GetEngineFactoryOpenGLType GetEngineFactoryOpenGL = nullptr;
				// Load the dll and import GetEngineFactoryOpenGL() function
				LoadGraphicsEngineOpenGL(GetEngineFactoryOpenGL);
#endif
				auto* pFactoryOpenGL = GetEngineFactoryOpenGL();
				m_pEngineFactory = pFactoryOpenGL;
				Diligent::EngineGLCreateInfo CreationAttribs;
				CreationAttribs.pNativeWndHandle = NativeWindowHandle;
#if PLATFORM_LINUX
				CreationAttribs.pDisplay = display;
#endif
				getEngineInitializationAttribs(m_DeviceType, CreationAttribs);
				if (CreationAttribs.NumDeferredContexts != 0)
				{
					LOGERR("Deferred contexts are not supported in OpenGL mode");
					CreationAttribs.NumDeferredContexts = 0;
				}
				ppContexts.resize(1 + CreationAttribs.NumDeferredContexts);
				pFactoryOpenGL->CreateDeviceAndSwapChainGL(
					CreationAttribs, &m_pDevice, ppContexts.data(), SCDesc, &m_pSwapChain);
			}
			break;
#endif

#if VULKAN_SUPPORTED
			case Diligent::DeviceType::Vulkan:
			{
#if EXPLICITLY_LOAD_ENGINE_VK_DLL
				Diligent::GetEngineFactoryVkType GetEngineFactoryVk = nullptr;
				// Load the dll and import GetEngineFactoryVk() function
				LoadGraphicsEngineVk(GetEngineFactoryVk);
#endif
				Diligent::EngineVkCreateInfo EngVkAttribs;
#ifdef _DEBUG
				EngVkAttribs.EnableValidation = true;
#endif

				getEngineInitializationAttribs(m_DeviceType, EngVkAttribs);
				ppContexts.resize(1 + EngVkAttribs.NumDeferredContexts);
				auto* pFactoryVk = GetEngineFactoryVk();
				m_pEngineFactory = pFactoryVk;
				pFactoryVk->CreateDeviceAndContextsVk(EngVkAttribs, &m_pDevice, ppContexts.data());

				if (!m_pSwapChain && NativeWindowHandle != nullptr)
					pFactoryVk->CreateSwapChainVk(m_pDevice, ppContexts[0], SCDesc, NativeWindowHandle, &m_pSwapChain);
			}
			break;
#endif

			default:
				LOGERR("Unknown device type");
				break;
			}

			m_pImmediateContext.Attach(ppContexts[0]);
			auto NumDeferredCtx = ppContexts.size() - 1;
			m_pDeferredContexts.resize(NumDeferredCtx);
			for (Diligent::Uint32 ctx = 0; ctx < NumDeferredCtx; ++ctx)
				m_pDeferredContexts[ctx].Attach(ppContexts[1 + ctx]);

			//InitializeDiligent(m_pEngineFactory, m_pDevice, ppContexts.data(), NumDeferredCtx, m_pSwapChain);

			/*std::vector<Diligent::IDeviceContext*> ppContexts(1 + m_pDeferredContexts.size());
			ppContexts[0] = m_pImmediateContext;
			Diligent::Uint32 NumDeferredCtx = static_cast<Diligent::Uint32>(m_pDeferredContexts.size());
			for (size_t ctx = 0; ctx < m_pDeferredContexts.size(); ++ctx)
				ppContexts[1 + ctx] = m_pDeferredContexts[ctx];*/
		}

		void DiligentRenderAPI::getEngineInitializationAttribs(Diligent::DeviceType DevType, Diligent::EngineCreateInfo& EngineCI)
		{
			switch (DevType)
			{
#if D3D11_SUPPORTED
			case Diligent::DeviceType::D3D11:
			{
				Diligent::EngineD3D11CreateInfo& EngineD3D11CI = static_cast<Diligent::EngineD3D11CreateInfo&>(EngineCI);
				EngineD3D11CI.DebugFlags =
					(Diligent::Uint32)Diligent::EngineD3D11DebugFlags::VerifyCommittedShaderResources |
					(Diligent::Uint32)Diligent::EngineD3D11DebugFlags::VerifyCommittedResourceRelevance;
			}
			break;
#endif

#if D3D12_SUPPORTED
			case Diligent::DeviceType::D3D12:
			{
				Diligent::EngineD3D12CreateInfo& EngineD3D12CI = static_cast<Diligent::EngineD3D12CreateInfo &>(EngineCI);
				EngineD3D12CI.GPUDescriptorHeapDynamicSize[0] = 32768;
				EngineD3D12CI.GPUDescriptorHeapSize[1] = 128;
				EngineD3D12CI.GPUDescriptorHeapDynamicSize[1] = 2048 - 128;
				EngineD3D12CI.DynamicDescriptorAllocationChunkSize[0] = 32;
				EngineD3D12CI.DynamicDescriptorAllocationChunkSize[1] = 8; // D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER
			}
			break;
#endif

#if VULKAN_SUPPORTED
			case Diligent::DeviceType::Vulkan:
			{
				// EngineVkCreateInfo& EngVkAttribs = static_cast<EngineVkCreateInfo&>(EngineCI);
			}
			break;
#endif

#if GL_SUPPORTED
			case Diligent::DeviceType::OpenGL:
			{
				// Nothing to do
			}
			break;
#endif

#if GLES_SUPPORTED
			case Diligent::DeviceType::OpenGLES:
			{
				// Nothing to do
			}
			break;
#endif

			default:
				LOGERR("Unknown device type");
				break;
			}
		}

		void DiligentRenderAPI::initCapabilities()
		{
			mNumDevices = 1;
			mCurrentCapabilities = bs_newN<RenderAPICapabilities>(mNumDevices);
			mCurrentCapabilities->deviceName = "Diligent";
			mCurrentCapabilities->renderAPIName = getName();
			mCurrentCapabilities->deviceVendor = GPU_UNKNOWN;
		}

		const StringID& DiligentRenderAPI::getName() const
		{
			static StringID strName("DiligentRenderAPI");
			return strName;
		}

		void DiligentRenderAPI::initialize()
		{
			THROW_IF_NOT_CORE_THREAD;

			initializeDiligentEngine(
#if PLATFORM_LINUX
				nullptr,
#endif
				nullptr);

			mVideoModeInfo = bs_shared_ptr_new<VideoModeInfo>(); // fix

			GPUInfo gpuInfo;
			gpuInfo.numGPUs = 1U;



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

			initCapabilities();

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

			if (mDiligentProgramFactory != nullptr)
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
					if (param.type == GPDT_STRUCT)
						size = Math::divideAndRoundUp(param.elementSize, 16U) * 4;
					else
						size = Math::divideAndRoundUp(typeInfo.size, 16U) * 4;

					block.blockSize = Math::divideAndRoundUp(block.blockSize, 4U) * 4;

					param.elementSize = size;
					param.arrayElementStride = size;
					param.cpuMemOffset = block.blockSize;
					param.gpuMemOffset = 0;

					// Last array element isn't rounded up to four component vectors unless it's a struct
					if (param.type != GPDT_STRUCT)
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
					if (param.type == GPDT_STRUCT)
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

		void DiligentRenderAPI::setGraphicsPipeline(const SPtr<GraphicsPipelineState>& pipelineState,
			const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setComputePipeline(const SPtr<ComputePipelineState>& pipelineState,
			const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setGpuParams(const SPtr<GpuParams>& gpuParams, const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setViewport(const Rect2& vp, const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setVertexBuffers(UINT32 index, SPtr<VertexBuffer>* buffers, UINT32 numBuffers,
			const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setIndexBuffer(const SPtr<IndexBuffer>& buffer, const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setVertexDeclaration(const SPtr<VertexDeclaration>& vertexDeclaration,
			const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::setDrawOperation(DrawOperationType op, const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::draw(UINT32 vertexOffset, UINT32 vertexCount, UINT32 instanceCount,
			const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::drawIndexed(UINT32 startIndex, UINT32 indexCount, UINT32 vertexOffset, UINT32 vertexCount,
			UINT32 instanceCount, const SPtr<CommandBuffer>& commandBuffer)
		{
			
		}

		void DiligentRenderAPI::dispatchCompute(UINT32 numGroupsX, UINT32 numGroupsY, UINT32 numGroupsZ,
			const SPtr<CommandBuffer>& commandBuffer)
		{
		}

		void DiligentRenderAPI::setScissorRect(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom,
			const SPtr<CommandBuffer>& commandBuffer)
		{
		}

		void DiligentRenderAPI::setStencilRef(UINT32 value, const SPtr<CommandBuffer>& commandBuffer)
		{
		}

		void DiligentRenderAPI::clearViewport(UINT32 buffers, const Color& color, float depth, UINT16 stencil, UINT8 targetMask,
			const SPtr<CommandBuffer>& commandBuffer)
		{
		}

		void DiligentRenderAPI::clearRenderTarget(UINT32 buffers, const Color& color, float depth, UINT16 stencil,
			UINT8 targetMask, const SPtr<CommandBuffer>& commandBuffer)
		{
		}

		void DiligentRenderAPI::setRenderTarget(const SPtr<RenderTarget>& target, UINT32 readOnlyFlags,
			RenderSurfaceMask loadMask, const SPtr<CommandBuffer>& commandBuffer)
		{
		}

		void DiligentRenderAPI::swapBuffers(const SPtr<RenderTarget>& target, UINT32 syncMask)
		{
			THROW_IF_NOT_CORE_THREAD;
		}

		void DiligentRenderAPI::addCommands(const SPtr<CommandBuffer>& commandBuffer, const SPtr<CommandBuffer>& secondary)
		{
			BS_EXCEPT(NotImplementedException, "Secondary command buffers not implemented");
		}

		void DiligentRenderAPI::submitCommandBuffer(const SPtr<CommandBuffer>& commandBuffer, UINT32 syncMask)
		{
			THROW_IF_NOT_CORE_THREAD;
		}

		void DiligentRenderAPI::convertProjectionMatrix(const Matrix4& matrix, Matrix4& dest)
		{
			dest = matrix;

			// Flip Y axis
			dest[1][1] = -dest[1][1];

			// Convert depth range from [-1,1] to [0,1]
			dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
			dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
			dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
			dest[2][3] = (dest[2][3] + dest[3][3]) / 2;
		}

		/*VulkanCommandBuffer* DiligentRenderAPI::getCB(const SPtr<CommandBuffer>& buffer)
		{
			if (buffer != nullptr)
				return static_cast<VulkanCommandBuffer*>(buffer.get());

			return static_cast<VulkanCommandBuffer*>(mMainCommandBuffer.get());
		}*/

		DiligentRenderAPI& gDiligentRenderAPI()
		{
			return static_cast<DiligentRenderAPI&>(RenderAPI::instance());
		}
	}
}
