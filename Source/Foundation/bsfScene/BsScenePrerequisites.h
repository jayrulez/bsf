//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Debug/BsDebug.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup Scene Scene
 *	Scene management layer that provides scene objects, components, and prefab functionality.
 *  @{
 */

/** @defgroup Scene-Engine Scene
  *	Scene objects, components, and prefab functionality.
  */

/** @} */
/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_SCENE_EXPORT
#    else
#      if defined(BS_SCENE_EXPORTS)
#        define BS_SCENE_EXPORT __declspec(dllexport)
#      else
#        define BS_SCENE_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_SCENE_EXPORT
#    else
#      if defined(BS_SCENE_EXPORTS)
#        define BS_SCENE_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_SCENE_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_SCENE_HIDDEN
#else // Linux/Mac settings
#  define BS_SCENE_EXPORT __attribute__ ((visibility ("default")))
#  define BS_SCENE_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	class GameObject;
	class GameObjectHandleBase;
	class GameObjectManager;
	class SceneObject;
	class SceneManager;
	class Component;
	class Prefab;
	class PrefabDiff;
	class PrefabUtility;
	class Transform;
	class SceneActor;
	class SceneInstance;

	template<class T>
	class GameObjectHandle;

	/**	Contains instance data that is held by all GameObject handles. */
	struct GameObjectInstanceData
	{
		GameObjectInstanceData()
			:object(nullptr), mInstanceId(0)
		{ }

		SPtr<GameObject> object;
		UINT64 mInstanceId;
	};

	typedef SPtr<GameObjectInstanceData> GameObjectInstanceDataPtr;

	/**
	 * Uniquely identifies a type in the RTTI system.
	 */
	enum TypeID_Scene
	{
		TID_Component = 1006,
		TID_SceneObject = 1059,
		TID_GameObject = 1060,
		TID_GameObjectHandleBase = 1066,
		TID_Prefab = 1077,
		TID_PrefabDiff = 1078,
		TID_PrefabObjectDiff = 1079,
		TID_PrefabComponentDiff = 1080,
		TID_Transform = 1139,
		TID_SceneActor = 1140,
		TID_CoreSerializationContext = 1186,
	};

	/** Flags used for notifying child scene object and components when a transform has been changed. */
	enum TransformChangedFlags
	{
		TCF_None = 0x00, /**< Component will not be notified about any events relating to the transform. */
		TCF_Transform = 0x01, /**< Component will be notified when the its position, rotation or scale has changed. */
		TCF_Parent = 0x02, /**< Component will be notified when its parent changes. */
		TCF_Mobility = 0x04 /**< Component will be notified when mobility state changes. */
	};

	// Component type mapping
	template<class T>
	struct ComponentType
	{ };

#define COMPONENT_FORWARD_DECLARE(TYPE)								\
	class C##TYPE;													\
	template<> struct ComponentType<TYPE> { typedef C##TYPE Type; };

	COMPONENT_FORWARD_DECLARE(Collider)
	COMPONENT_FORWARD_DECLARE(Rigidbody)
	COMPONENT_FORWARD_DECLARE(BoxCollider)
	COMPONENT_FORWARD_DECLARE(SphereCollider)
	COMPONENT_FORWARD_DECLARE(PlaneCollider)
	COMPONENT_FORWARD_DECLARE(CapsuleCollider)
	COMPONENT_FORWARD_DECLARE(MeshCollider)
	COMPONENT_FORWARD_DECLARE(Joint)
	COMPONENT_FORWARD_DECLARE(HingeJoint)
	COMPONENT_FORWARD_DECLARE(DistanceJoint)
	COMPONENT_FORWARD_DECLARE(FixedJoint)
	COMPONENT_FORWARD_DECLARE(SphericalJoint)
	COMPONENT_FORWARD_DECLARE(SliderJoint)
	COMPONENT_FORWARD_DECLARE(D6Joint)
	COMPONENT_FORWARD_DECLARE(CharacterController)
	COMPONENT_FORWARD_DECLARE(Camera)
	COMPONENT_FORWARD_DECLARE(Renderable)
	COMPONENT_FORWARD_DECLARE(Light)
	COMPONENT_FORWARD_DECLARE(Animation)
	COMPONENT_FORWARD_DECLARE(Bone)
	COMPONENT_FORWARD_DECLARE(AudioSource)
	COMPONENT_FORWARD_DECLARE(AudioListener)
	COMPONENT_FORWARD_DECLARE(ReflectionProbe)
	COMPONENT_FORWARD_DECLARE(Skybox)
	COMPONENT_FORWARD_DECLARE(LightProbeVolume)
	COMPONENT_FORWARD_DECLARE(ParticleSystem)
	COMPONENT_FORWARD_DECLARE(Decal)

	/** Helper type that can contain either a component or scene actor version of an object. */
	template<class T>
	struct ComponentOrActor
	{
		using ComponentType = typename ComponentType<T>::Type;
		using HandleType = GameObjectHandle<ComponentType>;

		ComponentOrActor() = default;

		ComponentOrActor(const GameObjectHandle<ComponentType>& component)
			:mComponent(component)
		{ }

		ComponentOrActor(const SPtr<T>& actor)
			:mActor(actor)
		{ }

		/** Returns true if both the component and the actor fields are not assigned. */
		bool empty() const
		{
			return !mActor && !mComponent;
		}

		/** Returns the assigned value as a scene actor. */
		SPtr<T> getActor() const
		{
			if(mActor)
				return mActor;

			return mComponent->_getInternal();
		}

		/** Returns the assigned value as a component. */
		HandleType getComponent() const
		{
			return mComponent;
		}

	private:
		GameObjectHandle<ComponentType> mComponent;
		SPtr<T> mActor;
	};

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<Prefab> HPrefab;

	/************************************************************************/
	/* 						Game object handle references             		*/
	/************************************************************************/

	template<class T>
	class GameObjectHandle;

	// Game object handles
	typedef GameObjectHandle<GameObject> HGameObject;
	typedef GameObjectHandle<SceneObject> HSceneObject;
	typedef GameObjectHandle<Component> HComponent;

	/**
	 * Controls what kind of mobility restrictions a scene object has. This is used primarily as a performance hint to
	 * other systems. Generally the more restricted the mobility the higher performance can be achieved.
	 */
	enum class BS_SCRIPT_EXPORT() ObjectMobility
	{
		/** Scene object can be moved and has no mobility restrictions. */
		Movable,
		/**
		 * Scene object isn't allowed to be moved but is allowed to be visually changed in other ways (e.g. changing the
		 * displayed mesh or light intensity (depends on attached components).
		 */
		Immovable,
		/** Scene object isn't allowed to be moved nor is it allowed to be visually changed. Object must be fully static. */
		Static
	};

	// Log categories for bsfScene
	BS_LOG_CATEGORY(Scene, 22)
}
