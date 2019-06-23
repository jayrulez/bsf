# Source files and their filters
include(CMakeSources.cmake)
	
# Target
add_library(bsfDiligentRenderAPI SHARED ${BS_DILIGENTRENDERAPI_SRC})

# Common flags
add_common_flags(bsfDiligentRenderAPI)

# Includes
target_include_directories(bsfDiligentRenderAPI PRIVATE "./")
	
# Libraries
## Local libs
target_link_libraries(bsfDiligentRenderAPI PRIVATE bsf)

# IDE specific
set_property(TARGET bsfDiligentRenderAPI PROPERTY FOLDER Plugins)

# Install
if(RENDER_API_MODULE MATCHES "Diligent")
	install_bsf_target(bsfDiligentRenderAPI)
endif()

conditional_cotire(bsfDiligentRenderAPI)
