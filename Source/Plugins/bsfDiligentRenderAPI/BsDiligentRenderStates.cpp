//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentRenderStates.h"

namespace bs { namespace ct
{
	SPtr<SamplerState> DiligentRenderStateManager::createSamplerStateInternal(const SAMPLER_STATE_DESC& desc, 
		GpuDeviceFlags deviceMask) const
	{
		SPtr<DiligentSamplerState> ret = bs_shared_ptr_new<DiligentSamplerState>(desc, deviceMask);
		ret->_setThisPtr(ret);

		return ret;
	}

	SPtr<BlendState> DiligentRenderStateManager::createBlendStateInternal(const BLEND_STATE_DESC& desc, UINT32 id) const
	{
		SPtr<DiligentBlendState> ret = bs_shared_ptr_new<DiligentBlendState>(desc, id);
		ret->_setThisPtr(ret);

		return ret;
	}

	SPtr<RasterizerState> DiligentRenderStateManager::createRasterizerStateInternal(const RASTERIZER_STATE_DESC& desc, UINT32 id) const
	{
		SPtr<DiligentRasterizerState> ret = bs_shared_ptr_new<DiligentRasterizerState>(desc, id);
		ret->_setThisPtr(ret);

		return ret;
	}

	SPtr<DepthStencilState> DiligentRenderStateManager::createDepthStencilStateInternal(const DEPTH_STENCIL_STATE_DESC& desc, UINT32 id) const
	{
		SPtr<DiligentDepthStencilState> ret = bs_shared_ptr_new<DiligentDepthStencilState>(desc, id);
		ret->_setThisPtr(ret);

		return ret;
	}
}}