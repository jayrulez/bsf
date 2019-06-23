//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentCommandBuffer.h"

namespace bs { namespace ct
{
	SPtr<CommandBuffer> DiligentCommandBufferManager::createInternal(GpuQueueType type, UINT32 deviceIdx,
		UINT32 queueIdx, bool secondary)
	{
		CommandBuffer* buffer = new (bs_alloc<DiligentCommandBuffer>()) DiligentCommandBuffer(type, deviceIdx, queueIdx, secondary);
		return bs_shared_ptr(buffer);
	}
}}