//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsRenderAPI.h"

#include "RenderAPI/BsViewport.h"
#include "RenderAPI/BsRenderTarget.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Mesh/BsMesh.h"
#include "RenderAPI/BsGpuParams.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsRasterizerState.h"
#include "RenderAPI/BsGpuBuffer.h"
#include "RenderAPI/BsGpuPipelineState.h"

using namespace std::placeholders;

namespace bs
{
	void RenderAPI::setGpuParams(const SPtr<GpuParams>& gpuParams)
	{
		ct::RenderAPI::instancePtr()->setGpuParams(gpuParams->getCore(), nullptr);
	}

	void RenderAPI::setGraphicsPipeline(const SPtr<GraphicsPipelineState>& pipelineState)
	{
		ct::RenderAPI::instancePtr()->setGraphicsPipeline(pipelineState->getCore(), nullptr);
	}

	void RenderAPI::setComputePipeline(const SPtr<ComputePipelineState>& pipelineState)
	{
		ct::RenderAPI::instancePtr()->setComputePipeline(pipelineState->getCore(), nullptr);
	}

	void RenderAPI::setVertexBuffers(UINT32 index, const Vector<SPtr<VertexBuffer>>& buffers)
	{
		ct::RenderAPI::instancePtr()->setVertexBuffers(index, (SPtr<VertexBuffer>*)buffers.data(), (UINT32)buffers.size());
	}

	void RenderAPI::setIndexBuffer(const SPtr<IndexBuffer>& buffer)
	{
		ct::RenderAPI::instancePtr()->setIndexBuffer(buffer, nullptr);
	}

	void RenderAPI::setVertexDeclaration(const SPtr<VertexDeclaration>& vertexDeclaration)
	{
		ct::RenderAPI::instancePtr()->setVertexDeclaration(vertexDeclaration, nullptr);
	}

	void RenderAPI::setViewport(const Rect2& vp)
	{
		ct::RenderAPI::instancePtr()->setViewport(vp, nullptr);
	}

	void RenderAPI::setStencilRef(UINT32 value)
	{
		ct::RenderAPI::instancePtr()->setStencilRef(value, nullptr);
	}

	void RenderAPI::setDrawOperation(DrawOperationType op)
	{
		ct::RenderAPI::instancePtr()->setDrawOperation(op,
			nullptr);
	}

	void RenderAPI::setScissorRect(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom)
	{
		ct::RenderAPI::instancePtr()->setScissorRect(left, top, right, bottom,
			nullptr);
	}

	void RenderAPI::setRenderTarget(const SPtr<RenderTarget>& target, UINT32 readOnlyFlags,
									RenderSurfaceMask loadMask)
	{
		ct::RenderAPI::instancePtr()->setRenderTarget(target, readOnlyFlags, loadMask, nullptr);
	}

	void RenderAPI::clearRenderTarget(UINT32 buffers, const Color& color, float depth,
		UINT16 stencil, UINT8 targetMask)
	{
		ct::RenderAPI::instancePtr()->clearRenderTarget(buffers, color,
			depth, stencil, targetMask, nullptr);
	}

	void RenderAPI::clearViewport(UINT32 buffers, const Color& color, float depth, UINT16 stencil,
		UINT8 targetMask)
	{
		ct::RenderAPI::instancePtr()->clearViewport(buffers, color, depth,
			stencil, targetMask, nullptr);
	}

	void RenderAPI::swapBuffers(const SPtr<RenderTarget>& target)
	{
		ct::RenderAPI::instancePtr()->swapBuffers(target, 1);
	}

	void RenderAPI::draw(UINT32 vertexOffset, UINT32 vertexCount, UINT32 instanceCount)
	{
		ct::RenderAPI::instancePtr()->draw(vertexOffset,
			vertexCount, instanceCount, nullptr);
	}

	void RenderAPI::drawIndexed(UINT32 startIndex, UINT32 indexCount, UINT32 vertexOffset,
		UINT32 vertexCount, UINT32 instanceCount)
	{
		ct::RenderAPI::instancePtr()->drawIndexed(startIndex, indexCount, vertexOffset, vertexCount, instanceCount, nullptr);
	}

	void RenderAPI::dispatchCompute(UINT32 numGroupsX, UINT32 numGroupsY, UINT32 numGroupsZ)
	{
		ct::RenderAPI::instancePtr()->dispatchCompute(numGroupsX, numGroupsY, numGroupsZ, nullptr);
	}

	const VideoModeInfo& RenderAPI::getVideoModeInfo()
	{
		return ct::RenderAPI::instance().getVideoModeInfo();
	}

	void RenderAPI::convertProjectionMatrix(const Matrix4& matrix, Matrix4& dest)
	{
		ct::RenderAPI::instance().convertProjectionMatrix(matrix, dest);
	}

	namespace ct
	{
	RenderAPI::RenderAPI()
		: mCurrentCapabilities(nullptr), mNumDevices(0)
	{
	}

	RenderAPI::~RenderAPI()
	{
		// Base classes need to call virtual destroy_internal method instead of a destructor

		bs_deleteN(mCurrentCapabilities, mNumDevices);
		mCurrentCapabilities = nullptr;
	}

	SPtr<bs::RenderWindow> RenderAPI::initialize(const RENDER_WINDOW_DESC& primaryWindowDesc)
	{
		initialize();

		RENDER_WINDOW_DESC windowDesc = primaryWindowDesc;
		SPtr<bs::RenderWindow> renderWindow = bs::RenderWindow::create(windowDesc, nullptr);

		initializeWithWindow(renderWindow);

		return renderWindow;
	}

	void RenderAPI::initialize()
	{
		// Do nothing
	}

	void RenderAPI::initializeWithWindow(const SPtr<RenderWindow>& primaryWindow)
	{
	}

	void RenderAPI::destroy()
	{
		destroyCore();
	}

	void RenderAPI::destroyCore()
	{
		mActiveRenderTarget = nullptr;
	}

	const RenderAPICapabilities& RenderAPI::getCapabilities(UINT32 deviceIdx) const
	{
		if(deviceIdx >= mNumDevices)
		{
			BS_LOG(Warning, RenderBackend, "Invalid device index provided: {0}. Valid range is: [0, {1}).",
				deviceIdx, mNumDevices);
			return mCurrentCapabilities[0];
		}

		return mCurrentCapabilities[deviceIdx];
	}

	UINT32 RenderAPI::vertexCountToPrimCount(DrawOperationType type, UINT32 elementCount)
	{
		UINT32 primCount = 0;
		switch (type)
		{
		case DOT_POINT_LIST:
			primCount = elementCount;
			break;

		case DOT_LINE_LIST:
			primCount = elementCount / 2;
			break;

		case DOT_LINE_STRIP:
			primCount = elementCount - 1;
			break;

		case DOT_TRIANGLE_LIST:
			primCount = elementCount / 3;
			break;

		case DOT_TRIANGLE_STRIP:
			primCount = elementCount - 2;
			break;

		case DOT_TRIANGLE_FAN:
			primCount = elementCount - 2;
			break;
		}

		return primCount;
	}
	}
}
