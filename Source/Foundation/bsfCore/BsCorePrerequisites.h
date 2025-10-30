//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"


#define BS_FORCE_SINGLETHREADED_RENDERING 0

/**
 * Runs the core thead on the application's main (initial) thread, rather than on a separate worker. Important for macOS
 * which has limitations regarding what can run on non-main threads.
 */
#define BS_CORE_THREAD_IS_MAIN 0

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_CORE_EXPORT
#    else
#      if defined(BS_CORE_EXPORTS)
#        define BS_CORE_EXPORT __declspec(dllexport)
#      else
#        define BS_CORE_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_CORE_EXPORT
#    else
#      if defined(BS_CORE_EXPORTS)
#        define BS_CORE_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_CORE_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_CORE_HIDDEN
#else // Linux/Mac settings
#  define BS_CORE_EXPORT __attribute__ ((visibility ("default")))
#  define BS_CORE_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Resources merged into Core - alias exports for compatibility
#define BS_RESOURCES_EXPORT BS_CORE_EXPORT
#define BS_RESOURCES_HIDDEN BS_CORE_HIDDEN

// Importer merged into Core - alias exports for compatibility
#define BS_IMPORTER_EXPORT BS_CORE_EXPORT

#include "String/BsStringID.h"

namespace bs
{
	// CoreObject forward declarations
	class CoreObject;
	class CoreObjectCore;
	class CoreObjectManager;
	class CoreThread;
	class CommandQueueBase;

	template<class T>
	class CommandQueue;

	// Resource forward declarations
	class Resource;
	class Resources;
	class ResourceManifest;
	class ResourceMetaData;
	class IResourceListener;
	class SavedResourceData;
	class GpuResourceData;
	class ShaderInclude;
	class ImportOptions;
	class GpuProgramImportOptions;
	class IShaderIncludeHandler;
	class SpecificImporter;
	class Importer;
	class StringTable;

	template<class T, bool WeakHandle>
	class TResourceHandle;

	template<class T>
	using ResourceHandle = TResourceHandle<T, false>;

	template<class T>
	using WeakResourceHandle = TResourceHandle<T, true>;

	using HResource = ResourceHandle<Resource>;
	using HShaderInclude = ResourceHandle<ShaderInclude>;

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

	/** @} */

	/** Helper template for mapping sim thread object types to core thread object types. */
	template<class T>
	struct CoreThreadType
	{ };

#define CORE_OBJECT_FORWARD_DECLARE(TYPE)				\
	class TYPE;											\
	namespace ct { class TYPE; }						\
	template<> struct CoreThreadType<TYPE> { typedef ct::TYPE Type; };

#define CORE_OBJECT_FORWARD_DECLARE_STRUCT(TYPE)		\
	struct TYPE;										\
	namespace ct { struct TYPE; }						\
	template<> struct CoreThreadType<TYPE> { typedef ct::TYPE Type; };

	/**
	 * Helper template that returns either a simulation thread or core thread version of an object, depending on the Core
	 * parameter. If the object has no core thread equivalent, same type is returned regardless of the Core parameter.
	 */
	template<class T, bool Core>
	struct CoreVariant { };

	template<class T>
	struct CoreVariant<T, false> { typedef T Type; };

	template<class T> struct CoreVariant<T, true> { typedef typename CoreThreadType<T>::Type Type; };

	/**
	 * Allows a simple way to define a member that can be both CoreObject variants depending on the Core template
	 * parameter.
	 */
	template<class T, bool Core>
	using CoreVariantType = typename CoreVariant<T, Core>::Type;

	template<class T, bool Core>
	struct CoreVariantHandle { };

	template<class T>
	struct CoreVariantHandle<T, false> { typedef ResourceHandle<T> Type; };

	template<class T> struct CoreVariantHandle<T, true> { typedef SPtr<typename CoreThreadType<T>::Type> Type; };

	/**
	 * Allows a simple way to define a member that can be both CoreObject variants depending on the Core template
	 * parameter. Sim thread type is wrapped in as a resource handle while the core thread variant is wrapped in a shared
	 * pointer.
	 */
	template<class T, bool Core>
	using CoreVariantHandleType = typename CoreVariantHandle<T, Core>::Type;
}

/************************************************************************/
/* 									RTTI                      			*/
/************************************************************************/
namespace bs
{
	/**
	 * Uniquely identifies a type in the RTTI system.
	 */
	enum TypeID_Resources
	{
		TID_ResourceHandle = 1009,
		TID_ResourceHandleData = 1011,
		TID_ImportOptions = 1048,
		TID_PixelData = 1062,
		TID_GpuResourceData = 1063,
		TID_VertexDataDesc = 1064,
		TID_ResourceManifest = 1067,
		TID_ResourceManifestEntry = 1068,
		TID_EmulatedParamBlock = 1069,
		TID_ResourceMetaData = 1071,
		TID_ShaderInclude = 1072,
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

	/**
	 * Note: Additional TypeIDs have been moved to their respective module Prerequisites files:
	 * - RenderAPI types -> BsRenderAPIPrerequisites.h (TypeID_RenderAPI)
	 * - Renderer types -> BsRendererPrerequisites.h (TypeID_Renderer)
	 * - Physics types -> BsPhysicsPrerequisites.h (TypeID_Physics)
	 * - Audio types -> BsAudioPrerequisites.h (TypeID_Audio)
	 * - Animation types -> BsAnimationPrerequisites.h (TypeID_Animation)
	 * - Text types -> BsTextPrerequisites.h (TypeID_Text)
	 * - GUI types -> BsGUIPrerequisites.h (TypeID_GUI)
	 * - Localization types -> BsLocalizationPrerequisites.h (TypeID_Localization)
	 * - Scene types -> BsScenePrerequisites.h (TypeID_Scene)
	 */
}

/************************************************************************/
/* 							Resource references                   		*/
/************************************************************************/

/**
 * Additional resource handle typedefs have been moved to their respective module Prerequisites files:
 * - Renderer types (HTexture, HMesh, HMaterial, HShader, HSpriteTexture, HVectorField) -> BsRendererPrerequisites.h
 * - Text types (HFont) -> BsTextPrerequisites.h
 * - Scene types (HPrefab) -> BsScenePrerequisites.h
 * - Localization types (HStringTable) -> BsLocalizationPrerequisites.h
 * - Physics types (HPhysicsMaterial, HPhysicsMesh) -> BsPhysicsPrerequisites.h
 * - Audio types (HAudioClip) -> BsAudioPrerequisites.h
 * - Animation types (HAnimationClip) -> BsAnimationPrerequisites.h
 */

/**
 * Game object handle typedefs have been moved to their respective module Prerequisites files:
 * - Scene types (HGameObject, HSceneObject, HComponent) -> BsScenePrerequisites.h
 * - Renderer types (HCamera, HRenderable, HLight, HReflectionProbe, HSkybox, HLightProbeVolume, HParticleSystem, HDecal) -> BsRendererPrerequisites.h
 * - Animation types (HAnimation, HBone) -> BsAnimationPrerequisites.h
 * - Physics types (HRigidbody, HCollider, HBoxCollider, HSphereCollider, HCapsuleCollider, HPlaneCollider, HMeshCollider, HJoint, HHingeJoint, HSliderJoint, HDistanceJoint, HSphericalJoint, HFixedJoint, HD6Joint, HCharacterController) -> BsPhysicsPrerequisites.h
 * - Audio types (HAudioSource, HAudioListener) -> BsAudioPrerequisites.h
 */

namespace bs
{
	BS_LOG_CATEGORY(CoreThread, 10)
	BS_LOG_CATEGORY(Resources, 20)
	BS_LOG_CATEGORY(Renderer, 21)
	BS_LOG_CATEGORY(Physics, 23)
	BS_LOG_CATEGORY(Audio, 24)
	BS_LOG_CATEGORY(RenderBackend, 25)
	BS_LOG_CATEGORY(BSLCompiler, 26)
	BS_LOG_CATEGORY(Particles, 27)
	BS_LOG_CATEGORY(FBXImporter, 29)
	BS_LOG_CATEGORY(PixelUtility, 30)
	BS_LOG_CATEGORY(Texture, 31)
	BS_LOG_CATEGORY(Mesh, 32)
	BS_LOG_CATEGORY(GUI, 33)
	BS_LOG_CATEGORY(Profiler, 34)
	BS_LOG_CATEGORY(Material, 35)
	BS_LOG_CATEGORY(FreeImageImporter, 36)
	BS_LOG_CATEGORY(Script, 37)
	BS_LOG_CATEGORY(Importer, 38)
	BS_LOG_CATEGORY(Network, 39)
}

#include "Utility/BsCommonTypes.h"
