# Source files and their filters
include(CMakeSources.cmake)
	
add_subdirectory(DiligentCore)

# Target
add_library(bsfDiligentRenderAPI SHARED ${BS_DILIGENTRENDERAPI_SRC})

# Common flags
add_common_flags(bsfDiligentRenderAPI)

# Defines
target_compile_definitions(bsfDiligentRenderAPI PRIVATE -DUNICODE -DENGINE_DLL)

# Includes
target_include_directories(bsfDiligentRenderAPI PRIVATE "./" "./DiligentCore/")
	
# Libraries
target_link_libraries(bsfDiligentRenderAPI PRIVATE 
	Diligent-GraphicsEngineD3D11-shared
    Diligent-GraphicsEngineOpenGL-shared
    Diligent-GraphicsEngineD3D12-shared
    Diligent-GraphicsEngineVk-shared
)

## Local libs
target_link_libraries(bsfDiligentRenderAPI PRIVATE bsf)

# IDE specific
set_property(TARGET bsfDiligentRenderAPI PROPERTY FOLDER Plugins)

# Install
if(RENDER_API_MODULE MATCHES "DiligentOpenGL" OR RENDER_API_MODULE MATCHES "DiligentOpenGLES" OR RENDER_API_MODULE MATCHES "DiligentD3D11" OR RENDER_API_MODULE MATCHES "DiligentD3D12" OR RENDER_API_MODULE MATCHES "DiligentVulkan")
	install_bsf_target(bsfDiligentRenderAPI)
endif()

conditional_cotire(bsfDiligentRenderAPI)
