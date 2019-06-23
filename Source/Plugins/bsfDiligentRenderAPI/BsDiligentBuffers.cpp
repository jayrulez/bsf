//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsDiligentBuffers.h"

namespace bs { namespace ct
{
	SPtr<VertexBuffer> DiligentHardwareBufferManager::createVertexBufferInternal(const VERTEX_BUFFER_DESC& desc, 
		GpuDeviceFlags deviceMask)
	{
		SPtr<DiligentVertexBuffer> ret = bs_shared_ptr_new<DiligentVertexBuffer>(desc, deviceMask);
		ret->_setThisPtr(ret);

		return ret;
	}

	SPtr<IndexBuffer> DiligentHardwareBufferManager::createIndexBufferInternal(const INDEX_BUFFER_DESC& desc, 
		GpuDeviceFlags deviceMask)
	{
		SPtr<DiligentIndexBuffer> ret = bs_shared_ptr_new<DiligentIndexBuffer>(desc, deviceMask);
		ret->_setThisPtr(ret);

		return ret;
	}

	SPtr<GpuParamBlockBuffer> DiligentHardwareBufferManager::createGpuParamBlockBufferInternal(UINT32 size, 
		GpuBufferUsage usage, GpuDeviceFlags deviceMask)
	{
		SPtr<GpuParamBlockBuffer> paramBlockBufferPtr = bs_shared_ptr_new<DiligentGpuParamBlockBuffer>(size, usage, deviceMask);
		paramBlockBufferPtr->_setThisPtr(paramBlockBufferPtr);

		return paramBlockBufferPtr;
	}

	SPtr<GpuBuffer> DiligentHardwareBufferManager::createGpuBufferInternal(const GPU_BUFFER_DESC& desc,
		GpuDeviceFlags deviceMask)
	{
		SPtr<DiligentGpuBuffer> bufferPtr = bs_shared_ptr_new<DiligentGpuBuffer>(desc, deviceMask);
		bufferPtr->_setThisPtr(bufferPtr);

		return bufferPtr;
	}

	SPtr<GpuBuffer> DiligentHardwareBufferManager::createGpuBufferInternal(const GPU_BUFFER_DESC& desc,
		SPtr<HardwareBuffer> underlyingBuffer)
	{
		SPtr<DiligentGpuBuffer> bufferPtr = bs_shared_ptr_new<DiligentGpuBuffer>(desc, std::move(underlyingBuffer));
		bufferPtr->_setThisPtr(bufferPtr);

		return bufferPtr;
	}

	static void deleteBuffer(HardwareBuffer* buffer)
	{
		bs_pool_delete(static_cast<DiligentHardwareBuffer*>(buffer));
	}

	DiligentGpuBuffer::DiligentGpuBuffer(const GPU_BUFFER_DESC& desc, GpuDeviceFlags deviceMask)
		: GpuBuffer(desc, deviceMask)
	{ }

	DiligentGpuBuffer::DiligentGpuBuffer(const GPU_BUFFER_DESC& desc, SPtr<HardwareBuffer> underlyingBuffer)
		: GpuBuffer(desc, std::move(underlyingBuffer))
	{ }

	void DiligentGpuBuffer::initialize()
	{
		const GpuBufferProperties& props = getProperties();
		mBufferDeleter = &deleteBuffer;

		// Create a new buffer if not wrapping an external one
		if(!mBuffer)
			mBuffer = bs_pool_new<DiligentHardwareBuffer>(props.getUsage(), props.getElementCount(), props.getElementSize());

		GpuBuffer::initialize();
	}

	DiligentGpuParamBlockBuffer::DiligentGpuParamBlockBuffer(UINT32 size, GpuBufferUsage usage, GpuDeviceFlags deviceMask)
		:GpuParamBlockBuffer(size, usage, deviceMask)
	{ }

	void DiligentGpuParamBlockBuffer::initialize()
	{
		mBuffer = bs_pool_new<DiligentHardwareBuffer>(mUsage, 1, mSize);
		GpuParamBlockBuffer::initialize();
	}

	DiligentHardwareBuffer::DiligentHardwareBuffer(GpuBufferUsage usage, UINT32 elementCount, UINT32 elementSize)
		: HardwareBuffer(elementCount * elementSize, usage, GDF_DEFAULT)
	{ }

	void* DiligentHardwareBuffer::map(UINT32 offset, UINT32 length, GpuLockOptions options, UINT32 deviceIdx, UINT32 queueIdx)
	{
		assert(mStagingBuffer == nullptr);

		mStagingBuffer = bs_alloc(mSize);
		return mStagingBuffer;
	}

	void DiligentHardwareBuffer::unmap()
	{
		bs_free(mStagingBuffer);
		mStagingBuffer = nullptr;
	}

	DiligentIndexBuffer::DiligentIndexBuffer(const INDEX_BUFFER_DESC& desc, GpuDeviceFlags deviceMask)
		:IndexBuffer(desc, deviceMask)
	{ }

	void DiligentIndexBuffer::initialize()
	{
		mBuffer = bs_pool_new<DiligentHardwareBuffer>(mUsage, 1, mSize);
		mBufferDeleter = &deleteBuffer;

		IndexBuffer::initialize();
	}

	DiligentVertexBuffer::DiligentVertexBuffer(const VERTEX_BUFFER_DESC& desc, GpuDeviceFlags deviceMask)
		: VertexBuffer(desc, deviceMask)
	{ }

	void DiligentVertexBuffer::initialize()
	{
		mBuffer = bs_pool_new<DiligentHardwareBuffer>(mUsage, 1, mSize);
		mBufferDeleter = &deleteBuffer;

		VertexBuffer::initialize();
	}
}}
