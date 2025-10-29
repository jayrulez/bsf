//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Debug/BsDebug.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup CoreObject CoreObject
 *	Core object system layer providing the base classes for thread-aware core objects.
 *  @{
 */

/** @defgroup CoreThread Core thread
 *	Core objects and interaction with the core (rendering) thread.
 */

/** @} */
/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_COREOBJECT_EXPORT
#    else
#      if defined(BS_COREOBJECT_EXPORTS)
#        define BS_COREOBJECT_EXPORT __declspec(dllexport)
#      else
#        define BS_COREOBJECT_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_COREOBJECT_EXPORT
#    else
#      if defined(BS_COREOBJECT_EXPORTS)
#        define BS_COREOBJECT_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_COREOBJECT_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_COREOBJECT_HIDDEN
#else // Linux/Mac settings
#  define BS_COREOBJECT_EXPORT __attribute__ ((visibility ("default")))
#  define BS_COREOBJECT_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	class CoreObject;
	class CoreObjectCore;
	class CoreObjectManager;
	class CoreThread;
	class CommandQueueBase;

	template<class T>
	class CommandQueue;

	/** @addtogroup CoreThread
	 *  @{
	 */

	/** Helper class for syncing dirty data from sim CoreObject to core CoreObject and other way around. */
	class CoreSyncData
	{
	public:
		CoreSyncData()
			:data(nullptr), size(0)
		{ }

		CoreSyncData(UINT8* data, UINT32 size)
			:data(data), size(size)
		{ }

		/** Gets the internal data and checks the data is of valid size. */
		template<class T>
		const T& getData() const
		{
			assert(sizeof(T) == size);

			return *(T*)data;
		}

		/**	Returns a pointer to internal data buffer. */
		UINT8* getBuffer() const { return data; }

		/**	Returns the size of the internal data buffer. */
		UINT32 getBufferSize() const { return size; }

	private:
		UINT8* data;
		UINT32 size;
	};

	/** Used for marking a CoreObject dependency as dirty. */
	static constexpr INT32 DIRTY_DEPENDENCY_MASK = 1 << 31;

	// Log categories for bsfCoreObject
	BS_LOG_CATEGORY(CoreThread, 10)

	/** @} */
}
