//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup Physics Physics
 *	Physics simulation and collision detection.
 *  @{
 */

/** @defgroup Physics-Engine Physics
  *	Physics simulation, collision detection, rigidbodies, and joints.
  */

/** @} */
/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_PHYSICS_EXPORT
#    else
#      if defined(BS_PHYSICS_EXPORTS)
#        define BS_PHYSICS_EXPORT __declspec(dllexport)
#      else
#        define BS_PHYSICS_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_PHYSICS_EXPORT
#    else
#      if defined(BS_PHYSICS_EXPORTS)
#        define BS_PHYSICS_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_PHYSICS_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_PHYSICS_HIDDEN
#else // Linux/Mac settings
#  define BS_PHYSICS_EXPORT __attribute__ ((visibility ("default")))
#  define BS_PHYSICS_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	class Physics;
	class PhysicsManager;
	class PhysicsMaterial;
	class Rigidbody;
	class Collider;
	class BoxCollider;
	class SphereCollider;
	class CapsuleCollider;
	class PlaneCollider;
	class MeshCollider;
	class FCollider;
	class PhysicsMesh;
	class Joint;
	class FJoint;
	class FixedJoint;
	class DistanceJoint;
	class HingeJoint;
	class SliderJoint;
	class SphericalJoint;
	class D6Joint;
	class CharacterController;

	struct CollisionData;

	// Desc structs
	struct CHAR_CONTROLLER_DESC;
	struct JOINT_DESC;
	struct FIXED_JOINT_DESC;
	struct DISTANCE_JOINT_DESC;
	struct HINGE_JOINT_DESC;
	struct SLIDER_JOINT_DESC;
	struct SPHERICAL_JOINT_DESC;
	struct D6_JOINT_DESC;

	/** Bitmask that includes all physics layers. */
	#define BS_ALL_LAYERS 0xFFFFFFFFFFFFFFFF

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<PhysicsMaterial> HPhysicsMaterial;
	typedef ResourceHandle<PhysicsMesh> HPhysicsMesh;

	/************************************************************************/
	/* 						Game object handle references             		*/
	/************************************************************************/

	template<class T>
	class GameObjectHandle;

	// Game object handles
	typedef GameObjectHandle<CRigidbody> HRigidbody;
	typedef GameObjectHandle<CCollider> HCollider;
	typedef GameObjectHandle<CBoxCollider> HBoxCollider;
	typedef GameObjectHandle<CSphereCollider> HSphereCollider;
	typedef GameObjectHandle<CCapsuleCollider> HCapsuleCollider;
	typedef GameObjectHandle<CPlaneCollider> HPlaneCollider;
	typedef GameObjectHandle<CMeshCollider> HMeshCollider;
	typedef GameObjectHandle<CJoint> HJoint;
	typedef GameObjectHandle<CHingeJoint> HHingeJoint;
	typedef GameObjectHandle<CSliderJoint> HSliderJoint;
	typedef GameObjectHandle<CDistanceJoint> HDistanceJoint;
	typedef GameObjectHandle<CSphericalJoint> HSphericalJoint;
	typedef GameObjectHandle<CFixedJoint> HFixedJoint;
	typedef GameObjectHandle<CD6Joint> HD6Joint;
	typedef GameObjectHandle<CCharacterController> HCharacterController;

	/** Valid types of a mesh used for physics. */
	enum class BS_SCRIPT_EXPORT() PhysicsMeshType
	{
		/**
		 * A regular triangle mesh. Mesh can be of arbitrary size but cannot be used for triggers and non-kinematic
		 * objects. Incurrs a significantly larger performance impact than convex meshes.
		 */
		Triangle,
		/**
		 * Mesh representing a convex shape. Mesh will not have more than 256 vertices. Incurrs a significantly lower
		 * performance impact than triangle meshes.
		 */
		Convex
	};

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for Physics module RTTI */
	enum TypeID_Physics
	{
		TID_PhysicsMaterial = 1092,
		TID_CCollider = 1093,
		TID_CBoxCollider = 1094,
		TID_CSphereCollider = 1095,
		TID_CCapsuleCollider = 1096,
		TID_CPlaneCollider = 1097,
		TID_CRigidbody = 1098,
		TID_PhysicsMesh = 1099,
		TID_CMeshCollider = 1100,
		TID_CJoint = 1101,
		TID_CFixedJoint = 1102,
		TID_CDistanceJoint = 1103,
		TID_CHingeJoint = 1104,
		TID_CSphericalJoint = 1105,
		TID_CSliderJoint = 1106,
		TID_CD6Joint = 1107,
		TID_CCharacterController = 1108,
		TID_FPhysicsMesh = 1109,
	};
}
