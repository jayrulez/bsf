//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "bsfCore/BsCorePrerequisites.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup Renderer Renderer
 *	High-level rendering system.
 *  @{
 */

/** @defgroup Renderer-Engine Renderer
  *	High-level rendering, cameras, lights, and materials.
  */

/** @} */
/** @} */

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_RENDERER_EXPORT
#    else
#      if defined(BS_RENDERER_EXPORTS)
#        define BS_RENDERER_EXPORT __declspec(dllexport)
#      else
#        define BS_RENDERER_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_RENDERER_EXPORT
#    else
#      if defined(BS_RENDERER_EXPORTS)
#        define BS_RENDERER_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_RENDERER_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_RENDERER_HIDDEN
#else // Linux/Mac settings
#  define BS_RENDERER_EXPORT __attribute__ ((visibility ("default")))
#  define BS_RENDERER_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	// Core object forward declarations (sim and core thread objects)
	CORE_OBJECT_FORWARD_DECLARE(Pass)
	CORE_OBJECT_FORWARD_DECLARE(Technique)
	CORE_OBJECT_FORWARD_DECLARE(Shader)
	CORE_OBJECT_FORWARD_DECLARE(Material)
	CORE_OBJECT_FORWARD_DECLARE(MaterialParams)
	CORE_OBJECT_FORWARD_DECLARE(Light)
	CORE_OBJECT_FORWARD_DECLARE(Camera)
	CORE_OBJECT_FORWARD_DECLARE(Renderable)
	CORE_OBJECT_FORWARD_DECLARE(ReflectionProbe)
	CORE_OBJECT_FORWARD_DECLARE(ParticleSystem)
	CORE_OBJECT_FORWARD_DECLARE(Skybox)
	CORE_OBJECT_FORWARD_DECLARE(Decal)
	CORE_OBJECT_FORWARD_DECLARE(LightProbeVolume)
	CORE_OBJECT_FORWARD_DECLARE(Mesh)
	CORE_OBJECT_FORWARD_DECLARE(MeshBase)
	CORE_OBJECT_FORWARD_DECLARE(TransientMesh)
	CORE_OBJECT_FORWARD_DECLARE(MeshHeap)
	CORE_OBJECT_FORWARD_DECLARE(SpriteTexture)
	CORE_OBJECT_FORWARD_DECLARE(VectorField)

	CORE_OBJECT_FORWARD_DECLARE_STRUCT(DepthOfFieldSettings)
	CORE_OBJECT_FORWARD_DECLARE_STRUCT(ChromaticAberrationSettings)
	CORE_OBJECT_FORWARD_DECLARE_STRUCT(RenderSettings)

	class Renderer;
	class RendererManager;
	class RendererFactory;
	class RendererExtension;
	class RendererMeshData;
	class RenderQueue;
	class LightProbeVolume;
	class MeshBase;
	class TransientMesh;
	class MeshHeap;
	class MeshRTTI;

	// Import options
	class MeshImportOptions;
	class TextureImportOptions;
	class ShaderImportOptions;

	namespace ct
	{
		class Renderer;
		class MeshBase;
		class TransientMesh;
		class MeshHeap;
	}

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<Texture> HTexture;
	typedef ResourceHandle<Mesh> HMesh;
	typedef ResourceHandle<Material> HMaterial;
	typedef ResourceHandle<Shader> HShader;
	typedef ResourceHandle<SpriteTexture> HSpriteTexture;
	typedef ResourceHandle<VectorField> HVectorField;

	/************************************************************************/
	/* 						Game object handle references             		*/
	/************************************************************************/

	template<class T>
	class GameObjectHandle;

	// Game object handles
	typedef GameObjectHandle<CCamera> HCamera;
	typedef GameObjectHandle<CRenderable> HRenderable;
	typedef GameObjectHandle<CLight> HLight;
	typedef GameObjectHandle<CReflectionProbe> HReflectionProbe;
	typedef GameObjectHandle<CSkybox> HSkybox;
	typedef GameObjectHandle<CLightProbeVolume> HLightProbeVolume;
	typedef GameObjectHandle<CParticleSystem> HParticleSystem;
	typedef GameObjectHandle<CDecal> HDecal;

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for Renderer module RTTI */
	enum TypeID_Renderer
	{
		TID_Mesh = 1002,
		TID_MeshData = 1003,
		TID_Pass = 1014,
		TID_Technique = 1015,
		TID_Shader = 1016,
		TID_Material = 1017,
		TID_SHADER_DATA_PARAM_DESC = 1035,
		TID_SHADER_OBJECT_PARAM_DESC = 1036,
		TID_SHADER_PARAM_BLOCK_DESC = 1047,
		TID_MeshBase = 1065,
		TID_TextureImportOptions = 1070,
		TID_ShaderMetaData = 1075,
		TID_MeshImportOptions = 1076,
		TID_MaterialParamColor = 1086,
		TID_TextureParamData = 1088,
		TID_StructParamData = 1089,
		TID_MaterialParams = 1090,
		TID_MaterialRTTIParam = 1091,
		TID_ShaderImportOptions = 1110,
		TID_MaterialParamData = 1126,
		TID_RenderSettings = 1127,
		TID_ReflectionProbe = 1131,
		TID_CReflectionProbe = 1132,
		TID_CachedTextureData = 1133,
		TID_Skybox = 1134,
		TID_CSkybox = 1135,
		TID_LightProbeVolume = 1136,
		TID_SavedLightProbeInfo = 1137,
		TID_CLightProbeVolume = 1138,
		TID_ShaderVariationParam = 1143,
		TID_ShaderVariation = 1144,
		TID_SubShader = 1154,
		TID_ParticleSystem = 1155,
		TID_ColorDistribution = 1156,
		TID_TDistribution = 1157,
		TID_SHADER_PARAM_ATTRIBUTE = 1158,
		TID_DataParamInfo = 1159,
		TID_SpriteSheetGridAnimation = 1160,
		TID_ParticleEmitter = 1161,
		TID_ParticleEmitterConeShape = 1162,
		TID_ParticleEmitterSphereShape = 1163,
		TID_ParticleEmitterHemisphereShape = 1164,
		TID_ParticleEmitterBoxShape = 1165,
		TID_ParticleEmitterCircleShape = 1166,
		TID_ParticleEmitterRectShape = 1167,
		TID_ParticleEmitterLineShape = 1168,
		TID_ParticleEmitterStaticMeshShape = 1169,
		TID_ParticleEmitterSkinnedMeshShape = 1170,
		TID_ParticleTextureAnimation = 1171,
		TID_ParticleCollisions = 1172,
		TID_ParticleOrbit = 1173,
		TID_ParticleVelocity = 1174,
		TID_ParticleSystemSettings = 1175,
		TID_ParticleSystemEmitters = 1176,
		TID_ParticleSystemEvolvers = 1177,
		TID_CParticleSystem = 1178,
		TID_ParticleGravity = 1179,
		TID_VectorField = 1180,
		TID_ParticleVectorFieldSettings = 1181,
		TID_ParticleGpuSimulationSettings = 1182,
		TID_ParticleDepthCollisionSettings = 1183,
		TID_BloomSettings = 1184,
		TID_ParticleBurst = 1185,
		TID_ParticleForce = 1187,
		TID_ParticleSize = 1188,
		TID_ParticleColor = 1189,
		TID_ParticleRotation = 1190,
		TID_Decal = 1191,
		TID_CDecal = 1192,
		TID_ShaderVariationParamInfo = 1196,
		TID_ShaderVariationParamValue = 1197,
		TID_ScreenSpaceLensFlareSettings = 1198,
		TID_ChromaticAberrationSettings = 1199,
		TID_FilmGrainSettings = 1200,
		TID_AutoExposureSettings = 1201,
		TID_TonemappingSettings = 1202,
		TID_WhiteBalanceSettings = 1203,
		TID_ColorGradingSettings = 1204,
		TID_DepthOfFieldSettings = 1205,
		TID_AmbientOcclusionSettings = 1206,
		TID_ScreenSpaceReflectionsSettings = 1207,
		TID_ShadowSettings = 1208,
		TID_MotionBlurSettings = 1209,
		TID_TemporalAASettings = 1210,

		// Moved from Engine layer
		TID_CCamera = 30000,
		TID_CRenderable = 30001,
		TID_SpriteTexture = 30002,
		TID_Camera = 30003,
		TID_Renderable = 30004,
		TID_Light = 30011,
		TID_CLight = 30012,
	};
}
