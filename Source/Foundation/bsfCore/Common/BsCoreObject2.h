//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Threading/BsAsyncOp.h"

namespace bs
{
	/** @addtogroup Common
	 *  @{
	 */

	/**
	 * Core objects provide a standardized way to initialize/destroy objects.
	 *
	 */
	class BS_CORE_EXPORT CoreObject2
	{
	protected:
		/** Values that represent current state of the core object */
		enum Flags
		{
			CGO2_NONE = 0x00, /* Object has not yet been initialized or destroyed. */
			CGO2_INITIALIZED = 0x01, /**< Object's initialize() method has been called. */
			CGO2_DESTROYED = 0x02 /**< Object has been destroyed and shouldn't be used. */
		};

	public:
		/**
		 * Frees all the data held by this object.
		 * 			
		 */
		virtual void destroy();

		/**
		 * Initializes all the internal resources of this object. Must be called right after construction. Generally you
		 * should call this from a factory method to avoid the issue where user forgets to call it.
		 *
		 */
		virtual void initialize();

		/** Returns true if the object has been initialized. Non-initialized object should not be used. */
		bool isInitialized() const { return (mFlags & CGO2_INITIALIZED) != 0; }

		/** Returns true if the object has been destroyed. Destroyed object should not be used. */
		bool isDestroyed() const { return (mFlags & CGO2_DESTROYED) != 0; }

		/** Returns an unique identifier for this object. */
		UINT64 getInternalID() const { return mInternalID; }

		/** Returns a shared_ptr version of "this" pointer. */
		SPtr<CoreObject2> getThisPtr() const { return mThis.lock(); }

	public: // ***** INTERNAL ******
		/** @name Internal
		 *  @{
		 */

		/**
		 * Sets a shared this pointer to this object. This must be called immediately after construction, but before
		 * initialize().
		 *
		 * @note	This should be called by the factory creation methods so user doesn't have to call it manually.
		 */
		void _setThisPtr(SPtr<CoreObject2> ptrThis);

		/** Schedules the object to be destroyed, and then deleted. */
		template<class T, class MemAlloc>
		static void _delete(CoreObject2* obj)
		{
			if (!obj->isDestroyed())
				obj->destroy();

			bs_delete<T, MemAlloc>((T*)obj);
		}

		/** @} */
	protected:
		/**
		 * Constructs a new core object.
		 *
		 */
		CoreObject2();
		virtual ~CoreObject2();

		void setIsDestroyed(bool destroyed) { mFlags = destroyed ? mFlags | CGO2_DESTROYED : mFlags & ~CGO2_DESTROYED; }
	private:
		friend class CoreObject2Manager;

		volatile UINT8 mFlags;
		UINT64 mInternalID; // ID == 0 is not a valid ID
		std::weak_ptr<CoreObject2> mThis;
	};

	/**
	 * Creates a new core object using the specified allocators and returns a shared pointer to it.
	 *
	 */
	template<class Type, class MainAlloc, class PtrDataAlloc, class... Args>
	SPtr<Type> bs_core_ptr2_new(Args &&...args)
	{
		return SPtr<Type>(bs_new<Type, MainAlloc>(std::forward<Args>(args)...),
			&CoreObject2::_delete<Type, MainAlloc>, StdAlloc<Type, PtrDataAlloc>());
	}

	/**
	 * Creates a new core object using the specified allocator and returns a shared pointer to it.
	 *
	 */
	template<class Type, class MainAlloc, class... Args>
	SPtr<Type> bs_core_ptr2_new(Args &&...args)
	{
		return SPtr<Type>(bs_new<Type, MainAlloc>(std::forward<Args>(args)...),
			&CoreObject2::_delete<Type, MainAlloc>, StdAlloc<Type, GenAlloc>());
	}

	/**
	 * Creates a new core object and returns a shared pointer to it.
	 *
	 */
	template<class Type, class... Args>
	SPtr<Type> bs_core_ptr2_new(Args &&...args)
	{
		return SPtr<Type>(bs_new<Type, GenAlloc>(std::forward<Args>(args)...),
			&CoreObject2::_delete<Type, GenAlloc>, StdAlloc<Type, GenAlloc>());
	}

	/**
	 * Creates a core object shared pointer using a previously constructed object.
	 *
	 */
	template<class Type, class MainAlloc = GenAlloc, class PtrDataAlloc = GenAlloc>
	SPtr<Type> bs_core_ptr2(Type* data)
	{
		return SPtr<Type>(data, &CoreObject2::_delete<Type, MainAlloc>, StdAlloc<Type, PtrDataAlloc>());
	}

	/** @} */
}

