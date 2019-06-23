//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsDiligentPrerequisites.h"
#include "Managers/BsRenderStateManager.h"

namespace bs { namespace ct
{
	/** @addtogroup DiligentRenderAPI
	 *  @{
	 */

	/**	Handles creation of Diligent pipeline states. */
	class DiligentRenderStateManager : public RenderStateManager
	{
	protected:
		/** @copydoc RenderStateManager::createSamplerStateInternal */
		SPtr<SamplerState> createSamplerStateInternal(const SAMPLER_STATE_DESC& desc, GpuDeviceFlags deviceMask) const override;

		/** @copydoc RenderStateManager::createBlendStateInternal */
		SPtr<BlendState> createBlendStateInternal(const BLEND_STATE_DESC& desc, UINT32 id) const override;

		/** @copydoc RenderStateManager::createRasterizerStateInternal */
		SPtr<RasterizerState> createRasterizerStateInternal(const RASTERIZER_STATE_DESC& desc, UINT32 id) const override;

		/** @copydoc RenderStateManager::createDepthStencilStateInternal */
		SPtr<DepthStencilState> createDepthStencilStateInternal(const DEPTH_STENCIL_STATE_DESC& desc, UINT32 id) const override;
	};

	/** @copydoc BlendState */
	class DiligentBlendState : public BlendState
	{
	public:
		DiligentBlendState(const BLEND_STATE_DESC& desc, UINT32 id)
			:BlendState(desc, id)
		{ }
	};

	/** @copydoc DepthStencilState */
	class DiligentDepthStencilState : public DepthStencilState 
	{
	public:
		DiligentDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc, UINT32 id)
			:DepthStencilState(desc, id)
		{ }
	};

	/** @copydoc RasterizerState */
	class DiligentRasterizerState : public RasterizerState 
	{
	public:
		DiligentRasterizerState(const RASTERIZER_STATE_DESC& desc, UINT32 id)
			:RasterizerState(desc, id)
		{ }
	};

	/** @copydoc SamplerState */
	class DiligentSamplerState : public SamplerState 
	{
	public:
		DiligentSamplerState(const SAMPLER_STATE_DESC& desc, GpuDeviceFlags deviceMask)
			:SamplerState(desc, deviceMask)
		{ }
	};

	/** @} */
}}