//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "bsfCore/BsCorePrerequisites.h"

/** @addtogroup Layers
 *  @{
 */

/** @defgroup RenderAPI RenderAPI
 *	Low-level rendering API abstraction layer.
 *  @{
 */

/** @defgroup RenderAPI-Engine RenderAPI
  *	Low-level rendering API functionality.
  */

/** @} */
/** @} */

/** Maximum number of color surfaces that can be attached to a multi render target. */
#define BS_MAX_MULTIPLE_RENDER_TARGETS 8

/** Maximum number of individual GPU queues, per type. */
#define BS_MAX_QUEUES_PER_TYPE 8

/** Maximum number of hardware devices usable at once. */
#define BS_MAX_DEVICES 5U

/** Maximum number of devices one resource can exist at the same time. */
#define BS_MAX_LINKED_DEVICES 4U

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32 // Windows
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_RENDERAPI_EXPORT
#    else
#      if defined(BS_RENDERAPI_EXPORTS)
#        define BS_RENDERAPI_EXPORT __declspec(dllexport)
#      else
#        define BS_RENDERAPI_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_RENDERAPI_EXPORT
#    else
#      if defined(BS_RENDERAPI_EXPORTS)
#        define BS_RENDERAPI_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_RENDERAPI_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_RENDERAPI_HIDDEN
#else // Linux/Mac settings
#  define BS_RENDERAPI_EXPORT __attribute__ ((visibility ("default")))
#  define BS_RENDERAPI_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

// Forward declarations
namespace bs
{
	// Core object forward declarations (sim and core thread objects)
	CORE_OBJECT_FORWARD_DECLARE(IndexBuffer)
	CORE_OBJECT_FORWARD_DECLARE(VertexBuffer)
	CORE_OBJECT_FORWARD_DECLARE(GpuBuffer)
	CORE_OBJECT_FORWARD_DECLARE(GpuProgram)
	CORE_OBJECT_FORWARD_DECLARE(RenderTarget)
	CORE_OBJECT_FORWARD_DECLARE(RenderTexture)
	CORE_OBJECT_FORWARD_DECLARE(RenderWindow)
	CORE_OBJECT_FORWARD_DECLARE(SamplerState)
	CORE_OBJECT_FORWARD_DECLARE(Viewport)
	CORE_OBJECT_FORWARD_DECLARE(VertexDeclaration)
	CORE_OBJECT_FORWARD_DECLARE(DepthStencilState)
	CORE_OBJECT_FORWARD_DECLARE(RasterizerState)
	CORE_OBJECT_FORWARD_DECLARE(BlendState)
	CORE_OBJECT_FORWARD_DECLARE(GpuParamBlockBuffer)
	CORE_OBJECT_FORWARD_DECLARE(GpuParams)
	CORE_OBJECT_FORWARD_DECLARE(GpuParamsSet)
	CORE_OBJECT_FORWARD_DECLARE(GraphicsPipelineState)
	CORE_OBJECT_FORWARD_DECLARE(ComputePipelineState)
	CORE_OBJECT_FORWARD_DECLARE(Texture)

	class RenderAPI;
	class RenderWindow;
	class RenderTexture;
	class RenderTarget;
	class Texture;
	class IndexBuffer;
	class VertexBuffer;
	class VertexDeclaration;
	class VertexDataDesc;
	class GpuParams;
	class GpuParamsSet;
	class GpuProgram;
	class GpuPipelineState;
	class GraphicsPipelineState;
	class ComputePipelineState;
	class BlendState;
	class RasterizerState;
	class DepthStencilState;
	class SamplerState;
	class GpuParamBlockBuffer;
	class GpuParamBlock;
	class GpuBuffer;
	class EventQuery;
	class TimerQuery;
	class OcclusionQuery;
	class Viewport;
	class VideoMode;
	class VideoOutputInfo;
	class VideoModeInfo;
	class CommandBuffer;
	class TextureView;
	class GpuProgramManager;
	class GpuProgramFactory;
	class HardwareBufferManager;
	class RenderStateManager;
	class RenderAPIFactory;
	class TextureManager;
	class GpuResourceData;
	class IndexData;
	class GpuPipelineParamInfo;
	class ShaderDefines;

	struct RenderAPICapabilities;
	struct TextureProperties;
	struct RenderWindowProperties;
	struct RenderTextureProperties;
	struct RenderTargetProperties;
	struct RenderOperation;
	struct SubMesh;
	struct GpuParamDesc;
	struct GpuParamDataDesc;
	struct GpuParamObjectDesc;
	struct GpuParamBlockDesc;

	// Desc structs
	struct SAMPLER_STATE_DESC;
	struct DEPTH_STENCIL_STATE_DESC;
	struct RASTERIZER_STATE_DESC;
	struct BLEND_STATE_DESC;
	struct RENDER_TARGET_BLEND_STATE_DESC;
	struct RENDER_TEXTURE_DESC;
	struct RENDER_WINDOW_DESC;

	namespace ct
	{
		class RenderAPI;
		class VertexData;
		class MeshBase;
		class TransientMesh;
		class MeshHeap;
		class GpuPipelineParamInfo;
		class CommandBuffer;
		class EventQuery;
		class TimerQuery;
		class OcclusionQuery;
		class TextureView;
		class RenderElement;
		class RenderWindowManager;
		class RenderStateManager;
		class HardwareBufferManager;
	}

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for RenderAPI module RTTI */
	enum TypeID_RenderAPI
	{
		TID_Texture = 1001,
		TID_VertexDeclaration = 1004,
		TID_VertexElementData = 1005,
		TID_GpuProgram = 1010,
		TID_SamplerState = 1021,
		TID_BlendState = 1023,
		TID_RasterizerState = 1024,
		TID_DepthStencilState = 1025,
		TID_BLEND_STATE_DESC = 1034,
		TID_Viewport = 1073,
		TID_GpuProgramBytecode = 1145,
		TID_GpuParamBlockDesc = 1146,
		TID_GpuParamDataDesc = 1147,
		TID_GpuParamObjectDesc = 1148,
		TID_GpuParamDesc = 1149,
		TID_BlendStateDesc = 1150,
		TID_RasterizerStateDesc = 1151,
		TID_DepthStencilStateDesc = 1152,
		TID_SerializedGpuProgramData = 1153,
		TID_RenderTarget = 1193,
		TID_RenderTexture = 1194,
		TID_RenderWindow = 1195,
	};
}
