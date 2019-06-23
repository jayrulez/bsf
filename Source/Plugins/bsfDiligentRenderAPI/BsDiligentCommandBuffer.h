//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsDiligentPrerequisites.h"
#include "RenderAPI/BsCommandBuffer.h"
#include "Managers/BsCommandBufferManager.h"

namespace bs { namespace ct
{
	/** @addtogroup DiligentRenderAPI
	 *  @{
	 */

	/** Handles creation of Diligent command buffers. See CommandBuffer. */
	class DiligentCommandBufferManager final : public CommandBufferManager
	{
	public:
		/** @copydoc CommandBufferManager::createInternal() */
		SPtr<CommandBuffer> createInternal(GpuQueueType type, UINT32 deviceIdx = 0, UINT32 queueIdx = 0,
			bool secondary = false) override;
	};

	/** Command buffer implementation for the Diligent render backend. */
	class DiligentCommandBuffer final : public CommandBuffer
	{
	private:
		friend class DiligentCommandBufferManager;

		DiligentCommandBuffer(GpuQueueType type, UINT32 deviceIdx, UINT32 queueIdx, bool secondary)
			: CommandBuffer(type, deviceIdx, queueIdx, secondary)
		{ }
	};

	/** @} */
}}
