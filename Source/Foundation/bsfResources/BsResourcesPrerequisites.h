//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCoreObjectPrerequisites.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup Resources Resources
 *	Resource management layer that provides core resource types and resource management functionality (loading, saving, etc.).
 *  @{
 */

/** @defgroup Resources-Engine Resources
  *	Core resource types and resource management functionality (loading, saving, etc.).
  */

/** @} */
/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_RESOURCES_EXPORT
#    else
#      if defined(BS_RESOURCES_EXPORTS)
#        define BS_RESOURCES_EXPORT __declspec(dllexport)
#      else
#        define BS_RESOURCES_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_RESOURCES_EXPORT
#    else
#      if defined(BS_RESOURCES_EXPORTS)
#        define BS_RESOURCES_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_RESOURCES_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_RESOURCES_HIDDEN
#else // Linux/Mac settings
#  define BS_RESOURCES_EXPORT __attribute__ ((visibility ("default")))
#  define BS_RESOURCES_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	class Resource;
	class Resources;
	class ResourceManifest;
	class ResourceMetaData;
	class IResourceListener;
	class SavedResourceData;
	class GpuResourceData;

	template<class T, bool WeakHandle>
	class TResourceHandle;

	template<class T>
	using ResourceHandle = TResourceHandle<T, false>;

	template<class T>
	using WeakResourceHandle = TResourceHandle<T, true>;

	using HResource = ResourceHandle<Resource>;

	/**
	 * Uniquely identifies a type in the RTTI system.
	 */
	enum TypeID_Resources
	{
		TID_ResourceHandle = 1009,
		TID_ResourceHandleData = 1011,
		TID_GpuResourceData = 1063,
		TID_ResourceManifest = 1067,
		TID_ResourceManifestEntry = 1068,
		TID_ResourceMetaData = 1071,
		TID_ResourceDependencies = 1074,
		TID_WeakResourceHandle = 1087,
	};

	/** Flags that are provided to the serialization system to control serialization/deserialization. */
	enum SerializationFlags
	{
		/**
		 * Used when deserializing resources. Lets the system know not to discard any intermediate resource data that might
		 * be required if the resource needs to be serialized.
		 */
		SF_KeepResourceSourceData = 1 << 0,

		/** Only serializes elements with network replication flag enabled. */
		SF_ReplicableOnly = 1 << 1
	};

	// Log categories for bsfResources
	BS_LOG_CATEGORY(Resources, 20)
}
