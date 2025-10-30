set(BS_RENDERER_INC_RENDERER
	"bsfRenderer/Renderer/BsLight.h"
	"bsfRenderer/Renderer/BsRenderable.h"
	"bsfRenderer/Renderer/BsRendererManager.h"
	"bsfRenderer/Renderer/BsRendererFactory.h"
	"bsfRenderer/Renderer/BsRenderer.h"
	"bsfRenderer/Renderer/BsRendererMeshData.h"
	"bsfRenderer/Renderer/BsParamBlocks.h"
	"bsfRenderer/Renderer/BsCamera.h"
	"bsfRenderer/Renderer/BsRenderSettings.h"
	"bsfRenderer/Renderer/BsRendererExtension.h"
	"bsfRenderer/Renderer/BsReflectionProbe.h"
	"bsfRenderer/Renderer/BsSkybox.h"
	"bsfRenderer/Renderer/BsLightProbeVolume.h"
	"bsfRenderer/Renderer/BsIBLUtility.h"
	"bsfRenderer/Renderer/BsGpuResourcePool.h"
	"bsfRenderer/Renderer/BsDecal.h"
	"bsfRenderer/Renderer/BsRendererMaterial.h"
	"bsfRenderer/Renderer/BsRendererMaterialManager.h"
	"bsfRenderer/Renderer/BsRenderElement.h"
	"bsfRenderer/Renderer/BsRenderQueue.h"
	"bsfRenderer/Renderer/BsRendererUtility.h"
)

set(BS_RENDERER_SRC_RENDERER
	"bsfRenderer/Renderer/BsLight.cpp"
	"bsfRenderer/Renderer/BsRenderable.cpp"
	"bsfRenderer/Renderer/BsRendererManager.cpp"
	"bsfRenderer/Renderer/BsRenderer.cpp"
	"bsfRenderer/Renderer/BsRendererMeshData.cpp"
	"bsfRenderer/Renderer/BsParamBlocks.cpp"
	"bsfRenderer/Renderer/BsCamera.cpp"
	"bsfRenderer/Renderer/BsRenderSettings.cpp"
	"bsfRenderer/Renderer/BsRendererExtension.cpp"
	"bsfRenderer/Renderer/BsReflectionProbe.cpp"
	"bsfRenderer/Renderer/BsSkybox.cpp"
	"bsfRenderer/Renderer/BsLightProbeVolume.cpp"
	"bsfRenderer/Renderer/BsIBLUtility.cpp"
	"bsfRenderer/Renderer/BsGpuResourcePool.cpp"
	"bsfRenderer/Renderer/BsDecal.cpp"
	"bsfRenderer/Renderer/BsRendererMaterialManager.cpp"
	"bsfRenderer/Renderer/BsRenderQueue.cpp"
	"bsfRenderer/Renderer/BsRendererUtility.cpp"
)

set(BS_RENDERER_INC_UTILITY
	"bsfRenderer/Utility/BsRendererEnums.h"
	"bsfRenderer/Utility/BsRendererBuiltinResources.h"
	"bsfRenderer/Utility/BsDrawHelper.h"
	"bsfRenderer/Utility/BsShapeMeshes2D.h"
	"bsfRenderer/Utility/BsShapeMeshes3D.h"
)

set(BS_RENDERER_SRC_UTILITY
	"bsfRenderer/Utility/BsRendererBuiltinResources.cpp"
	"bsfRenderer/Utility/BsDrawHelper.cpp"
	"bsfRenderer/Utility/BsShapeMeshes2D.cpp"
	"bsfRenderer/Utility/BsShapeMeshes3D.cpp"
)

set(BS_RENDERER_INC_DEBUG
	"bsfRenderer/Debug/BsDebugDraw.h"
)

set(BS_RENDERER_SRC_DEBUG
	"bsfRenderer/Debug/BsDebugDraw.cpp"
)

set(BS_RENDERER_INC_MESH
	"bsfRenderer/Mesh/BsTransientMesh.h"
	"bsfRenderer/Mesh/BsMeshHeap.h"
	"bsfRenderer/Mesh/BsMeshData.h"
	"bsfRenderer/Mesh/BsMeshBase.h"
	"bsfRenderer/Mesh/BsMesh.h"
	"bsfRenderer/Mesh/BsMeshManager.h"
	"bsfRenderer/Mesh/BsMeshUtility.h"
)

set(BS_RENDERER_SRC_MESH
	"bsfRenderer/Mesh/BsMesh.cpp"
	"bsfRenderer/Mesh/BsMeshBase.cpp"
	"bsfRenderer/Mesh/BsMeshData.cpp"
	"bsfRenderer/Mesh/BsMeshHeap.cpp"
	"bsfRenderer/Mesh/BsTransientMesh.cpp"
	"bsfRenderer/Mesh/BsMeshManager.cpp"
	"bsfRenderer/Mesh/BsMeshUtility.cpp"
)

set(BS_RENDERER_INC_MATERIAL
	"bsfRenderer/Material/BsTechnique.h"
	"bsfRenderer/Material/BsShader.h"
	"bsfRenderer/Material/BsPass.h"
	"bsfRenderer/Material/BsMaterial.h"
	"bsfRenderer/Material/BsMaterialParam.h"
	"bsfRenderer/Material/BsShaderManager.h"
	"bsfRenderer/Material/BsMaterialParams.h"
	"bsfRenderer/Material/BsGpuParamsSet.h"
	"bsfRenderer/Material/BsShaderInclude.h"
	"bsfRenderer/Material/BsShaderVariation.h"
)

set(BS_RENDERER_SRC_MATERIAL
	"bsfRenderer/Material/BsMaterial.cpp"
	"bsfRenderer/Material/BsPass.cpp"
	"bsfRenderer/Material/BsShader.cpp"
	"bsfRenderer/Material/BsTechnique.cpp"
	"bsfRenderer/Material/BsMaterialParam.cpp"
	"bsfRenderer/Material/BsShaderManager.cpp"
	"bsfRenderer/Material/BsMaterialParams.cpp"
	"bsfRenderer/Material/BsGpuParamsSet.cpp"
	"bsfRenderer/Material/BsShaderInclude.cpp"
	"bsfRenderer/Material/BsShaderVariation.cpp"
)

set(BS_RENDERER_INC_IMPORTER
	"bsfRenderer/Importer/BsTextureImportOptions.h"
	"bsfRenderer/Importer/BsShaderIncludeImporter.h"
	"bsfRenderer/Importer/BsMeshImportOptions.h"
	"bsfRenderer/Importer/BsShaderImportOptions.h"
)

set(BS_RENDERER_SRC_IMPORTER
	"bsfRenderer/Importer/BsTextureImportOptions.cpp"
	"bsfRenderer/Importer/BsShaderIncludeImporter.cpp"
	"bsfRenderer/Importer/BsMeshImportOptions.cpp"
	"bsfRenderer/Importer/BsShaderImportOptions.cpp"
)

set(BS_RENDERER_SRC_RTTI
	"bsfRenderer/Private/RTTI/BsMaterialRTTI.cpp"
)

set(BS_RENDERER_INC_RTTI
	"bsfRenderer/Private/RTTI/BsTextureImportOptionsRTTI.h"
	"bsfRenderer/Private/RTTI/BsShaderImportOptionsRTTI.h"
	"bsfRenderer/Private/RTTI/BsMeshImportOptionsRTTI.h"
	"bsfRenderer/Private/RTTI/BsShaderIncludeRTTI.h"
	"bsfRenderer/Private/RTTI/BsMaterialRTTI.h"
	"bsfRenderer/Private/RTTI/BsMaterialParamsRTTI.h"
	"bsfRenderer/Private/RTTI/BsMeshRTTI.h"
	"bsfRenderer/Private/RTTI/BsMeshBaseRTTI.h"
	"bsfRenderer/Private/RTTI/BsMeshDataRTTI.h"
	"bsfRenderer/Private/RTTI/BsShaderRTTI.h"
	"bsfRenderer/Private/RTTI/BsShaderVariationRTTI.h"
	"bsfRenderer/Private/RTTI/BsTechniqueRTTI.h"
	"bsfRenderer/Private/RTTI/BsPassRTTI.h"
	"bsfRenderer/Private/RTTI/BsCameraRTTI.h"
	"bsfRenderer/Private/RTTI/BsCCameraRTTI.h"
	"bsfRenderer/Private/RTTI/BsRenderableRTTI.h"
	"bsfRenderer/Private/RTTI/BsCRenderableRTTI.h"
	"bsfRenderer/Private/RTTI/BsLightRTTI.h"
	"bsfRenderer/Private/RTTI/BsCLightRTTI.h"
	"bsfRenderer/Private/RTTI/BsReflectionProbeRTTI.h"
	"bsfRenderer/Private/RTTI/BsCReflectionProbeRTTI.h"
	"bsfRenderer/Private/RTTI/BsSkyboxRTTI.h"
	"bsfRenderer/Private/RTTI/BsCSkyboxRTTI.h"
	"bsfRenderer/Private/RTTI/BsLightProbeVolumeRTTI.h"
	"bsfRenderer/Private/RTTI/BsCLightProbeVolumeRTTI.h"
	"bsfRenderer/Private/RTTI/BsDecalRTTI.h"
	"bsfRenderer/Private/RTTI/BsCDecalRTTI.h"
	"bsfRenderer/Private/RTTI/BsRenderSettingsRTTI.h"
	"bsfRenderer/Private/RTTI/BsRenderTargetRTTI.h"
)

set(BS_RENDERER_INC_NOFILTER
	"bsfRenderer/BsRendererPrerequisites.h"
)

source_group("Renderer" FILES ${BS_RENDERER_INC_RENDERER} ${BS_RENDERER_SRC_RENDERER})
source_group("Utility" FILES ${BS_RENDERER_INC_UTILITY} ${BS_RENDERER_SRC_UTILITY})
source_group("Debug" FILES ${BS_RENDERER_INC_DEBUG} ${BS_RENDERER_SRC_DEBUG})
source_group("Mesh" FILES ${BS_RENDERER_INC_MESH} ${BS_RENDERER_SRC_MESH})
source_group("Material" FILES ${BS_RENDERER_INC_MATERIAL} ${BS_RENDERER_SRC_MATERIAL})
source_group("Importer" FILES ${BS_RENDERER_INC_IMPORTER} ${BS_RENDERER_SRC_IMPORTER})
source_group("RTTI" FILES ${BS_RENDERER_INC_RTTI} ${BS_RENDERER_SRC_RTTI})
source_group("" FILES ${BS_RENDERER_INC_NOFILTER})

set(BS_RENDERER_SRC
	${BS_RENDERER_INC_RENDERER}
	${BS_RENDERER_SRC_RENDERER}
	${BS_RENDERER_INC_UTILITY}
	${BS_RENDERER_SRC_UTILITY}
	${BS_RENDERER_INC_DEBUG}
	${BS_RENDERER_SRC_DEBUG}
	${BS_RENDERER_INC_MESH}
	${BS_RENDERER_SRC_MESH}
	${BS_RENDERER_INC_MATERIAL}
	${BS_RENDERER_SRC_MATERIAL}
	${BS_RENDERER_INC_IMPORTER}
	${BS_RENDERER_SRC_IMPORTER}
	${BS_RENDERER_INC_RTTI}
	${BS_RENDERER_SRC_RTTI}
	${BS_RENDERER_INC_NOFILTER}
)
